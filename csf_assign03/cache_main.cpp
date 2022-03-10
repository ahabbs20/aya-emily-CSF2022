// Main Cache function
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

/*
   isPowerOfTwo Code:
      https://stackoverflow.com/questions/57025836/how-to-check-if-a-given-number-is-a-power-of-two

*/


#include <iostream>
#include <vector>

using namespace std;
using std::vector;


class Slot {
   private: 
      unsigned int tag;
      unsigned int load_ts; // FIFO
      unsigned int access_ts; // LRU
      bool valid;
      bool dirty;


   public: 
      Slot(unsigned int tag, unsigned int load_ts, unsigned int access_ts): tag(tag), load_ts(load_ts), access_ts(access_ts) {
         valid = true;
      }

      unsigned int get_Load_Time () {
         return load_ts;
      }

      unsigned int get_access_Time () {
         return access_ts;
      }
      
      void set_dirty() {
         dirty = true;
      }
      

   private:
      
};

class Set {
   

   public:

      Set(int size, int m_slots) : set(size), max_slots(m_slots) { }

      int max_slots;
      vector<Slot> set;
};

class Cache {
   /* TODO:
      var that tracks how much of the address to read for tag
      var that tracks how much of address to read for index

   */

   public:
      //Default constructor
      Cache() {
         num_sets = 0;
         num_blocks = 0;
         block_size = 0;
      };
      
      //Probably main constructor
      Cache(int num_s, int num_b, int block_s, bool * write_a, bool * write_th): 
         num_sets(num_s), num_blocks(num_b), block_size(block_s), write_allocate(*write_a), write_through(*write_th) {};
         
      unsigned int decodeTag(unsigned int address) {
         return 0;
      }
      
      unsigned int decode_index(unsigned int address) {
         return 0;
      }

      void load(unsigned int tag, unsigned int index) {
         
      }

      void write(unsigned int tag, unsigned int index) {

      }

   private:
      bool write_allocate;
      bool write_through;
      int num_sets;
      int num_blocks;
      int block_size;
      vector<Set> cache;

      int load_hit_count = 0;
      int load_miss_count = 0;
      int store_hit_count = 0;
      int store_miss_count = 0;
      int total_cycle = 0;      

};

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
   string result1 = argv[4];
   string result2 = argv[5];

   string message = validate(argc, numSets, numBlocks, blockSize, &writeBack, &writeAllocate, result1, result2);
   if (message.length() != 0) {
      cerr << message;
      return -1;
   }


   Cache cache = Cache(numSets, numBlocks, blockSize, writeAllocate, writeBack);

   char  op;
   string adrs;
   string theRest;

   unsigned int tag;
   unsigned int index;
   
   while (cin.peek() != EOF) {
      cin >> operation >> address >> theRest;
      unsigned int address = atoi(adrs);
      tag = cache.decode_tag(address);
      index = cache.decode_index(address);

      if (op == 'l') {
         cache.load(tag, index);
      } else {
         cache.store(tag, index);
      }

   }

   /*
   TODO
      print everything
   */

   return 0;
}

string validate(int argc, int num_sets, int num_blocks, int block_size, bool *writeBack, bool *writeAllocate, string result1, string result2) {
   if (argc < 6) {
      return "Error: Have not passed minimum number of arguments\n";
   }

   if (!isPowerOfTwo(num_sets)) {
      return "Error: Number of sets must be power of two.\n";
   }

   if (block_size < 4) {
      return "Error: block size is less than 4.\n";
   } else if (!isPowerOfTwo(block_size)) {
      return "Error: block size must be power of 2.\n";
   }

   if (result1.compare("write-allocate") == 0) {
      *writeAllocate = true; //come back to it
   } else if (result2.compare("no-write-allocate") == 0) {
      *writeAllocate = false; //come back to it
   } else {
      return "Error: (no-)write-allocate unspecified.\n";
   }

   if (result2.compare("write-through") == 0) {
      *writeBack = false; //come back to this
   } else if (result2.compare("write-back") == 0) {
      *writeBack = true; //come back to this
   } else {
      return "Error: write-back/write-through unspecified\n";
   }

   if (!writeAllocate && writeBack) { //come back to this too
      return "Error: Cannot have no_write_allocate and write_back\n";
   }

   return "";
} 
