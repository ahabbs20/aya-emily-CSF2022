// Main Cache function
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

/*
   isPowerOfTwo Code:
      https://stackoverflow.com/questions/57025836/how-to-check-if-a-given-number-is-a-power-of-two

*/

#include <iostream>

using namespace std;

bool isPowerOfTwo(int num) {
   return ((num & (num - 1)) == 0) && (num != 0);
}

int main(int argc, char *argv[]) {
   // number of sets in the cache
   // number of blocks in each set
   // number of bytes in each block
   // write-allocate or no-write allocate
   // write through or write back
   // lru or fifo

   int numSets = atoi(argv[1]);
   int numBlocks = atoi(argv[2]);
   int blockSize = atoi(argv[3]);
   bool writeBack = false;
   bool writeAllocate = false;

   if (argc < 6) {
      cerr << "Error: Have not passed minimum number of arguments\n";
      return -1;
   }


   if (!isPowerOfTwo(numSets)) {
      cerr << "Error: Number of sets must be power of two.\n";
      return -1;
   }

   if (blockSize < 4) {
      cerr << "Error: block size is less than 4.\n";
      return -1;
   } else if (!isPowerOfTwo(blockSize)) {
      cerr << "Error: block size must be power of 2.\n";
      return -1;
   }

   string result = argv[4];
   if (result.compare("write-allocate") == 0) {
      writeAllocate = true;
   } else if (result.compare("no-write-allocate") == 0) {
      writeAllocate = false;
   } else {
      cerr << "Error: (no-)write-allocate unspecified.\n";
      return -1;
   }

   result = argv[5];
   if (result.compare("write-through") == 0) {
      writeBack = false;
   } else if (result.compare("write-back") == 0) {
      writeBack = true;
   } else {
      cerr << "Error: write-back/write-through unspecified\n";
      return -1;
   }

   return 0;
}
