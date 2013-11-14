#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <vector>

#include "decoder.h"
#include "mpi_utils.h"
#include "vector_utils.h"

using namespace std;

void printUsage();

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage();

  MpiContext context(&argc, &argv);

  FloatVector* vec1 = FloatVector::fromFile(context, argv[1]);
  FloatVector* vec2 = FloatVector::fromFile(context, argv[2]);
  FloatVector* vec3 = FloatVector::sum(context, vec1, vec2);

  if (!vec1 || !vec2 || !vec3)
    abort();
}

void printUsage() {
  fprintf(stderr, "Usage: ./main file1 file2\n");
  abort();
}
