#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <thread>
#include <mutex>

class Clock {
   public:
    // Creates a new Clock object and starts a new thread
    static Clock& create();

    void run();
    void pause();

    // Resets the BPM and PPQN counts
    void reset_counts();

    std::string name  = "MainClock";  
    Clock& set_name(std::string name);

    Clock& set_BPM(int bpm);
    int get_BPM() {
        return bpm;
    }

    Clock& set_PPQN(int ppqn);
    int get_PPQN() {
        return ppqn;
    }

    Clock& add_BPM_callback(void (*bpm_callback)(uint32_t, uint32_t));

    Clock& add_PPQN_callback(void (*ppqn_callback)(uint32_t));

    // So that main thread can continue to run while the clock thread is running
    void join();

   protected:
    static inline constexpr int DEFAULT_BPM = 120;
    static inline constexpr int MIN_BPM = 20;
    static inline constexpr int MAX_BPM = 300;

    static inline constexpr int DEFAULT_PPQN = 24;
    static inline constexpr int MIN_PPQN = 1;
    static inline constexpr int MAX_PPQN = 192;
 
    // This function is to do the abusrdly complicated converting of a double to a duration
    static std::chrono::steady_clock::duration convert_to_duration(double seconds);

    // The main loop that processes each clock tick
    void loop();

    // Called when clock frequency is changed. Resets the counts so that can 
    // determine exactly when PPQN clock tick should occur.
    void reset_clock_timing();

    // Determines how long to sleep before next clock tick. Can be a nagative duration
    // if too much time already elapsed and shouldn't sleep at all.
    std::chrono::steady_clock::duration determine_sleep_time();

   private:
    // Constructor is private to force start() to be used instead
    Clock() {}

    // The separate thread that the clock loop runs in
    std::thread thread;

    enum State { RUNNING, PAUSED };
    State state = PAUSED;

    // For making sure clock timing is exactly correct
    std::chrono::steady_clock::time_point clock_reset_time;
    long clock_ticks_since_reset;
    std::recursive_mutex clock_reset_mutex;

    // Number of times BPM tick has occurred
    int bpm_count = 0;

    // Number of times PPQN tick has occurred
    int ppqn_count = 0;

    // BPM is Beats Per Minute
    int bpm = DEFAULT_BPM;

    // List of callbacks to call when BPM tick occurs
    std::vector<void (*)(uint32_t, uint32_t)> bpm_callbacks;

    // PPQN is Pulses Per Quarter Note
    int ppqn = DEFAULT_PPQN;

    // List of callbacks to call when PPQN tick occurs
    std::vector<void (*)(uint32_t)> ppqn_callbacks;
};

#endif  // CLOCK_H