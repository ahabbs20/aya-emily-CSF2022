#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>
#include "Slot.h"
#include "Statistics.h"
#include "Set.h"

using namespace std;

class Cache {
   public:
      Statistics holder;

      //Default constructor
      Cache();
      
      Cache(uint32_t num_s, uint32_t num_b, uint32_t block_s, bool write_a, bool write_th, bool lru);
         

      uint32_t decode_tag(uint32_t address);
      
      uint32_t decode_index(uint32_t address);

      void load_miss(uint32_t tag, uint32_t index);

      void load(uint32_t tag, uint32_t index);

      void lru_remove(int index);

      void fifo_remove(int index);

      int find(int index, uint32_t tag);

      void store(uint32_t tag, uint32_t index);

      uint32_t log_2(uint32_t num);

   private:
      uint32_t num_sets;
      uint32_t num_blocks;
      uint32_t block_size;
      uint32_t cycle_memory;
      bool write_allocate;
      bool write_through;
      bool lru;
      vector<Set> cache;
      uint32_t creation_timestamp;
      uint32_t access_timestamp;

      uint32_t bits_in_offset;
      uint32_t bits_in_index;
      uint32_t bits_in_tag;
};

#endif
