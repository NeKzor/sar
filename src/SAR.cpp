#include "SAR.hpp"

#include "Utils/Platform.hpp"
#include <cstring>

#include "Features.hpp"
#include "Modules.hpp"

#include "Cheats.hpp"
#include "Command.hpp"
#include "Game.hpp"
#include "Interface.hpp"
#include "Variable.hpp"

SAR sar;

SAR::SAR()
    : modules()
    , features()
    , cheats()
    , game()
{
}

bool SAR::Load()
{
    console = new Console();
    if (!console->Init())
        return false;

    this->game = Game::CreateNew();
    if (this->game) {
        this->game->LoadOffsets();

        tier1 = new Tier1();
        if (tier1->Init()) {
            this->features = new Features();
            this->features->AddFeature<Config>(&config);
            this->features->AddFeature<Cvars>(&cvars);
#ifndef _WIN32
            this->features->AddFeature<Rebinder>(&rebinder);
#endif
            this->game->Is(SourceGame_INFRA)
                ? this->features->AddFeature<InfraSession>(reinterpret_cast<InfraSession**>(&session))
                : this->features->AddFeature<Session>(&session);
            this->features->AddFeature<StepCounter>(&stepCounter);
            this->features->AddFeature<Summary>(&summary);
            this->features->AddFeature<Teleporter>(&teleporter);
            this->features->AddFeature<Tracer>(&tracer);
            this->features->AddFeature<SpeedrunTimer>(&speedrun);
            this->features->AddFeature<Stats>(&stats);
            this->features->AddFeature<CommandQueuer>(&cmdQueuer);
            this->features->AddFeature<ReplayRecorder>(&replayRecorder1);
            this->features->AddFeature<ReplayRecorder>(&replayRecorder2);
            this->features->AddFeature<ReplayPlayer>(&replayPlayer1);
            this->features->AddFeature<ReplayPlayer>(&replayPlayer2);
            this->features->AddFeature<ReplayProvider>(&replayProvider);
            this->features->AddFeature<Timer>(&timer);
            this->features->AddFeature<EntityInspector>(&inspector);
            this->features->AddFeature<ClassDumper>(&classDumper);
            this->features->AddFeature<EntityList>(&entityList);
            this->features->AddFeature<OffsetFinder>(&offsetFinder);
            this->features->AddFeature<AutoStrafer>(&autoStrafer);
            this->features->AddFeature<PauseTimer>(&pauseTimer);
            this->features->AddFeature<DataMapDumper>(&dataMapDumper);
            this->features->AddFeature<FovChanger>(&fovChanger);

            this->modules = new Modules();
            this->modules->AddModule<InputSystem>(&inputSystem);
            this->modules->AddModule<Scheme>(&scheme);
            this->modules->AddModule<Surface>(&surface);
            this->modules->AddModule<VGui>(&vgui);
            this->modules->AddModule<Engine>(&engine);
            this->modules->AddModule<Client>(&client);
            this->modules->AddModule<Server>(&server);
            this->modules->InitAll();

            if (engine && engine->hasLoaded) {
                engine->demoplayer->Init();
                engine->demorecorder->Init();

                this->cheats = new Cheats();
                this->cheats->Init();

                this->features->AddFeature<TasTools>(&tasTools);

                if (this->game->Is(SourceGame_Portal2 | SourceGame_ApertureTag)) {
                    this->features->AddFeature<Listener>(&listener);
                    this->features->AddFeature<WorkshopList>(&workshop);
                    this->features->AddFeature<Imitator>(&imitator);
                }

                if (listener) {
                    listener->Init();
                }

                speedrun->LoadRules(this->game);

                config->Load();

                console->PrintActive("Loaded SourceAutoRecord, Version %s\n", SAR_VERSION);
                return true;
            } else {
                console->Warning("Failed to load engine module!\n");
            }
        } else {
            console->Warning("Failed to load tier1 module!\n");
        }
    } else {
        console->Warning("Game not supported!\n");
    }

    if (sar.cheats) {
        sar.cheats->Shutdown();
    }
    if (sar.features) {
        sar.features->DeleteAll();
    }

    if (sar.modules) {
        sar.modules->ShutdownAll();
    }

    SAFE_DELETE(sar.features)
    SAFE_DELETE(sar.cheats)
    SAFE_DELETE(sar.modules)
    SAFE_DELETE(sar.game)
    SAFE_DELETE(tier1)
    SAFE_DELETE(console)
    return false;
}

void SAR::Unload()
{
    if (sar.cheats) {
        sar.cheats->Shutdown();
    }
    if (sar.features) {
        sar.features->DeleteAll();
    }

    if (sar.modules) {
        sar.modules->ShutdownAll();
    }    

    SAFE_DELETE(sar.features)
    SAFE_DELETE(sar.cheats)
    SAFE_DELETE(sar.modules)
    SAFE_DELETE(sar.game)

    if (console) {
        console->Print("Cya :)\n");
    }

    SAFE_DELETE(tier1)
    SAFE_DELETE(console)
}

CON_COMMAND(sar_session, "Prints the current tick of the server since it has loaded.\n")
{
    auto tick = session->GetTick();
    console->Print("Session Tick: %i (%.3f)\n", tick, engine->ToTime(tick));
    if (*engine->demorecorder->m_bRecording) {
        tick = engine->demorecorder->GetTick();
        console->Print("Demo Recorder Tick: %i (%.3f)\n", tick, engine->ToTime(tick));
    }
    if (engine->demoplayer->IsPlaying()) {
        tick = engine->demoplayer->GetTick();
        console->Print("Demo Player Tick: %i (%.3f)\n", tick, engine->ToTime(tick));
    }
}
CON_COMMAND(sar_about, "Prints info about SAR plugin.\n")
{
    console->Print("SourceAutoRecord is a speedrun plugin for Source Engine games.\n");
    console->Print("More information at: %s\n", sar.Website());
    console->Print("Game: %s\n", sar.game->Version());
    console->Print("Version: %s\n", sar.Version());
    console->Print("Build: %s\n", sar.Build());
}
CON_COMMAND(sar_cvars_save, "Saves important SAR cvars.\n")
{
    if (!config->Save()) {
        console->Print("Failed to create config file!\n");
    } else {
        console->Print("Saved important settings to cfg/_sar_cvars.cfg!\n");
    }
}
CON_COMMAND(sar_cvars_load, "Loads important SAR cvars.\n")
{
    if (!config->Load()) {
        console->Print("Config file not found!\n");
    }
}
CON_COMMAND(sar_cvars_dump, "Dumps all cvars to a file.\n")
{
    std::ofstream file("game.cvars", std::ios::out | std::ios::trunc | std::ios::binary);
    auto result = cvars->Dump(file);
    file.close();

    console->Print("Dumped %i cvars to game.cvars!\n", result);
}
CON_COMMAND(sar_cvars_dump_doc, "Dumps all SAR cvars to a file.\n")
{
    std::ofstream file("sar.cvars", std::ios::out | std::ios::trunc | std::ios::binary);
    auto result = cvars->DumpDoc(file);
    file.close();

    console->Print("Dumped %i cvars to sar.cvars!\n", result);
}
CON_COMMAND(sar_cvars_lock, "Restores default flags of unlocked cvars.\n")
{
    cvars->Lock();
}
CON_COMMAND(sar_cvars_unlock, "Unlocks all special cvars.\n")
{
    cvars->Unlock();
}
CON_COMMAND(sar_cvarlist, "Lists all SAR cvars and unlocked engine cvars.\n")
{
    cvars->ListAll();
}
CON_COMMAND(sar_rename, "Changes your name. Usage: sar_rename <name>\n")
{
    if (args.ArgC() != 2) {
        return console->Print(sar_rename.ThisPtr()->m_pszHelpString);
    }

    auto name = Variable("name");
    if (!!name) {
        name.DisableChange();
        name.SetValue(args[1]);
        name.EnableChange();
    }
}

#if _WIN64
HMODULE module_handle = {};
auto sar_shutdown() -> void
{
    FreeLibraryAndExitThread(module_handle, 0);
}
#endif

CON_COMMAND_F(sar_exit, "Removes all function hooks, registered commands and unloads the module.\n", FCVAR_CLIENTCMD_CAN_EXECUTE)
{
    sar.Unload();
#if _WIN64
    CreateRemoteThread(GetCurrentProcess(), 0, 0, LPTHREAD_START_ROUTINE(sar_shutdown), 0, 0, 0);
#endif
}

DLL_EXPORT auto sar_load() -> int
{
    return sar.Load() ? 0 : 1;
}

DLL_EXPORT auto sar_unload() -> void
{
    sar.Unload();
}

#ifdef _WIN64
auto APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) -> BOOL
{
    if (reason == DLL_PROCESS_ATTACH) {
        module_handle = module;
        return sar_load() == 0;
    } else if (reason == DLL_PROCESS_DETACH) {
        sar_unload();
    }
    return TRUE;
}
#endif
