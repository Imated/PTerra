#include "Logger.h"
#include <Windows.h>

void Logger::Log(const char* message, unsigned char color, bool newLine, ...)
{
    const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    static unsigned char levels[6] = { 64, 4, 6, 2, 1, 8 };
    SetConsoleTextAttribute(consoleHandle, levels[color]);
    char buffer[1024];
    va_list args;
    va_start(args, newLine);
    vsnprintf(buffer, sizeof(buffer), message, args);
    va_end(args);
    const LPDWORD numWritten = nullptr;
    WriteConsoleA(consoleHandle, buffer, static_cast<DWORD>(strlen(buffer)), numWritten, nullptr);
    if (newLine)
        WriteConsoleA(consoleHandle, "\n", 1, numWritten, nullptr);
    else
        WriteConsoleA(consoleHandle, "\r                    \r", 22, nullptr, nullptr);
    SetConsoleTextAttribute(consoleHandle, 7);
}

void Logger::Log(const char* message, unsigned char color, ...)
{
    const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    static unsigned char levels[6] = { 64, 4, 6, 2, 1, 8 };
    SetConsoleTextAttribute(consoleHandle, levels[color]);
    char buffer[1024];
    va_list args;
    va_start(args, color);
    vsnprintf(buffer, sizeof(buffer), message, args);
    va_end(args);
    const LPDWORD numWritten = nullptr;
    WriteConsoleA(consoleHandle, buffer, static_cast<DWORD>(strlen(buffer)), numWritten, nullptr);
    WriteConsoleA(consoleHandle, "\n", 1, numWritten, nullptr);
    SetConsoleTextAttribute(consoleHandle, 7);
}

void Logger::Log(const char* message, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, sizeof(buffer), message, args);
    va_end(args);
    const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const unsigned long long length = strlen(message);
    const LPDWORD numWritten = nullptr;
    WriteConsoleA(consoleHandle, buffer, static_cast<DWORD>(strlen(buffer)), numWritten, nullptr);
    WriteConsoleA(consoleHandle, "\n", 1, numWritten, nullptr);
}
