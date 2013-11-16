#include "float_vector.h"

#include <iostream>
#include <fstream>
#include <mpi.h>

#include "cuda/vector_add.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

extern MpiContext* context;

static float* getChunkSum(float* vec1, float* vec2, int floatsPerNode);

FloatVector* FloatVector::fromFile(const char* filename) {
  // TODO: Return NULL if bad filename or something.
  return new FloatVector(filename);
}

FloatVector* FloatVector::sum(const FloatVector* vec1, const FloatVector* vec2) {
  int floatsPerNode;
  if (context->isRoot()) {
    if (vec1->len() != vec2->len()) {
      fprintf(stderr, "Unequal vector lengths: %d (%s), %d (%s)\n",
          vec1->len(), vec1->filename(), vec2->len(), vec2->filename());
      mpiAbort(-1);
    }

    if (vec1->len() % context->size != 0) {
      fprintf(stderr, "Unable to evenly distribute %d floats among %d nodes\n", vec1->len(), context->size);
      mpiAbort(-1);
    }

    floatsPerNode = vec1->len() / context->size;
  }

  MPI_CHECK(MPI_Bcast(&floatsPerNode, sizeof(int), MPI_INT, 0, context->comm));

  float* chunk1 = new float[floatsPerNode];
  float* chunk2 = new float[floatsPerNode];
  float* chunkSum = new float[floatsPerNode];

  MPI_CHECK(MPI_Scatter(vec1->data(), floatsPerNode, MPI_FLOAT, chunk1, floatsPerNode, MPI_FLOAT, 0, context->comm));
  MPI_CHECK(MPI_Scatter(vec2->data(), floatsPerNode, MPI_FLOAT, chunk2, floatsPerNode, MPI_FLOAT, 0, context->comm));

  cudaVectorAdd(chunk1, chunk2, chunkSum, floatsPerNode);

  float* vectorSumData;
  if (context->isRoot())
    vectorSumData = new float[vec1->len()];

  MPI_CHECK(MPI_Gather(chunkSum, floatsPerNode, MPI_FLOAT, vectorSumData, floatsPerNode, MPI_FLOAT, 0, context->comm));

  if (context->isRoot())
     return new FloatVector(vectorSumData, vec1->len());
  return NULL;
}

void FloatVector::debugPrint() {
  cout << len_ << " floats." << endl;
  for (int i = 0; i < len_; ++i)
    cout << data_[i] << ' ';
  cout << endl;
}

FloatVector::FloatVector(const char* filename) : filename_(filename) {
  if (context->isRoot()) {
    ifstream infile(filename, ios::binary);

    infile.read((char*) &len_, sizeof(int));
    data_ = new float[len_];

    for (int i = 0; i < len_; ++i)
      infile.read((char*) (data_ + i), sizeof(float));
  }
}

FloatVector::FloatVector(float* data, int len) : filename_(NULL), data_(data), len_(len) {
}
