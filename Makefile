###############################################################################
# C++ / MPI                                                                   #
###############################################################################
CC=mpic++

CFLAGS=-Wall -O3
LDFLAGS=-Wall

MAIN=vectoradd.o
SRCS=$(filter-out src/cuda/*.cpp, $(wildcard src/*.cpp src/**/*.cpp))
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
INC=-Isrc
LIBS=-lmpi

###############################################################################
# CUDA                                                                        #
###############################################################################
CUDA_CC=nvcc
CUDA_CFLAGS=-O3 -DTHREADS_PER_BLOCK=1024

CUDA_SRCS=$(wildcard src/cuda/*.cu)
CUDA_OBJS=$(patsubst %.cu,%.o,$(CUDA_SRCS))
CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

###############################################################################

all: vectoradd

debug: CC += -DDEBUG
debug: CUDA_CC += -DDEBUG
debug: vectoradd

vectoradd: makeDirectories $(OBJS) $(CUDA_OBJS) $(MAIN)
	$(CC) $(LDFLAGS) $(INC) $(CUDA_INC) $(OBJS) $(CUDA_OBJS) $(MAIN) $(LIBS) $(CUDA_LIBS) -o bin/vectoradd

encode:
	$(CC)

$(CUDA_OBJS): %.o : %.cu
	$(CUDA_CC) $(INC) $(CUDA_INC) $(CUDA_CFLAGS) -c $< -o $@

$(OBJS) $(MAIN): %.o : %.cpp
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

.PHONY: makeDirectories
makeDirectories:
	@mkdir -p bin

clean:
	@rm -f $(OBJS) $(MAIN) $(CUDA_OBJS)
	@rm -rf bin
