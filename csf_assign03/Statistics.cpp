#include "Statistics.h"

Statistics::Statistics() {
  load_counter = 0;
  store_counter = 0;  
  load_hit_count = 0;
  load_miss_count = 0;
  store_hit_count = 0;
  store_miss_count = 0;
  total_cycle = 0; 
}

void Statistics::inc_total(uint32_t to_increase) {
  total_cycle += to_increase;
}

void Statistics::inc_load_counter() {
  load_counter++;
}

void Statistics::inc_store_counter() {
  store_counter++;
}

void Statistics::inc_load_hit_count() {
  load_hit_count++;
}

void Statistics::inc_load_miss_count() {
  load_miss_count++;
}

void Statistics::inc_store_hit_count() {
  store_hit_count++;
}

void Statistics::inc_store_miss_count() {
  store_miss_count++;
}

void Statistics::output_statistics() {
  
  cout << "Total loads: " << load_counter << endl;
  cout << "Total stores: " << store_counter << endl;
  cout << "Load hits: " << load_hit_count << endl;
  cout << "Load misses: " << load_miss_count << endl;
  cout << "Store hits: " << store_hit_count << endl;
  cout << "Store misses: " << store_miss_count << endl;
  cout << "Total cycles: " << total_cycle << endl;
  
}
 