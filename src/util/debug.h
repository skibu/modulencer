// Provides a macro debug() for debugging output. If DEBUG is defined, debug() will print to stderr.
// Otherwise, debug() will do nothing, and will be optimized out by the compiler and therefore have
// no runtime overhead.
//
// To use this, define DEBUG before including this file. For example:
//   #define DEBUG
//   #include "debug.h"

#include <cstdio>
#include <filesystem>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

// Returns the current thread's ID as a string.
std::string thread_id();

// Returns the current thread's ID as a string, but only the last 8 characters.
std::string short_thread_id();

std::string time_str();

// See https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c#1644898
// for further explanation of this complicated macro. Also, to successfully get just the filename
// from __FILE__ and pass it as part of varargs to fprintf, we need to first convert it to a string literal.
#define debug(fmt, ...)                                                                \
    do {                                                                               \
        if (DEBUG_TEST) {                                                              \
            const char* filename = std::filesystem::path(__FILE__).filename().c_str(); \
            fprintf(stderr, "%s DEBUG %s %s:%d:%s() - " fmt,                           \
                    time_str().c_str(), short_thread_id().c_str(), filename,           \
                    __LINE__, __func__, ##__VA_ARGS__);                                \
        }                                                                              \
    } while (0)
