#include <boost/scoped_ptr.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <mpi.h>
#include <vector>

#include "float_vector.h"
#include "logging.h"
#include "mpi/context.h"
#include "mpi/utils.h"
#include "timer.h"

using boost::scoped_ptr;
using std::endl;

void printUsage();
void writeHistogram(FloatVector* vec, const char* filename);

scoped_ptr<MpiContext> context;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage();

  TIME(
      context.reset(new MpiContext(&argc, &argv, 0, MPI_COMM_WORLD)),
      "MPI initialized"
  );

  scoped_ptr<FloatVector> vec(FloatVector::sum(argv[1], argv[2]));
  writeHistogram(vec.get(), "hist.c");

  context->finalize();

  return 0;
}

void printUsage() {
  fprintf(stderr, "Usage: ./main file1 file2\n");
  exit(EXIT_FAILURE);
}

void writeHistogram(FloatVector* vec, const char* filename) {
  int* histogram = vec->histogram();

  if (context->isRoot()) {
    FILE* histogramFile = fopen(filename, "w");
    for (int i = 0; i < NUM_BINS; ++i)
      fprintf(histogramFile, "%d, %d\n", i, histogram[i]);
    fclose(histogramFile);
    LOG("Output \"%s\"\n", filename);
  }
}
