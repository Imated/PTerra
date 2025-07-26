#pragma once

#include <string>

#undef ERROR

#define STATIC_ASSERT(condition, msg) if(!condition) Logger::log(msg, ASSERTION)
#define ASSERT(msg, ...) Logger::log(msg, ASSERTION, true, ##__VA_ARGS__)
#define ERR(msg, ...) Logger::log(msg, ERROR, true, ##__VA_ARGS__)
#define WARN(msg, ...) Logger::log(msg, WARNING, true, ##__VA_ARGS__)
#define INFO(msg, ...) Logger::log(msg, INFO, true, ##__VA_ARGS__)
#define TRACE(msg, ...) Logger::log(msg, TRACE, true, ##__VA_ARGS__)

#ifndef NDEBUG
#define DEBUGN(msg, ...) Logger::log(msg, DEBUG, false, ##__VA_ARGS__)
#define DEBUG(msg, ...) Logger::log(msg, DEBUG, true, ##__VA_ARGS__)
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
    static void log(const char *message, unsigned char color, bool newLine, ...);
    static void log(const char* message, unsigned char color, ...);
    static void log(const char* message, ...);
};
