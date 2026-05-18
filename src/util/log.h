#if !defined(NDEBUG) || defined(DEBUG_ASTEROIDS)
#include <stdio.h>
#define LOG_DEBUG(...) printf(__VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif