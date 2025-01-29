#include "debug.h"

#include <chrono>
#include <iostream>
#include <thread>

std::string thread_id() {
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

std::string short_thread_id() {
    return thread_id().substr(8);
}

std::string time_str() {
    using namespace std::chrono;

    auto now = high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto microsecs = duration_cast<microseconds>(duration).count();

    return std::to_string(microsecs / 1000000.0);
}
