  #include "memory.h"
  #include "multiboot.h"
  #include <string.h>
  #include <stdbool.h>

//     bool test_kmalloc() {
//         printf("test0");
//         char *dummy = kmalloc();
//         printf("test1");
//         memset(dummy, 'a', 2);
//         return dummy[0] == 'a' && dummy[1] == 'a';
//     }

//   void init_test_memory(multiboot_info_t *info)
//   {
    /* Memory initialization, mandatory for API use */
    //memory_init(info);
    //memory_dump();
    /* Reserves a 2048 bytes cluster */
    // void *cluster = memory_reserve(2048);
    // memory_dump();
    // printf("cluster => %p\n", cluster);
    // memset(cluster, 'a', 10);
    // printf("%s\n", cluster);
    /* Reserves a range of bsize addresses starting at offset base_addr */
    //void *kernel = memory_reserve_ex(cluster, 2048);
    //printf("kernel -> %p\n", kernel);
    /* Memory Usage */

      /* Reserve enough space to permits cache creation */
    // size_t nbBlocks = 5;
    // size_t blockSize = 10;
    // void *ptr = memory_reserve(nbBlocks * blockSize);

    /* Create the cache to have nmemb slots of size bsize */
    // struct cache *my_cache = cache_new(ptr, nbBlocks, blockSize);

    // /* Get a pointer to one one the cache slots */
    // void *buff = cache_alloc(_cache);
    // memset(buff, 'a', 10);
    // printf("%s\n", buff);
//     printf("%d\n", test_kmalloc());

//   }