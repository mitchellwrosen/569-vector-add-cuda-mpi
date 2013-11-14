#include "mpi_utils.h"

#include <mpi.h>

void abort() {
  MPI_Abort(MPI_COMM_WORLD, -1);
}
