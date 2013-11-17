#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void printUsage(char* progname);
void encode(const char* filename);
void readFloatsFile(const char* filename, vector<float>* nums);
void decode(const char* filename);

int main(int argc, char** argv) {
  if (argc != 2)
    printUsage(argv[0]);
  encode(argv[1]);
}

void printUsage(char* progname) {
  printf("Usage: %s infile > outfile\n", progname);
  exit(EXIT_FAILURE);
}

void encode(const char* filename) {
  vector<float> nums;
  readFloatsFile(filename, &nums);

  int len = nums.size();
  cout.write((const char*) &len, sizeof(int));          // First byte, length
  for (int i = 0; i < len; ++i)
    cout.write((const char*) &nums[i], sizeof(float));  // Remaining bytes, binary floats
}

void readFloatsFile(const char* filename, vector<float>* nums) {
  float n;
  ifstream infile(filename);
  while (infile >> n)
    nums->push_back(n);
}

// Here's how you might decode
void decode(const char* filename) {
  ifstream infile(filename, ios::binary);

  int len;
  infile.read((char*) &len, sizeof(int));
  float* nums = new float[len];

  for (int i = 0; i < len; ++i)
    infile.read((char*) (nums + i), sizeof(float));
}
