#ifndef SLOT_H
#define SLOT_H

#include <iostream>
using namespace std;

class Slot {
   private: 
      uint32_t tag;
      uint32_t load_ts; // FIFO
      uint32_t access_ts; // LRU
      bool valid;
      bool dirty;

   public: 
      Slot();

      Slot(uint32_t tag, uint32_t load_ts, uint32_t access_ts);

      uint32_t get_Load_Time () { return load_ts; }

      uint32_t get_Access_Time () { return access_ts; }
      
      void inc_Access_Time(){ access_ts++; }

      void set_Access_Time(uint32_t new_Access_Time) { access_ts = new_Access_Time; }
      
      void set_dirty() { dirty = true; }
      
      uint32_t get_tag() const { return tag; }

      bool get_valid() { return valid; }
};

#endif