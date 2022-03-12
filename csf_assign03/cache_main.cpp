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

      unsigned int get_Access_Time () {
         return access_ts;
      }
      
      void set_dirty() {
         dirty = true;
      }
      
      unsigned int get_tag() {
         return tag;
      }

      bool operator ==(Slot second) {
         /*
            Have to define equality but also think about it- we do some 
            comparisons to NULL and how would that need to work
         */
         if (tag == second.get_tag()) {
            return true;
         }
         return false;
      }
      
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
      
      //Probably main constructor --> !!!!!!! Why are we passing bool pointers? We can just pass bools... pointers were because we didnt' want to refactor validate
      //and it needed to set stuff that will be also used in main
      Cache(int num_s, int num_b, int block_s, bool * write_a, bool * write_th, bool * lru): 
         num_sets(num_s), num_blocks(num_b), block_size(block_s), write_allocate(*write_a), write_through(*write_th), lru(*lru) {
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
         
         // Deadass struggling to define this function...
         Slot found = find(cache[index].set, tag); //*(find(cache[index]));
         Slot blank = Slot();
         if (found == blank)  {
            vector<Slot> target_set = cache[index].set;

            if (target_set.size() < cache[index].max_slots) {
               target_set.push_back(Slot(tag, load_counter, access_counter));
            } else {
               if (lru) {
                  lru_remove(target_set);
                  target_set.push_back(Slot(tag, load_counter, access_counter));
                  load_counter++;
               } else {
                  fifo_remove(target_set);
                  target_set.push_back(Slot(tag, load_counter, access_counter));
                  load_counter++;
               }

            }


         }

      }

      void lru_remove(vector<Slot> set) {
         vector<Slot>::iterator lowest = set.begin();

         for (vector<Slot>::iterator it = set.begin(); it != set.end(); it++) {
            if ((*it).get_Access_Time() < (*lowest).get_Access_Time()) {
               lowest = it;
            }
         }

         set.erase(lowest);
      }

      void fifo_remove(vector<Slot> set) {
         vector<Slot>::iterator lowest = set.begin();

         for (vector<Slot>::iterator it = set.begin(); it != set.end(); it++) {
            if ((*it).get_Load_Time() < (*lowest).get_Load_Time()) {
               lowest = it;
            }
         }

         set.erase(lowest);
      }

      Slot find(vector<Slot> set, unsigned int tag) {
         if (set.size() == 1) {             //not sure if this is necessary
            return set[0];
         }

         Slot target = Slot();
         for (vector<Slot>::iterator it = set.begin(); it != set.end(); it++) {
            if ((*it).get_tag() == tag) {
               target = (*it);
            }
         }
         return target;
      }


      void store(unsigned int tag, unsigned int index) {
         Slot found = (find(cache[index].set, tag));
         Slot blank = Slot();
         //TODO: DEFINE SLOT EQUALITY SOMEWHERE
         if (found == blank) {
            if (*write_allocate) {
               load(tag, index);
            }
         } else {
            if (!*write_through) {
               found.set_dirty();
            } else {

            }
         }
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

      
      int log_2(int num, int result) {
         while (num != 1) {
            num = num >> 1;
            result++;
         }
         return result;
      }

   private:
      int num_sets;
      int num_blocks;
      int block_size;
      bool *write_allocate;
      bool *write_through;
      bool lru;
      vector<Set> cache;

      int load_hit_count = 0;
      int load_miss_count = 0;
      int store_hit_count = 0;
      int store_miss_count = 0;
      int total_cycle = 0; 

      int load_counter;
      int access_counter;     

      unsigned int bits_in_offset = 0;
      unsigned int bits_in_index = 0;
      unsigned int bits_in_tag = 0;

};

bool isPowerOfTwo(int num) {
   return ((num & (num - 1)) == 0) && (num != 0);
}

string validate(int argc, char * argv[],  int num_sets, int num_blocks, int block_size, bool *writeBack,  
               bool *writeAllocate, bool *lru, string check_write_allocate, string check_write_back); 

int main(int argc, char *argv[]) {
   int numSets = atoi(argv[1]);
   int numBlocks = atoi(argv[2]);
   int blockSize = atoi(argv[3]);
   bool writeBack = false;
   bool writeAllocate = false;
   bool lru = false;
   string check_write_allocate = argv[4];
   string check_write_back = argv[5];

   string message = validate(argc, argv, numSets, numBlocks, blockSize, &writeBack, &writeAllocate, &lru, check_write_allocate, check_write_back);
   if (message.length() != 0) {
      cerr << message;
      return -1;
   }


   Cache cache = Cache(numSets, numBlocks, blockSize, &writeAllocate, &writeBack, &lru);

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
   } else if (check_write_back.compare("no-write-allocate") == 0) {
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

   if (!writeAllocate && writeBack) { //come back to this too
      return "Error: Cannot have no_write_allocate and write_back\n";
   }



   return "";
} 
