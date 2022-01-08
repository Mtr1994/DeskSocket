#include "logger.h"

#include <QDateTime>
#include <QString>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

Logger::Logger()
{

}

Logger::~Logger()
{
    spdlog::drop_all();
    spdlog::shutdown();
}

void Logger::init()
{
    QString file = "logs/" + QDateTime::currentDateTime().toString("yyyy_MM_dd") + "/" + QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss")+".log";
    auto logger = spdlog::basic_logger_mt("file_log", file.toStdString());
    logger->flush_on(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(60));
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%l][%t] <%s> {%#}: %v");

    // Set the default logger to file logger
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
}

