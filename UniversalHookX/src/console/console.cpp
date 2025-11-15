#include "console.hpp"
#include <Windows.h>
#include <cstdio>
#include <spdlog/sinks/stdout_color_sinks.h>

void Console::Alloc( ) {
#ifdef DISABLE_LOGGING_CONSOLE
    AllocConsole( );
    ShowWindow(GetConsoleWindow( ), SW_HIDE);
#else
    AllocConsole( );
    SetConsoleTitleA("UniversalHookX - Debug Console");

    FILE* fp;
    freopen_s(&fp, "conin$", "r", stdin);
    freopen_s(&fp, "conout$", "w", stdout);
    ShowWindow(GetConsoleWindow( ), SW_SHOW);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode)) {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, mode);
        }
    }

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>( );

    console_sink->set_pattern(
    "\033[36m[%Y-%m-%d %H:%M:%S.%e]\033[0m %^[%l] %v%$"
);

    auto logger = std::make_shared<spdlog::logger>("UniversalHookX", console_sink);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
#endif
}

void Console::Free( ) {
#ifdef DISABLE_LOGGING_CONSOLE
    FreeConsole( );
#else
    fclose(stdin);
    fclose(stdout);
    FreeConsole( );
#endif
}
