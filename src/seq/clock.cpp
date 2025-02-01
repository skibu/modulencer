#include "clock.h"

#include <chrono>
#include <iostream>
#include <thread>

#define DEBUG
#include "../util/debug.h"

Clock Clock::create() {
    Clock theClock = Clock();
    theClock.thread = std::thread(&Clock::loop, &theClock);
    return theClock;
}

void Clock::run() {
    debug("Running clock %s...", name.c_str());
    state = RUNNING;
}

void Clock::pause() {
    debug("Pausing clock %s...", name.c_str());
    state = PAUSED;
}

void Clock::reset() {
    debug("Resetting clock %s...", name.c_str());
    bpm_count = 0;
    ppqn_count = 0;
}

Clock& Clock::setNam2(std::string nam2) {
    this->nam2 = nam2;
    return *this;
}

Clock& Clock::setName(std::string name) {
    this->name = name;
    return *this;
}

Clock& Clock::setBPM(int bpm) {
    this->bpm = std::clamp(bpm, MIN_BPM, MAX_BPM);
    return *this;
}

Clock& Clock::setPPQN(int ppqn) {
    this->ppqn = std::clamp(ppqn, MIN_PPQN, MAX_PPQN);
    return *this;
}

Clock& Clock::addBPMCallback(void (*bpm_callback)(uint32_t, uint32_t)) {
    bpm_callbacks.push_back(bpm_callback);
    return *this;
}

Clock& Clock::addPPQNCallback(void (*ppqn_callback)(uint32_t)) {
    ppqn_callbacks.push_back(ppqn_callback);
    return *this;
}

void Clock::loop() {
    debug("In loop for clock %s...", name.c_str());
    while (true) {
        // Keep track of when PPQN clock tick started
        using namespace std::chrono;
        auto start_of_tick = steady_clock::now();

        if (state == RUNNING) {
            ppqn_count++;
            debug("Calling ppqn callbacks for ppqn_count=%d", ppqn_count);
            for (auto callback : ppqn_callbacks)
                callback(ppqn_count);

            if ((ppqn_count - 1) % ppqn == 0) {
                bpm_count++;
                debug("Calling bpm callbacks for bpm_count=%d ppqn_count=%d", bpm_count, ppqn_count);
                for (auto callback : bpm_callbacks)
                    callback(bpm_count, ppqn_count);
            }
        }

        // Sleep until next PPQN. Some time will have elapsed between the start of the tick and now,
        // so we need to subtract that from the sleep time
        duration<float> secs_between_ppqn(60.0f / (ppqn * bpm));
        auto sleep_time = secs_between_ppqn - (steady_clock::now() - start_of_tick);
        std::this_thread::sleep_for(sleep_time);
    }
}

void Clock::join() {
    debug("Joining clock %s...", name.c_str());
    thread.join();
}
