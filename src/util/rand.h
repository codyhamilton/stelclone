#ifndef RAND_H
#define RAND_H

#include <inttypes.h>
#include "pcg_basic.h"

/**
 * Generate a random number between 0 and bound
 * 
 * @param bound The bound of the random number (exclusive)
 * @return The random number
 */
static inline uint32_t prng(uint32_t bound) {
    return pcg32_boundedrand(bound);
}

/**
 * Generate a random number between min and max
 * 
 * @param min The minimum value of the random number (inclusive)
 * @param max The maximum value of the random number (inclusive)
 * @return The random number
 */
static inline uint32_t prng_range(uint32_t min, uint32_t max) {
    return pcg32_boundedrand(max - min + 1) + min;
}

/**
 * Generate a random number between 0 and 2^32
 * 
 * @return The random number
 */
static inline uint32_t prng_random() {
    return pcg32_random();
}

#endif

