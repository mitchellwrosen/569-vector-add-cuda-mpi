CC=mpic++
NVCC=nvcc
CFLAGS=-Wall -O3
LDFLAGS=-Wall

###############################################################################
# Source                                                                      #
###############################################################################
MAIN=main.o

SRCS=$(filter-out src/cuda/*.cpp, $(wildcard src/*.cpp src/**/*.cpp))
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
INC=-Isrc
LIBS=-lmpi

CUDA_SRCS=$(wildcard src/cuda/*.cu)
CUDA_OBJS=$(patsubst %.cu,%.o,$(CUDA_SRCS))
CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

###############################################################################

all: makeDirectories $(OBJS) $(CUDA_OBJS) $(MAIN)
	$(CC) $(LDFLAGS) $(INC) $(CUDA_INC) $(OBJS) $(CUDA_OBJS) $(MAIN) $(LIBS) $(CUDA_LIBS) -o bin/main

encode:
	ghc -O2 ./Encode.hs -o encode

$(CUDA_OBJS): %.o : %.cu %.h
	$(NVCC) $(INC) $(CUDA_INC) -O3 -c $< -o $@

$(OBJS): %.o : %.cpp %.h
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

.PHONY: makeDirectories
makeDirectories:
	@mkdir -p bin

clean:
	@rm -f $(OBJS) $(TEST_OBJS)
