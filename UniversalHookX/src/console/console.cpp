#include "console.hpp"
#include <Windows.h>
#include <cstdio>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

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

    size_t queue_size = 8192;
    spdlog::init_thread_pool(queue_size, 4);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>( );
    console_sink->set_pattern("\033[36m[%Y-%m-%d %H:%M:%S.%e]\033[0m %^[%l] %v%$");

    auto async_logger = std::make_shared<spdlog::async_logger>(
        "UniversalHookX",
        console_sink,
        spdlog::thread_pool( ),
        spdlog::async_overflow_policy::overrun_oldest
    );

    spdlog::set_default_logger(async_logger);
    spdlog::set_level(spdlog::level::debug);

    spdlog::flush_every(std::chrono::seconds(5));
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
