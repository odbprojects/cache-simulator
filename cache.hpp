//cache_cpp
#ifndef CACHE_HPP
#define CACHE_HPP

#include <queue>
#include <string>
#include <vector>
#include <unordered_map>

// Function to check if a number is a power of 2
bool is_power_of_2(int x);

class Cache {
    public:

        Cache(int s, int b, int bytes, bool write_allocate, 
            bool write_back, bool lru_policy);

        void load(unsigned int address);
        void store(unsigned int address);
        void print_statistics() const;


    private:
        struct Block {
            bool valid;             //True-has meaninful data(has been changed) False-hasn't been loaded yet
            bool dirty;             //True-has been changed since it was last loaded into memory False-has been loaded into memory
            unsigned int tag;
            int last_access;        // For LRU
        };

        int sets;
        int blocks_per_set;
        int bytes_per_block;
        bool write_allocate;
        bool write_back;
        bool lru; 

        std::vector<std::vector<Block>> cache_sets;
        std::vector<std::queue<int>> fifo_queues;

        // Statistics for trace
        int total_loads = 0;
        int total_stores = 0;
        int load_hits = 0;
        int load_misses = 0;
        int store_hits = 0;
        int store_misses = 0;
        int total_cycles = 0;
        int current_time = 0; 

        //access and management functions
        int get_set_index(unsigned int address);
        unsigned int get_tag(unsigned int address);
        int find_block(int set_index, unsigned int tag) const;
        int find_block_to_remove(int set_index) const;
        void update_cache(int set_index, unsigned int tag, bool is_store);
};

#endif // CACHE_HPP