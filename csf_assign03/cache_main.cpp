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
      uint32_t tag;
      uint32_t load_ts; // FIFO
      uint32_t access_ts; // LRU
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

      Slot(uint32_t tag, uint32_t load_ts, uint32_t access_ts): tag(tag), load_ts(load_ts), access_ts(access_ts) {
         valid = true;
      }

      uint32_t get_Load_Time () {
         return load_ts;
      }

      uint32_t get_Access_Time () {
         return access_ts;
      }
      
      void inc_Access_Time(){
         access_ts++;
      }

      void set_Access_Time(uint32_t new_Access_Time) {
         access_ts = new_Access_Time;
      }
      
      void set_dirty() {
         dirty = true;
      }
      
      uint32_t get_tag() const {
         return tag;
      }

      bool get_valid() {
         return valid;
      }

      /*bool operator ==(Slot second) {
         return tag == second.get_tag();
      }*/
};

class Set {
   public:
      Set() {

      }

      Set(int size) : set(size) { 
         vector<Slot> temp(size);
         set = temp;
      }

      vector<Slot> set;
};

class Cache {
   public:
      uint32_t load_counter = 0;
      uint32_t store_counter = 0;  
      uint32_t load_hit_count = 0;
      uint32_t load_miss_count = 0;
      uint32_t store_hit_count = 0;
      uint32_t store_miss_count = 0;
      uint32_t total_cycle = 0; 

      //Default constructor
      Cache() {
         num_sets = 0;
         num_blocks = 0;
         block_size = 0;
      };
      
      Cache(uint32_t num_s, uint32_t num_b, uint32_t block_s, bool write_a, bool write_th, bool lru): 
         num_sets(num_s), num_blocks(num_b), block_size(block_s), write_allocate(write_a), write_through(!write_th), lru(lru) {
         bits_in_offset = log_2(block_size);
         bits_in_index = log_2(num_sets);
         bits_in_tag = 32 - bits_in_offset - bits_in_index;
         vector<Set> temp(num_s);
         cache = temp;
      };
         

      uint32_t decode_tag(uint32_t address) {
         if (bits_in_offset + bits_in_index == 32) { // undefined behaviour, must be edge case
            return 0;
         }

         address = address >> (bits_in_offset + bits_in_index);

         return address;
      }
      
      uint32_t decode_index(uint32_t address) {
         if (bits_in_offset + bits_in_tag == 32) { // undefined behaviour, must be edge case.
            return 0;            
         }

         address = address >> bits_in_offset;
         address = address << (bits_in_offset + bits_in_tag);
         address = address >> (bits_in_offset + bits_in_tag);

         return address;
      }

      void load_miss(uint32_t tag, uint32_t index) {
         creation_timestamp++;
         if (cache[index].set.size() >= num_blocks) {

            if (!write_through) {
               total_cycle += 100;
            }

            if (lru) {
               lru_remove(index);
            } else {
               fifo_remove(index);
            }
         }         

         cache[index].set.push_back(Slot(tag, creation_timestamp, store_counter));
         cache[index].set.back().set_Access_Time(access_timestamp);
      }

      void load(uint32_t tag, uint32_t index) {
         load_counter++;
         access_timestamp++;
         int found = find(index, tag); 
         
         if (found == -1)  {
            load_miss_count++;
            load_miss(tag, index);
            total_cycle += 100;
         } else {
            load_hit_count++;
            cache[index].set[found].set_Access_Time(access_timestamp);
            total_cycle++;
         }
      }

      void lru_remove(int index) {
         vector<Slot>::iterator lowest = cache[index].set.begin();

         // for each
         for (vector<Slot>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); it++) {
            if ((*it).get_Access_Time() <= (*lowest).get_Access_Time()) {
               lowest = it;
            }
         }

         cache[index].set.erase(lowest);
      }

      void fifo_remove(int index) {
         vector<Slot>::iterator lowest = cache[index].set.begin();

         //for each
         for (vector<Slot>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); it++) {
            if ((*it).get_Load_Time() < (*lowest).get_Load_Time()) {
               lowest = it;
            }
         }
         
         cache[index].set.erase(lowest);
      }

      int find(int index, uint32_t tag) {
         const vector<Slot> &set = cache[index].set;
         //for each
         for (vector<Slot>::const_iterator it = set.cbegin(); it != set.cend(); it++) {
            if ((*it).get_tag() == tag) {
               return it - set.begin();
            }
         }

         return -1;
      }

      void store(uint32_t tag, uint32_t index) {
         store_counter++;
         
         int found = find(index, tag);
               
         if (found == -1) {
            store_miss_count++;
            total_cycle += 100;
            if (write_allocate) {
               load_miss(tag, index);
               access_timestamp++;
               cache[index].set.back().set_Access_Time(access_timestamp);
               total_cycle++;
            } 
         } else {
            total_cycle++;
            store_hit_count++;
            cache[index].set[found].set_Access_Time(access_timestamp);
            if (!write_through) {
               access_timestamp++;
               cache[index].set[found].set_dirty();
            } else {
               total_cycle += 100;
            }
         }

      }

      uint32_t log_2(uint32_t num) {
         uint32_t result = 0;

         while (num != 1) { //might be wrong
            num = num >> 1;
            result++;
         }

         return result;
      }

   private:
      uint32_t num_sets;
      uint32_t num_blocks;
      uint32_t block_size;
      bool write_allocate;
      bool write_through;
      bool lru;
      vector<Set> cache;
      uint32_t creation_timestamp = 0;
      uint32_t access_timestamp = 0;

      uint32_t bits_in_offset = 0;
      uint32_t bits_in_index = 0;
      uint32_t bits_in_tag = 0;
};

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

   cout << "Total loads: " << cache.load_counter << endl;
   cout << "Total stores: " << cache.store_counter << endl;
   cout << "Load hits: " << cache.load_hit_count << endl;
   cout << "Load misses: " << cache.load_miss_count << endl;
   cout << "Store hits: " << cache.store_hit_count << endl;
   cout << "Store misses: " << cache.store_miss_count << endl;
   cout << "Total cycles: " << cache.total_cycle << endl;
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