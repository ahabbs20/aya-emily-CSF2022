#include "Cache.h"

Cache::Cache() {
  num_sets = 0;
  num_blocks = 0;
  block_size = 0;
  creation_timestamp = 0;
  access_timestamp = 0;
  bits_in_offset = 0;
  bits_in_index = 0;
  bits_in_tag = 0;
  holder = Statistics();
}
      
Cache::Cache(uint32_t num_s, uint32_t num_b, uint32_t block_s, bool write_a, bool write_th, bool lru): 
  num_sets(num_s), num_blocks(num_b), block_size(block_s), write_allocate(write_a), write_through(!write_th), lru(lru) {
  bits_in_offset = log_2(block_size);
  bits_in_index = log_2(num_sets);
  bits_in_tag = 32 - bits_in_offset - bits_in_index;
  cycle_memory = 100 * (block_size / 4);
  vector<Set> temp(num_s);
  cache = temp;
  creation_timestamp = 0;
  access_timestamp = 0;
  holder = Statistics();
}
         

uint32_t Cache::decode_tag(uint32_t address) {
  if (bits_in_offset + bits_in_index == 32) { // undefined behaviour, must be edge case
    return 0;
  }

  address = address >> (bits_in_offset + bits_in_index);

  return address;
}
      
uint32_t Cache::decode_index(uint32_t address) {
  if (bits_in_offset + bits_in_tag == 32) { // undefined behaviour, must be edge case.
    return 0;            
  }

  address = address >> bits_in_offset;
  address = address << (bits_in_offset + bits_in_tag);
  address = address >> (bits_in_offset + bits_in_tag);

  return address;
}  

void Cache::load_miss(uint32_t tag, uint32_t index) {
  creation_timestamp++;
  if (cache[index].set.size() >= num_blocks) {
    if (!write_through) {
      holder.inc_total(cycle_memory);
    }
    if (lru) {
      lru_remove(index);
    } else {
      fifo_remove(index);
    }
  }         

  cache[index].set.push_back(Slot(tag, creation_timestamp, holder.get_store_counter()));
  cache[index].set.back().set_Access_Time(access_timestamp);
}

void Cache::load(uint32_t tag, uint32_t index) {
  holder.inc_load_counter();
  access_timestamp++;
  int found = find(index, tag); 
         
  if (found == -1)  {
    holder.inc_total(cycle_memory);
    holder.inc_load_miss_count();
    load_miss(tag, index);
  } else {
    holder.inc_load_hit_count();
    cache[index].set[found].set_Access_Time(access_timestamp);
    holder.inc_total(1);
  }
}

void Cache::lru_remove(int index) {
  vector<Slot>::iterator lowest = cache[index].set.begin();

  // for each
  for (vector<Slot>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); it++) {
    if ((*it).get_Access_Time() <= (*lowest).get_Access_Time()) {
      lowest = it;
    }
  }

  cache[index].set.erase(lowest);
}

void Cache::fifo_remove(int index) {
  vector<Slot>::iterator lowest = cache[index].set.begin();

         //for each
  for (vector<Slot>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); it++) {
    if ((*it).get_Load_Time() < (*lowest).get_Load_Time()) {
      lowest = it;
    }
  }
         
  cache[index].set.erase(lowest);
}

int Cache::find(int index, uint32_t tag) {
  const vector<Slot> &set = cache[index].set;
  //for each
  for (vector<Slot>::const_iterator it = set.cbegin(); it != set.cend(); it++) {
    if ((*it).get_tag() == tag) {
      return it - set.begin();
    }
  }

  return -1;
}

void Cache::store(uint32_t tag, uint32_t index) {
  holder.inc_store_counter();
         
  int found = find(index, tag);
               
  if (found == -1) {
    holder.inc_store_miss_count();
    if (write_allocate) {
      load_miss(tag, index);
      access_timestamp++;
      cache[index].set.back().set_Access_Time(access_timestamp);
      holder.inc_total(cycle_memory);
      if (write_through) {
        holder.inc_total(100);
      }
    } else {
      holder.inc_total(100);
    }
  } else {
    holder.inc_store_hit_count();
    cache[index].set[found].set_Access_Time(access_timestamp);
    if (!write_through) {
      access_timestamp++;
      cache[index].set[found].set_dirty();
      holder.inc_total(1);
    } else {
      holder.inc_total(100);
    }
  }

}

uint32_t Cache::log_2(uint32_t num) {
  uint32_t result = 0;

  while (num != 1) { 
    num = num >> 1;
    result++;
  }

  return result;
}

