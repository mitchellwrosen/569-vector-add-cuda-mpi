#ifndef SRC_MPI_CONTEXT_H_
#define SRC_MPI_CONTEXT_H_

#include <mpi.h>

class MpiContext {
 public:
  MpiContext(int* argc, char*** argv, MPI_Comm comm);

  bool isRoot() const;
  void finalize();

  int size;
  int rank;
  MPI_Comm comm;
};

#endif  // SRC_MPI_CONTEXT_H_
