#ifndef SRC_FLOAT_VECTOR_H_
#define SRC_FLOAT_VECTOR_H_

#include <iostream>

#define MIN_VAL -20.0
#define MAX_VAL 20.0
#define NUM_BINS 80
#define BIN_WIDTH 0.50

using std::ostream;

/**
 * A distributed float vector, with operations that utilize MPI and/or CUDA.
 * Some functions only return sensible values on the root node, although these
 * functions should still be called by all nodes (i.e. not inside an if-guard).
 */
class FloatVector {
 public:
  /**
   * Create a distributed vector from a binary-encoded file (first integer is
   * the number of floats, 32-bit little endian, followed by the floats).
   */
  explicit FloatVector(const char* filename);
  virtual ~FloatVector();

  /**
   * Sum two distributed vectors, either from vectors or vector files. The
   * summed vector is also distributed.
   */
  static FloatVector* sum(const FloatVector* vec1, const FloatVector* vec2);
  static FloatVector* sum(const char* vecfile1, const char* vecfile2);

  /**
   * Creates a histogram from this vector, with min/max/width constants defined
   * at compile time inside this file. The returned histogram is only meaningful
   * on the root process - all others will return NULL.
   */
  int* histogram();

  /**
   * Print the contents of this vector to stdout, or to the specified file.
   */
  void debugPrint();
  void debugPrint(const char* filename);

  // Getters.
  const char* filename() const { return filename_; }
  float* data() const { return data_; }
  int len() const { return len_; }

 private:
  FloatVector(float* data, int len);

  int getBinNum(float val);
  void debugPrint(ostream& stream);

  const char* filename_;
  float* data_;
  int len_;
};

#endif  // SRC_FLOAT_VECTOR_H_
