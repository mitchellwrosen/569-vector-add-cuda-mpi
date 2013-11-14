#ifndef SRC_VECTOR_UTILS_H_
#define SRC_VECTOR_UTILS_H_

#include <iostream>
#include <vector>

using namespace std;

template <class T>
void printVector(T* vec, int len) {
  for (int i = 0; i < len; ++i)
    cout << vec[i] << ' ';
  cout << endl;
}

#endif  // SRC_VECTOR_UTILS_H_
