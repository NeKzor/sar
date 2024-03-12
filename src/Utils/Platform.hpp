#pragma once

#define _GAME_PATH(x) #x

#ifdef _WIN32
#define IS_WINDOWS true
#define IS_LINUX false
#define MODULE_EXTENSION ".dll"
// clang-format off
#define GAME_PATH(x) _GAME_PATH(Games/Windows/##x.hpp)
// clang-format on
#if __x86_64
#define __rescall __fastcall

#define DECL_DETOUR(name, ...)                                  \
    using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                        \
    static int __fastcall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DECL_DETOUR_T(type, name, ...)                           \
    using _##name = type(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                         \
    static type __fastcall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DECL_DETOUR_B(name, ...)                                \
    using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                        \
    static _##name name##Base;                                  \
    static int __fastcall name##_Hook(void* thisptr, ##__VA_ARGS__)

#define DETOUR(name, ...) \
    int __fastcall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DETOUR_T(type, name, ...) \
    type __fastcall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DETOUR_B(name, ...) \
    int __fastcall name##_Hook(void* thisptr, ##__VA_ARGS__)
#else
#define __rescall __thiscall

#define DECL_DETOUR(name, ...)                                  \
    using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                        \
    static int __fastcall name##_Hook(void* thisptr, int edx, ##__VA_ARGS__)
#define DECL_DETOUR_T(type, name, ...)                           \
    using _##name = type(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                         \
    static type __fastcall name##_Hook(void* thisptr, int edx, ##__VA_ARGS__)
#define DECL_DETOUR_B(name, ...)                                \
    using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                        \
    static _##name name##Base;                                  \
    static int __fastcall name##_Hook(void* thisptr, int edx, ##__VA_ARGS__)

#define DETOUR(name, ...) \
    int __fastcall name##_Hook(void* thisptr, int edx, ##__VA_ARGS__)
#define DETOUR_T(type, name, ...) \
    type __fastcall name##_Hook(void* thisptr, int edx, ##__VA_ARGS__)
#define DETOUR_B(name, ...) \
    int __fastcall name##_Hook(void* thisptr, int edx, ##__VA_ARGS__)
#endif
#define DLL_EXPORT extern "C" __declspec(dllexport)
#define SEEK_DIR_CUR std::ios_base::_Seekdir::_Seekcur
#else
#define IS_WINDOWS false
#define IS_LINUX true
#define MODULE_EXTENSION ".so"
// clang-format off
#define GAME_PATH(x) _GAME_PATH(Games/Linux/x.hpp)
// clang-format on
#if __x86_64
#define __rescall __attribute__((__fastcall__))
#define __cdecl __attribute__((__fastcall__))
#define __stdcall __attribute__((__fastcall__))
#define __fastcall __attribute__((__fastcall__))
#else
#define __rescall __attribute__((__cdecl__))
#define __cdecl __attribute__((__cdecl__))
#define __stdcall __attribute__((__stdcall__))
#define __fastcall __attribute__((__fastcall__))
#endif
#define DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define SEEK_DIR_CUR std::ios_base::seekdir::_S_cur

#define DECL_DETOUR(name, ...)                                  \
    using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                        \
    static int __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DECL_DETOUR_T(type, name, ...)                           \
    using _##name = type(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                         \
    static type __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DECL_DETOUR_B(name, ...)                                \
    using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
    static _##name name;                                        \
    static _##name name##Base;                                  \
    static int __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)

#define DETOUR(name, ...) \
    int __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DETOUR_T(type, name, ...) \
    type __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DETOUR_B(name, ...) \
    int __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#endif

#ifdef __x86_64
#define DECL_DETOUR_STD(type, name, ...)              \
    using _##name = type(__fastcall*)(##__VA_ARGS__); \
    static _##name name;                              \
    static type __fastcall name##_Hook(##__VA_ARGS__)
#define DETOUR_STD(type, name, ...) \
    type __fastcall name##_Hook(##__VA_ARGS__)
#else
#define DECL_DETOUR_STD(type, name, ...)             \
    using _##name = type(__stdcall*)(##__VA_ARGS__); \
    static _##name name;                             \
    static type __stdcall name##_Hook(##__VA_ARGS__)
#define DETOUR_STD(type, name, ...) \
    type __stdcall name##_Hook(##__VA_ARGS__)
#endif
