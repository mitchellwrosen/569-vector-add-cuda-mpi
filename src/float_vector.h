#ifndef SRC_FLOAT_VECTOR_H_
#define SRC_FLOAT_VECTOR_H_

#include "mpi/context.h"

class FloatVector {
 public:
  static FloatVector* fromFile(const MpiContext& context, const char* filename);

  /**
   * Sum two vectors (distributed & accelerated). Returns NULL on all processes except the root process.
   * Returns NULL on the root process if the vectors are unequal length, or other similar errors.
   */
  static FloatVector* sum(const MpiContext& context, const FloatVector* vec1, const FloatVector* vec2);

  void debugPrint();

  // Getters.
  const char* filename() const { return filename_; }
  float* data() const { return data_; }
  int len() const { return len_;  }

 private:
  FloatVector(const MpiContext& context, const char* filename);
  FloatVector(const MpiContext& context, float* data, int len);

  const MpiContext& context_;
  const char* filename_;
  float* data_;
  int len_;
};

#endif  // SRC_FLOAT_VECTOR_H_
