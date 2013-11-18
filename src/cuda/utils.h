#ifndef SRC_CUDA_UTILS_H_
#define SRC_CUDA_UTILS_H_

#define CUDA_CHECK(call) \
  if ((call) != cudaSuccess) { \
    cudaError_t err = cudaGetLastError(); \
    fprintf(stderr, "CUDA error calling \""#call"\", code is %d\n", cudaGetLastError()); \
    mpiAbort(err); }

#endif  // SRC_CUDA_UTILS_H_
