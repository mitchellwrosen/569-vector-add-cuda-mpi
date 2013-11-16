#include "mpi/context.h"

#include <mpi.h>

#include "mpi/utils.h"

MpiContext::MpiContext(int* argc, char*** argv, int root, MPI_Comm comm) : root(root), comm(comm) {
  MPI_CHECK(MPI_Init(argc, argv));

  MPI_CHECK(MPI_Comm_size(comm, &size));
  MPI_CHECK(MPI_Comm_rank(comm, &rank));
}

bool MpiContext::isRoot() const {
  return rank == root;
}

void MpiContext::finalize() {
  MPI_CHECK(MPI_Finalize());
}
