#include "InputSystem.hpp"

#include "Cheats.hpp"
#include "Command.hpp"
#include "Interface.hpp"
#include "Module.hpp"
#include "Offsets.hpp"
#include "SAR.hpp"
#include "Utils.hpp"

REDECL(InputSystem::SleepUntilInput);

int InputSystem::GetButton(const char* pString)
{
    return this->StringToButtonCode(this->g_InputSystem->ThisPtr(), pString);
}

// CInputSystem::SleepUntilInput
DETOUR(InputSystem::SleepUntilInput, int nMaxSleepTimeMS)
{
    if (sar_disable_no_focus_sleep.GetBool()) {
        nMaxSleepTimeMS = 0;
    }

    return InputSystem::SleepUntilInput(thisptr, nMaxSleepTimeMS);
}

bool InputSystem::Init()
{
    this->g_InputSystem = Interface::Create(this->Name(), "InputSystemVersion0");
    if (this->g_InputSystem) {
        this->StringToButtonCode = this->g_InputSystem->Original<_StringToButtonCode>(Offsets::StringToButtonCode);

        if (sar.game->Is(SourceGame_Portal2Engine | SourceGame_StrataEngine)) {
            this->g_InputSystem->Hook(InputSystem::SleepUntilInput_Hook, InputSystem::SleepUntilInput, Offsets::SleepUntilInput);
        }
    }

#ifndef _WIN32
    auto unbind = Command("unbind");
    if (!!unbind) {
        auto cc_unbind_callback = (uintptr_t)unbind.ThisPtr()->m_pCommandCallback;
        this->KeySetBinding = Memory::Read<_KeySetBinding>(cc_unbind_callback + Offsets::Key_SetBinding);
    }
#endif

    return this->hasLoaded = !!this->g_InputSystem;
}
void InputSystem::Shutdown()
{
    Interface::Delete(this->g_InputSystem);
}

InputSystem* inputSystem;
