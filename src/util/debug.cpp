#include "debug.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>


std::string thread_id() {
    // Yes, getting thread ID as a string is this complicated in C++.
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

std::string short_thread_id() {
    // Jusr return last 6 characters of thread ID.
    int MAX_LENGTH = 6;
    std::string id = thread_id();
    return id.substr(std::max((int) id.size() - MAX_LENGTH, 0));
}

// Store start time for application
using namespace std::chrono;
auto g_start_time = steady_clock::now();

std::string time_str() {
    nanoseconds duration = steady_clock::now() - g_start_time;
    microseconds elapsed_usecs = duration_cast<microseconds>(duration);
    return std::to_string(elapsed_usecs.count() / 1'000'000.0);
}
