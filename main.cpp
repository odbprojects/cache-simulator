#include <cstring>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <sstream>
#include "cache.hpp"


int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: ./csim <sets> <blocks_per_set> <bytes_per_block> <write-allocate|no-write-allocate> <write-through|write-back> <lru|fifo> <sometracefile>\n";
        return 1;
    }


    //read variables from input
    int sets = std::stoi(argv[1]);
    int blocks_per_set = std::stoi(argv[2]);
    int bytes_per_block = std::stoi(argv[3]);
    bool write_allocate = (std::strcmp(argv[4], "write-allocate") == 0);
    bool write_back = (std::strcmp(argv[5], "write-back") == 0);
    bool lru = (std::strcmp(argv[6], "lru") == 0);
    // std::string trace_file = argv[7];
    // std::cout<<trace_file<<std::endl;


    //REPORTING INVALID CACHE PARAMETERS
    if (!is_power_of_2(sets) || !is_power_of_2(blocks_per_set) || !is_power_of_2(bytes_per_block)) {
        std::cerr << "Set, Block, and byte size must all be powers of two";
        return 1;
    }
    if (bytes_per_block < 4) {
        std::cerr << "There must be at least 4 bytes per block";
        return 1;
    }
    if ((write_back && !write_allocate)) {
        std::cerr << "Cannot combine no-write-allocate with write-back";
        return 1;
    }
    // std::ifstream infile(trace_file);
    // if (!infile) {
    //     std::cerr << "Error opening file: " << trace_file << std::endl;
    //     return 1;
    // }
    //END REPORTING INVALID CACHE PARAMETERS

    Cache cache(sets, blocks_per_set, bytes_per_block, write_allocate, write_back, lru);


    //RUN SIMULATION
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        char operation;
        std::string address_str;
        int size;

        if (!(iss >> operation >> address_str >> size)) {
            std::cerr << "Error reading trace line: " << line << std::endl;
            continue;
        }

        // Remove the "0x" prefix from the address string and convert to integer
        unsigned int address = std::stoul(address_str.substr(2), nullptr, 16);

        if (operation == 'l') {
            cache.load(address);
        } else if (operation == 's') {
            cache.store(address);
        } else {
            std::cerr << "Unknown operation: " << operation << std::endl;
        }
    }

    // Output the cache statistics
    cache.print_statistics();

    return 0;

}