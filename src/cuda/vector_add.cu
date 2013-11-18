#include "cuda/vector_add.h"

#include <cstdio>

#include "cuda/utils.h"
#include "mpi/utils.h"

#define THREADS_PER_BLOCK 512

__global__ void vectorAdd(float *Md, float *Nd, float *Pd, int width);

void cudaVectorAdd(float* chunk1, float* chunk2, float* chunkSum, int floatsPerNode) {
  float *gpuChunk1, *gpuChunk2, *gpuChunkSum;
  CUDA_CHECK(cudaMalloc((void**) &gpuChunk1, floatsPerNode * sizeof(float)));
  CUDA_CHECK(cudaMalloc((void**) &gpuChunk2, floatsPerNode * sizeof(float)));
  CUDA_CHECK(cudaMalloc((void**) &gpuChunkSum, floatsPerNode * sizeof(float)));

  CUDA_CHECK(cudaMemcpy(gpuChunk1, chunk1, floatsPerNode * sizeof(float), cudaMemcpyHostToDevice));
  CUDA_CHECK(cudaMemcpy(gpuChunk2, chunk2, floatsPerNode * sizeof(float), cudaMemcpyHostToDevice));

  int threadsPerBlock = THREADS_PER_BLOCK;
  int blocksPerGrid = floatsPerNode / THREADS_PER_BLOCK;
  if (floatsPerNode % THREADS_PER_BLOCK != 0)
    blocksPerGrid++;

  vectorAdd<<<blocksPerGrid,threadsPerBlock>>>(gpuChunk1, gpuChunk2, gpuChunkSum, floatsPerNode);

  CUDA_CHECK(cudaMemcpy(chunkSum, gpuChunkSum, floatsPerNode * sizeof(float), cudaMemcpyDeviceToHost));

  CUDA_CHECK(cudaFree(gpuChunk1));
  CUDA_CHECK(cudaFree(gpuChunk2));
  CUDA_CHECK(cudaFree(gpuChunkSum));
}

__global__ void vectorAdd(float *Md, float *Nd, float *Pd, int width) {
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  while (tid < width) {
    Pd[tid] = Md[tid] + Nd[tid];
    tid += blockDim.x * gridDim.x;
  }
}
