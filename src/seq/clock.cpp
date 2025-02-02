#include "clock.h"

#include <chrono>
#include <iostream>
#include <thread>

#define DEBUG
#include "../util/debug.h"

Clock& Clock::create() {
    Clock* clock_ptr = new Clock();
    clock_ptr->thread = std::thread(&Clock::loop, clock_ptr);

    // So can chain calls
    return *clock_ptr;
}

void Clock::run() {
    debug("Running clock %s...", name.c_str());
    state = RUNNING;
}

void Clock::pause() {
    debug("Pausing clock %s...", name.c_str());
    state = PAUSED;
}

void Clock::reset_counts() {
    debug("Resetting clock %s...", name.c_str());
    bpm_count = 0;
    ppqn_count = 0;
}

Clock& Clock::set_name(std::string new_name) {
    this->name = new_name;

    // So can chain calls
    return *this;
}

Clock& Clock::set_BPM(int new_bpm) {
    clock_reset_mutex.lock();
    {
        this->bpm = std::clamp(new_bpm, MIN_BPM, MAX_BPM);
        reset_clock_timing();
    }
    clock_reset_mutex.unlock();

    // So can chain calls
    return *this;
}

Clock& Clock::set_PPQN(int new_ppqn) {
    clock_reset_mutex.lock();
    {
        this->ppqn = std::clamp(new_ppqn, MIN_PPQN, MAX_PPQN);
        reset_clock_timing();
    }
    clock_reset_mutex.unlock();

    // So can chain calls
    return *this;
}

Clock& Clock::add_BPM_callback(void (*callback)(uint32_t, uint32_t)) {
    bpm_callbacks.push_back(callback);
    return *this;
}

Clock& Clock::add_PPQN_callback(void (*callback)(uint32_t)) {
    ppqn_callbacks.push_back(callback);
    return *this;
}

void Clock::reset_clock_timing() {
    clock_reset_mutex.lock();
    {
        // Reset time to 0 by using default constructor. The time will then be set 
        // to the current time at the beginning of the clock tick in the loop() function.
        clock_reset_time = {}; 

        clock_ticks_since_reset = 0;
    }
    clock_reset_mutex.unlock();
}


std::chrono::steady_clock::duration Clock::convert_to_duration(double seconds) {
    return std::chrono::duration_cast<std::chrono::steady_clock::duration>(
        std::chrono::duration<double>(seconds));
}


std::chrono::steady_clock::duration Clock::determine_sleep_time() {
    ++clock_ticks_since_reset;

    // Determine how long should sleep before next clock tick. Can be 
    // negative if too much time already elapsed.
    using namespace std::chrono;
    double tick_duration = 60.0 / (ppqn * bpm);
    auto sleep_duration = 
        clock_reset_time 
        + convert_to_duration(clock_ticks_since_reset * tick_duration)
        - steady_clock::now();

    return sleep_duration;
}


void Clock::loop() {
    debug("In loop for clock %s...", name.c_str());

    // So can determine how late next tick is compared to when it should have been
    reset_clock_timing();

    // Loops each PPWN clock tick
    while (true) {
        // If the clock timer reset time hasn't been initialized, do so now
        if (clock_reset_time.time_since_epoch().count() == 0) 
            clock_reset_time = std::chrono::steady_clock::now();

        if (state == RUNNING) {
            ppqn_count++;
            // debug("Calling ppqn callbacks for ppqn_count=%d", ppqn_count);
            for (auto callback : ppqn_callbacks)
                callback(ppqn_count);

            if ((ppqn_count - 1) % ppqn == 0) {
                bpm_count++;
                debug("Calling bpm callbacks for bpm_count=%d ppqn_count=%d", bpm_count, ppqn_count);
                for (auto callback : bpm_callbacks)
                    callback(bpm_count, ppqn_count);
            }
        }

        // Sleep until next PPQN tick
        std::chrono::duration sleep_time = determine_sleep_time();
        //debug("Sleeping for %.6f seconds", sleep_time.count()/1'000'000'000.0);
        if (sleep_time.count() > 0) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            debug("Clock tick took too long. Not sleeping.");
        }
    }
}

void Clock::join() {
    debug("Joining clock %s...", name.c_str());
    thread.join();
}
