#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores

////=========================Function Headers==========================================

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

////=========================Function Headers End=====================================

int main(int argc, char *argv[])
{
	void *cache1;
	void *cache2;
	void *cache3;
	int blockSize = 2;
	int cacheSize = 64;
	int type = 0;
	printf("Creating direct mapped cache.\nBlocksize: %d\ncacheSize: %d, type: %d\n\n", blockSize, cacheSize, type);
	cache1 = createAndInitialize(blockSize, cacheSize, type);
	accessCache
}

