#ifndef LOGGER_H
#define LOGGER_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "spdlog/spdlog.h"

#define LOG_INFO(...) SPDLOG_LOGGER_INFO(spdlog::get("file_log"), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(spdlog::get("file_log"), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::get("file_log"), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::get("file_log"), __VA_ARGS__)

class Logger
{
public:
    Logger();
    ~Logger();

    void init();
};

#endif // LOGGER_H
