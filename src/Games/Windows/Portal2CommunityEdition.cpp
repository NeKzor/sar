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
    GetClientStateFunction = 10; //x CEngineClient::ClientCmd
    Cbuf_AddText = 46; //x CEngineClient::ClientCmd
    s_CommandBuffer = 121; //x Cbuf_AddText
    CCommandBufferSize = 8304; //x Cbuf_AddText
    //m_bWaitEnabled = 8265; //unused CCommandBuffer::AddText
    GetLocalPlayer = 45; //x CEngineClient
    GetViewAngles = 51; //x CEngineClient
    SetViewAngles = 52; //x CEngineClient
    GetLocalClient = 128; //x CEngineClient::SetViewAngles
    g_SplitScreenMgr = 12; // GetLocalClient
    viewangles = 134824; //x CEngineClient::SetViewAngles
    GetMaxClients = 53; //x CEngineClient
    GetGameDirectory = 12; //x CEngineClient
    GetActiveSplitScreenPlayerSlot = 5; //x GetLocalClient
    //GetSteamAPIContext = 178; //unused CEngineClient
    ExecuteClientCmd = 127; // CEngineClient
    StringToButtonCode = 42; // CInputSystem/unbind
    SleepUntilInput = 44; // CInputSystem
    GetRecordingTick = 12; //x CDemoRecorder
    net_time = 26; //x CDemoRecorder::GetRecordingTick
    SetSignonState = 0; //x CDemoRecorder
    StopRecording = 10; //x CDemoRecorder
    GetPlaybackTick = 3; // CDemoPlayer::SkipToTick
    //StartPlayback = 6; //unused CDemoPlayer
    IsPlayingBack = 4; // CDemoRecorder::SetSignonState
    m_szFileName = 16; // CDemoPlayer::WriteTimeDemoResults
    m_szDemoBaseName = 1364; // CDemoRecorder::StartupDemoFile
    m_nDemoNumber = 1628; // CDemoRecorder::StartupDemoFile
    m_bRecording = 1626; // CDemoRecorder::SetSignonState
    Paint = 14; // CEngineVGui
    ProcessTick = 13; //x CClientState/NETMsg_Tick
    //g_ClientGlobalVariables = 234; //unused CClientState::ProcessTick
    tickcount = 211; //x CClientState::ProcessTick
    interval_per_tick = 220; //x CClientState::ProcessTick
    //HostState_OnClientConnected = 735; //unused CClientState::SetSignonState
    hoststate = 1740; //x CClientState::SetSignonState/HostState_OnClientConnected
    Disconnect = 48; //x CClientState
    demoplayer = 107; //x CClientState::Disconnect
    demorecorder = 120; //x CClientState::Disconnect
    GetCurrentMap = 14; //x CEngineTool
    //sv = 9; //unused CEngineTool::GetCurrentMap
    m_szMapname = 59; //x CEngineTool::GetCurrentMap/m_szMapname
    AddListener = 3; // CGameEventManager
    RemoveListener = 5; // CGameEventManager
    //FireEventClientSide = 9; //unused CGameEventManager
    //FireEventIntern = 12; //unused CGameEventManager::FireEventClientSide
    //ConPrintEvent = 254; //unused CGameEventManager::FireEventIntern
    AutoCompletionFunc = 65; //x listdemo_CompletionFunc
    //Key_SetBinding = 118; //unused unbind
    IsRunningSimulation = 17; //x CEngineAPI
    eng = 7; //x CEngineAPI::IsRunningSimulation
    Frame = 5; //x CEngine
    m_bLoadGame = 996; //x CGameClient::ActivatePlayer/CBaseServer::m_szMapname
    //ScreenPosition = 11; //unused CIVDebugOverlay
    MAX_SPLITSCREEN_PLAYERS = 2; //x maxplayers
    //OnGameOverlayActivated = 152; //unused CSteam3Client
    ClientCommand = 68; // CVEngineServer
    OnDemoPlaybackStart = 55; //x CL_PlayDemo_f
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
    Think = 30; //x CServerGameDLL
    UTIL_PlayerByIndex = 64; //x CServerGameDLL::Think
    gpGlobals = 15; //x UTIL_PlayerByIndex
    player = 8; // CPortalGameMovement::PlayerMove
    m_MoveType = 366; // CPortalGameMovement::PlayerMove
    m_iClassName = 168; // CBaseEntity/CPointEntityFinder::FindByDistance
    S_m_vecAbsOrigin = 684; // CBaseEntity::SetAbsOrigin/ent_setpos
    S_m_angAbsRotation = 696; // CBaseEntity::SetAbsAngles/ent_setang
    m_iEFlags = 340; // CBaseEntity/EnableNoClip
    m_flGravity = 1224; // CBaseEntity/CGameMovement::StartGravity
    NUM_ENT_ENTRIES = 8192; // CServerTools::GetIServerEntity
    GetIServerEntity = 1; // CServerTools
    m_EntPtrArray = 47; // CServerTools::GetIServerEntity
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

    GetAllClasses = 8; //x CHLClient
    HudProcessInput = 10; //x CHLClient
    HudUpdate = 11; //x CHLClient
    C_m_vecAbsOrigin = 232; // C_BasePlayer::GetAbsOrigin/SplitScreenTeleport
    C_m_angAbsRotation = 268; // C_BasePlayer::GetAbsAngles/SplitScreenTeleport
    GetClientMode = 8; //x CHLClient::HudProcessInput
    g_pClientMode = 22; //x GetClientMode
    CreateMove = 24; // ClientModeShared
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
    KeyDown = 435; //x CInput::JoyStickApplyMovement
    KeyUp = 340; //x CInput::JoyStickApplyMovement

    // vguimatsurface.dll

    DrawSetColor = 15; // CMatSystemSurface
    DrawFilledRect = 16; // CMatSystemSurface
    DrawLine = 20; // CMatSystemSurface
    DrawSetTextFont = 24; // CMatSystemSurface
    DrawSetTextColor = 26; // CMatSystemSurface
    GetFontTall = 76; // CMatSystemSurface
    PaintTraverseEx = 120; // CMatSystemSurface
    StartDrawing = 170; // CMatSystemSurface::PaintTraverseEx
    FinishDrawing = 732; // CMatSystemSurface::PaintTraverseEx
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
