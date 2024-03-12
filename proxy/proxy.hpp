// Copyright (c) 2024, NeKz
// SPDX-License-Identifier: MIT

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <format>
#include <string>
#include <windows.h>

#define MODULE_NAME "scenefilecache"
#define MODULE_EXTENSION ".dll"
#define MODULE_ORIGINAL MODULE_NAME "_original" MODULE_EXTENSION

template <typename... Args>
inline auto println(const std::string format, Args&&... args) -> void
{
#if _DEBUG
    auto msg = std::vformat(format, std::make_format_args(args...));
    OutputDebugStringA(("[" MODULE_NAME "] " + msg + "\n").c_str());
#endif
}

__declspec(noinline) auto wait_for_debugger() -> void
{
#ifdef _DEBUG
    while (!IsDebuggerPresent()) {
        Sleep(420);
    }
    __debugbreak();
#endif
}
