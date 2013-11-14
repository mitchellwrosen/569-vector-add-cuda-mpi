#ifndef SRC_CUDA_VECTOR_ADD_H_
#define SRC_CUDA_VECTOR_ADD_H_

void cudaVectorAdd(float* chunk1, float* chunk2, float* chunkSum, int floatsPerNode);

#endif  // SRC_CUDA_VECTOR_ADD_H_
