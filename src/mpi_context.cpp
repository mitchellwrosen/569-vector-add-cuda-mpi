#include "mpi_context.h"

#include <mpi.h>

#include "mpi_utils.h"

MpiContext::MpiContext(int* argc, char*** argv) {
  MPI_CHECK(MPI_Init(argc, argv));

  MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD, &this->size));
  MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &this->rank));
}

bool MpiContext::isRoot() const {
  return rank == 0;
}
