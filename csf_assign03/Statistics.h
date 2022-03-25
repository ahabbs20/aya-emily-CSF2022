#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
using namespace std;

class Statistics {
  uint32_t load_counter;
  uint32_t store_counter;  
  uint32_t load_hit_count;
  uint32_t load_miss_count;
  uint32_t store_hit_count;
  uint32_t store_miss_count;
  uint32_t total_cycle; 

  public: 

    Statistics();

    void inc_total(uint32_t to_increase);

    void inc_load_counter();

    void inc_store_counter();

    void inc_load_hit_count();

    void inc_load_miss_count();

    void inc_store_hit_count();

    void inc_store_miss_count();

    uint32_t get_store_counter() { return store_counter; };

    void output_statistics();

};

#endif