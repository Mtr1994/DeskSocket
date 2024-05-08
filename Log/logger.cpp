#include "logger.h"

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"

#include <string>

// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting

Logger::Logger()
{

}

Logger::~Logger()
{
    spdlog::drop_all();
    spdlog::shutdown();
}

void Logger::init(int type)
{
    if (type == LOG_CONSOLE)
    {
        auto console = spdlog::stdout_color_mt("console_log");
        console->set_pattern("[%H:%M:%S.%e] [%^%l%$] [t:%t] [%s:%#]: %v");
        spdlog::set_default_logger(console);
        spdlog::set_level(spdlog::level::debug);
    }
    else if (type == LOG_SINK)
    {
        // Create a file rotating logger with 2 MB size max and 1 rotated files
        auto logger = spdlog::rotating_logger_mt("rotate_log", "./logs/sink_log.txt", 1024 * 1024 * 2, 0);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%s:%#]: %v");
        logger->flush_on(spdlog::level::debug);

        spdlog::flush_every(std::chrono::seconds(60));
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
    }
    else if(type == LOG_DAILY)
    {
        // Create a daily logger - a new file is created every day at 2:30 am
        auto logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.txt", 2, 30);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%s:%#]: %v");
        logger->flush_on(spdlog::level::debug);

        spdlog::flush_every(std::chrono::seconds(60));
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::debug);
    }

    LOG_DEBUG("------------------------- logger system start -------------------------");

    std::string logtype = (type == LOG_CONSOLE) ? "LOG_CONSOLE" : ((type == LOG_SINK) ? "LOG_SINK" : "LOG_DAILY");
    LOG_DEBUG("{} init success", logtype);
}

