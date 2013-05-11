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
	int blockSize = 2;
	int cacheSize = 64;
	int type = 2;
	printf("Creating direct mapped cache.\nBlocksize: %d\ncacheSize: %d\ntype: %d\n\n", blockSize, cacheSize, type);
	//printf("About to create and init.\n");
	cache1 = createAndInitialize(blockSize, cacheSize, type);
	blockSize = 512;
	cacheSize = 256;
	type = 1;
	//printf("About to run accessCache\n");
	accessCache(cache1, 999505);
	
	//accessCache
	
	//Debugging our access methods.
	int x = missesSoFar(cache1);
}
