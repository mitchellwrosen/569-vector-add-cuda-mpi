#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <vector>

#include "float_vector.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

void printUsage();

MpiContext* context;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage();

  context = new MpiContext(&argc, &argv, MPI_COMM_WORLD);

  FloatVector* vec1 = FloatVector::fromFile(argv[1]);
  FloatVector* vec2 = FloatVector::fromFile(argv[2]);
  if (!vec1 || !vec2)
    mpiAbort(-1);

  FloatVector* vec3 = FloatVector::sum(vec1, vec2);
  if (context->isRoot()) {
    vec3->debugPrint();
  }

  context->finalize();

  delete context;
  return 0;
}

void printUsage() {
  fprintf(stderr, "Usage: ./main file1 file2\n");
  exit(EXIT_FAILURE);
}
