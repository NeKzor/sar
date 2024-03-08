#include "Console.hpp"

bool Console::Init()
{
    auto tier0 = Memory::GetModuleHandleByName(this->Name());
    if (tier0) {
        this->LoggingSystemRegisterLoggingChannel = Memory::GetSymbolAddress<_LoggingSystemRegisterLoggingChannel>(tier0, LoggingSystem_RegisterLoggingChannel_SYMBOL);
        this->LoggingSystemRegisterLoggingChannel2 = Memory::GetSymbolAddress<_LoggingSystemRegisterLoggingChannel2>(tier0, LoggingSystem_RegisterLoggingChannel2_SYMBOL);
        this->LoggingSystemIsChannelEnabled = Memory::GetSymbolAddress<_LoggingSystemIsChannelEnabled>(tier0, LoggingSystem_IsChannelEnabled_SYMBOL);
        this->LoggingSystemGetChannelColor = Memory::GetSymbolAddress<_LoggingSystemGetChannelColor>(tier0, LoggingSystem_GetChannelColor_SYMBOL);
        this->LoggingSystemLog = Memory::GetSymbolAddress<_LoggingSystemLog>(tier0, LoggingSystem_Log_SYMBOL);
        this->LoggingSystemLogColor = Memory::GetSymbolAddress<_LoggingSystemLogColor>(tier0, LoggingSystem_LogColor_SYMBOL);
        this->LoggingSystemLogDirect = Memory::GetSymbolAddress<_LoggingSystemLogDirect>(tier0, LoggingSystem_LogDirect_SYMBOL);

        this->logGeneral = this->LoggingSystemRegisterLoggingChannel2(
            "General",
            nullptr,
            LoggingChannelFlags::None,
            LoggingSeverity::Message,
            UNSPECIFIED_LOGGING_COLOR,
            SAR_PRINT_MODULE_NAME);

        this->logDeveloper = this->LoggingSystemRegisterLoggingChannel2(
            "Developer",
            nullptr,
            LoggingChannelFlags::None,
            LoggingSeverity::Message,
            UNSPECIFIED_LOGGING_COLOR,
            SAR_PRINT_MODULE_NAME);

        Memory::CloseModuleHandle(tier0);
    }

    return this->hasLoaded = tier0
        && this->LoggingSystemRegisterLoggingChannel
        && this->LoggingSystemRegisterLoggingChannel2
        && this->LoggingSystemIsChannelEnabled
        && this->LoggingSystemGetChannelColor
        && this->LoggingSystemLog
        && this->LoggingSystemLogColor
        && this->LoggingSystemLogDirect;
}
void Console::Shutdown()
{
}

Console* console;
