CC=mpic++
CFLAGS=-Wall -O3
LDFLAGS=-Wall
INC_DIRS=-Isrc

###############################################################################
# Source                                                                      #
###############################################################################
MAIN=main.o

SRCS=$(wildcard src/*.cpp src/**/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
LIBS=-lmpi

###############################################################################
# Test                                                                        #
###############################################################################
TEST_MAIN=test/main.o

TEST_SRCS=$(SRCS) $(wildcard test/*.cpp test/**/*.cpp)
TEST_OBJS=$(patsubst %.cpp,%.o,$(TEST_SRCS))
TEST_LIBS=-lgtest

###############################################################################

all: makeDirectories $(OBJS) $(MAIN)
	$(CC) $(LDFLAGS) $(INC_DIRS) $(OBJS) $(MAIN) $(LIBS) -o bin/main

encode:
	ghc -O2 ./Encode.hs -o encode

test: makeDirectories $(TEST_OBJS)
	$(CC) $(LDFLAGS) $(INC_DIRS) $(TEST_OBJS) $(TEST_LIBS) -o bin/test-linux

%.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@

.PHONY: makeDirectories
makeDirectories:
	@mkdir -p bin

clean:
	@rm -f $(OBJS) $(TEST_OBJS)
