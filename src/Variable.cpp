#include "Variable.hpp"

#include <cstring>

#include "Modules/Tier1.hpp"

#include "Game.hpp"
#include "Offsets.hpp"
#include "SAR.hpp"

std::vector<Variable*>& Variable::GetList()
{
    static std::vector<Variable*> list;
    return list;
}

Variable::Variable()
    : ptr(nullptr)
    , originalFlags(0)
    , originalFnChangeCallback(nullptr)
    , version(SourceGame_Unknown)
    , isRegistered(false)
    , isReference(false)
{
}
Variable::~Variable()
{
    if (!this->isReference) {
        SAFE_DELETE(this->ptr)
    }
}
Variable::Variable(const char* name)
    : Variable()
{
    this->ptr = reinterpret_cast<ConVar*>(tier1->FindCommandBase(tier1->g_pCVar->ThisPtr(), name));
    this->isReference = true;
}
// Boolean or String
Variable::Variable(const char* name, const char* value, const char* helpstr, int flags)
    : Variable()
{
    if (flags != 0)
        Create(name, value, flags, helpstr, true, 0, true, 1);
    else
        Create(name, value, flags, helpstr);
}
// Float
Variable::Variable(const char* name, const char* value, float min, const char* helpstr, int flags)
    : Variable()
{
    Create(name, value, flags, helpstr, true, min);
}
// Float
Variable::Variable(const char* name, const char* value, float min, float max, const char* helpstr, int flags)
    : Variable()
{
    Create(name, value, flags, helpstr, true, min, true, max);
}
void Variable::Create(const char* name, const char* value, int flags, const char* helpstr, bool hasmin, float min, bool hasmax, float max)
{
    this->ptr = new ConVar(name, value, flags, helpstr, hasmin, min, hasmax, max);

    Variable::GetList().push_back(this);
}
ConVar* Variable::ThisPtr()
{
    return this->ptr;
}
bool Variable::GetBool()
{
    return !!GetInt();
}
int Variable::GetInt()
{
    return this->ptr->m_nValue ^ int(uintptr_t(this->ptr->m_pParent));
}
float Variable::GetFloat()
{
    union {
        float m_fValue;
        int m_nValue;
    } value = {};

    value.m_fValue = this->ptr->m_fValue;
    value.m_nValue ^= uintptr_t(this->ptr->m_pParent);

    return value.m_fValue;
}
const char* Variable::GetString()
{
    return this->ptr->m_pszString;
}
const int Variable::GetFlags()
{
    return this->ptr->m_nFlags;
}
void Variable::SetValue(const char* value)
{
#if _WIN64
    auto ptr = &this->ptr->ConVar_VTable;
    Memory::VMT<_InternalSetValue>(ptr, Offsets::InternalSetValue)(ptr, value);
#else
    Memory::VMT<_InternalSetValue>(this->ptr, Offsets::InternalSetValue)(this->ptr, value);
#endif
}
void Variable::SetValue(float value)
{
#if _WIN64
    auto ptr = &this->ptr->ConVar_VTable;
    Memory::VMT<_InternalSetFloatValue>(ptr, Offsets::InternalSetFloatValue)(ptr, value);
#else
    Memory::VMT<_InternalSetFloatValue>(this->ptr, Offsets::InternalSetFloatValue)(this->ptr, value);
#endif
}
void Variable::SetValue(int value)
{
#if _WIN64
    auto ptr = &this->ptr->ConVar_VTable;
    Memory::VMT<_InternalSetIntValue>(ptr, Offsets::InternalSetIntValue)(ptr, value);
#else
    Memory::VMT<_InternalSetIntValue>(this->ptr, Offsets::InternalSetIntValue)(this->ptr, value);
#endif
}
void Variable::SetFlags(int value)
{
    this->ptr->m_nFlags = value;
}
void Variable::AddFlag(int value)
{
    this->SetFlags(this->GetFlags() | value);
}
void Variable::RemoveFlag(int value)
{
    this->SetFlags(this->GetFlags() & ~(value));
}
void Variable::Unlock(bool asCheat)
{
    if (this->ptr) {
        this->originalFlags = this->ptr->m_nFlags;
        this->RemoveFlag(FCVAR_DEVELOPMENTONLY | FCVAR_HIDDEN);
        if (asCheat) {
            this->AddFlag(FCVAR_CHEAT);
        }

        if (this->isReference) {
            this->GetList().push_back(this);
        }
    }
}
void Variable::Lock()
{
    if (this->ptr) {
        this->SetFlags(this->originalFlags);
        this->SetValue(this->ptr->m_pszDefaultValue);
    }
}
void Variable::DisableChange()
{
    if (this->ptr) {
        this->originalSize = this->ptr->m_fnChangeCallback.m_Size;
        this->ptr->m_fnChangeCallback.m_Size = 0;
    }
}
void Variable::EnableChange()
{
    if (this->ptr) {
        this->ptr->m_fnChangeCallback.m_Size = this->originalSize;
    }
}
void Variable::UniqueFor(int version)
{
    this->version = version;
}
void Variable::Register()
{
    if (!this->isRegistered && !this->isReference && this->ptr) {
        this->isRegistered = true;

        this->ptr->ConCommandBase_VTable = tier1->ConVar_VTable;
        this->ptr->ConVar_VTable = tier1->ConVar_VTable2;

        ConVarChangeCallbackWrapper<FnChangeCallback_t> callback = { nullptr, nullptr };
        ConVarChangeCallbackWrapper<FnChangeCallback2_t> callback2 = { nullptr, nullptr };

        tier1->Create(this->ptr,
            this->ptr->m_pszName,
            this->ptr->m_pszDefaultValue,
            this->ptr->m_nFlags,
            this->ptr->m_pszHelpString,
            this->ptr->m_bHasMin,
            this->ptr->m_fMinVal,
            this->ptr->m_bHasMax,
            this->ptr->m_fMaxVal,
#ifdef __x86_64
            &callback,
            &callback2
#else
            nullptr
#endif
        );
    }
}
void Variable::Unregister()
{
    if (this->isRegistered && !this->isReference && this->ptr) {
        this->isRegistered = false;
        tier1->UnregisterConCommand(tier1->g_pCVar->ThisPtr(), this->ptr);
#ifdef _WIN32
        tier1->Dtor(this->ptr, 0);
#else
        tier1->Dtor(this->ptr);
#endif
        SAFE_DELETE(this->ptr)
    }
}
bool Variable::operator!()
{
    return this->ptr == nullptr;
}
int Variable::RegisterAll()
{
    auto result = 0;
    for (const auto& var : Variable::GetList()) {
        if (var->version != SourceGame_Unknown && !sar.game->Is(var->version)) {
            continue;
        }
        var->Register();
        ++result;
    }
    return result;
}
void Variable::UnregisterAll()
{
    for (const auto& var : Variable::GetList()) {
        var->Unregister();
    }
}
Variable* Variable::Find(const char* name)
{
    for (const auto& var : Variable::GetList()) {
        if (!std::strcmp(var->ThisPtr()->m_pszName, name)) {
            return var;
        }
    }
    return nullptr;
}
