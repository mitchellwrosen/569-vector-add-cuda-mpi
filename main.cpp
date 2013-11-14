#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <vector>

#include "float_vector.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

void printUsage();

int main(int argc, char** argv) {
  if (argc != 3)
    printUsage();

  MpiContext context(&argc, &argv);

  FloatVector* vec1 = FloatVector::fromFile(context, argv[1]);
  FloatVector* vec2 = FloatVector::fromFile(context, argv[2]);
  if (!vec1 || !vec2)
    abort(-1);

  FloatVector* vec3 = FloatVector::sum(context, vec1, vec2);
  if (context.isRoot()) {
    if (!vec3)
      abort(-1);
    vec3->debugPrint();
  }

  context.finalize();
  return 0;

}

void printUsage() {
  fprintf(stderr, "Usage: ./main file1 file2\n");
  abort(-1);
}
