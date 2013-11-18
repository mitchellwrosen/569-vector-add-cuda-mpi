#include "float_vector.h"

#include <boost/scoped_ptr.hpp>
#include <boost/timer.hpp>
#include <cstring>
#include <iostream>
#include <fstream>
#include <mpi.h>

#include "cuda/vector_add.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;
using boost::scoped_ptr;

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

  infile.seekg(sizeof(int) + len_ * sizeof(float) * context->rank);
  for (int i = 0; i < len_; ++i)
    infile.read((char*) (data_ + i), sizeof(float));
}

FloatVector::FloatVector(float* data, int len) : filename_(NULL), data_(data), len_(len) {
}

FloatVector::~FloatVector() {
  if (data_)
    delete[] data_;
}

FloatVector* FloatVector::sum(const FloatVector* vec1, const FloatVector* vec2) {
  if (vec1->len() != vec2->len()) {
    fprintf(stderr, "Unequal vector lengths: %d (%s), %d (%s)\n",
        vec1->len(), vec1->filename(), vec2->len(), vec2->filename());
    mpiAbort(-1);
  }

  int floatsPerNode = vec1->len();
  float* chunkSum = new float[floatsPerNode];

  boost::timer timer;
  cudaVectorAdd(vec1->data(), vec2->data(), chunkSum, floatsPerNode);
  fprintf(stderr, "Summed %d floats on node %d (%0.1lf seconds)\n", floatsPerNode, context->rank, timer.elapsed());

  return new FloatVector(chunkSum, floatsPerNode);
}

FloatVector* FloatVector::sum(const char* vecfile1, const char* vecfile2) {
  boost::timer timer;
  scoped_ptr<FloatVector> vec1(new FloatVector(vecfile1));
  fprintf(stderr, "Read %d floats from %s on node %d (%0.1lf seconds)\n", vec1->len(), vecfile1, context->rank, timer.elapsed());

  timer.restart();
  scoped_ptr<FloatVector> vec2(new FloatVector(vecfile2));
  fprintf(stderr, "Read %d floats from %s on node %d (%0.1lf seconds)\n", vec2->len(), vecfile2, context->rank, timer.elapsed());

  return FloatVector::sum(vec1.get(), vec2.get());
}

int* FloatVector::histogram() {
  int* localBins = new int[NUM_BINS]();

  for (int i = 0; i < len_; ++i)
    localBins[getBinNum(data_[i])]++;

  int *histogram = NULL;
  if (context->isRoot())
    histogram = new int[NUM_BINS]();

  context->reduce(localBins, histogram, NUM_BINS, MPI_INT, MPI_SUM);
  delete[] localBins;
  return histogram;
}

int FloatVector::getBinNum(float val) {
  int binNum = (val - MIN_VAL) / BIN_WIDTH;
  if (binNum == NUM_BINS)
     binNum--;
  return binNum;
}

void FloatVector::debugPrint() {
  debugPrint(cout);
}

void FloatVector::debugPrint(const char* filename) {
  ofstream outFile(filename);
  debugPrint(outFile);
}

void FloatVector::debugPrint(ostream& stream) {
  int totalFloats = len_ * context->size;

  float* totalData = NULL;
  if (context->isRoot())
    totalData = new float[totalFloats];

  context->gather(data_, len_, MPI_FLOAT, totalData, len_, MPI_FLOAT);

  if (context->isRoot()) {
    stream << totalFloats << " floats." << endl;
    for (int i = 0; i < totalFloats; ++i)
      stream << totalData[i] << ' ';
    stream << endl;
  }
}
