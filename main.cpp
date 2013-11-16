#include <boost/timer.hpp>
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

  boost::timer timer;

  context = new MpiContext(&argc, &argv, MPI_COMM_WORLD);

  FloatVector* vec1 = new FloatVector(argv[1]);
  fprintf(stderr, "Read %d floats onto node %d (%lf seconds)\n", vec1->len(), context->rank, timer.elapsed());

  timer.restart();
  FloatVector* vec2 = new FloatVector(argv[2]);
  fprintf(stderr, "Read %d floats onto node %d (%lf seconds)\n", vec2->len(), context->rank, timer.elapsed());

  FloatVector* vec3 = FloatVector::sum(vec1, vec2);
  vec3->debugPrint();

  context->finalize();

  delete context;
  return 0;
}

void printUsage() {
  fprintf(stderr, "Usage: ./main file1 file2\n");
  exit(EXIT_FAILURE);
}
