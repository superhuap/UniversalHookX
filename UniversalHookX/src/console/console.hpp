#pragma once
#include <spdlog/spdlog.h>

// #define DISABLE_LOGGING_CONSOLE

#ifdef DISABLE_LOGGING_CONSOLE
#define LOG(...)
#else
#define LOG(...) spdlog::log(__VA_ARGS__)
#endif

namespace Console {
    void Alloc( );
    void Free( );
} // namespace Console
