#ifndef SRC_MPI_UTILS_H_
#define SRC_MPI_UTILS_H_

#define MPI_CHECK(call) \
  if ((call) != MPI_SUCCESS) { \
    std::cerr << "MPI error calling \""#call"\"\n"; \
    abort(-1); \
  }

void abort(int err);

#endif  // SRC_MPI_UTILS_H_
