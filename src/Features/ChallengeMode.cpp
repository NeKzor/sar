#include "ChallengeMode.hpp"

#include <cstring>
#include <cmath>
#include <vector>

#include "Features/Stats/Stats.hpp"
#include "Features/EntityList.hpp"

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Server.hpp"

#include "Cheats.hpp"
#include "Game.hpp"

ChallengeMode* cm;

ChallengeMode::ChallengeMode()
{
    tier1->InstallGlobalChangeCallback(tier1->g_pCVar->ThisPtr(), ChallengeMode::OnCheatsChanged);
}
ChallengeMode::~ChallengeMode()
{
    tier1->RemoveGlobalChangeCallback(tier1->g_pCVar->ThisPtr(), ChallengeMode::OnCheatsChanged);
}
void ChallengeMode::OnCheatsChanged(IConVar* pVar, const char* pOldString, float flOldValue)
{
    if (!sar_challenge_mode.ThisPtr() || !sar_challenge_mode.GetBool()) {
        return;
    }

    auto name = pVar->GetName();

    if (!std::strcmp(name, "sv_bonus_challenge")) {
        pVar->SetValue(1);
    }
}
void ChallengeMode::ReloadNodes()
{
    for (auto& node : ChallengeMode::customNodes) {
        if (!std::strcmp(node.m_szMapName, engine->hoststate->m_levelName)) {
            server->Create("challenge_mode_end_node", node.m_vecNodeOrigin, node.m_vecNodeAngles, nullptr);
            return;
        }
    }
}
void ChallengeMode::LoadNodes(SourceGameVersion version)
{
    switch (version) {
    case SourceGame_PortalStoriesMel:
        ChallengeMode::customNodes = {
            { "st_a1_tramride", { -5885, 12050, 125 }, { 0, 270, 0 } },
            { "st_a1_mel_intro", { 600, 4720, 12335 }, { 0, 0, 0 } },
            { "st_a1_lift", { 190, 1195, -9030 }, { 270, 270, 0 } },
            { "st_a1_garden", { 3755, 1840, -610 }, { 0, 180, 0 } },
            { "st_a2_garden_de", { -6080, 4448, -610 }, { 0, 0, 0 } },
            { "st_a2_underbounce", { 1050, 784, 445 }, { 0, 0, 0 } },
            { "st_a2_once_upon", { 1640, -2070, 2475 }, { 0, 270, 0 } },
            { "st_a2_past_power", { -4068, 5010, -145 }, { 0, 270, 0 } },
            { "st_a2_ramp", { -630, -1664, 15 }, { 0, 0, 0 } },
            { "st_a2_firestorm", { 5240, 2065, 2495 }, { 0, 270, 0 } },
            { "st_a3_junkyard", { 0, 770, 60 }, { 0, 180, 0 } },
            { "st_a3_concepts", { 1632, 190, 65 }, { 0, 90, 0 } },
            { "st_a3_paint_fling", { -512, 2145, 510 }, { 0, 270, 0 } },
            { "st_a3_faith_plate", { 3472, 4675, 590 }, { 0, 90, 0 } },
            { "st_a3_transition", { 2032, 2285, 345 }, { 0, 270, 0 } },
            { "st_a4_overgrown", { 2336, 1373, 350 }, { 0, 270, 0 } },
            { "st_a4_tb_over_goo", { 320, -4990, 590 }, { 0, 90, 0 } },
            { "st_a4_two_of_a_kind", { -672, 3500, 705 }, { 0, 270, 0 } },
            { "st_a4_destroyed", { 96, 1435, 880 }, { 0, 270, 0 } },
            { "st_a4_factory", { 10560, -1968, 580 }, { 0, 180, 0 } },
            { "st_a4_core_access", { 0, 1995, 845 }, { 0, 270, 0 } },
            { "st_a4_finale", { 16, 570, 380 }, { 0, 270, 0 } },
            { "sp_a1_tramride", { -5885, 12050, 125 }, { 0, 270, 0 } },
            { "sp_a1_mel_intro", { 600, 4720, 12335 }, { 0, 0, 0 } },
            { "sp_a1_lift", { 190, 1195, -9030 }, { 270, 270, 0 } },
            { "sp_a1_garden", { 3755, 1840, -610 }, { 0, 180, 0 } },
            { "sp_a2_garden_de", { -6080, 4448, -610 }, { 0, 0, 0 } },
            { "sp_a2_underbounce", { 1050, 784, 445 }, { 0, 0, 0 } },
            { "sp_a2_once_upon", { 1640, -2070, 2475 }, { 0, 270, 0 } },
            { "sp_a2_past_power", { -4068, 5010, -145 }, { 0, 270, 0 } },
            { "sp_a2_ramp", { -630, -1664, 15 }, { 0, 0, 0 } },
            { "sp_a2_firestorm", { 5240, 2065, 2495 }, { 0, 270, 0 } },
            { "sp_a3_junkyard", { 0, 770, 60 }, { 0, 180, 0 } },
            { "sp_a3_concepts", { 1632, 190, 65 }, { 0, 90, 0 } },
            { "sp_a3_paint_fling", { -512, 2145, 510 }, { 0, 270, 0 } },
            { "sp_a3_faith_plate", { 3472, 4675, 590 }, { 0, 90, 0 } },
            { "sp_a3_transition", { 2032, 2285, 345 }, { 0, 270, 0 } },
            { "sp_a4_overgrown", { 2336, 1373, 350 }, { 0, 270, 0 } },
            { "sp_a4_tb_over_goo", { 320, -4990, 590 }, { 0, 90, 0 } },
            { "sp_a4_two_of_a_kind", { -672, 3500, 705 }, { 0, 270, 0 } },
            { "sp_a4_destroyed", { 96, 1435, 880 }, { 0, 270, 0 } },
            { "sp_a4_factory", { 10560, -1968, 580 }, { 0, 180, 0 } },
            { "sp_a4_core_access", { 0, 1995, 845 }, { 0, 270, 0 } },
            { "sp_a4_finale", { 16, 570, 380 }, { 0, 270, 0 } },
        };
        break;
    case SourceGame_ApertureTag:
        ChallengeMode::customNodes = {
            { "gg_intro_wakeup", { 1184, 480, 300 }, { 90, 0, 0 } },
            { "gg_blue_only", { 2750, -768, -970 }, { 0, 180, 0 } },
            { "gg_blue_only_2", { -448, -570, 190 }, { 0, 90, 0 } },
            { "gg_blue_only_3", { 1470, 384, 570 }, { 0, 180, 0 } },
            { "gg_blue_only_2_pt2", { 3458, -2560, 570 }, { 0, 180, 0 } },
            { "gg_a1_intro4", { 870, 320, 950 }, { 0, 0, 0 } },
            { "gg_blue_upplatform", { -704, 1090, 180 }, { 0, 270, 0 } },
            { "gg_red_only", { -3135, -2688, -70 }, { 0, 0, 0 } },
            { "gg_red_surf", { 2880, 64, 60 }, { 0, 90, 0 } },
            { "gg_all_intro", { -450, -192, 520 }, { 0, 90, 0 } },
            { "gg_all_rotating_wall", { 1600, 2625, 180 }, { 0, 270, 0 } },
            { "gg_all_fizzler", { 768, -400, 310 }, { 0, 90, 0 } },
            { "gg_all_intro_2", { 896, 130, 890 }, { 0, 90, 0 } },
            { "gg_all_puzzle2", { 384, 1790, 310 }, { 0, 270, 0 } },
            { "gg_a2_column_blocker", { -1100, -228, 50 }, { 0, 45, 0 } },
            { "gg_all2_puzzle1", { 384, -895, 50 }, { 0, 90, 0 } },
            { "gg_all_puzzle1", { 1152, -190, 440 }, { 0, 90, 0 } },
            { "gg_all2_escape", { 7200, 640, -405 }, { 0, 180, 0 } },
            { "gg_stage_reveal", { 704, 3135, 820 }, { 0, 270, 0 } },
            { "gg_stage_bridgebounce_2", { 64, -1535, 50 }, { 0, 90, 0 } },
            { "gg_stage_redfirst", { 1150, -1024, 440 }, { 0, 90, 0 } },
            { "gg_stage_laserrelay", { 1152, 1920, 630 }, { 0, 270, 0 } },
            { "gg_stage_beamscotty", { 384, 2175, 50 }, { 0, 270, 0 } },
            { "gg_stage_bridgebounce", { 895, -1024, 690 }, { 0, 180, 0 } },
            { "gg_stage_roofbounce", { -256, -510, 440 }, { 0, 90, 0 } },
            { "gg_stage_pickbounce", { -704, -385, 50 }, { 0, 90, 0 } },
            { "gg_stage_theend", { 64, 2500, 80 }, { 0, 270, 0 } },
            { "gg_tag_remix", { 0, 1536, 520 }, { 0, 0, 0 } },
            { "gg_trailer_map", { 1760, 1024, 310 }, { 0, 180, 0 } },
        };
        break;
    case SourceGame_ThinkingWithTimeMachine:
        ChallengeMode::customNodes = {
            { "tm_intro_01", { 1990, -545, 310 }, { 0, 90, 0 } },
            { "tm_training_01b", { -190, -385, 1050 }, { 0, 90, 0 } },
            { "tm_map_01b", { 0, 840, 310 }, { 0, -90, 0 } },
            { "tm_scene_map-update2", { 3165, 890, -490 }, { 0, 90, 0 } },
            { "tm_map_02b", { -255, 295, 55 }, { 0, -90, 0 } },
            { "tm_map_03b", { -320, -165, 54 }, { 0, 90, 0 } },
            { "tm_map_04a", { -1000, 0, 120 }, { 0, 0, 0 } },
            { "tm_map_05a-update2", { -40, 65, 55 }, { 0, 0, 0 } },
            { "tm_map_06a", { 0, 1065, 695 }, { 0, -90, 0 } },
            { "tm_map_07", { 65, 105, 180 }, { 0, -90, 0 } },
            { "tm_map_08", { -40, 320, 180 }, { 0, 0, 0 } },
            { "tm_map_final", { 0, 250, -125 }, { -90, -120, 0 } },
        };
        break;
    case SourceGame_PortalReloaded:
        ChallengeMode::customNodes = {
            { "sp_a1_pr_map_002", { 1630, 415, 0 }, { 0, -90, 0 } },
            { "sp_a1_pr_map_003", { 1310, -2590, -40 }, { 0, -90, 0 } },
            { "sp_a1_pr_map_004", { -2270, -4385, -425 }, { 0, -90, 0 } },
            { "sp_a1_pr_map_005", { 3550, 290, -295 }, { 0, 90, 0 } },
            { "sp_a1_pr_map_006", { -4640, -415, --5 }, { 0, -90, 0 } },
            { "sp_a1_pr_map_007", { -3615, 30, -40 }, { 0, 90, 0 } },
            { "sp_a1_pr_map_008", { 3615, 255, 35 }, { 0, 90, 0 } },
            { "sp_a1_pr_map_009", { -1570, -5665, 2 }, { 0, -180, 0 } },
            { "sp_a1_pr_map_010", { -3585, -1535, 35 }, { 0, -90, 0 } },
            { "sp_a1_pr_map_011", { 1570, -1825, 35 }, { 0, -90, 0 } },
            { "sp_a1_pr_map_012", { 6910, 6655, 3895 }, { 0, 90, 0 } },
            { "mp_coop_pr_cubes", { -1280, -835, 1945 }, { 0, -90, 0 } },
            { "mp_coop_pr_portals", { -1150, -1155, 185 }, { 0, -90, 0 } },
            { "mp_coop_pr_teamwork", { -1790, -3545, 315 }, { 0, -90, 0 } },
            { "mp_coop_pr_fling", { -2105, -1730, 440 }, { 0, 0, 0 } },
            { "mp_coop_pr_loop", { -1250, -1155, 250 }, { 0, -90, 0 } },
            { "mp_coop_pr_catapult", { -1310, -2805, 380 }, { 0, 90, 0 } },
            { "mp_coop_pr_laser", { -3680, -520, 185 }, { 0, -90, 0 } },
            { "mp_coop_pr_bridge", { -1985, -905, 60 }, { 0, -90, 0 } },
            { "mp_coop_pr_tbeam", { -650, -1630, 540 }, { 0, -180, 0 } },
            { "mp_coop_pr_bts", { -1695, -1840, 55 }, { 0, 0, 0 } },
        };
        break;
    default:
        break;
    }

    console->DevMsg("Loaded %i nodes!\n", ChallengeMode::customNodes.size());
}

std::vector<ChallengeNodeData_t> ChallengeMode::customNodes;
