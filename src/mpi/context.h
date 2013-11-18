#ifndef SRC_MPI_CONTEXT_H_
#define SRC_MPI_CONTEXT_H_

#include <mpi.h>

/**
 * A convenient struct for MPI-related variables. Intended to be used as a
 * singleton global variable. Singleton pattern is not enforced for
 * syntactic convenience, i.e. context->size vs. context->getInstance()->size.
 *
 * Also offers wrapper methods around MPI functions, which simply fill in
 * whatever defaults it can from its own member variables, and wraps the entire
 * call in an MPI_CHECK from src/mpi/mpi_utils.h.
 */
struct MpiContext {
  MpiContext(int* argc, char*** argv, int root, MPI_Comm comm);

  /**
   * Is the caller the root process?
   */
  bool isRoot() const;

  void finalize();

  // Wrapper methods.
  void reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op);
  void gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype);

  int size;
  int rank;
  int root;
  MPI_Comm comm;
};

#endif  // SRC_MPI_CONTEXT_H_
