#include "float_vector.h"

#include <fstream>
#include <mpi.h>

#include "mpi_context.h"
#include "mpi_utils.h"

using namespace std;

FloatVector* FloatVector::fromFile(const MpiContext& context, const char* filename) {
  // TODO: Return NULL if bad filename or something.
  return new FloatVector(context, filename);
}

FloatVector* FloatVector::sum(const MpiContext& context, const FloatVector* vec1, const FloatVector* vec2) {
  if (!vec1 || !vec2)
    return NULL;

  if (vec1->len() != vec2->len()) {
    fprintf(stderr, "Unequal vector lengths: %d (%s), %d (%s)\n", 
        vec1->len(), vec1->filename(), vec2->len(), vec2->filename());
    return NULL;
  }

  if (vec1->len() % context.size != 0) {
    fprintf(stderr, "Unable to evenly distribute %d floats among %d nodes\n", vec1->len(), context.size);
    return NULL;
  }

  int floatsPerNode = vec1->len() / context.size;
  float* chunk1 = new float[floatsPerNode];
  float* chunk2 = new float[floatsPerNode];

  MPI_CHECK(MPI_Scatter(vec1->data(), floatsPerNode, MPI_FLOAT, chunk1, floatsPerNode, MPI_FLOAT, 0, MPI_COMM_WORLD));
  MPI_CHECK(MPI_Scatter(vec2->data(), floatsPerNode, MPI_FLOAT, chunk2, floatsPerNode, MPI_FLOAT, 0, MPI_COMM_WORLD));
}

FloatVector::FloatVector(const MpiContext& context, const char* filename) 
    : context_(context)
    , filename_(filename) {
  if (context.isRoot()) {
    ifstream infile(filename, ios::binary);

    infile.read((char*) &len_, sizeof(int));
    data_ = new float[len_];

    for (int i = 0; i < len_; ++i)
      infile.read((char*) (data_ + i), sizeof(float));
  }
}
