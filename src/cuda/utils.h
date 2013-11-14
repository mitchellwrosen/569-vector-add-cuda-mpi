#ifndef SRC_CUDA_UTILS_H_
#define SRC_CUDA_UTILS_H_

#define CUDA_CHECK(call) \
  if ((call) != cudaSuccess) { \
    cudaError_t err = cudaGetLastError(); \
    std::cerr << "CUDA error calling \""#call"\", code is " << err << std::endl; \
    abort(err); \
  }

#endif  // SRC_CUDA_UTILS_H_
