#include "PortalRevolution.hpp"

#include "Game.hpp"
#include "Offsets.hpp"

PortalRevolution::PortalRevolution()
{
    this->version = SourceGame_PortalRevolution;
}
void PortalRevolution::LoadOffsets()
{
    using namespace Offsets;

    // engine.so

    Dtor = 0; //x ConVar
    InternalSetValue = 17; //x ConVar
    InternalSetFloatValue = 18; //x ConVar
    InternalSetIntValue = 19; //x ConVar
    Create = 24; //x ConVar
    GetScreenSize = 42; //x CEngineClient
    ClientCmd = 44; //x CEngineClient
    GetClientStateFunction = 11; //x CEngineClient::ClientCmd
    Cbuf_AddText = 42; //x CEngineClient::ClientCmd
    s_CommandBuffer = 95; //x Cbuf_AddText
    CCommandBufferSize = 8304; //x Cbuf_AddText
    //m_bWaitEnabled = 8265; //unused CCommandBuffer::AddText
    GetLocalPlayer = 47; //x CEngineClient
    GetViewAngles = 53; //x CEngineClient
    SetViewAngles = 54; //x CEngineClient
    GetLocalClient = 96; //x CEngineClient::SetViewAngles
    g_SplitScreenMgr = 12; //x GetLocalClient
    viewangles = 134824; //x CEngineClient::SetViewAngles
    GetMaxClients = 55; //x CEngineClient
    GetGameDirectory = 14; //x CEngineClient
    GetActiveSplitScreenPlayerSlot = 5; //x GetLocalClient
    //GetSteamAPIContext = 178; //unused CEngineClient
    ExecuteClientCmd = 124; // TODO: CEngineClient
    StringToButtonCode = 42; //x CInputSystem/unbind
    SleepUntilInput = 44; //x CInputSystem
    GetRecordingTick = 2; //x CDemoRecorder
    net_time = 26; //x CDemoRecorder::GetRecordingTick
    SetSignonState = 0; //x CDemoRecorder
    StopRecording = 9; //x CDemoRecorder
    GetPlaybackTick = 4; //x CDemoPlayer::SkipToTick
    //StartPlayback = 6; //unused CDemoPlayer
    IsPlayingBack = 5; //x CDemoRecorder::SetSignonState
    m_szFileName = 16; //x CDemoPlayer::WriteTimeDemoResults
    m_szDemoBaseName = 1364; //x CDemoRecorder::StartupDemoFile
    m_nDemoNumber = 1628; //x CDemoRecorder::StartupDemoFile
    m_bRecording = 1626; //x CDemoRecorder::SetSignonState
    Paint = 15; //x CEngineVGui
    ProcessTick = 15; //x CClientState
    g_ClientGlobalVariables = 234; //x CClientState::ProcessTick
    tickcount = 36; //x CClientState::ProcessTick
    interval_per_tick = 40; //x CClientState::ProcessTick
    //HostState_OnClientConnected = 735; //unused CClientState::SetSignonState
    hoststate = 1702; //x CClientState::SetSignonState/HostState_OnClientConnected
    Disconnect = 50; //x CClientState
    demoplayer = 68; //x CClientState::Disconnect
    demorecorder = 87; //x CClientState::Disconnect
    GetCurrentMap = 23; //x CEngineTool
    sv = 9; //x CEngineTool::GetCurrentMap
    m_szMapname = 20; //x CEngineTool::GetCurrentMap/m_szMapname
    AddListener = 4; //x CGameEventManager
    RemoveListener = 6; //x CGameEventManager
    FireEventClientSide = 10; //x CGameEventManager
    FireEventIntern = 13; //x CGameEventManager::FireEventClientSide
    //ConPrintEvent = 254; //unused CGameEventManager::FireEventIntern
    AutoCompletionFunc = 42; //x listdemo_CompletionFunc
    Key_SetBinding = 118; //x unbind
    IsRunningSimulation = 17; //x CEngineAPI
    eng = 7; //x CEngineAPI::IsRunningSimulation
    Frame = 6; //x CEngine
    m_bLoadGame = 1016; //x CGameClient::ActivatePlayer
    //ScreenPosition = 11; //unused CIVDebugOverlay
    MAX_SPLITSCREEN_PLAYERS = 2; //x maxplayers
    //OnGameOverlayActivated = 152; //unused CSteam3Client
    ClientCommand = 68; //x CVEngineServer
    OnDemoPlaybackStart = 55; //x CL_PlayDemo_f
    GetClientEntity = 3; //x CClientEntityList/CL_TakeSnapshotAndSwap

    // libvstdlib.so

    RegisterConCommand = 10; //x CCVar
    UnregisterConCommand = 11; //x CCvar
    FindCommandBase = 14; //x CCvar
    InstallGlobalChangeCallback = 20; //x CCvar
    RemoveGlobalChangeCallback = 21; //x CCvar
    m_pConCommandList = 80; //x CCvar
    IsCommand = 2; //x ConCommandBase

    // vgui2.so

    GetIScheme = 9; //x CSchemeManager
    GetFont = 4; //x CScheme

    // server.so

    ProcessMovement = 2; //x CGameMovement
    PlayerMove = 18; //x CPortalGameMovement
    AirAccelerate = 29; //x CPortalGameMovement
    AirMove = 30; //x CPortalGameMovement
    AirMove_Offset1 = 10; //x CGameMovement::~CPortalGameMovement
    //AirMove_Offset2 = 12; //unused CGameMovement::~CGameMovement
    FinishGravity = 44; //x CPortalGameMovement
    CheckJumpButton = 46; //x CPortalGameMovement
    FullTossMove = 47; //x CGameMovement
    mv = 16; //x CPortalGameMovement::CheckJumpButton
    GameFrame = 4; //x CServerGameDLL
    GetAllServerClasses = 10; //x CServerGameDLL
    IsRestoring = 24; //x CServerGameDLL
    Think = 30; //x CServerGameDLL
    UTIL_PlayerByIndex = 47; //x CServerGameDLL::Think
    gpGlobals = 19; //x UTIL_PlayerByIndex
    player = 8; //x CPortalGameMovement::PlayerMove
    m_MoveType = 382; //x CPortalGameMovement::PlayerMove
    m_iClassName = 184; //x CBaseEntity/CPointEntityFinder::FindByDistance
    S_m_vecAbsOrigin = 700; //x CBaseEntity::SetAbsOrigin/ent_setpos
    S_m_angAbsRotation = 712; //x CBaseEntity::SetAbsAngles/ent_setang
    m_iEFlags = 356; //x CBaseEntity/EnableNoClip
    m_flGravity = 1264; //x CBaseEntity/CGameMovement::StartGravity
    NUM_ENT_ENTRIES = 8192; //x CServerTools::GetIServerEntity
    GetIServerEntity = 2; //x CServerTools
    m_EntPtrArray = 55; //x CServerTools::GetIServerEntity
    IsPlayer = 100; //x ForEachPlayer
    m_pSurfaceData = 6072; //x CGameMovement::CheckJumpButton
    jumpFactor = 84; //x CGameMovement::CheckJumpButton
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
    m_pShadowStand = 4888; //x CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow
    m_pShadowCrouch = 4896; //x CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow

    // client.so

    GetAllClasses = 8; //x CHLClient
    HudProcessInput = 10; //x CHLClient
    HudUpdate = 11; //x CHLClient
    C_m_vecAbsOrigin = 216; //x C_BasePlayer::GetAbsOrigin/SplitScreenTeleport
    C_m_angAbsRotation = 252; //x C_BasePlayer::GetAbsAngles/SplitScreenTeleport
    GetClientMode = 9; //x CHLClient::HudProcessInput
    g_pClientMode = 25; //x GetClientMode
    CreateMove = 25; //x ClientModeShared
    //GetName = 11; //unused CHud
    //GetHud = 104; //unused cc_leaderboard_enable
    //FindElement = 120; //unused cc_leaderboard_enable
    DecodeUserCmdFromBuffer = 7; //x CInput
    PerUserInput_tSize = 272; //x CInput::DecodeUserCmdFromBuffer
    m_pCommands = 256; //x CInput::DecodeUserCmdFromBuffer
    CUserCmdSize = 120; //x CInput::DecodeUserCmdFromBuffer
    MULTIPLAYER_BACKUP = 150; //x CInput::DecodeUserCmdFromBuffer
    IN_ActivateMouse = 16; //x CHLClient
    g_Input = 7; //x CHLClient::IN_ActivateMouse
    GetButtonBits = 2; //x CInput
    JoyStickApplyMovement = 67; //x CInput
    KeyDown = 442; //x CInput::JoyStickApplyMovement
    KeyUp = 389; //x CInput::JoyStickApplyMovement

    // vguimatsurface.so

    DrawSetColor = 14; //x CMatSystemSurface
    DrawFilledRect = 16; //x CMatSystemSurface
    DrawLine = 19; //x CMatSystemSurface
    DrawSetTextFont = 23; //x CMatSystemSurface
    DrawSetTextColor = 25; //x CMatSystemSurface
    GetFontTall = 75; //x CMatSystemSurface
    PaintTraverseEx = 120; //x CMatSystemSurface
    StartDrawing = 248; //x CMatSystemSurface::PaintTraverseEx
    FinishDrawing = 1513; //x CMatSystemSurface::PaintTraverseEx
    DrawColoredText = 162; //x CMatSystemSurface
    DrawTextLen = 165; //x CMatSystemSurface
}
const char* PortalRevolution::Version()
{
    return "Portal: Revolution (9197)";
}
const char* PortalRevolution::ModDir()
{
    return "revolution";
}
const char* PortalRevolution::GameDir()
{
    return "Portal Revolution";
}
