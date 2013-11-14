#ifndef SRC_MPI_CONTEXT_H_
#define SRC_MPI_CONTEXT_H_

class MpiContext {
 public:
  MpiContext(int* argc, char*** argv);

  bool isRoot() const;
  void finalize();

  int size;
  int rank;
};

#endif  // SRC_MPI_CONTEXT_H_
