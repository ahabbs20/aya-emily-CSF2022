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

string validate(int argc, int num_sets, int num_blocks, int block_size,
                bool *writeBack, bool *writeAllocate, bool *lru, string check_write_allocate,
                 string check_write_back, string lru_string); 

int main(int argc, char *argv[]) {
   uint32_t numSets = strtoul(argv[1], NULL, 10);
   uint32_t numBlocks = strtoul(argv[2], NULL, 10);
   uint32_t blockSize = strtoul(argv[3], NULL, 10);
   string check_write_allocate = argv[4];
   string check_write_back = argv[5];
   string lru_string = (string) argv[6];
   bool writeBack = false;
   bool writeAllocate = false;
   bool lru = false;
   
   string message = validate(argc, numSets, numBlocks, blockSize, &writeBack, &writeAllocate, &lru, check_write_allocate, check_write_back, lru_string);
   
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

bool string_equals(string s1, string s2) {
   return s1.compare(s2) == 0;
}

string check_size_and_sets(int num_sets, int num_blocks, int block_size) {
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
   }   return "";
}
 
string validate(int argc, int num_sets, int num_blocks, int block_size,
                bool *writeBack, bool *writeAllocate, bool *lru, string check_write_allocate,
                 string check_write_back, string lru_string) {

   if (argc != 7) {
      return "Error: Have not passed minimum number of arguments.\n";
   }

   if (!string_equals(check_write_allocate, "write-allocate") && !string_equals(check_write_allocate, "no-write-allocate")) {
      return "Error: (no-)write-allocate unspecified.\n";
   } else {
      *writeAllocate = string_equals(check_write_allocate, "write-allocate");
   }

   if (!string_equals(check_write_back, "write-through") && !string_equals(check_write_back, "write-back")) {
     return "Error: write-back/write-through unspecified.\n";
   } else {
     *writeBack = string_equals(check_write_back, "write-back"); 
   }

   if (!string_equals(lru_string, "lru") && !string_equals(lru_string, "fifo")) {
     return "Error: Lru or fifo unspecified.\n";
   } else {
     *lru = string_equals(lru_string, "lru"); //come back to this
   } 

   if ((!*writeAllocate) && *writeBack) { //come back to this too
      return "Error: Cannot have no_write_allocate and write_back.\n";
   }

   return check_size_and_sets(num_sets, num_blocks, block_size);
   }
   