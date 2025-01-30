#include "clock.h"

#include <chrono>
#include <iostream>
#include <thread>

#define DEBUG
#include "../util/debug.h"

void Clock::run() {
    std::thread t(loop);

    // Fixme for now wait till thread is done, which means wait forever
    t.join();
}

Clock::Clock() {
    start = std::chrono::high_resolution_clock::now();
}

std::chrono::time_point<std::chrono::high_resolution_clock> start;

void Clock::loop() {
    auto clock = Clock();

    while (true) {
        std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - clock.start;

        debug("Count = %f", duration.count());

        std::cout << duration.count() << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::this_thread::sleep_for(std::chrono::microseconds(1000000));
    }
}

void Clock::reset() {
    start = std::chrono::high_resolution_clock::now();
}

float Clock::elapsed() {
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - start;
    return duration.count();
}
