// Copyright (c) 2024, NeKz
// SPDX-License-Identifier: MIT

#include "Interface2.hpp"

#include "Utils/Memory.hpp"
#include "Utils/SDK.hpp"

#include <cstring>
#include <expected>
#include <format>

Interface2::Interface2()
    : baseclass(nullptr)
    , vtable(nullptr)
    , vtableSize(0)
    , isHooked(false)
    , copy(nullptr)
{
}
Interface2::Interface2(void* baseclass, bool copyVtable, bool autoHook)
    : Interface2()
{
    this->baseclass = reinterpret_cast<uintptr_t**>(baseclass);
    this->vtable = *this->baseclass;

    while (this->vtable[this->vtableSize]) {
        ++this->vtableSize;
    }

    if (copyVtable) {
        this->CopyVtable();
        if (autoHook) {
            this->EnableHooks();
        }
    }
}
Interface2::~Interface2()
{
    this->DisableHooks();
    if (this->copy) {
        this->copy.reset();
    }
}
void Interface2::CopyVtable()
{
    if (!this->copy) {
        this->copy = std::make_unique<uintptr_t[]>(this->vtableSize + 1);
        std::memcpy(this->copy.get(), this->vtable - 1, sizeof(uintptr_t) + this->vtableSize * sizeof(uintptr_t));
    }
}
void Interface2::EnableHooks()
{
    if (!this->isHooked) {
        *this->baseclass = this->copy.get() + 1;
        this->isHooked = true;
    }
}
void Interface2::DisableHooks()
{
    if (this->isHooked) {
        *this->baseclass = this->vtable;
        this->isHooked = false;
    }
}
bool Interface2::Unhook(int index)
{
    if (index >= 0 && index < this->vtableSize) {
        this->copy[index + 1] = this->vtable[index];
        return true;
    }
    return false;
}
auto Interface2::Create(void* ptr, bool copyVtable, bool autoHook) -> Interface2*
{
    return (ptr) ? new Interface2(ptr, copyVtable, autoHook) : nullptr;
}
auto Interface2::Create(const char* filename, const char* interfaceSymbol, bool copyVtable, bool autoHook) -> std::expected<Interface2*, std::string_view>
{
    auto ptr = Interface2::GetPtr(filename, interfaceSymbol);
    if (!ptr) {
        return std::unexpected(ptr.error());
    }

    return new Interface2(ptr.value(), copyVtable, autoHook);
}
auto Interface2::Delete(Interface2* ptr) -> void 
{
    if (ptr) {
        delete ptr;
        ptr = nullptr;
    }
}
auto Interface2::GetPtr(const char* filename, const char* interfaceSymbol) -> std::expected<void*, std::string_view>
{
    auto handle = Memory::GetModuleHandleByName(filename);
    if (!handle) {
        return std::unexpected(std::format("Failed to open module {}!", filename));
    }

#if _WIN64
    auto CreateInterface = Memory::GetSymbolAddress<uintptr_t>(handle, "CreateInterface");
    Memory::CloseModuleHandle(handle);

    if (!CreateInterface) {
        return std::unexpected(std::format("Failed to find symbol CreateInterface for {}!", interfaceSymbol));
    }

    auto offset = *(uint8_t*)CreateInterface == 0x56 ? 10 : 3;
    auto s_pInterfaceRegsPtr = Memory::Read<InterfaceReg**>(CreateInterface + offset);
#else
    auto s_pInterfaceRegsPtr = Memory::GetSymbolAddress<InterfaceReg**>(handle, "s_pInterfaceRegs");
    Memory::CloseModuleHandle(handle);
#endif

    if (!s_pInterfaceRegsPtr) {
        return std::unexpected("Failed to find s_pInterfaceRegs!");
    }

    auto s_pInterfaceRegs = *s_pInterfaceRegsPtr;

    void* result = nullptr;
    for (auto& current = s_pInterfaceRegs; current; current = current->m_pNext) {
        if (std::strncmp(current->m_pName, interfaceSymbol, std::strlen(interfaceSymbol)) == 0) {
            result = current->m_CreateFn();
            break;
        }
    }

    if (!result) {
        return std::unexpected(std::format("Failed to find interface with symbol {} in {}", interfaceSymbol, filename));
    }

    return result;
}
