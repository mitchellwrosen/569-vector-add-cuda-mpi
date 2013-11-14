#ifndef SRC_FLOAT_VECTOR_H_
#define SRC_FLOAT_VECTOR_H_

#include "mpi_context.h"

class FloatVector {
 public:
  static FloatVector* fromFile(const MpiContext& context, const char* filename);
  static FloatVector* sum(const MpiContext& context, const FloatVector* vec1, const FloatVector* vec2);

  // Getters.
  const char* filename() const { return filename_; }
  float* data() const { return data_; }
  int len() const { return len_;  }

 private:
  FloatVector(const MpiContext& context, const char* filename);

  const MpiContext& context_;
  const char* filename_;
  float* data_;
  int len_;
};

#endif  // SRC_FLOAT_VECTOR_H_
