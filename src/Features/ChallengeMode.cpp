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
            { "gg_intro_wakeup", { 1184, 480, 268 }, { 90, 0, 0 } },
            { "gg_blue_only", { 2750, -768, -1002 }, { 0, 180, 0 } },
            { "gg_blue_only_2", { -448, -570, 158 }, { 0, 90, 0 } },
            { "gg_blue_only_3", { 1470, 384, 538 }, { 0, 180, 0 } },
            { "gg_blue_only_2_pt2", { 3458, -2560, 538 }, { 0, 180, 0 } },
            { "gg_a1_intro4", { 870, 320, 918 }, { 0, 0, 0 } },
            { "gg_blue_upplatform", { -704, 1090, 148 }, { 0, 270, 0 } },
            { "gg_red_only", { -3135, -2688, -102 }, { 0, 0, 0 } },
            { "gg_red_surf", { 2880, 64, 28 }, { 0, 90, 0 } },
            { "gg_all_intro", { -450, -192, 488 }, { 0, 90, 0 } },
            { "gg_all_rotating_wall", { 1600, 2625, 148 }, { 0, 270, 0 } },
            { "gg_all_fizzler", { 768, -400, 278 }, { 0, 90, 0 } },
            { "gg_all_intro_2", { 896, 130, 858 }, { 0, 90, 0 } },
            { "gg_all_puzzle2", { 384, 1790, 278 }, { 0, 270, 0 } },
            { "gg_a2_column_blocker", { -1100, -228, 18 }, { 0, 45, 0 } },
            { "gg_all2_puzzle1", { 384, -895, 18 }, { 0, 90, 0 } },
            { "gg_all_puzzle1", { 1152, -190, 408 }, { 0, 90, 0 } },
            { "gg_all2_escape", { 7200, 640, -437 }, { 0, 180, 0 } },
            { "gg_stage_reveal", { 704, 3135, 788 }, { 0, 270, 0 } },
            { "gg_stage_bridgebounce_2", { 64, -1535, 18 }, { 0, 90, 0 } },
            { "gg_stage_redfirst", { 1150, -1024, 408 }, { 0, 90, 0 } },
            { "gg_stage_laserrelay", { 1152, 1920, 598 }, { 0, 270, 0 } },
            { "gg_stage_beamscotty", { 384, 2175, 18 }, { 0, 270, 0 } },
            { "gg_stage_bridgebounce", { 895, -1024, 658 }, { 0, 180, 0 } },
            { "gg_stage_roofbounce", { -256, -510, 408 }, { 0, 90, 0 } },
            { "gg_stage_pickbounce", { -704, -385, 18 }, { 0, 90, 0 } },
            { "gg_stage_theend", { 64, 2500, 48 }, { 0, 270, 0 } },
            { "gg_tag_remix", { 0, 1536, 488 }, { 0, 0, 0 } },
            { "gg_trailer_map", { 1760, 1024, 278 }, { 0, 180, 0 } },
        };
        break;
    case SourceGame_ThinkingWithTimeMachine:
        ChallengeMode::customNodes = {
            // TODO
        };
        break;
    case SourceGame_PortalReloaded:
        ChallengeMode::customNodes = {
            // TODO
        };
        break;
    default:
        break;
    }

    console->DevMsg("Loaded %i nodes!\n", ChallengeMode::customNodes.size());
}

std::vector<ChallengeNodeData_t> ChallengeMode::customNodes;
