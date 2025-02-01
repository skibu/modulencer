#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <thread>

class Clock {
   public:
    // Creates a new Clock object and starts a new thread
    static Clock create();

    void run();
    void pause();
    void reset();

    std::string nam2  = "Main";  
    Clock& setNam2(std::string nam2);
    
    Clock& setName(std::string name);

    Clock& setBPM(int bpm);
    int getBPM() {
        return bpm;
    }

    Clock& setPPQN(int ppqn);
    int getPPQN() {
        return ppqn;
    }

    Clock& addBPMCallback(void (*bpm_callback)(uint32_t, uint32_t));

    Clock& addPPQNCallback(void (*ppqn_callback)(uint32_t));

    // So that main thread can continue to run while the clock thread is running
    void join();

   protected:
    void loop();

   private:
    // Constructor is private to force start() to be used instead
    Clock() {}

    std::thread thread;

    enum State { RUNNING, PAUSED };
    State state = PAUSED;

    uint32_t bpm_count = 0;
    uint32_t ppqn_count = 0;

    std::string name = "Main";

    // BPM is Beats Per Minute
    static inline constexpr int DEFAULT_BPM = 120;
    static inline constexpr int MIN_BPM = 20;
    static inline constexpr int MAX_BPM = 300;
    int bpm = DEFAULT_BPM;
    std::vector<void (*)(uint32_t, uint32_t)> bpm_callbacks;

    // PPQN is Pulses Per Quarter Note
    static inline constexpr int DEFAULT_PPQN = 24;
    static inline constexpr int MIN_PPQN = 1;
    static inline constexpr int MAX_PPQN = 192;
    int ppqn = DEFAULT_PPQN;
    std::vector<void (*)(uint32_t)> ppqn_callbacks;

    static void bpm_callback(uint32_t, uint32_t);
    static void ppqn_callback(uint32_t);
};

#endif  // CLOCK_H