#ifndef FASTRANDOM_H
#define FASTRANDOM_H

// fastRandom provides a random number generator based on the original implementation.
// This implementation is known to not be truly random. But for many cases where speed
// is more important than true randomness, this implementation is sufficient.

// Used to seed the generator. Not necessary to call if it is okay to get same 
// sequence of random values          
void fast_srand(int seed);

// Returns a pseudo-random integer between 0 and 32767.
int fast_rand();

// Returns a pseudo-random integer between min and max, up to 32767.
int fast_rand(int min, int max);

#endif // FASTRANDOM_H
