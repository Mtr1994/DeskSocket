#include "logger.h"

#include <QDateTime>
#include <QString>
#include <QDir>

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
    // 清理旧的日志
    QDir logDir("./logs");
    logDir.removeRecursively();

    QString file = "logs/" + QDateTime::currentDateTime().toString("yyyyMMdd")+".log";
    auto logger = spdlog::basic_logger_mt("file_log", file.toStdString());
    logger->flush_on(spdlog::level::debug);
    spdlog::flush_every(std::chrono::seconds(60));
    logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%l][%t] <%s> {%#}: %v");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
}

