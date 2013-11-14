#include "mpi/utils.h"

#include <mpi.h>

void mpiAbort(int err) {
  MPI_Abort(MPI_COMM_WORLD, err);
}
