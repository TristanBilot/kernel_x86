#include "memory.h"
#include "multiboot.h"
#include <string.h>
#include <stdio.h>
#include "kmalloc.h"

#define DEFAULT_NB_BLOCKS 10
#define DEFAULT_BLOCK_SIZE 128

static void *_low_limit = memory_reserve(DEFAULT_NB_BLOCKS * DEFAULT_BLOCK_SIZE);
static struct cache *_cache = cache_new(_low_limit, DEFAULT_NB_BLOCKS, DEFAULT_BLOCK_SIZE);

void init_kmalloc(void) {
    // _low_limit 
    // printf("sss");
    // _cache 
    // printf("sss2");
}

/// Returns a freshly allocated pointer of size DEFAULT_BLOCK_SIZE.
void *kmalloc(void) {
    return cache_alloc(_cache);
}

void kfree(void *ptr){
    cache_free(_cache, ptr);
}

/// Returns a freshly allocated pointer of size n * size filled with 0s.
void *kcalloc(size_t size, size_t n)
{
    size_t total = size * n;
    void *low_limit = memory_reserve(total);
    struct cache *cache = cache_new(low_limit, size, total);
    void *ptr = cache_alloc(cache);
    memset(ptr, 0, total);
    return ptr;
}
