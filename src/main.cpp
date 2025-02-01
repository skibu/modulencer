#include <iostream>
#include <thread>

#define DEBUG
#include "seq/clock.h"
#include "util/debug.h"
#include "util/fastRandom.h"
#include "util/json.hpp"

// Just for testing json
#include <fstream>

int main() {
    debug("This is a test of the debug macro");

    using json = nlohmann::json;
    std::ifstream f("example.json");

    try {
        json data = json::parse(f);
        float pi = data["pi"];
        int e = data["answer"]["everything"];
        auto answer = data["answer"];
        int every = answer["everything"];
        int x = 9;
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }

    for (int i = 0; i < 2; i++) {
        debug("fast_rand()=%d", fast_rand());
    }

    for (int i = 0; i < 2; i++) {
        debug("fast_rand(1, 100)=%d", fast_rand(1, 100));
    }

    std::cout << "Hello, World!" << std::endl;

    // Run separate thread forever (for now)
    auto clock = Clock::create();

    clock.setBPM(60).setPPQN(4).run();
    clock.join();

    return 0;
}
