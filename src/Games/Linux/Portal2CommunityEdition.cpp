#include "Portal2CommunityEdition.hpp"

#include "Game.hpp"
#include "Offsets.hpp"

Portal2CommunityEdition::Portal2CommunityEdition()
{
    this->version = SourceGame_Portal2CommunityEdition;
}
void Portal2CommunityEdition::LoadOffsets()
{
    using namespace Offsets;

    // engine.so

    Dtor = 0; // ConVar
    InternalSetValue = 17; // ConVar
    InternalSetFloatValue = 18; // ConVar
    InternalSetIntValue = 19; // ConVar
    Create = 24; // ConVar
    GetScreenSize = 40; // CEngineClient
    ClientCmd = 42; // CEngineClient
    GetClientStateFunction = 11; // CEngineClient::ClientCmd
    Cbuf_AddText = 46; // CEngineClient::ClientCmd
    s_CommandBuffer = 95; // Cbuf_AddText
    CCommandBufferSize = 8304; // Cbuf_AddText
    //m_bWaitEnabled = 8265; // unused CCommandBuffer::AddText
    GetLocalPlayer = 45; // CEngineClient
    GetViewAngles = 51; // CEngineClient
    SetViewAngles = 52; // CEngineClient
    GetLocalClient = 98; // CEngineClient::SetViewAngles
    g_SplitScreenMgr = 12; // GetLocalClient
    viewangles = 134824; // CEngineClient::SetViewAngles
    GetMaxClients = 53; // CEngineClient
    GetGameDirectory = 12; // CEngineClient
    GetActiveSplitScreenPlayerSlot = 5; // GetLocalClient
    //GetSteamAPIContext = 178; //unused CEngineClient
    ExecuteClientCmd = 122; // CEngineClient
    StringToButtonCode = 42; // CInputSystem/unbind
    SleepUntilInput = 44; // CInputSystem
    GetRecordingTick = 2; // CDemoRecorder
    net_time = 26; // CDemoRecorder::GetRecordingTick
    SetSignonState = 0; // CDemoRecorder
    StopRecording = 9; // CDemoRecorder
    GetPlaybackTick = 4; // CDemoPlayer::SkipToTick
    //StartPlayback = 6; //unused CDemoPlayer
    IsPlayingBack = 5; // DemoRecorder::SetSignonState
    m_szFileName = 16; // CDemoPlayer::WriteTimeDemoResults
    m_szDemoBaseName = 1364; // CDemoRecorder::StartupDemoFile
    m_nDemoNumber = 1628; // CDemoRecorder::StartupDemoFile
    m_bRecording = 1626; // CDemoRecorder::SetSignonState
    Paint = 15; // CEngineVGui
    ProcessTick = 15; // CClientState
    g_ClientGlobalVariables = 234; // CClientState::ProcessTick
    tickcount = 36; // CClientState::ProcessTick
    interval_per_tick = 40; // CClientState::ProcessTick
    //HostState_OnClientConnected = 735; //unused CClientState::SetSignonState
    hoststate = 1702; // CClientState::SetSignonState/HostState_OnClientConnected
    Disconnect = 50; // CClientState
    demoplayer = 68; // CClientState::Disconnect
    demorecorder = 87; // CClientState::Disconnect
    GetCurrentMap = 23; // CEngineTool
    sv = 9; // CEngineTool::GetCurrentMap
    m_szLevelName = 20; // CEngineTool::GetCurrentMap/m_szMapname
    AddListener = 4; // CGameEventManager
    RemoveListener = 6; // CGameEventManager
    FireEventClientSide = 10; // CGameEventManager
    FireEventIntern = 13; // CGameEventManager::FireEventClientSide
    //ConPrintEvent = 254; //unused CGameEventManager::FireEventIntern
    AutoCompletionFunc = 42; // listdemo_CompletionFunc
    Key_SetBinding = 118; // unbind
    IsRunningSimulation = 17; // CEngineAPI
    eng = 7; // CEngineAPI::IsRunningSimulation
    Frame = 6; // CEngine
    m_bLoadGame = 1016; // CGameClient::ActivatePlayer
    //ScreenPosition = 11; //unused CIVDebugOverlay
    MAX_SPLITSCREEN_PLAYERS = 2; // maxplayers
    //OnGameOverlayActivated = 152; //unused CSteam3Client
    ClientCommand = 68; // CVEngineServer
    OnDemoPlaybackStart = 55; // CL_PlayDemo_f
    GetClientEntity = 3; // CClientEntityList/CL_TakeSnapshotAndSwap

    // libvstdlib.so

    RegisterConCommand = 10; // CCVar
    UnregisterConCommand = 11; // CCvar
    FindCommandBase = 14; // CCvar
    InstallGlobalChangeCallback = 20; // CCvar
    RemoveGlobalChangeCallback = 21; // CCvar
    m_pConCommandList = 80; // CCvar
    IsCommand = 2; // ConCommandBase

    // vgui2.so

    GetIScheme = 9; // CSchemeManager
    GetFont = 4; // CScheme

    // server.so

    ProcessMovement = 2; // CGameMovement
    PlayerMove = 18; // CPortalGameMovement
    AirAccelerate = 29; // CPortalGameMovement
    AirMove = 30; // CPortalGameMovement
    AirMove_Offset1 = 10; // CGameMovement::~CPortalGameMovement
    //AirMove_Offset2 = 12; //unused CGameMovement::~CGameMovement
    FinishGravity = 44; // CPortalGameMovement
    CheckJumpButton = 46; // CPortalGameMovement
    FullTossMove = 47; // CGameMovement
    mv = 16; // CPortalGameMovement::CheckJumpButton
    GameFrame = 4; // CServerGameDLL
    GetAllServerClasses = 10; // CServerGameDLL
    IsRestoring = 24; // CServerGameDLL
    Think = 30; // CServerGameDLL
    UTIL_PlayerByIndex = 51; // CServerGameDLL::Think
    gpGlobals = 19; // UTIL_PlayerByIndex
    player = 8; // CPortalGameMovement::PlayerMove
    m_MoveType = 382; // CPortalGameMovement::PlayerMove
    m_iClassName = 184; // CBaseEntity/CPointEntityFinder::FindByDistance
    S_m_vecAbsOrigin = 700; // CBaseEntity::SetAbsOrigin/ent_setpos
    S_m_angAbsRotation = 712; // CBaseEntity::SetAbsAngles/ent_setang
    m_iEFlags = 356; // CBaseEntity/EnableNoClip
    m_flGravity = 1264; // CBaseEntity/CGameMovement::StartGravity
    NUM_ENT_ENTRIES = 8192; // CServerTools::GetIServerEntity
    GetIServerEntity = 2; // CServerTools
    m_EntPtrArray = 55; // CServerTools::GetIServerEntity
    IsPlayer = 100; // ForEachPlayer
    m_pSurfaceData = 6072; // CGameMovement::CheckJumpButton
    jumpFactor = 84; // CGameMovement::CheckJumpButton
    IsAsleep = 3;
    IsCollisionEnabled = 7;
    IsGravityEnabled = 8;
    GetPosition = 49;
    GetVelocity = 53;
    SetPosition = 47;
    SetVelocity = 51;
    EnableGravity = 14;
    Wake = 25;
    Sleep = 26;
    m_pShadowStand = 4880; // CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow
    m_pShadowCrouch = 4888; // CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow

    // client.so

    GetAllClasses = 8; // CHLClient
    HudProcessInput = 10; // CHLClient
    HudUpdate = 11; // CHLClient
    C_m_vecAbsOrigin = 216; // C_BasePlayer::GetAbsOrigin/SplitScreenTeleport
    C_m_angAbsRotation = 252; // C_BasePlayer::GetAbsAngles/SplitScreenTeleport
    GetClientMode = 9; // CHLClient::HudProcessInput
    g_pClientMode = 25; // GetClientMode
    CreateMove = 25; // ClientModeShared
    //GetName = 11; //unused CHud
    //GetHud = 104; //unused cc_leaderboard_enable
    //FindElement = 120; //unused cc_leaderboard_enable
    DecodeUserCmdFromBuffer = 7; // CInput
    PerUserInput_tSize = 272; // CInput::DecodeUserCmdFromBuffer
    m_pCommands = 256; // CInput::DecodeUserCmdFromBuffer
    CUserCmdSize = 120; // CInput::DecodeUserCmdFromBuffer
    MULTIPLAYER_BACKUP = 150; // CInput::DecodeUserCmdFromBuffer
    IN_ActivateMouse = 16; // CHLClient
    g_Input = 7; // CHLClient::IN_ActivateMouse
    GetButtonBits = 2; // CInput
    JoyStickApplyMovement = 67; // CInput
    KeyDown = 442; // CInput::JoyStickApplyMovement
    KeyUp = 389; // CInput::JoyStickApplyMovement

    // vguimatsurface.so

    DrawSetColor = 14; // CMatSystemSurface
    DrawFilledRect = 16; // CMatSystemSurface
    DrawLine = 19; // CMatSystemSurface
    DrawSetTextFont = 23; // CMatSystemSurface
    DrawSetTextColor = 25; // CMatSystemSurface
    GetFontTall = 75; // CMatSystemSurface
    PaintTraverseEx = 120; // CMatSystemSurface
    StartDrawing = 248; // CMatSystemSurface::PaintTraverseEx
    FinishDrawing = 1513; // CMatSystemSurface::PaintTraverseEx
    DrawColoredText = 162; // CMatSystemSurface
    DrawTextLen = 165; // CMatSystemSurface
}
const char* Portal2CommunityEdition::Version()
{
    return "Portal 2: Community Edition (9256)";
}
const float Portal2CommunityEdition::Tickrate()
{
    return 60;
}
const char* Portal2CommunityEdition::ModDir()
{
    return "p2ce";
}
const char* Portal2CommunityEdition::GameDir()
{
    return "Portal 2 Community Edition";
}
