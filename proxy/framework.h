// Copyright (c) 2024, NeKz
// SPDX-License-Identifier: MIT

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <format>
#include <string>
#include <windows.h>

template <typename... Args>
inline auto println(const std::string format, Args&&... args) -> void
{
#if _DEBUG
    auto msg = std::vformat(format, std::make_format_args(args...));
    OutputDebugStringA(("[audio_minimp3.dll] " + msg + "\n").c_str());
#endif
}
