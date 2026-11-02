#include <os/logger.h>
#include <rex/logging.h>
#include <rex/diagnostics/policy.h>

#include <spdlog/sinks/wincolor_sink.h>
#include <spdlog/sinks/msvc_sink.h>

void os::logger::PlatformInitSinks()
{
    if (!rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kLogging)) return;
    // Colored console output for attached terminals.
    auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
    rex::AddSink(console_sink);

    // Mirror log output to the Visual Studio / WinDbg output window.
    auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    rex::AddSink(debug_sink);
}

void os::logger::PlatformShutdownSinks()
{
    // spdlog handles sink cleanup on shutdown; nothing to do here.
}

// Mirrors logger_macos.cpp: rex owns logger setup, but without its console
// sink, since PlatformInitSinks installs the wincolor one.
void os::logger::Init()
{
    if (!rex::diagnostics::IsEnabled(
            rex::diagnostics::Category::kLogging)) return;
    rex::LogConfig cfg;
    cfg.log_to_console = false;
    cfg.default_level  = spdlog::level::trace;
    cfg.flush_level    = spdlog::level::trace;
    rex::InitLogging(cfg);

    PlatformInitSinks();
}

void os::logger::Shutdown()
{
    PlatformShutdownSinks();
}
