#include "float_vector.h"

#include <iostream>
#include <fstream>
#include <mpi.h>

#include "cuda/vector_add.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

FloatVector* FloatVector::fromFile(const MpiContext& context, const char* filename) {
  // TODO: Return NULL if bad filename or something.
  return new FloatVector(context, filename);
}

FloatVector* FloatVector::sum(const MpiContext& context, const FloatVector* vec1, const FloatVector* vec2) {
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
  float* chunkSum = new float[floatsPerNode];

  MPI_CHECK(MPI_Scatter(vec1->data(), floatsPerNode, MPI_FLOAT, chunk1, floatsPerNode, MPI_FLOAT, 0, MPI_COMM_WORLD));
  MPI_CHECK(MPI_Scatter(vec2->data(), floatsPerNode, MPI_FLOAT, chunk2, floatsPerNode, MPI_FLOAT, 0, MPI_COMM_WORLD));

  cudaVectorAdd(chunk1, chunk2, chunkSum, floatsPerNode);

  if (context.isRoot()) {
    FloatVector* vectorSum = new FloatVector(context, new float[vec1->len()], vec1->len());
    MPI_CHECK(MPI_Gather(chunkSum, floatsPerNode, MPI_FLOAT, vectorSum->data_, floatsPerNode, MPI_FLOAT, 0, MPI_COMM_WORLD));
    return vectorSum; 
  } else {
    MPI_CHECK(MPI_Gather(chunkSum, floatsPerNode, MPI_FLOAT, NULL, floatsPerNode, MPI_FLOAT, 0, MPI_COMM_WORLD));
    return NULL;
  }
}

void FloatVector::debugPrint() {
  for (int i = 0; i < len_; ++i)
    cout << data_[i] << ' ';
  cout << endl;
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

FloatVector::FloatVector(const MpiContext& context, float* data, int len)
    : context_(context), filename_(NULL), data_(data), len_(len) {
}
