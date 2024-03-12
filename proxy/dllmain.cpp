// Copyright (c) 2024, NeKz
// SPDX-License-Identifier: MIT

#include "proxy.hpp"

#include "Interface2.hpp"
#include "Utils.hpp"

#include <Shlwapi.h>
#include <intrin.h>
#include <map>

#define EXPORT __declspec(dllexport)

#define ORIGINAL(name) name##_original

#define DECL_DETOUR_API(type, cc, name, ...)  \
    using _##name = type(cc*)(##__VA_ARGS__); \
    _##name ORIGINAL(name);                   \
    EXPORT type cc name(##__VA_ARGS__)

#define DETOUR_API(type, cc, name, ...) EXPORT type cc name(##__VA_ARGS__)

DECL_DETOUR_API(__int64, __fastcall, CreateInterface, __int64 a1, void* a2);

DETOUR_API(__int64, __fastcall, CreateInterface, __int64 a1, void* a2)
{
    auto function = ORIGINAL(CreateInterface);
    auto result = function(a1, a2);

    println(
        "[0x{:06x}] [0x{:06x}] {}({:x}, {:x}) -> {:x} | {}",
        uintptr_t(function),
        uintptr_t(_ReturnAddress()),
        "CreateInterface",
        uintptr_t(a1),
        uintptr_t(a2),
        uintptr_t(result),
        uintptr_t(result));

    return result;
}

Interface2* g_pMaterialSystem = nullptr;

#define FinishRenderTargetAllocation_Vtable_Offfset 130

// CMaterialSystem::FinishRenderTargetAllocation
DECL_DETOUR(FinishRenderTargetAllocation);
DETOUR(FinishRenderTargetAllocation)
{
    println("Hello from FinishRenderTargetAllocation!");

    Interface2::Delete(g_pMaterialSystem);

    println("Loading sar.dll");

    auto sar = LoadLibraryA("sar.dll");
    println("Loaded sar.dll {:x}", uintptr_t(sar));

    return FinishRenderTargetAllocation(thisptr);
}

static auto init_sar_hook() -> void
{
    //wait_for_debugger();

    auto materialSystem = Interface2::Create(MODULE("materialsystem"), "VMaterialSystem0");

    g_pMaterialSystem = materialSystem.value();
    println("Interface g_pMaterialSystem {:x}", uintptr_t(g_pMaterialSystem->ThisPtr()));

    if (!materialSystem) {
        return println("{}", materialSystem.error());
    }

    g_pMaterialSystem->Hook(
        FinishRenderTargetAllocation_Hook,
        FinishRenderTargetAllocation,
        FinishRenderTargetAllocation_Vtable_Offfset);
}

static auto init_proxy() -> void
{
    println("Proxying...");

    char current_dir[MAX_PATH] = {};

    GetModuleFileNameA(NULL, current_dir, MAX_PATH);
    PathRemoveFileSpecA(current_dir);

    char original_path[MAX_PATH] = {};
    lstrcpyA(original_path, current_dir);
    PathAppendA(original_path, MODULE_ORIGINAL);

    auto module_to_proxy = LoadLibraryA(original_path);

    println("Original " MODULE_ORIGINAL " {:x} in {}", uintptr_t(module_to_proxy), original_path);

    if (!module_to_proxy) {
        return;
    }

#define PROXY_IMPORT(name)                                                                    \
    auto name##_original_address = uintptr_t(GetProcAddress(module_to_proxy, LPCSTR(#name))); \
    if (name##_original_address) {                                                            \
        ORIGINAL(name) = decltype(ORIGINAL(name))(name##_original_address);                   \
        println("Proxied {} at {:x}", #name, name##_original_address);                        \
    } else {                                                                                  \
        println("Unable to proxy " #name);                                                    \
    }

    PROXY_IMPORT(CreateInterface);

#undef PROXY_IMPORT

    init_sar_hook();
}

static auto once = false;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        if (!once) {
            once = true;
            init_proxy();
        }
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
