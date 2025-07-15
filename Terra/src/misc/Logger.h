#pragma once

#include <string>
#include <Windows.h>

#undef ERROR

#define STATIC_ASSERT(condition, msg) if(!condition) Logger::Log(msg, ASSERTION)
#define ASSERT(msg, ...) Logger::Log(msg, ASSERTION, ##__VA_ARGS__)
#define ERR(msg, ...) Logger::Log(msg, ERROR, ##__VA_ARGS__)
#define WARN(msg, ...) Logger::Log(msg, WARNING, ##__VA_ARGS__)
#define INFO(msg, ...) Logger::Log(msg, INFO, ##__VA_ARGS__)
#define TRACE(msg, ...) Logger::Log(msg, TRACE, ##__VA_ARGS__)

#ifndef NDEBUG
#define DEBUG(msg, ...) Logger::Log(msg, DEBUG, ##__VA_ARGS__)
#else
#define DEBUG(msg, ...)
#endif

enum
{
    ASSERTION = 0,
    ERROR,
    WARNING,
    DEBUG,
    INFO,
    TRACE,
    BLACK
};

class Logger
{
public:
    static void Log(const char* message, unsigned char color, ...);
    static void Log(const char* message, ...);
};
