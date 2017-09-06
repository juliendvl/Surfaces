#include "utils/Logger.h"

#include <iostream>


Logger Logger::ILogger;

LogLevel Logger::MinLogLevel = LogLevel::WARNING_LVL;

const char* Logger::LogTags[5] = {
    "[DEBUG] ",
    "[INFO] ",
    "[WARNING] ",
    "[ERROR] ",
    "[FATAL] "
};


void Logger::SetMinLogLevel(LogLevel minLevel)
{
    Logger::MinLogLevel = minLevel;
}

void Logger::Log(LogLevel level, const std::string& message)
{
    if (level >= Logger::MinLogLevel)
    {
        if (level <= LogLevel::INFO_LVL)
            std::cout << Logger::LogTags[(unsigned char)level] << message << std::endl;
        else
            std::cerr << Logger::LogTags[(unsigned char)level] << message << std::endl;
    }
}

void Logger::Debug(const std::string& message)
{
    Logger::Log(LogLevel::DEBUG_LVL, message);
}

void Logger::Info(const std::string& message)
{
    Logger::Log(LogLevel::INFO_LVL, message);
}

void Logger::Warning(const std::string& message)
{
    Logger::Log(LogLevel::WARNING_LVL, message);
}

void Logger::Error(const std::string& message)
{
    Logger::Log(LogLevel::ERROR_LVL, message);
}

void Logger::Fatal(const std::string& message)
{
    Logger::Log(LogLevel::FATAL_LVL, message);
    std::terminate();
}
