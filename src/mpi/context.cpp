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

void MpiContext::reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op) {
  MPI_CHECK(MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm));
}

void gather(void* sendbuf, int sendcnt, MPI_Datatype sendtype, void* recvbuf, int recvcnt, MPI_Datatype recvtype) {
  MPI_CHECK(MPI_Gather(data_, len_, MPI_FLOAT, totalData, len_, MPI_FLOAT, root, comm));
}
