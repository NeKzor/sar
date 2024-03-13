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

    // engine.dll

    Dtor = 0; //x ConVar
    InternalSetValue = 3; //x ConVar:SetValue (23)
    InternalSetFloatValue = 2; //x ConVar::SetValue (22)
    InternalSetIntValue = 1; //x ConVar::SetValue (21)
    Create = 17; //x ConVar
    GetScreenSize = 42; //x CEngineClient
    ClientCmd = 44; //x CEngineClient
    GetClientStateFunction = 14; //x CEngineClient::ClientCmd
    Cbuf_AddText = 54; //x CEngineClient::ClientCmd
    s_CommandBuffer = 120; //x Cbuf_AddText
    CCommandBufferSize = 8304; //x Cbuf_AddText
    //m_bWaitEnabled = 8265; //unused CCommandBuffer::AddText
    GetLocalPlayer = 47; //x CEngineClient
    GetViewAngles = 53; //x CEngineClient
    SetViewAngles = 54; //x CEngineClient
    GetLocalClient = 80; //x CEngineClient::SetViewAngles
    g_SplitScreenMgr = 9; //x GetLocalClient
    viewangles = 134824; //x CEngineClient::SetViewAngles
    GetMaxClients = 55; //x CEngineClient
    GetGameDirectory = 14; //x CEngineClient
    GetActiveSplitScreenPlayerSlot = 5; //x GetLocalClient
    //GetSteamAPIContext = 178; //unused CEngineClient
    ExecuteClientCmd = 124; //x CEngineClient
    StringToButtonCode = 42; //x CInputSystem/unbind
    SleepUntilInput = 44; //x CInputSystem
    GetRecordingTick = 12; //x CDemoRecorder
    net_time = 27; //x CDemoRecorder::GetRecordingTick
    SetSignonState = 0; //x CDemoRecorder
    StopRecording = 10; //x CDemoRecorder
    GetPlaybackTick = 3; //x CDemoPlayer::SkipToTick
    //StartPlayback = 6; //unused CDemoPlayer
    IsPlayingBack = 4; //x CDemoRecorder::SetSignonState
    m_szFileName = 16; //x CDemoPlayer::WriteTimeDemoResults
    m_szDemoBaseName = 1364; //x CDemoRecorder::StartupDemoFile
    m_nDemoNumber = 1628; //x CDemoRecorder::StartupDemoFile
    m_bRecording = 1626; //x CDemoRecorder::SetSignonState
    Paint = 14; //x CEngineVGui
    ProcessTick = 13; //x CClientState
    //g_ClientGlobalVariables = 234; //unused CClientState::ProcessTick
    tickcount = 264; //x CClientState::ProcessTick
    interval_per_tick = 242; //x CClientState::ProcessTick
    //HostState_OnClientConnected = 735; //unused CClientState::SetSignonState
    hoststate = 1089; //x CClientState::SetSignonState/HostState_OnClientConnected
    Disconnect = 48; //x CClientState
    demoplayer = 97; //x CClientState::Disconnect
    demorecorder = 110; //x CClientState::Disconnect
    GetCurrentMap = 23; //x CEngineTool
    //sv = 9; //unused CEngineTool::GetCurrentMap
    m_szLevelName = 49; //x CEngineTool::GetCurrentMap/m_szMapname
    AddListener = 3; //x CGameEventManager
    RemoveListener = 5; //x CGameEventManager
    //FireEventClientSide = 9; //unused CGameEventManager
    //FireEventIntern = 12; //unused CGameEventManager::FireEventClientSide
    //ConPrintEvent = 254; //unused CGameEventManager::FireEventIntern
    AutoCompletionFunc = 78; //x listdemo_CompletionFunc
    //Key_SetBinding = 118; //unused unbind
    IsRunningSimulation = 17; //x CEngineAPI
    eng = 7; //x CEngineAPI::IsRunningSimulation
    Frame = 5; //x CEngine
    m_bLoadGame = -12; //x CEngineTool::GetCurrentMap/m_szMapname
    //ScreenPosition = 11; //unused CIVDebugOverlay
    MAX_SPLITSCREEN_PLAYERS = 2; //x maxplayers
    //OnGameOverlayActivated = 152; //unused CSteam3Client
    ClientCommand = 68; //x CVEngineServer
    OnDemoPlaybackStart = 55; //x CL_PlayDemo_f
    GetClientEntity = 3; //x CClientEntityList/CL_TakeSnapshotAndSwap

    // libvstdlib.dll

    RegisterConCommand = 10; //x CCVar
    UnregisterConCommand = 11; //x CCvar
    FindCommandBase = 14; //x CCvar
    InstallGlobalChangeCallback = 20; //x CCvar
    RemoveGlobalChangeCallback = 21; //x CCvar
    m_pConCommandList = 80; //x CCvar
    IsCommand = 2; //x ConCommandBase

    // vgui2.dll

    GetIScheme = 8; //x CSchemeManager
    GetFont = 3; //x CScheme

    // server.dll

    ProcessMovement = 1; //x CGameMovement
    PlayerMove = 19; //x CPortalGameMovement
    AirAccelerate = 30; //x CPortalGameMovement
    AirMove = 31; //x CPortalGameMovement
    AirMove_Offset1 = 16; //x CGameMovement::~CPortalGameMovement
    AirMove_Offset2 = 9; //x CGameMovement::~CGameMovement
    FinishGravity = 43; //x CPortalGameMovement
    CheckJumpButton = 45; //x CPortalGameMovement
    FullTossMove = 46; //x CGameMovement
    mv = 16; //x CPortalGameMovement::CheckJumpButton
    GameFrame = 4; //x CServerGameDLL
    GetAllServerClasses = 10; //x CServerGameDLL
    IsRestoring = 24; //x CServerGameDLL
    Think = 30; //x CServerGameDLL
    UTIL_PlayerByIndex = 52; //x CServerGameDLL::Think
    gpGlobals = 7; //x UTIL_PlayerByIndex
    player = 8; //x CPortalGameMovement::PlayerMove
    m_MoveType = 366; //x CPortalGameMovement::PlayerMove
    m_iClassName = 168; //x CBaseEntity/CPointEntityFinder::FindByDistance
    S_m_vecAbsOrigin = 684; //x CBaseEntity::SetAbsOrigin/ent_setpos
    S_m_angAbsRotation = 696; //x CBaseEntity::SetAbsAngles/ent_setang
    m_iEFlags = 340; //x CBaseEntity/EnableNoClip
    m_flGravity = 1224; //x CBaseEntity/CGameMovement::StartGravity
    NUM_ENT_ENTRIES = 8192; //x CServerTools::GetIServerEntity
    GetIServerEntity = 1; //x CServerTools
    m_EntPtrArray = 48; //x CServerTools::GetIServerEntity
    IsPlayer = 99; //x ForEachPlayer/mp_disable_autokick
    m_pSurfaceData = 6032; //x CGameMovement::CheckJumpButton
    jumpFactor = 84; //x CGameMovement::CheckJumpButton
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
    m_pShadowStand = 4848; //x CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow
    m_pShadowCrouch = 4856; //x CBasePlayer::InitVCollision/CBasePlayer::SetupVPhysicsShadow

    // client.dll

    GetAllClasses = 8; //x CHLClient
    HudProcessInput = 10; //x CHLClient
    HudUpdate = 11; //x CHLClient
    C_m_vecAbsOrigin = 232; //x C_BasePlayer::GetAbsOrigin/SplitScreenTeleport
    C_m_angAbsRotation = 268; //x C_BasePlayer::GetAbsAngles/SplitScreenTeleport
    GetClientMode = 10; //x CHLClient::HudProcessInput
    g_pClientMode = 22; //x GetClientMode
    CreateMove = 24; //x ClientModeShared
    //GetName = 11; //unused CHud
    //GetHud = 104; //unused cc_leaderboard_enable
    //FindElement = 120; //unused cc_leaderboard_enable
    DecodeUserCmdFromBuffer = 7; //x CInput
    PerUserInput_tSize = 272; //x CInput::DecodeUserCmdFromBuffer
    m_pCommands = 256; //x CInput::DecodeUserCmdFromBuffer
    CUserCmdSize = 120; //x CInput::DecodeUserCmdFromBuffer
    MULTIPLAYER_BACKUP = 150; //x CInput::DecodeUserCmdFromBuffer
    IN_ActivateMouse = 16; //x CHLClient
    g_Input = 3; //x CHLClient::IN_ActivateMouse
    GetButtonBits = 2; //x CInput
    JoyStickApplyMovement = 67; //x CInput
    KeyDown = 478; //x CInput::JoyStickApplyMovement
    KeyUp = 459; //x CInput::JoyStickApplyMovement

    // vguimatsurface.dll

    DrawSetColor = 15; //x CMatSystemSurface
    DrawFilledRect = 16; //x CMatSystemSurface
    DrawLine = 20; //x CMatSystemSurface
    DrawSetTextFont = 24; //x CMatSystemSurface
    DrawSetTextColor = 26; //x CMatSystemSurface
    GetFontTall = 76; //x CMatSystemSurface
    PaintTraverseEx = 120; //x CMatSystemSurface
    StartDrawing = 186; //x CMatSystemSurface::PaintTraverseEx
    FinishDrawing = 761; //x CMatSystemSurface::PaintTraverseEx
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
