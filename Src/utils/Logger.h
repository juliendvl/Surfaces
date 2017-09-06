#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>


enum class LogLevel : unsigned char
{
    DEBUG_LVL = 0,
    INFO_LVL,
    WARNING_LVL,
    ERROR_LVL,
    FATAL_LVL
};


class Logger
{
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static void SetMinLogLevel(LogLevel minLevel);

    static void Log(LogLevel level, const std::string& message);

    static void Debug(const std::string& message);
    static void Info(const std::string& message);
    static void Warning(const std::string& message);
    static void Error(const std::string& message);
    static void Fatal(const std::string& message);

private:
    static Logger ILogger;
    static LogLevel MinLogLevel;
    static const char* LogTags[5];

    Logger() = default;
};

#endif // __LOGGER_H__
