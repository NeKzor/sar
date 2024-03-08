#pragma once
#include "Module.hpp"

#include "Utils.hpp"
#include "Utils/Platform.hpp"

#include <cstdint>

#ifdef _WIN32
#define TIER0 "tier0"
#define LoggingSystem_LogColor_SYMBOL "?LoggingSystem_Log@@YA?AW4LoggingResponse_t@@HW4LoggingSeverity_t@@VColor@@PEBDZZ"
#else
#define TIER0 "libtier0"
#define LoggingSystem_LogColor_SYMBOL "_Z17LoggingSystem_Logi17LoggingSeverity_t5ColorPKcz"
#endif

#define LoggingSystem_RegisterLoggingChannel_SYMBOL "LoggingSystem_RegisterLoggingChannel"
#define LoggingSystem_RegisterLoggingChannel2_SYMBOL "LoggingSystem_RegisterLoggingChannel2"
#define LoggingSystem_IsChannelEnabled_SYMBOL "LoggingSystem_IsChannelEnabled"
#define LoggingSystem_GetChannelColor_SYMBOL "LoggingSystem_GetChannelColor"
#define LoggingSystem_Log_SYMBOL "LoggingSystem_Log"
#define LoggingSystem_LogDirect_SYMBOL "LoggingSystem_LogDirect"

const auto UNSPECIFIED_LOGGING_COLOR = Color(0, 0, 0, 0);

typedef void* RegisterTagsFunc ;
typedef int32_t LoggingChannelId;

enum class LoggingChannelFlags : int32_t {
    None = 0,
    ConsoleOnly = 1,
    DoNotEcho = 2,
};

enum class LoggingSeverity : int8_t {
    Message = 0,
    Warning = 1,
    Assert = 2,
    Error = 3,
    HighestSeverity = 4,
};

enum class LoggingResponse : int64_t {
    Continue = 0,
    Debugger = 1,
    Abort = 2,
};

#define SAR_PRINT_MODULE_NAME "sar"
#define SAR_PRINT_COLOR Color(247, 214, 68)
#define SAR_PRINT_ACTIVE_COLOR Color(110, 247, 76)
#define SAR_PRINT_WARNING_COLOR Color(235, 229, 52)
#define SAR_PRINT_ERROR_COLOR Color(235, 52, 52)
#define SAR_PRINT_DEFAULT_COLOR Color(255, 255, 255)

class Console : public Module {
public:
    using _LoggingSystemRegisterLoggingChannel = LoggingChannelId(__fastcall*)(
        const char * name,
        RegisterTagsFunc register_tags_func,
        LoggingChannelFlags flags,
        LoggingSeverity minimum_severity,
        Color spew_color
    );
    using _LoggingSystemRegisterLoggingChannel2 = LoggingChannelId(__fastcall*)(
        const char * name,
        RegisterTagsFunc register_tags_func,
        LoggingChannelFlags flags,
        LoggingSeverity minimum_severity,
        Color spew_color,
        const char * module_name
    );
    using _LoggingSystemIsChannelEnabled = bool(__fastcall*)(
        LoggingChannelId channel_id,
        LoggingSeverity severity
    );
    using _LoggingSystemGetChannelColor = Color(__fastcall*)(
        LoggingChannelId channel_id
    );
    using _LoggingSystemLog = LoggingResponse(__cdecl*)(
        LoggingChannelId channel_id,
        LoggingSeverity severity,
        const char * msg_format,
        ...
    );
    using _LoggingSystemLogColor = LoggingResponse(__cdecl*)(
        LoggingChannelId channel_id,
        LoggingSeverity severity,
        Color spew_color,
        const char * msg_format,
        ...
    );
    using _LoggingSystemLogDirect = void*(__fastcall*)(
        LoggingChannelId channel_id,
        LoggingSeverity severity,
        Color spew_color,
        const char * message
    );

    _LoggingSystemRegisterLoggingChannel LoggingSystemRegisterLoggingChannel = nullptr;
    _LoggingSystemRegisterLoggingChannel2 LoggingSystemRegisterLoggingChannel2 = nullptr;
    _LoggingSystemIsChannelEnabled LoggingSystemIsChannelEnabled = nullptr;
    _LoggingSystemGetChannelColor LoggingSystemGetChannelColor = nullptr;
    _LoggingSystemLog LoggingSystemLog = nullptr;
    _LoggingSystemLogColor LoggingSystemLogColor = nullptr;
    _LoggingSystemLogDirect LoggingSystemLogDirect = nullptr;

    LoggingChannelId logGeneral = 0;
    LoggingChannelId logDeveloper = 0;

public:
    template <typename... T>
    void Print(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logGeneral, LoggingSeverity::Message)) {
            this->LoggingSystemLogColor(this->logGeneral, LoggingSeverity::Message, SAR_PRINT_COLOR, fmt, args...);
        }
    }
    template <typename... T>
    void PrintActive(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logGeneral, LoggingSeverity::Message)) {
            this->LoggingSystemLogColor(this->logGeneral, LoggingSeverity::Message, SAR_PRINT_ACTIVE_COLOR, fmt, args...);
        }
    }
    template <typename... T>
    void Warning(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logGeneral, LoggingSeverity::Warning)) {
            this->LoggingSystemLogColor(this->logGeneral, LoggingSeverity::Warning, SAR_PRINT_WARNING_COLOR, fmt, args...);
        }
    }
    template <typename... T>
    void Error(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logGeneral, LoggingSeverity::Error)) {
            this->LoggingSystemLogColor(this->logGeneral, LoggingSeverity::Error, SAR_PRINT_ERROR_COLOR, fmt, args...);
        }
    }
    template <typename... T>
    void Msg(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logGeneral, LoggingSeverity::Message)) {
            this->LoggingSystemLogColor(this->logGeneral, LoggingSeverity::Message, SAR_PRINT_DEFAULT_COLOR, fmt, args...);
        }
    }
    template <typename... T>
    void DevMsg(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logDeveloper, LoggingSeverity::Message)) {
            this->LoggingSystemLogColor(this->logDeveloper, LoggingSeverity::Message, SAR_PRINT_DEFAULT_COLOR, fmt, args...);
        }
    }
    template <typename... T>
    void DevWarning(const char* fmt, T... args)
    {
        if (this->LoggingSystemIsChannelEnabled(this->logDeveloper, LoggingSeverity::Warning)) {
            this->LoggingSystemLogColor(this->logDeveloper, LoggingSeverity::Warning, SAR_PRINT_WARNING_COLOR, fmt, args...);
        }
    }

    bool Init() override;
    void Shutdown() override;
    const char* Name() override { return MODULE(TIER0); }
};

extern Console* console;
