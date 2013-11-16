#include "float_vector.h"

#include <boost/timer.hpp>
#include <iostream>
#include <fstream>
#include <mpi.h>
#include <vector>

#include "cuda/vector_add.h"
#include "mpi/context.h"
#include "mpi/utils.h"

using namespace std;

extern MpiContext* context;

FloatVector::FloatVector(const char* filename) : filename_(filename) {
  if (context->isRoot()) {
    cerr << "Reading " << filename << " on root node... ";
    boost::timer timer;

    ifstream infile(filename);

    float n;
    while (infile >> n)
      data_.push_back(n);

    cerr << timer.elapsed() << " seconds." << endl;
  }
}

FloatVector::FloatVector(float* data, int len) : filename_(NULL) {
  data_.assign(data, data + len);
}

FloatVector* FloatVector::fromFile(const char* filename) {
  // TODO: Return NULL if bad filename or something.
  return new FloatVector(filename);
}

FloatVector* FloatVector::sum(const FloatVector* vec1, const FloatVector* vec2) {
  int totalFloats;
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

    totalFloats = vec1->len();
    floatsPerNode = totalFloats / context->size;
  }

  MPI_CHECK(MPI_Bcast(&floatsPerNode, sizeof(int), MPI_INT, 0, context->comm));

  float* chunk1 = new float[floatsPerNode];
  float* chunk2 = new float[floatsPerNode];
  float* chunkSum = new float[floatsPerNode];

  boost::timer timer;
  if (context->isRoot())
     cerr << "Scattering " << floatsPerNode << " floats to each node... ";

  MPI_CHECK(MPI_Scatter(const_cast<float*>(vec1->data()), floatsPerNode, MPI_FLOAT, chunk1, floatsPerNode, MPI_FLOAT, 0, context->comm));
  MPI_CHECK(MPI_Scatter(const_cast<float*>(vec2->data()), floatsPerNode, MPI_FLOAT, chunk2, floatsPerNode, MPI_FLOAT, 0, context->comm));

  if (context->isRoot()) {
     cerr << timer.elapsed() << " seconds. " << endl << "Summing floats on GPU... ";
     timer.restart();
  }

  cudaVectorAdd(chunk1, chunk2, chunkSum, floatsPerNode);

  float* vectorSumData;
  if (context->isRoot()) {
    cerr << timer.elapsed() << "seconds." << endl;
    vectorSumData = new float[totalFloats];
    cerr << "Gathering floats... ";
    timer.restart();
  }

  MPI_CHECK(MPI_Gather(chunkSum, floatsPerNode, MPI_FLOAT, vectorSumData, floatsPerNode, MPI_FLOAT, 0, context->comm));

  if (context->isRoot()) {
    cerr << timer.elapsed() << " seconds." << endl;
    return new FloatVector(vectorSumData, totalFloats);
  }
  return NULL;
}

void FloatVector::debugPrint() {
  cout << data_.size() << " floats." << endl;
  for (vector<float>::const_iterator iter = data_.begin(); iter != data_.end(); ++iter)
    cout << *iter << ' ';
  cout << endl;
}
