#pragma once

#ifdef __unix__

#define TARGET_OS_UNIX 1
#define TARGET_OS_WINDOWS 0

#elif defined(_WIN32) || defined(WIN32)

#define TARGET_OS_UNIX 0
#define TARGET_OS_WINDOWS 1

#else

#error "The target platform is not recognized."

#endif

