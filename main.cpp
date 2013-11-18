#include <boost/timer.hpp>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <vector>

#include "float_vector.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

void printUsage();
void writeHistogram(FloatVector* vec, const char* filename);

MpiContext* context;

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage();

  boost::timer timer;

  context = new MpiContext(&argc, &argv, 0, MPI_COMM_WORLD);

  FloatVector* vec1 = new FloatVector(argv[1]);
  fprintf(stderr, "Read %d floats from %s on node %d (%0.1lf seconds)\n", vec1->len(), argv[1], context->rank, timer.elapsed());

  timer.restart();
  FloatVector* vec2 = new FloatVector(argv[2]);
  fprintf(stderr, "Read %d floats from %s on node %d (%0.1lf seconds)\n", vec2->len(), argv[2], context->rank, timer.elapsed());

  FloatVector* vec3 = FloatVector::sum(vec1, vec2);

  delete vec1;
  delete vec2;

  writeHistogram(vec3, "hist.c");

  context->finalize();

  delete context;
  return 0;
}

void printUsage() {
  fprintf(stderr, "Usage: ./main file1 file2\n");
  exit(EXIT_FAILURE);
}

void writeHistogram(FloatVector* vec, const char* filename) {
  int* histogram = vec->histogram();
  if (context->isRoot()) {
    ofstream histogramFile(filename);
    for (int i = 0; i < NUM_BINS; ++i)
      histogramFile << histogram[i] << endl;
    histogramFile.close();
    fprintf(stderr, "Output \"%s\"\n", filename);
  }
}
