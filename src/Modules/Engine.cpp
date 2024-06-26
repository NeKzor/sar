#include "Engine.hpp"

#include "Offsets.hpp"
#include "Utils/Memory.hpp"
#include <cstdint>
#include <cstring>

#include "Features/Cvars.hpp"
#include "Features/Session.hpp"
#include "Features/Speedrun/SpeedrunTimer.hpp"

#include "Console.hpp"
#include "EngineDemoPlayer.hpp"
#include "EngineDemoRecorder.hpp"
#include "Server.hpp"

#include "Game.hpp"
#include "Interface.hpp"
#include "SAR.hpp"
#include "Utils.hpp"
#include "Variable.hpp"

Variable host_framerate;
Variable net_showmsg;

REDECL(Engine::Disconnect);
REDECL(Engine::Disconnect2);
REDECL(Engine::SetSignonState);
REDECL(Engine::SetSignonState2);
REDECL(Engine::Frame);
REDECL(Engine::OnGameOverlayActivated);
REDECL(Engine::OnGameOverlayActivatedBase);
REDECL(Engine::exit_callback);
REDECL(Engine::quit_callback);
REDECL(Engine::help_callback);
REDECL(Engine::gameui_activate_callback);
#ifdef _WIN32
REDECL(Engine::connect_callback);
#endif

void Engine::ExecuteCommand(const char* cmd, bool immediately)
{
    this->SendToCommandBuffer(cmd, 0);
    if (immediately) {
		this->ExecuteClientCmd(this->engineClient->ThisPtr(), "");
    }
}
int Engine::GetTick() const
{
    return (this->GetMaxClients() < 2) ? *this->tickcount : TIME_TO_TICKS(*this->net_time);
}
float Engine::ToTime(int tick) const
{
    return tick * *this->interval_per_tick;
}
int Engine::GetLocalPlayerIndex()
{
    return this->GetLocalPlayer(this->engineClient->ThisPtr());
}
edict_t* Engine::PEntityOfEntIndex(int iEntIndex)
{
    if (iEntIndex >= 0 && iEntIndex < server->gpGlobals->maxEntities) {
        auto pEdict = reinterpret_cast<edict_t*>((uintptr_t)server->gpGlobals->pEdicts + iEntIndex * sizeof(edict_t));
        if (!pEdict->IsFree()) {
            return pEdict;
        }
    }

    return nullptr;
}
QAngle Engine::GetAngles(int nSlot)
{
    auto va = QAngle();
    if (this->GetLocalClient) {
        auto client = this->GetLocalClient(nSlot);
        if (client) {
            va = *reinterpret_cast<QAngle*>((uintptr_t)client + Offsets::viewangles);
        }
    } else {
        this->GetViewAngles(this->engineClient->ThisPtr(), va);
    }
    return va;
}
void Engine::SetAngles(int nSlot, QAngle va)
{
    if (this->GetLocalClient) {
        auto client = this->GetLocalClient(nSlot);
        if (client) {
            auto viewangles = reinterpret_cast<QAngle*>((uintptr_t)client + Offsets::viewangles);
            viewangles->x = Math::AngleNormalize(va.x);
            viewangles->y = Math::AngleNormalize(va.y);
            viewangles->z = Math::AngleNormalize(va.z);
        }
    } else {
        this->SetViewAngles(this->engineClient->ThisPtr(), va);
    }
}
void Engine::SendToCommandBuffer(const char* text, int delay)
{
    if (sar.game->Is(SourceGame_Portal2Engine | SourceGame_StrataEngine)) {
#ifdef _WIN32
        auto slot = this->GetActiveSplitScreenPlayerSlot();
#else
        auto slot = this->GetActiveSplitScreenPlayerSlot(nullptr);
#endif
#ifdef __x86_64
        this->Cbuf_AddText(slot, text, 0, delay);
#else
        this->Cbuf_AddText(slot, text, delay);
#endif
    } else if (sar.game->Is(SourceGame_HalfLife2Engine)) {
        this->AddText(this->s_CommandBuffer, text, delay);
    }
}
int Engine::PointToScreen(const Vector& point, Vector& screen) const
{
#ifdef _WIN32
    return this->ScreenPosition(point, screen);
#else
    return this->ScreenPosition(nullptr, point, screen);
#endif
}
void Engine::SafeUnload(const char* postCommand)
{
    // The exit command will handle everything
    this->ExecuteCommand("sar_exit");

    if (postCommand) {
        this->SendToCommandBuffer(postCommand, SAFE_UNLOAD_TICK_DELAY);
    }
}

// CClientState::Disconnect
DETOUR(Engine::Disconnect, bool bShowMainMenu)
{
    session->Ended();
    return Engine::Disconnect(thisptr, bShowMainMenu);
}
#ifdef _WIN32
DETOUR(Engine::Disconnect2, int unk1, int unk2, int unk3)
{
    session->Ended();
    return Engine::Disconnect2(thisptr, unk1, unk2, unk3);
}
DETOUR_COMMAND(Engine::connect)
{
    session->Ended();
    Engine::connect_callback(args);
}
#else
DETOUR(Engine::Disconnect2, int unk, bool bShowMainMenu)
{
    session->Ended();
    return Engine::Disconnect2(thisptr, unk, bShowMainMenu);
}
#endif

// CClientState::SetSignonState
DETOUR(Engine::SetSignonState, int state, int count, void* unk)
{
    session->Changed(state);
    return Engine::SetSignonState(thisptr, state, count, unk);
}
DETOUR(Engine::SetSignonState2, int state, int count)
{
    session->Changed(state);
    return Engine::SetSignonState2(thisptr, state, count);
}

// CEngine::Frame
DETOUR(Engine::Frame)
{
    speedrun->PreUpdate(engine->GetTick(), engine->m_szMapname);

    if (engine->hoststate->m_currentState != session->prevState) {
        session->Changed();
    }
    session->prevState = engine->hoststate->m_currentState;

    if (engine->hoststate->m_activeGame || std::strlen(engine->m_szMapname) == 0) {
        speedrun->PostUpdate(engine->GetTick(), engine->m_szMapname);
    }

    return Engine::Frame(thisptr);
}

// CSteam3Client::OnGameOverlayActivated
DETOUR_B(Engine::OnGameOverlayActivated, GameOverlayActivated_t* pGameOverlayActivated)
{
    engine->overlayActivated = pGameOverlayActivated->m_bActive;
    return Engine::OnGameOverlayActivatedBase(thisptr, pGameOverlayActivated);
}

DETOUR_COMMAND(Engine::exit)
{
    engine->SafeUnload("exit");
}
DETOUR_COMMAND(Engine::quit)
{
    engine->SafeUnload("quit");
}
DETOUR_COMMAND(Engine::help)
{
    cvars->PrintHelp(args);
}
DETOUR_COMMAND(Engine::gameui_activate)
{
    if (sar_disable_steam_pause.GetBool() && engine->overlayActivated) {
        return;
    }

    Engine::gameui_activate_callback(args);
}

bool Engine::Init()
{
    this->engineClient = Interface::Create(this->Name(), "VEngineClient0", false);

    if (this->engineClient) {
        this->GetScreenSize = this->engineClient->Original<_GetScreenSize>(Offsets::GetScreenSize);
        this->ClientCmd = this->engineClient->Original<_ClientCmd>(Offsets::ClientCmd);
		this->ExecuteClientCmd = this->engineClient->Original<_ExecuteClientCmd>(Offsets::ExecuteClientCmd);
        this->GetLocalPlayer = this->engineClient->Original<_GetLocalPlayer>(Offsets::GetLocalPlayer);
        this->GetViewAngles = this->engineClient->Original<_GetViewAngles>(Offsets::GetViewAngles);
        this->SetViewAngles = this->engineClient->Original<_SetViewAngles>(Offsets::SetViewAngles);
        this->GetMaxClients = this->engineClient->Original<_GetMaxClients>(Offsets::GetMaxClients);
        this->GetGameDirectory = this->engineClient->Original<_GetGameDirectory>(Offsets::GetGameDirectory);

        Memory::Read<_Cbuf_AddText>((uintptr_t)this->ClientCmd + Offsets::Cbuf_AddText, &this->Cbuf_AddText);
        Memory::Deref<void*>((uintptr_t)this->Cbuf_AddText + Offsets::s_CommandBuffer, &this->s_CommandBuffer);

        if (sar.game->Is(SourceGame_Portal2Engine | SourceGame_StrataEngine)) {
            Memory::Read((uintptr_t)this->SetViewAngles + Offsets::GetLocalClient, &this->GetLocalClient);

            if (sar.game->Is(SourceGame_Portal2Game | SourceGame_INFRA)) {
                this->m_bWaitEnabled = reinterpret_cast<bool*>((uintptr_t)s_CommandBuffer + Offsets::m_bWaitEnabled);
                this->m_bWaitEnabled2 = reinterpret_cast<bool*>((uintptr_t)this->m_bWaitEnabled + Offsets::CCommandBufferSize);
            }

            if (sar.game->Is(SourceGame_Portal2Game)) {
                auto GetSteamAPIContext = this->engineClient->Original<uintptr_t (*)()>(Offsets::GetSteamAPIContext);
                auto OnGameOverlayActivated = reinterpret_cast<_OnGameOverlayActivated*>(GetSteamAPIContext() + Offsets::OnGameOverlayActivated);

                Engine::OnGameOverlayActivatedBase = *OnGameOverlayActivated;
                *OnGameOverlayActivated = reinterpret_cast<_OnGameOverlayActivated>(Engine::OnGameOverlayActivated_Hook);
            }

            if (auto g_VEngineServer = Interface::Create(this->Name(), "VEngineServer0", false)) {
                this->ClientCommand = g_VEngineServer->Original<_ClientCommand>(Offsets::ClientCommand);
                Interface::Delete(g_VEngineServer);
            }
        }

        void* clPtr = nullptr;
        if (sar.game->Is(SourceGame_Portal2Engine)) {
            typedef void* (*_GetClientState)();
            auto GetClientState = Memory::Read<_GetClientState>((uintptr_t)this->ClientCmd + Offsets::GetClientStateFunction);
            clPtr = GetClientState();

            this->GetActiveSplitScreenPlayerSlot = this->engineClient->Original<_GetActiveSplitScreenPlayerSlot>(Offsets::GetActiveSplitScreenPlayerSlot);
        } else if (sar.game->Is(SourceGame_HalfLife2Engine)) {
            auto ServerCmdKeyValues = this->engineClient->Original(Offsets::ServerCmdKeyValues);
            Memory::Deref<void*>(ServerCmdKeyValues + Offsets::cl, &clPtr);

            Memory::Read<_AddText>((uintptr_t)this->Cbuf_AddText + Offsets::AddText, &this->AddText);
        } else if (sar.game->Is(SourceGame_StrataEngine)) {
            typedef void* (*_GetClientState)();
            auto GetClientState = Memory::Read<_GetClientState>((uintptr_t)this->ClientCmd + Offsets::GetClientStateFunction);
            clPtr = GetClientState();

            if (this->GetLocalClient) {
                auto g_SplitScreenMgr = Memory::Read<void*>((uintptr_t)this->GetLocalClient + Offsets::g_SplitScreenMgr);
                this->GetActiveSplitScreenPlayerSlot = Memory::VMT<_GetActiveSplitScreenPlayerSlot>(g_SplitScreenMgr, Offsets::GetActiveSplitScreenPlayerSlot);
            }
        }

        if (this->cl = Interface::Create(clPtr)) {
            if (!this->demoplayer)
                this->demoplayer = new EngineDemoPlayer();
            if (!this->demorecorder)
                this->demorecorder = new EngineDemoRecorder();

            if (sar.game->Is(SourceGame_Portal2Engine | SourceGame_StrataEngine)) {
                this->cl->Hook(Engine::SetSignonState_Hook, Engine::SetSignonState, Offsets::Disconnect - 1);
                this->cl->Hook(Engine::Disconnect_Hook, Engine::Disconnect, Offsets::Disconnect);
            } else if (sar.game->Is(SourceGame_HalfLife2Engine)) {
                this->cl->Hook(Engine::SetSignonState2_Hook, Engine::SetSignonState2, Offsets::Disconnect - 1);
#ifdef _WIN32
                Command::Hook("connect", Engine::connect_callback_hook, Engine::connect_callback);
#else
                this->cl->Hook(Engine::Disconnect2_Hook, Engine::Disconnect2, Offsets::Disconnect);
#endif
            }
#if _WIN32 && !__x86_64
            auto IServerMessageHandler_VMT = Memory::Deref<uintptr_t>((uintptr_t)this->cl->ThisPtr() + IServerMessageHandler_VMT_Offset);
            auto ProcessTick = Memory::Deref<uintptr_t>(IServerMessageHandler_VMT + sizeof(uintptr_t) * Offsets::ProcessTick);
#else
            auto ProcessTick = this->cl->Original(Offsets::ProcessTick);
#endif

            if (sar.game->Is(SourceGame_StrataEngine)) {
                if (IS_LINUX) {
                    auto g_ClientGlobalVariables = Memory::Read(ProcessTick + Offsets::g_ClientGlobalVariables);
                    this->tickcount = reinterpret_cast<int*>(g_ClientGlobalVariables + Offsets::tickcount);
                    this->interval_per_tick = reinterpret_cast<float*>(g_ClientGlobalVariables + Offsets::interval_per_tick);
                } else {
                    Memory::Deref<int*>(ProcessTick + Offsets::tickcount, &this->tickcount);
                    Memory::Deref<float*>(ProcessTick + Offsets::interval_per_tick, &this->interval_per_tick);
                }

                auto SetSignonState = this->cl->Original(Offsets::Disconnect - 1);
                this->hoststate = *Memory::Read<CHostState**>(SetSignonState + Offsets::hoststate);
            } else {
                Memory::Deref<int*>(ProcessTick + Offsets::tickcount, &this->tickcount);
                Memory::Deref<float*>(ProcessTick + Offsets::interval_per_tick, &this->interval_per_tick);

                auto SetSignonState = this->cl->Original(Offsets::Disconnect - 1);
                auto HostState_OnClientConnected = Memory::Read(SetSignonState + Offsets::HostState_OnClientConnected);
                Memory::Deref<CHostState*>(HostState_OnClientConnected + Offsets::hoststate, &this->hoststate);
            }

            speedrun->SetIntervalPerTick(this->interval_per_tick);
        }
    }

    if (auto tool = Interface::Create(this->Name(), "VENGINETOOL0", false)) {
        auto GetCurrentMap = tool->Original(Offsets::GetCurrentMap);

        if (sar.game->Is(SourceGame_StrataEngine) && IS_LINUX) {
            auto sv = Memory::Read<uintptr_t>(GetCurrentMap + Offsets::sv);
            this->m_szMapname = reinterpret_cast<char*>(sv + Offsets::m_szMapname);
            this->m_bLoadgame = reinterpret_cast<bool*>(sv + Offsets::m_bLoadGame);
        } else {
            Memory::Deref<char*>(GetCurrentMap + Offsets::m_szMapname, &this->m_szMapname);
            this->m_bLoadgame = reinterpret_cast<bool*>((uintptr_t)this->m_szMapname + Offsets::m_bLoadGame);
        }

        if (sar.game->Is(SourceGame_HalfLife2Engine) && std::strlen(this->m_szMapname) != 0) {
            console->Warning("DO NOT load this plugin when the server is active!\n");
            return false;
        }

        Interface::Delete(tool);
    }

    if (auto s_EngineAPI = Interface::Create(this->Name(), "VENGINE_LAUNCHER_API_VERSION0", false)) {
        auto IsRunningSimulation = s_EngineAPI->Original(Offsets::IsRunningSimulation);
        auto engAddr = *Memory::Read<void**>(IsRunningSimulation + Offsets::eng);

        if (this->eng = Interface::Create(engAddr)) {
            if (this->tickcount && this->hoststate && this->m_szMapname) {
                this->eng->Hook(Engine::Frame_Hook, Engine::Frame, Offsets::Frame);
            }
        }
        Interface::Delete(s_EngineAPI);
    }

    if (sar.game->Is(SourceGame_Portal2 | SourceGame_ApertureTag)) {
        this->s_GameEventManager = Interface::Create(this->Name(), "GAMEEVENTSMANAGER002", false);
        if (this->s_GameEventManager) {
            this->AddListener = this->s_GameEventManager->Original<_AddListener>(Offsets::AddListener);
            this->RemoveListener = this->s_GameEventManager->Original<_RemoveListener>(Offsets::RemoveListener);

            if (!sar.game->Is(SourceGame_StrataEngine)) {
                auto FireEventClientSide = s_GameEventManager->Original(Offsets::FireEventClientSide);
                auto FireEventIntern = Memory::Read(FireEventClientSide + Offsets::FireEventIntern);
                Memory::Read<_ConPrintEvent>(FireEventIntern + Offsets::ConPrintEvent, &this->ConPrintEvent);
            }
        }
    }

#if _WIN32 && !__x86_64
    if (sar.game->Is(SourceGame_Portal2Game)) {
        auto parseSmoothingInfoAddr = Memory::Scan(this->Name(), "55 8B EC 0F 57 C0 81 EC ? ? ? ? B9 ? ? ? ? 8D 85 ? ? ? ? EB", 178);
        auto readCustomDataAddr = Memory::Scan(this->Name(), "55 8B EC F6 05 ? ? ? ? ? 53 56 57 8B F1 75 2F");

        console->DevMsg("CDemoSmootherPanel::ParseSmoothingInfo = %p\n", parseSmoothingInfoAddr);
        console->DevMsg("CDemoFile::ReadCustomData = %p\n", readCustomDataAddr);

        if (parseSmoothingInfoAddr && readCustomDataAddr) {
            MH_HOOK_MID(Engine::ParseSmoothingInfo_Mid, parseSmoothingInfoAddr);            // Hook switch-case
            Engine::ParseSmoothingInfo_Continue = parseSmoothingInfoAddr + 8;               // Back to original function
            Engine::ParseSmoothingInfo_Default = parseSmoothingInfoAddr + 133;              // Default case
            Engine::ParseSmoothingInfo_Skip = parseSmoothingInfoAddr - 29;                  // Continue loop
            Engine::ReadCustomData = reinterpret_cast<_ReadCustomData>(readCustomDataAddr); // Function that handles dem_customdata

            this->demoSmootherPatch = new Memory::Patch();
            unsigned char nop3[] = { 0x90, 0x90, 0x90 };
            this->demoSmootherPatch->Execute(parseSmoothingInfoAddr + 5, nop3);             // Nop rest
        }
    }
#endif

    if (auto debugoverlay = Interface::Create(this->Name(), "VDebugOverlay0", false)) {
        ScreenPosition = debugoverlay->Original<_ScreenPosition>(Offsets::ScreenPosition);
        Interface::Delete(debugoverlay);
    }

    Command::Hook("exit", Engine::exit_callback_hook, Engine::exit_callback);
    Command::Hook("quit", Engine::quit_callback_hook, Engine::quit_callback);
    Command::Hook("help", Engine::help_callback_hook, Engine::help_callback);

    if (sar.game->Is(SourceGame_Portal2Game)) {
        Command::Hook("gameui_activate", Engine::gameui_activate_callback_hook, Engine::gameui_activate_callback);
    }

    host_framerate = Variable("host_framerate");
    net_showmsg = Variable("net_showmsg");

    return this->hasLoaded = this->engineClient && this->demoplayer && this->demorecorder;
}
void Engine::Shutdown()
{
    if (this->engineClient && sar.game->Is(SourceGame_Portal2Game)) {
        auto GetSteamAPIContext = this->engineClient->Original<uintptr_t (*)()>(Offsets::GetSteamAPIContext);
        auto OnGameOverlayActivated = reinterpret_cast<_OnGameOverlayActivated*>(GetSteamAPIContext() + Offsets::OnGameOverlayActivated);
        *OnGameOverlayActivated = Engine::OnGameOverlayActivatedBase;
    }

    Interface::Delete(this->engineClient);
    Interface::Delete(this->cl);
    Interface::Delete(this->eng);
    Interface::Delete(this->s_GameEventManager);

#ifdef _WIN32
    Command::Unhook("connect", Engine::connect_callback);
#endif
    Command::Unhook("exit", Engine::exit_callback);
    Command::Unhook("quit", Engine::quit_callback);
    Command::Unhook("help", Engine::help_callback);
    Command::Unhook("gameui_activate", Engine::gameui_activate_callback);

    if (this->demoplayer) {
        this->demoplayer->Shutdown();
    }
    if (this->demorecorder) {
        this->demorecorder->Shutdown();
    }

    SAFE_DELETE(this->demoplayer)
    SAFE_DELETE(this->demorecorder)
}

Engine* engine;
