#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores

int main(int argc, char *argv[])
{
	void *cache1;
	void *cache2;
	void *cache3;
	int blockSize = 8;
	int cacheSize = 64;
	int type = 0;
	printf("Creating direct mapped cache.\nBlocksize: %d\ncacheSize: %d\ntype: %d\n\n", blockSize, cacheSize, type);
	//printf("About to create and init.\n");
	cache1 = createAndInitialize(blockSize, cacheSize, type);
	blockSize = 512;
	cacheSize = 256;

	//printf("About to run accessCache\n");
	accessCache(cache1, 42);
	accessCache(cache1, 42);
	accessCache(cache1, 42);
	accessCache(cache1, 3);
	accessCache(cache1, 4);
	
	//accessCache
	
	//Debugging our access methods.
	int x = missesSoFar(cache1);
	int y = accessesSoFar(cache1);
	printf("Hits: %d, Misses: %d\n", y, x);
}
