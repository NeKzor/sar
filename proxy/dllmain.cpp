// Copyright (c) 2024, NeKz
// SPDX-License-Identifier: MIT

#include "pch.h"
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

static auto init_proxy() -> void
{
    println("Proxying...");

    char current_dir[MAX_PATH] = {};

    GetModuleFileNameA(NULL, current_dir, MAX_PATH);
    PathRemoveFileSpecA(current_dir);

    char original_path[MAX_PATH] = {};
    lstrcpyA(original_path, current_dir);
    PathAppendA(original_path, "audio_minimp3_original.dll");

    auto audio_minimp3 = LoadLibraryA(original_path);

    println("Original audio_minimp3_original.dll {:x} in {}", uintptr_t(audio_minimp3), original_path);

    if (!audio_minimp3) {
        return;
    }

#define PROXY_IMPORT(name)                                                                  \
    auto name##_original_address = uintptr_t(GetProcAddress(audio_minimp3, LPCSTR(#name))); \
    if (name##_original_address) {                                                          \
        ORIGINAL(name) = decltype(ORIGINAL(name))(name##_original_address);                 \
        println("Proxied {} at {:x}", #name, name##_original_address);                      \
    } else {                                                                                \
        println("Unable to proxy " #name);                                                  \
    }

    PROXY_IMPORT(CreateInterface);

    auto sar = LoadLibraryA("sar.dll");
    println("Loaded sar.dll {:x}", uintptr_t(sar));

#undef PROXY_IMPORT
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
