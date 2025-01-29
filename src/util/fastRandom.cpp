#include "fastRandom.h"

static unsigned int g_seed;
         
void fast_srand(int seed) {
    g_seed = seed;
}

int fast_rand() {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

int fast_rand(int min, int max) {
    return fast_rand() % (max - min + 1) + min;
}
