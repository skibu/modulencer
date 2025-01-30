#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

class Clock {
   public:
    static void run();

   protected:
    Clock();
    void reset();
    float elapsed();
    
   private:
    static void loop();
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#endif  // CLOCK_H