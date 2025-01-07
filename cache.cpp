#include "cache.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

//function to make sure that sets, blocks, and bytes are powers
//of two
bool is_power_of_2(int x) {
    return (x & (x - 1)) == 0;
}

//Constructor
Cache::Cache(int s, int b, int bytes, bool write_allocate, 
            bool write_back, bool lru_policy): 
            sets(s), blocks_per_set(b), bytes_per_block(bytes), 
            write_allocate(write_allocate), write_back(write_back), 
            lru(lru_policy), cache_sets(s, std::vector<Block>(blocks_per_set, {false, false, 0, 0})), fifo_queues(s) {
    for (int i = 0; i < sets; ++i) {
        for (int j = 0; j < blocks_per_set; ++j) {
            fifo_queues[i].push(j);
        }
    }

}

// Get set index from address
int Cache::get_set_index(unsigned int address) {
    return (address / bytes_per_block) % sets;
}

// Get tag from address
unsigned int Cache::get_tag(unsigned int address) {
    return address / (bytes_per_block * sets);
}

int Cache::find_block(int set_index, unsigned int tag) const {
    for (int i = 0; i < blocks_per_set; ++i) {
        if (cache_sets[set_index][i].valid && cache_sets[set_index][i].tag == tag) {
            return i;
        }
    }
    return -1;
}

int Cache::find_block_to_remove(int set_index) const {
    if (lru) {
        int oldest_index = 0;
        for (int i = 1; i < blocks_per_set; ++i) {
            if (cache_sets[set_index][i].last_access < cache_sets[set_index][oldest_index].last_access) {
                oldest_index = i;
            }
        }
        return oldest_index;
    } else {
        return fifo_queues[set_index].front();
    }
}

void Cache::load(unsigned int address) {
    // Add statistics
    ++total_loads;
    ++current_time;
    
    // Get addresses
    int set_index = get_set_index(address);
    unsigned int tag = get_tag(address);
    int block_index = find_block(set_index, tag);
    if (block_index != -1) {
        ++load_hits;
        cache_sets[set_index][block_index].last_access = current_time;
        total_cycles += 1;  // Load hit cycle
    } else {
        ++load_misses;
        total_cycles += 100 * (bytes_per_block / 4);  // Time to transfer block from memory
        update_cache(set_index, tag, false);
        total_cycles += 1;  // Load miss cycle
    }
}

void Cache::store(unsigned int address) {
    ++total_stores;
    ++current_time;

    int set_index = get_set_index(address);
    unsigned int tag = get_tag(address);

    int block_index = find_block(set_index, tag);
    if (block_index != -1) {
        ++store_hits;
        cache_sets[set_index][block_index].dirty = write_back;
        cache_sets[set_index][block_index].last_access = current_time;
        if (!write_back) {
            total_cycles += 100 * (bytes_per_block / 4);  // Write-through cycle
        }
        total_cycles += 1;  // Store hit cycle
    } else {
        ++store_misses;
        if (write_allocate) {
            total_cycles += 100 * (bytes_per_block / 4);  // Fetch block for write-allocate
            update_cache(set_index, tag, true);
        } else {
            total_cycles += 100 * (bytes_per_block / 4);  // Write miss cycle
        }
        total_cycles += 1;  // Store miss cycle
    }
}



void Cache::print_statistics() const {
    std::cout << "Total loads: " << total_loads << std::endl;
    std::cout << "Total stores: " << total_stores << std::endl;
    std::cout << "Load hits: " << load_hits << std::endl;
    std::cout << "Load misses: " << load_misses << std::endl;
    std::cout << "Store hits: " << store_hits << std::endl;
    std::cout << "Store misses: " << store_misses << std::endl;
    std::cout << "Total cycles: " << total_cycles << std::endl;
}

void Cache::update_cache(int set_index, unsigned int tag, bool is_store) {
    int change_index = find_block_to_remove(set_index);
    if (cache_sets[set_index][change_index].dirty) {
        total_cycles += 100 * (bytes_per_block / 4);  // Write back dirty block
    }
    cache_sets[set_index][change_index].valid = true;
    cache_sets[set_index][change_index].dirty = is_store && write_back;
    cache_sets[set_index][change_index].tag = tag;
    cache_sets[set_index][change_index].last_access = current_time;

    if (!lru) {
        fifo_queues[set_index].pop();
        fifo_queues[set_index].push(change_index);
    }
}