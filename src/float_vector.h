#ifndef SRC_FLOAT_VECTOR_H_
#define SRC_FLOAT_VECTOR_H_

#define MIN_VAL -20.0
#define MAX_VAL 20.0
#define NUM_BINS 80
#define BIN_WIDTH 0.50

/**
 * A distributed float vector, with operations that utilize MPI and/or CUDA.
 */
class FloatVector {
 public:
  FloatVector(const char* filename);

  static FloatVector* sum(const FloatVector* vec1, const FloatVector* vec2);

  /**
   * Creates a histogram from this vector. The returned histogram is only
   * meaningful on the root process - all others will return NULL.
   */
  int* histogram();

  void debugPrint();

  // Getters.
  const char* filename() const { return filename_; }
  float* data() const { return data_; }
  int len() const { return len_; }

 private:
  FloatVector(float* data, int len);

  int getBinNum(float val);

  const char* filename_;
  float* data_;
  int len_;
};

#endif  // SRC_FLOAT_VECTOR_H_
