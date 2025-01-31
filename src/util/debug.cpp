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
    // Jusr return last 8 characters of thread ID.
    int MAX = 8;
    std::string id = thread_id();
    return id.substr(std::max((int) id.size() - MAX, 0));
}

std::string time_str() {
    using namespace std::chrono;

    const high_resolution_clock::time_point now = high_resolution_clock::now();
    const high_resolution_clock::duration duration = now.time_since_epoch();
    const auto microsecs = duration_cast<microseconds>(duration).count();

    return std::to_string(microsecs / 1000000.0);
}
