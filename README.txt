The cache simulation is implemented in the following way: 
    A cache object with a vector of vectors representing the cache. The exterior vector
    represents the whole cache, such that each individual index is a cache slot. The interior vectors
    at each index are present to simulate associative caching, and will all have the same length. For 
    example, if each interior vector has a length of 1 that would represent a directly mapped cache.
    If they all had a length of 4 that would represent a 4 way associative cache. If the exterior 
    vector was of length 1 and the interior vector at index=0 had length equal to the size of the cache,
    that would represent a fully associative cache. 

    The cache class also stores other member variables to indicate the number of sets, the blocks per set,
    the bytes per block, as well if the cache is write-allocate or not, if it's write-back vs write-through,
    and if it is fifo vs lru. 

    The cache also stored the relevent statistics on cache performance, including total_loads, total_stores,
    load_hits, load_misses, store_hits, store_misses, total_cycles, and current_time.

    The interior vectors contain a struct called Block at each index, which we defined inside the cache
    class. Block contained the information on tag, valid and dirty, as well as last access for lru caches. 

    Loading: Cache miss results in a cycle: 100 * (block_size/4) ie. accessing main memory
    Storing: Access the desired data to change using the actual index
    Cache Miss: Depending on the cache policy, a miss may involve:
        Write-Allocate: The block is fetched into the cache first, and then the write is performed.
        No-Write-Allocate: The write bypasses the cache and directly updates the main memory.
    
    Usage:
        Usage: ./csim <sets> <blocks_per_set> <bytes_per_block> <write-allocate|no-write-allocate> <write-through|write-back> <lru|fifo> < <sometracefile>
