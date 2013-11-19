#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

/**
 * Simple timer class, "turned off" if DEBUG is not defined.
 */
#ifdef DEBUG

#include <boost/timer.hpp>

class Timer : public boost::timer {
};

#define TIME(func, ...) { \
   boost::timer timer; \
   func; \
   fprintf(stderr, "Node %d: ", context->rank); fprintf(stderr, __VA_ARGS__); \
   fprintf(stderr, " (%0.2lf seconds)\n", timer.elapsed()); \
}

#else

class Timer {
};

#define TIME(func, ...) func;

#endif

#endif  // SRC_TIMER_H_

