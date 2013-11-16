#include "float_vector.h"

#include <boost/timer.hpp>
#include <iostream>
#include <fstream>
#include <mpi.h>

#include "cuda/vector_add.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

extern MpiContext* context;

FloatVector::FloatVector(const char* filename) : filename_(filename) {
  ifstream infile(filename, ios::binary);

  int totalLen;
  infile.read((char*) &totalLen, sizeof(int));

  if (totalLen % context->size != 0) {
    fprintf(stderr, "Unable to evenly distribute %d floats among %d nodes\n", totalLen, context->size);
    mpiAbort(-1);
  }

  len_ = totalLen / context->size;
  data_ = new float[len_];

  infile.seekg(sizeof(int) + len_ * sizeof(float));
  for (int i = 0; i < len_; ++i)
    infile.read((char*) (data_ + i), sizeof(float));
}

FloatVector::FloatVector(float* data, int len) : filename_(NULL), data_(data), len_(len) {
}

FloatVector* FloatVector::fromFile(const char* filename) {
  // TODO: Return NULL if bad filename or something.
  return new FloatVector(filename);
}

FloatVector* FloatVector::sum(const FloatVector* vec1, const FloatVector* vec2) {
  if (vec1->len() != vec2->len()) {
    fprintf(stderr, "Unequal vector lengths: %d (%s), %d (%s)\n",
        vec1->len(), vec1->filename(), vec2->len(), vec2->filename());
    mpiAbort(-1);
  }

  int floatsPerNode = vec1->len();
  int totalFloats = floatsPerNode * context->size;

  float* chunkSum = new float[floatsPerNode];
  cudaVectorAdd(vec1->data(), vec2->data(), chunkSum, floatsPerNode);

  float* vectorSumData;
  if (context->isRoot())
    vectorSumData = new float[totalFloats];

  MPI_CHECK(MPI_Gather(chunkSum, floatsPerNode, MPI_FLOAT, vectorSumData, floatsPerNode, MPI_FLOAT, 0, context->comm));

  if (context->isRoot())
    return new FloatVector(vectorSumData, totalFloats);
  return NULL;
}

void FloatVector::debugPrint() {
  cout << len_ << " floats." << endl;
  for (int i = 0; i < len_; ++i)
    cout << data_[i] << ' ';
  cout << endl;
}
