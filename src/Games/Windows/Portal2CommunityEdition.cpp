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

    // engine.dll

    Dtor = 0; // ConVar
    InternalSetValue = 3; // ConVar:SetValue (23)
    InternalSetFloatValue = 2; // ConVar::SetValue (22)
    InternalSetIntValue = 1; // ConVar::SetValue (21)
    Create = 17; // ConVar
    GetScreenSize = 40; //x CEngineClient
    ClientCmd = 42; //x CEngineClient
    GetClientStateFunction = 14; // CEngineClient::ClientCmd
    Cbuf_AddText = 54; //x CEngineClient::ClientCmd
    s_CommandBuffer = 120; //x Cbuf_AddText
    CCommandBufferSize = 8304; //x Cbuf_AddText
    //m_bWaitEnabled = 8265; //unused CCommandBuffer::AddText
    GetLocalPlayer = 45; //x CEngineClient
    GetViewAngles = 51; //x CEngineClient
    SetViewAngles = 52; //x CEngineClient
    GetLocalClient = 80; //x CEngineClient::SetViewAngles
    g_SplitScreenMgr = 9; //x GetLocalClient
    viewangles = 134824; //x CEngineClient::SetViewAngles
    GetMaxClients = 53; //x CEngineClient
    GetGameDirectory = 12; //x CEngineClient
    GetActiveSplitScreenPlayerSlot = 5; // GetLocalClient
    //GetSteamAPIContext = 178; //unused CEngineClient
    ExecuteClientCmd = 127; //x CEngineClient
    StringToButtonCode = 42; // CInputSystem/unbind
    SleepUntilInput = 44; // CInputSystem
    GetRecordingTick = 2; // CDemoRecorder
    net_time = 57; // CDemoRecorder::GetRecordingTick
    SetSignonState = 0; // CDemoRecorder
    StopRecording = 9; // CDemoRecorder
    GetPlaybackTick = 3; // CDemoPlayer::SkipToTick
    //StartPlayback = 6; //unused CDemoPlayer
    IsPlayingBack = 5; // DemoRecorder::SetSignonState
    m_szFileName = 16; // CDemoPlayer::WriteTimeDemoResults
    m_szDemoBaseName = 1364; // CDemoRecorder::StartupDemoFile
    m_nDemoNumber = 1628; // CDemoRecorder::StartupDemoFile
    m_bRecording = 1626; // CDemoRecorder::SetSignonState
    Paint = 14; // CEngineVGui
    ProcessTick = 13; // CClientState
    //g_ClientGlobalVariables = 234; //unused CClientState::ProcessTick
    tickcount = 264; // CClientState::ProcessTick
    interval_per_tick = 242; // CClientState::ProcessTick
    //HostState_OnClientConnected = 735; //unused CClientState::SetSignonState
    hoststate = 1089; // CClientState::SetSignonState/HostState_OnClientConnected
    Disconnect = 48; // CClientState
    demoplayer = 97; // CClientState::Disconnect
    demorecorder = 110; // CClientState::Disconnect
    GetCurrentMap = 23; // CEngineTool
    //sv = 9; //unused CEngineTool::GetCurrentMap
    m_szLevelName = 49; // CEngineTool::GetCurrentMap/m_szMapname
    AddListener = 3; // CGameEventManager
    RemoveListener = 5; // CGameEventManager
    //FireEventClientSide = 9; //unused CGameEventManager
    //FireEventIntern = 12; //unused CGameEventManager::FireEventClientSide
    //ConPrintEvent = 254; //unused CGameEventManager::FireEventIntern
    AutoCompletionFunc = 78; // listdemo_CompletionFunc
    //Key_SetBinding = 118; //unused unbind
    IsRunningSimulation = 17; // CEngineAPI
    eng = 7; // CEngineAPI::IsRunningSimulation
    Frame = 5; // CEngine
    m_bLoadGame = -12; // CEngineTool::GetCurrentMap/m_szMapname
    //ScreenPosition = 11; //unused CIVDebugOverlay
    MAX_SPLITSCREEN_PLAYERS = 2; // maxplayers
    //OnGameOverlayActivated = 152; //unused CSteam3Client
    ClientCommand = 68; // CVEngineServer
    OnDemoPlaybackStart = 55; // CL_PlayDemo_f
    GetClientEntity = 3; // CClientEntityList/CL_TakeSnapshotAndSwap

    // libvstdlib.dll

    RegisterConCommand = 10; // CCVar
    UnregisterConCommand = 11; // CCvar
    FindCommandBase = 14; // CCvar
    InstallGlobalChangeCallback = 20; // CCvar
    RemoveGlobalChangeCallback = 21; // CCvar
    m_pConCommandList = 80; // CCvar
    IsCommand = 2; // ConCommandBase

    // vgui2.dll

    GetIScheme = 8; // CSchemeManager
    GetFont = 3; // CScheme

    // server.dll

    ProcessMovement = 1; // CGameMovement
    PlayerMove = 19; // CPortalGameMovement
    AirAccelerate = 30; // CPortalGameMovement
    AirMove = 31; // CPortalGameMovement
    AirMove_Offset1 = 16; // CGameMovement::~CPortalGameMovement
    AirMove_Offset2 = 9; // CGameMovement::~CGameMovement
    FinishGravity = 43; // CPortalGameMovement
    CheckJumpButton = 45; // CPortalGameMovement
    FullTossMove = 46; // CGameMovement
    mv = 16; // CPortalGameMovement::CheckJumpButton
    GameFrame = 4; // CServerGameDLL
    GetAllServerClasses = 10; // CServerGameDLL
    IsRestoring = 24; // CServerGameDLL
    Think = 30; // CServerGameDLL
    UTIL_PlayerByIndex = 52; // CServerGameDLL::Think
    gpGlobals = 7; // UTIL_PlayerByIndex
    player = 8; // CPortalGameMovement::PlayerMove
    m_MoveType = 366; // CPortalGameMovement::PlayerMove
    m_iClassName = 168; // CBaseEntity/CPointEntityFinder::FindByDistance
    S_m_vecAbsOrigin = 684; // CBaseEntity::SetAbsOrigin/ent_setpos
    S_m_angAbsRotation = 696; // CBaseEntity::SetAbsAngles/ent_setang
    m_iEFlags = 340; // CBaseEntity/EnableNoClip
    m_flGravity = 1224; // CBaseEntity/CGameMovement::StartGravity
    NUM_ENT_ENTRIES = 8192; // CServerTools::GetIServerEntity
    GetIServerEntity = 1; // CServerTools
    m_EntPtrArray = 48; // CServerTools::GetIServerEntity
    IsPlayer = 99; // ForEachPlayer/mp_disable_autokick
    m_pSurfaceData = 6032; // CGameMovement::CheckJumpButton
    jumpFactor = 84; // CGameMovement::CheckJumpButton
    IsAsleep = 2;
    IsCollisionEnabled = 6;
    IsGravityEnabled = 7;
    GetPosition = 48;
    GetVelocity = 52;
    SetPosition = 46;
    SetVelocity = 50;
    EnableGravity = 13;
    Wake = 24;
    Sleep = 25;
    m_pShadowStand = 4840; // CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow
    m_pShadowCrouch = 4848; // CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow

    // client.dll

    GetAllClasses = 8; // CHLClient
    HudProcessInput = 10; // CHLClient
    HudUpdate = 11; // CHLClient
    C_m_vecAbsOrigin = 232; // C_BasePlayer::GetAbsOrigin/SplitScreenTeleport
    C_m_angAbsRotation = 268; // C_BasePlayer::GetAbsAngles/SplitScreenTeleport
    GetClientMode = 10; // CHLClient::HudProcessInput
    g_pClientMode = 22; // GetClientMode
    CreateMove = 24; // ClientModeShared
    //GetName = 11; //unused CHud
    //GetHud = 104; //unused cc_leaderboard_enable
    //FindElement = 120; //unused cc_leaderboard_enable
    DecodeUserCmdFromBuffer = 7; // CInput
    PerUserInput_tSize = 272; // CInput::DecodeUserCmdFromBuffer
    m_pCommands = 256; // CInput::DecodeUserCmdFromBuffer
    CUserCmdSize = 120; // CInput::DecodeUserCmdFromBuffer
    MULTIPLAYER_BACKUP = 150; // CInput::DecodeUserCmdFromBuffer
    IN_ActivateMouse = 16; // CHLClient
    g_Input = 3; // CHLClient::IN_ActivateMouse
    GetButtonBits = 2; // CInput
    JoyStickApplyMovement = 67; // CInput
    KeyDown = 478; // CInput::JoyStickApplyMovement
    KeyUp = 459; // CInput::JoyStickApplyMovement

    // vguimatsurface.dll

    DrawSetColor = 15; // CMatSystemSurface
    DrawFilledRect = 16; // CMatSystemSurface
    DrawLine = 20; // CMatSystemSurface
    DrawSetTextFont = 24; // CMatSystemSurface
    DrawSetTextColor = 26; // CMatSystemSurface
    GetFontTall = 76; // CMatSystemSurface
    PaintTraverseEx = 120; // CMatSystemSurface
    StartDrawing = 186; // CMatSystemSurface::PaintTraverseEx
    FinishDrawing = 761; // CMatSystemSurface::PaintTraverseEx
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
