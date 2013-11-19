#ifndef SRC_LOGGING_H_
#define SRC_LOGGING_H_

#ifdef DEBUG
#define LOG(...) fprintf(stderr, "Node %d: ", context->rank); fprintf(stderr, __VA_ARGS__)
#else
#define LOG(...) ;
#endif

#endif  // SRC_LOGGING_H_
