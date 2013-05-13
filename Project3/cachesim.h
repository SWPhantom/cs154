#ifndef CACHESIM_H
#define CACHESIM_H

////=========================Declarations==============================================
typedef struct _cache{
	int cacheSize;
	int blockSize;
	int type;
	int slots;
	int offsetSize;
	
	int *cacheBlock;
	int *validBlock;
	
	int misses;
	int accesses;
	int totalAccessTime;
} Cache;

////=======================Function Prototypes=========================================

/*
 *You have a struct that contains all of the information for one cache. In this
 *function, you create the cache and initialize it, returning a pointer to the struct.
 *Because you are determining the struct, you return a void * to our main. Type 0 is a
 *direct-mapped cache. Type 1 is a pseudo-associative cache. Type 2 is a 4-way set
 *associative cache.
 */
void *createAndInitialize(int blocksize, int cachesize, int type);

/*
 *In this function, we access the cache with a particular address. If the address
 *results in a hit, return 1. If it is a miss, return 0.
 */
int accessCache(void *cache, int address);

/*
 *This returns the number of misses that have occurred so far
 */
int missesSoFar(void *cache);

/*
 *This returns the number of accesses that have occurred so far
 */
int accessesSoFar(void *cache);

/*
 *This returns the total number of cycles that all of the accesses have taken so far.
 */
int totalAccessTime(void *cache);

/*
 *DEBUG: Print contents of cache
*/
void printCache(void *cache);
////=========================Function Definitions End==================================

#endif
