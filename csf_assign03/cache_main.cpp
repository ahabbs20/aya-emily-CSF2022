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
      Slot() {
         tag = 0;
         load_ts = 0;
         access_ts = 0;
         valid = false;
         dirty = false;

      }

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

      Set(int size, int m_slots) : max_slots(m_slots), set(size){ 
         vector<Slot> temp(size);
         set = temp;

      }
      
      int max_slots;
      vector<Slot> set;
};

class Cache {
   /* TODO:
      EMILY!!!! LOOK OVER HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      Emily's Tasks: 
         - store
         - find

      Aya's Tasks: 
         - Load
         - evict_Lru
         - evict_Fifo

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
         num_sets(num_s), num_blocks(num_b), block_size(block_s), write_allocate(*write_a), write_through(*write_th) {
           //Add vector in vectors here lmao
           bits_in_offset = log_2(block_size, 0);
           bits_in_index = log_2(num_blocks, 0);
           bits_in_tag = 64 - bits_in_offset - bits_in_index;

           vector<Set> temp(num_s);
           cache = temp;
         };
         
      
      //you have number of sets (num_sets)
      //number of blocks (num_blocks)
      //block size (block_size)

      //tag index offset
      unsigned int decode_tag(unsigned int address) {
         //shift out offset bits
         if (bits_in_offset + bits_in_index == 64) { // undefined behaviour, must be edge case
            return 0;
         }
         address = address >> (bits_in_offset + bits_in_index);

         return address;
      }
      
      unsigned int decode_index(unsigned int address) {
         if (bits_in_offset + bits_in_tag == 64) { // undefined behaviour, must be edge case.
            return 0;            
         }

         address = address >> bits_in_offset;
         address = address << bits_in_offset + bits_in_tag;
         address = address >> bits_in_offset + bits_in_tag;

         return address;
      }

      
      void load(unsigned int tag, unsigned int index) {
         /*
          go to index, look for slot
          if slot is found, do nothing
          if slot if not found;
            if (number of valid blocks in the set != max number of possible blocks) {
             add it
            } else {
               // if (LRU) {
                  remove_lru()
                  add it
               } else {
                  remove_fifo
                  add it
               }
            }
         */
      }

      /*
         remove lru
            set lru = first slot
            iterate thru
               if strictly less than, replace lru
            delete lru 


         remove fifo
            set first in = first slot
            iterate thru
               if strictly less than, replace first in
            delete first in

      */


      void write(unsigned int tag, unsigned int index) {
         /*
            if (hit) {
               if (write back)
                  mark dirty
            else 
               if write-allocate
                  load()
               else {}
            }


         */
      }

   private:
      int num_sets;
      int num_blocks;
      int block_size;
      bool write_allocate;
      bool write_through;
      vector<Set> cache;

      int load_hit_count = 0;
      int load_miss_count = 0;
      int store_hit_count = 0;
      int store_miss_count = 0;
      int total_cycle = 0;      

      unsigned int bits_in_offset = 0;
      unsigned int bits_in_index = 0;
      unsigned int bits_in_tag = 0;

      int log_2(int num, int result) {
         while (num != 1) {
            num = num >> 1;
            result++;
         }
         return result;
      }

};
bool isPowerOfTwo(int num) {
   return ((num & (num - 1)) == 0) && (num != 0);
}

string validate(int argc, int num_sets, int num_blocks, int block_size, bool *writeBack, bool *writeAllocate, string result1, string result2); 

int main(int argc, char *argv[]) {
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


   Cache cache = Cache(numSets, numBlocks, blockSize, &writeAllocate, &writeBack);

   char  op;
   char * adrs;
   char theRest;

   unsigned int tag;
   unsigned int index;
   unsigned int address;
   
   while (cin.peek() != EOF) {
      cin.clear();
      cin >> op >> adrs >> theRest;
      address = strtoul(adrs, NULL, 16);
      tag = cache.decode_tag(address);
      index = cache.decode_index(address);

      cout << tag << " " << index << "\n";
      /*
      if (op == 'l') {
         cache.load(tag, index);
      } else {
         cache.store(tag, index);
      }*/

      
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

   if (!isPowerOfTwo(num_blocks)) {
      return "Error: Number of blocks must be power of two.\n";
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
