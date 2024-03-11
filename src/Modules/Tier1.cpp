#include "Tier1.hpp"

#include "Game.hpp"
#include "Interface.hpp"
#include "Offsets.hpp"
#include "SAR.hpp"
#include "Utils.hpp"

bool Tier1::Init()
{
#if _WIN32
    this->g_pCVar = Interface::Create(this->Name(), "VEngineCvar0");
#else
    // Unfortunately s_pInterfaceRegs is not set in libvstdlib.so.
    // Thanks P2:CE!
    if (sar.game->Is(SourceGame_Portal2CommunityEdition)) {
        this->g_pCVar = Interface::Create(Memory::Absolute<void*>(this->Name(), 0x199B08));
    } else if (sar.game->Is(SourceGame_PortalRevolution)) {
        this->g_pCVar = Interface::Create(Memory::Absolute<void*>(this->Name(), 0x18DAE8));
    } else {
        this->g_pCVar = Interface::Create(this->Name(), "VEngineCvar0");
    }
#endif
    if (this->g_pCVar) {
        this->RegisterConCommand = this->g_pCVar->Original<_RegisterConCommand>(Offsets::RegisterConCommand);
        this->UnregisterConCommand = this->g_pCVar->Original<_UnregisterConCommand>(Offsets::UnregisterConCommand);
        this->FindCommandBase = this->g_pCVar->Original<_FindCommandBase>(Offsets::FindCommandBase);

        this->m_pConCommandList = reinterpret_cast<ConCommandBase*>((uintptr_t)this->g_pCVar->ThisPtr() + Offsets::m_pConCommandList);

        auto listdemo = reinterpret_cast<ConCommand*>(this->FindCommandBase(this->g_pCVar->ThisPtr(), "listdemo"));
        if (listdemo) {
            this->ConCommand_VTable = listdemo->ConCommandBase_VTable;

            if (listdemo->m_fnCompletionCallback) {
                auto callback = (uintptr_t)listdemo->m_fnCompletionCallback + Offsets::AutoCompletionFunc;
                this->AutoCompletionFunc = Memory::Read<_AutoCompletionFunc>(callback);
            }
        }

        auto sv_lan = reinterpret_cast<ConVar*>(this->FindCommandBase(this->g_pCVar->ThisPtr(), "sv_lan"));
        if (sv_lan) {
            this->ConVar_VTable = sv_lan->ConCommandBase_VTable;
            this->ConVar_VTable2 = sv_lan->ConVar_VTable;

            auto vtable =
#ifdef _WIN32
            sar.game->Is(SourceGame_HalfLife2Engine | SourceGame_StrataEngine)
                ? &this->ConVar_VTable
                : &this->ConVar_VTable2;
#else
                &this->ConVar_VTable;
#endif

            this->Dtor = Memory::VMT<_Dtor>(vtable, Offsets::Dtor);
            this->Create = Memory::VMT<_Create>(vtable, Offsets::Create);
        }

        if (sar.game->Is(SourceGame_Portal2 | SourceGame_ApertureTag)) {
            this->InstallGlobalChangeCallback = this->g_pCVar->Original<_InstallGlobalChangeCallback>(Offsets::InstallGlobalChangeCallback);
            this->RemoveGlobalChangeCallback = this->g_pCVar->Original<_RemoveGlobalChangeCallback>(Offsets::RemoveGlobalChangeCallback);
        }
    }

    return this->hasLoaded = this->g_pCVar
        && this->ConCommand_VTable
        && this->ConVar_VTable
        && this->ConVar_VTable2
        && this->AutoCompletionFunc;
}
void Tier1::Shutdown()
{
    Interface::Delete(this->g_pCVar);
}

Tier1* tier1;

CBaseAutoCompleteFileList::CBaseAutoCompleteFileList(const char* cmdname, const char* subdir, const char* extension)
{
    m_pszCommandName = cmdname;
    m_pszSubDir = subdir;
    m_pszExtension = extension;
}
int CBaseAutoCompleteFileList::AutoCompletionFunc(char const* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
    return tier1->AutoCompletionFunc(this, partial, commands);
}
