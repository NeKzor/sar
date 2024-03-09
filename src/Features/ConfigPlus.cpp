#include "ConfigPlus.hpp"

#include "Modules/Engine.hpp"

#include <vector>

#define MK_SAR_ON(name, when, immediately)                                                                                                 \
    static std::vector<std::string> _g_execs_##name;                                                                                       \
    CON_COMMAND_F(sar_on_##name, "sar_on_" #name " <command> [args]... - registers a command to be run " when "\n", FCVAR_DONTRECORD)      \
    {                                                                                                                                      \
        if (args.ArgC() < 2) {                                                                                                             \
            return console->Print(sar_on_##name.ThisPtr()->m_pszHelpString);                                                               \
        }                                                                                                                                  \
        const char* cmd = Utils::ArgContinuation(args, 1);                                                                                 \
        _g_execs_##name.push_back(std::string(cmd));                                                                                       \
    }                                                                                                                                      \
    CON_COMMAND_F(sar_on_##name##_clear, "sar_on_" #name "_clear - clears commands registered on event \"" #name "\"\n", FCVAR_DONTRECORD) \
    {                                                                                                                                      \
        console->Print("Cleared %d commands from event \"" #name "\"\n", _g_execs_##name.size());                                          \
        _g_execs_##name.clear();                                                                                                           \
    }                                                                                                                                      \
    CON_COMMAND_F(sar_on_##name##_list, "sar_on_" #name "_list - lists commands registered on event \"" #name "\"\n", FCVAR_DONTRECORD)    \
    {                                                                                                                                      \
        console->Print("%d commands on event \"" #name "\"\n", _g_execs_##name.size());                                                    \
        for (auto cmd : _g_execs_##name) {                                                                                                 \
            console->Print("%s\n", cmd.c_str());                                                                                           \
        }                                                                                                                                  \
    }                                                                                                                                      \
    static void _runExecs_##name()                                                                                                         \
    {                                                                                                                                      \
        for (auto cmd : _g_execs_##name) {                                                                                                 \
            engine->ExecuteCommand(cmd.c_str(), immediately);                                                                              \
        }                                                                                                                                  \
    }

#define RUN_EXECS(x) _runExecs_##x()

MK_SAR_ON(load, "on session start", true)
MK_SAR_ON(session_end, "on session end", true)

namespace ConfigPlus {
    auto ExecSessionStart() -> void {
        RUN_EXECS(load);
    }

    auto ExecSessionEnd() -> void {
        RUN_EXECS(session_end);
    }
};
