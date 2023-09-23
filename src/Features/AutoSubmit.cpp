#include "AutoSubmit.hpp"
#include "Cheats.hpp"
#include "Command.hpp"
#include "Event.hpp"
#include "Features/Hud/Toasts.hpp"
#include "Features/NetMessage.hpp"
#include "Features/Session.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Server.hpp"
#include "Utils/json11.hpp"
#include <cctype>
#include <fstream>
#include <sstream>
#include <map>
#include <optional>
#include <string>
#include <filesystem>
#include <curl/curl.h>

#define AUTOSUBMIT_TOAST_TAG "autosubmit"
#define COOP_NAME_MESSAGE_TYPE "coop-name"
#define API_KEY_FILE "autosubmit.key"

static std::string g_partner_name;
static bool g_cheated = false;
//static bool g_challenge_mode = false;

ON_EVENT(SESSION_START) {
	if (engine->IsCoop()) {
		const char *name = Variable("name").GetString();
		NetMessage::SendMsg(COOP_NAME_MESSAGE_TYPE, name, strlen(name));
	}
	g_partner_name = "(unknown partner)";
	g_cheated = sv_cheats.GetBool();
	//g_challenge_mode = !sv_cheats.GetBool() && sar_challenge_mode.GetBool();
}

// ON_EVENT(SESSION_END) {
// 	if (g_challenge_mode) {
// 		AutoSubmit::SubmitRun(event.time);
// 	}
// }

ON_INIT {
	NetMessage::RegisterHandler(COOP_NAME_MESSAGE_TYPE, +[](const void *data, size_t size) {
		g_partner_name = std::string((char *)data, size);
	});
}

ON_EVENT(PRE_TICK) {
	if (sv_cheats.GetBool()) g_cheated = true;
}

static std::string g_api_base;
static std::string g_api_key;
static bool g_key_valid;
static CURL *g_curl;
static std::thread g_worker;

static std::map<std::string, std::string> g_map_ids = {};

static bool ensureCurlReady() {
	if (!g_curl) {
		g_curl = curl_easy_init();

		if (!g_curl) {
			return false;
		}
	}

	return true;
}

static std::optional<std::string> request(std::string url) {
	curl_easy_setopt(g_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(g_curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(g_curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(g_curl, CURLOPT_TIMEOUT, 30);

#ifdef UNSAFELY_IGNORE_CERTIFICATE_ERROR
	curl_easy_setopt(g_curl, CURLOPT_SSL_VERIFYPEER, 0);
#endif

	curl_easy_setopt(
		g_curl,
		CURLOPT_WRITEFUNCTION,
		+[](void *ptr, size_t sz, size_t nmemb, std::string *data) -> size_t {
			data->append((char *)ptr, sz * nmemb);
			return sz * nmemb;
		}
	);

	std::string response;
	curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, &response);

	CURLcode res = curl_easy_perform(g_curl);

	long code;
	curl_easy_getinfo(g_curl, CURLINFO_RESPONSE_CODE, &code);

	if (res != CURLE_OK) {
		THREAD_PRINT("ERROR IN AUTOSUBMIT REQUEST TO %s: %s\n", url.c_str(), curl_easy_strerror(res));
	}

	return res == CURLE_OK && code == 200 ? response : std::optional<std::string>{};
}

static void testApiKey() {
	if (!ensureCurlReady()) {
		THREAD_PRINT("Failed to test API key!\n");
		return;
	}

	curl_mime *form = curl_mime_init(g_curl);
	curl_mimepart *field;

	field = curl_mime_addpart(form);
	curl_mime_name(field, "auth_hash");
	curl_mime_data(field, g_api_key.c_str(), CURL_ZERO_TERMINATED);

	curl_easy_setopt(g_curl, CURLOPT_MIMEPOST, form);

	auto response = request(g_api_base + "/validate-user");

	curl_mime_free(form);

	if (!response) {
		THREAD_PRINT("API key invalid!\n");
		return;
	}

	g_key_valid = true;
	THREAD_PRINT("API key valid!\n");

	response = request(g_api_base + "/download-maps");
	if (!response) {
		THREAD_PRINT("Failed to downloaded maps!\n");
		return;
	}

	std::string err;
	auto json = json11::Json::parse(*response, err);

	if (err != "") {
		return;
	}

	for (auto map : json["maps"].array_items()) {
		g_map_ids.insert({
			map["level_name"].string_value(),
			map["id"].string_value()
		});
	}

	THREAD_PRINT("Downloaded %i maps!\n", g_map_ids.size());
}

static std::optional<int> getCurrentPbScore(std::string& map_id) {
	if (!ensureCurlReady()) return {};

	curl_mime *form = curl_mime_init(g_curl);
	curl_mimepart *field;

	field = curl_mime_addpart(form);
	curl_mime_name(field, "auth_hash");
	curl_mime_data(field, g_api_key.c_str(), CURL_ZERO_TERMINATED);

	field = curl_mime_addpart(form);
	curl_mime_name(field, "mapId");
	curl_mime_data(field, map_id.c_str(), CURL_ZERO_TERMINATED);

	curl_easy_setopt(g_curl, CURLOPT_MIMEPOST, form);

	auto response = request(g_api_base + "/current-pb");

	curl_mime_free(form);

	if (!response) return {};

	std::string err;
	auto json = json11::Json::parse(*response, err);

	if (err != "") {
		return {};
	}

	if (json["score"].is_null()) {
		return -1; // No current PB
	}

	auto str = json["score"].string_value();
	return atoi(str.c_str());
}

static void submitTime(int score, std::string demopath, bool coop, std::string map_id, std::optional<std::string> rename_if_pb, std::optional<std::string> replay_append_if_pb) {
	auto score_str = std::to_string(score);

	if (!g_key_valid) {
		if (rename_if_pb) {
			std::filesystem::rename(demopath, *rename_if_pb);
		}
		if (replay_append_if_pb) {
			engine->demoplayer->replayName += *replay_append_if_pb;
		}
		return;
	}

	auto cur_pb = getCurrentPbScore(map_id);
	if (cur_pb) {
		if (*cur_pb > -1 && score >= *cur_pb) {
			THREAD_PRINT("Not PB; not submitting.\n");
			Scheduler::OnMainThread([=](){
				Event::Trigger<Event::MAYBE_AUTOSUBMIT>({score, coop, false});
			});
			return;
		}
	}

	// If we couldn't detect if this run PBd, rename the demo anyway to be
	// safe

	if (rename_if_pb) {
		std::filesystem::rename(demopath, *rename_if_pb);
		demopath = *rename_if_pb;
	}

	if (replay_append_if_pb) {
		engine->demoplayer->replayName += *replay_append_if_pb;
	}

	if (!cur_pb) {
		toastHud.AddToast(AUTOSUBMIT_TOAST_TAG, "An error occurred submitting this time");
		return;
	}

	if (!ensureCurlReady()) {
		toastHud.AddToast(AUTOSUBMIT_TOAST_TAG, "An error occurred submitting this time");
		return;
	}

	Scheduler::OnMainThread([=](){
		Event::Trigger<Event::MAYBE_AUTOSUBMIT>({score, coop, true});
	});

	curl_mime *form = curl_mime_init(g_curl);
	curl_mimepart *field;

	field = curl_mime_addpart(form);
	curl_mime_name(field, "auth_hash");
	curl_mime_data(field, g_api_key.c_str(), CURL_ZERO_TERMINATED);

	field = curl_mime_addpart(form);
	curl_mime_name(field, "mapId");
	curl_mime_data(field, map_id.c_str(), CURL_ZERO_TERMINATED);

	field = curl_mime_addpart(form);
	curl_mime_name(field, "score");
	curl_mime_data(field, score_str.c_str(), CURL_ZERO_TERMINATED);

	field = curl_mime_addpart(form);
	curl_mime_name(field, "demoFile");
	curl_mime_filedata(field, demopath.c_str());

	if (coop) {
		field = curl_mime_addpart(form);
		curl_mime_name(field, "comment");
		curl_mime_data(field, g_partner_name.c_str(), CURL_ZERO_TERMINATED);
	}

	curl_easy_setopt(g_curl, CURLOPT_MIMEPOST, form);

	auto resp = request(g_api_base + "/auto-submit");

	curl_mime_free(form);

	if (!resp) {
		toastHud.AddToast(AUTOSUBMIT_TOAST_TAG, "An error occurred submitting this time");
	} else {
		toastHud.AddToast(AUTOSUBMIT_TOAST_TAG, "Successfully submitted time to boards!");
	}
}

static void loadApiKey(bool output_nonexist) {
	if (!std::filesystem::exists(API_KEY_FILE)) {
		if (output_nonexist) {
			console->Print("API key file " API_KEY_FILE " does not exist!\n");
		}
		return;
	}

	std::string base;
	std::string key;

	{
		std::ifstream f(API_KEY_FILE);
		std::getline(f, base) && std::getline(f, key);
	}

	key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());

	bool valid = key.size() > 0;
	for (auto c : key) {
		if (c >= 'a' && c <= 'z') continue;
		if (c >= 'A' && c <= 'Z') continue;
		if (c >= '0' && c <= '9') continue;
		valid = false;
	}

	if (!valid) {
		console->Print("Invalid API key!\n");
		return;
	}

	g_api_base = "https://" + base + "/api-v2";
	g_api_key = key;
	g_key_valid = false;
	console->Print("Set API key! Testing...\n");

	if (g_worker.joinable()) g_worker.join();
	g_worker = std::thread(testApiKey);
}

ON_INIT {
	loadApiKey(false);
}

CON_COMMAND_F(sar_challenge_autosubmit_reload_api_key, "sar_challenge_autosubmit_reload_api_key - reload the board.portal2.sr API key from its file.\n", FCVAR_DONTRECORD) {
	if (args.ArgC() != 1) {
		return console->Print(sar_challenge_autosubmit_reload_api_key.ThisPtr()->m_pszHelpString);
	}

	loadApiKey(true);
}

void AutoSubmit::SubmitRun(float final_time) {
	Scheduler::InHostTicks(DEMO_AUTOSTOP_DELAY, [=]() {
		if (!engine->demorecorder->isRecordingDemo) return; // manual stop before autostop
		if (sar_challenge_autostop.GetInt() > 0) {
			std::string demoFile = engine->demorecorder->GetDemoFilename();

			engine->demorecorder->Stop();

			std::optional<std::string> rename_if_pb = {};
			std::optional<std::string> replay_append_if_pb = {};

			if (sar_challenge_autostop.GetInt() == 2 || sar_challenge_autostop.GetInt() == 3) {
				unsigned total = floor(final_time * 100);
				unsigned cs = total % 100;
				total /= 100;
				unsigned secs = total % 60;
				total /= 60;
				unsigned mins = total % 60;
				total /= 60;
				unsigned hrs = total;

				std::string time;

				if (hrs) {
					time = Utils::ssprintf("%d-%02d-%02d-%02d", hrs, mins, secs, cs);
				} else if (mins) {
					time = Utils::ssprintf("%d-%02d-%02d", mins, secs, cs);
				} else {
					time = Utils::ssprintf("%d-%02d", secs, cs);
				}

				auto newName = Utils::ssprintf("%s_%s.dem", demoFile.substr(0, demoFile.size() - 4).c_str(), time.c_str());
				if (sar_challenge_autostop.GetInt() == 2) {
					std::filesystem::rename(demoFile, newName);
					demoFile = newName;
					engine->demoplayer->replayName += "_";
					engine->demoplayer->replayName += time;
				} else { // autostop 3
					rename_if_pb = newName;
					replay_append_if_pb = std::string("_") + time;
				}
			}

			AutoSubmit::FinishRun(final_time, demoFile.c_str(), rename_if_pb, replay_append_if_pb);
		}
	});
}

void AutoSubmit::FinishRun(float final_time, const char *demopath, std::optional<std::string> rename_if_pb, std::optional<std::string> replay_append_if_pb) {
	if (g_cheated) {
		console->Print("Cheated; not autosubmitting\n");
		return;
	}

	auto it = g_map_ids.find(engine->GetCurrentMapName());
	if (it == g_map_ids.end()) {
		console->Print("Unknown map; not autosubmitting\n");
		if (rename_if_pb) {
			std::filesystem::rename(demopath, *rename_if_pb);
		}
		if (replay_append_if_pb) {
			engine->demoplayer->replayName += *replay_append_if_pb;
		}
		return;
	}

	auto map_id = it->second;

	int score = floor(final_time * 100);

	if (g_worker.joinable()) g_worker.join();
	g_worker = std::thread(submitTime, score, std::string(demopath), Utils::StartsWith(engine->GetCurrentMapName().c_str(), "mp_"), map_id, rename_if_pb, replay_append_if_pb);
}

ON_EVENT(SAR_UNLOAD) {
	if (g_worker.joinable()) g_worker.detach();
}
