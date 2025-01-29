#include <iostream>
#include <thread>

#define DEBUG
#include "util/debug.h"
#include "util/fastRandom.h"

int main() {
    debug("This is a test of the debug macro\n");

    for (int i = 0; i < 20; i++) {
        std::cout << fast_rand() << std::endl;
    }

    for (int i = 0; i < 20; i++) {
        std::cout << fast_rand(1, 100) << std::endl;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
