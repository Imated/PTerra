#include "Logger.h"
#include "Target.h"
#include <cstdarg>



void log_inner(const char* message, unsigned char color, bool newline, va_list args);

#if TARGET_OS_WINDOWS

    #include <Windows.h>

    void log_inner(const char* message, unsigned char color, bool newLine, va_list args) {
        const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        static unsigned char levels[6] = { 64, 4, 6, 2, 1, 8 };
        if (color >= sizeof(level_colors)) {
            color = ERROR;
        }
        SetConsoleTextAttribute(consoleHandle, levels[color]);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), message, args);
        const LPDWORD numWritten = nullptr;
        WriteConsoleA(consoleHandle, buffer, static_cast<DWORD>(strlen(buffer)), numWritten, nullptr);
        if (newline)
            WriteConsoleA(consoleHandle, "\n", 1, numWritten, nullptr);
        else
            WriteConsoleA(consoleHandle, "\r                    \r", 22, nullptr, nullptr);
        SetConsoleTextAttribute(consoleHandle, 7);
    }

#elif TARGET_OS_UNIX

    // In theory, this implementation should work with pretty much any ANSI-compatible terminal,
    // which includes many non-unix terminals.
    void log_inner(const char* message, unsigned char color, bool newline, va_list args) {
        const unsigned char level_colors[] = {
            44, // Assertion: Blue background
            31, // Error: Red foreground
            33, // Warning: Brown/yellow foreground
            32, // Debug: Green foreground
            34, // Info: Blue foreground
            90, // Trace: Dark Gray foreground
        };
        if (color >= sizeof(level_colors)) {
            color = ERROR;
        }
        // Set color corresponding to specified logging level
        printf("\e[%dm", level_colors[color]);
        
        printf(message, message, args);
        
        // Reset to default text formatting
        printf("\e[0m");
        
        if (newline)
            printf("\n");
        else
            printf("\r                    \r");
    }
#endif

// TODO: passing one byte objects before varargs technically leads to undefined behavior here.

void Logger::Log(const char* message, unsigned char color, bool newLine, ...)
{
    va_list args;
    va_start(args, newLine);
    log_inner(message, color, newLine, args);
    va_end(args);
}

void Logger::Log(const char* message, unsigned char color, ...)
{
    va_list args;
    va_start(args, color);
    log_inner(message, color, true, args);
    va_end(args);
}
    
void Logger::Log(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    // This appears to be the default you have going.
    log_inner(message, TRACE, true, args);
    va_end(args);
}
