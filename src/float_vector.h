#ifndef SRC_FLOAT_VECTOR_H_
#define SRC_FLOAT_VECTOR_H_

#include <vector>

using namespace std;

class FloatVector {
 public:
  static FloatVector* fromFile(const char* filename);

  /**
   * Sum two vectors (distributed & accelerated). Returns NULL on all processes except the root process.
   * Returns NULL on the root process if the vectors are unequal length, or other similar errors.
   */
  static FloatVector* sum(const FloatVector* vec1, const FloatVector* vec2);

  void debugPrint();

  // Getters.
  const char* filename() const { return filename_; }
  const float* data() const { return data_.data(); }
  int len() const { return data_.size();  }

 private:
  FloatVector(const char* filename);
  FloatVector(float* data, int len);

  const char* filename_;
  vector<float> data_;
};

#endif  // SRC_FLOAT_VECTOR_H_
