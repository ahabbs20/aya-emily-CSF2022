// Main Cache function
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

/*
   isPowerOfTwo Code:
      https://stackoverflow.com/questions/57025836/how-to-check-if-a-given-number-is-a-power-of-two

*/


#include <iostream>
#include <vector>
#include "Cache.h"

using namespace std;

bool isPowerOfTwo(int num) {
   return ((num & (num - 1)) == 0) && (num != 0);
}

string validate(int argc, char * argv[],  int num_sets, int num_blocks, int block_size, bool *writeBack, bool *writeAllocate, bool *lru, string check_write_allocate, string check_write_back); 

int main(int argc, char *argv[]) {
   uint32_t numSets = strtoul(argv[1], NULL, 10);
   uint32_t numBlocks = strtoul(argv[2], NULL, 10);
   uint32_t blockSize = strtoul(argv[3], NULL, 10);
   string check_write_allocate = argv[4];
   string check_write_back = argv[5];
   bool writeBack = false;
   bool writeAllocate = false;
   bool lru = false;
   
   string message = validate(argc, argv, numSets, numBlocks, blockSize, &writeBack, &writeAllocate, &lru, check_write_allocate, check_write_back);
   
   if (message.compare("") != 0) {
      cerr << message;
      return -1;
   }

   Cache cache = Cache(numSets, numBlocks, blockSize, writeAllocate, writeBack, lru);

   char op;
   string tempAddress; 
   string theRest;

   uint32_t tag;
   uint32_t index;
   uint32_t address;
   
   while (cin >> op >> tempAddress >> theRest) {
      cin.clear();
      const char* adrs = tempAddress.c_str();
      address = strtoul(adrs, NULL, 16);
      tag = cache.decode_tag(address);
      index = cache.decode_index(address);
      if (op == 'l') {
         cache.load(tag, index);
      } else if (op == 's') {
         cache.store(tag, index);
      }
   }

   cache.holder.output_statistics();

   return 0;
}

string validate(int argc, char * argv[], int num_sets, int num_blocks, int block_size, bool *writeBack, 
               bool *writeAllocate, bool *lru, string check_write_allocate, string check_write_back) {
   if (argc < 6) {
      return "Error: Have not passed minimum number of arguments\n";
   }

   if (!isPowerOfTwo(num_sets)) {
      return "Error: Number of sets must be power of two.\n";
   }

   if (!isPowerOfTwo(num_blocks)) {
      return "Error: Number of blocks must be power of two.\n";
   }

   if (block_size < 4) {
      return "Error: block size is less than 4.\n";
   } else if (!isPowerOfTwo(block_size)) {
      return "Error: block size must be power of 2.\n";
   }

   if (check_write_allocate.compare("write-allocate") == 0) {
      *writeAllocate = true; //come back to it
   } else if (check_write_allocate.compare("no-write-allocate") == 0) {
      *writeAllocate = false; //come back to it
   } else {
      return "Error: (no-)write-allocate unspecified.\n";
   }

   if (check_write_back.compare("write-through") == 0) {
      *writeBack = false; //come back to this
   } else if (check_write_back.compare("write-back") == 0) {
      *writeBack = true; //come back to this
   } else {
      return "Error: write-back/write-through unspecified\n";
   }

   if (argc == 7) {
      string lru_string = (string) argv[6];
      if (lru_string.compare("lru") == 0) {
         *lru = true;
      } else if (lru_string.compare("fifo") == 0) {
         *lru = false;
      } else {
         return "Error: Unable to parse parameter.";
      }
   } else {
      *lru = false;
   }

   if ((!*writeAllocate) && *writeBack) { //come back to this too
      return "Error: Cannot have no_write_allocate and write_back\n";
   }

   return "";
}