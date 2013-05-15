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
	void *cache4;
	int blockSize = 0;
	int cacheSize = 0;
	int type = 0;
	int x;
	int y;
	//test1
	{
		blockSize = 8;
		cacheSize = 64;
		type = 0;
//		printf("\n==========TEST 1==========\nCreating direct mapped cache1.\nBlocksize: %d\ncacheSize: %d\ntype: %d\n\n", blockSize, cacheSize, type);
		
		cache1 = createAndInitialize(blockSize, cacheSize, type);
		accessCache(cache1, 18);
		accessCache(cache1, 18);
		accessCache(cache1, 390);
		accessCache(cache1, 9099);
		accessCache(cache1, 18);
		accessCache(cache1, 42);
		accessCache(cache1, 42);
		accessCache(cache1, 42);
		accessCache(cache1, 234);
		accessCache(cache1, 106);
		accessCache(cache1, 42);
		accessCache(cache1, 234);
		accessCache(cache1, 106);
		accessCache(cache1, 42);
		accessCache(cache1, 234);
		accessCache(cache1, 106);
		accessCache(cache1, 3);
		accessCache(cache1, 4);
		accessCache(cache1, 5);
		accessCache(cache1, 6);
		accessCache(cache1, 7);
		accessCache(cache1, 8);
		
		//accessCache
	
		//Debugging our access methods.
		x = missesSoFar(cache1);
		y = accessesSoFar(cache1);
		printf("Hits: %d, Misses: %d\n", y, x);
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}
	//////////////////////////////////////////////
	//test2
	{
		blockSize = 32;
		cacheSize = 512;
		type = 0;
//		printf("\n==========TEST 2==========\nCreating direct mapped cache2.\nBlocksize: %d\ncacheSize: %d\ntype: %d\n\n", blockSize, cacheSize, type);
		
		cache2 = createAndInitialize(blockSize, cacheSize, type);
		//printf("About to run accessCache\n");
		accessCache(cache2, 18);
		accessCache(cache2, 18);
		accessCache(cache2, 390);
		accessCache(cache2, 9099);
		accessCache(cache2, 18);
		accessCache(cache2, 42);
		accessCache(cache2, 42);
		accessCache(cache2, 42);
		accessCache(cache2, 234);
		accessCache(cache2, 106);
		accessCache(cache2, 42);
		accessCache(cache2, 234);
		accessCache(cache2, 106);
		accessCache(cache2, 42);
		accessCache(cache2, 234);
		accessCache(cache2, 106);
		accessCache(cache2, 3);
		accessCache(cache2, 4);
		accessCache(cache2, 5);
		accessCache(cache2, 6);
		accessCache(cache2, 7);
		accessCache(cache2, 8);
	
		//Debugging our access methods.
		x = missesSoFar(cache2);
		y = accessesSoFar(cache2);
		printf("Hits: %d, Misses: %d\n", y, x);
		printf("Total Access Time: %d\n\n", totalAccessTime(cache2));
	}

		//////////////////////////////////////////////
	//test3
	{
		blockSize = 32;
		cacheSize = 1024;
		type = 1;
//		printf("\n==========TEST 3==========\nCreating Associative cache.\nBlocksize: %d\ncacheSize: %d\ntype: %d\n\n", blockSize, cacheSize, type);
		
		cache3 = createAndInitialize(blockSize, cacheSize, type);
		//printf("About to run accessCache\n");
		accessCache(cache3, 18);
		accessCache(cache3, 18);
		accessCache(cache3, 390);
		accessCache(cache3, 9099);
		accessCache(cache3, 18);
		accessCache(cache3, 42);
		accessCache(cache3, 42);
		accessCache(cache3, 42);
		accessCache(cache3, 234);
		accessCache(cache3, 106);
		accessCache(cache3, 42);
		accessCache(cache3, 234);
		accessCache(cache3, 106);
		accessCache(cache3, 42);
		accessCache(cache3, 234);
		accessCache(cache3, 106);
		accessCache(cache3, 3);
		accessCache(cache3, 4);
		accessCache(cache3, 5);
		accessCache(cache3, 6);
		accessCache(cache3, 7);
		accessCache(cache3, 8);
	
		//Debugging our access methods.
		x = missesSoFar(cache3);
		y = accessesSoFar(cache3);
		printf("Hits: %d, Misses: %d\n", y, x);
		printf("Total Access Time: %d\n\n", totalAccessTime(cache3));
	}

		//////////////////////////////////////////////
	//test4
	{
		blockSize = 64;
		cacheSize = 512;
		type = 2;
//		printf("\n==========TEST 4==========\nCreating 4-way Associative cache.\nBlocksize: %d\ncacheSize: %d\ntype: %d\n\n", blockSize, cacheSize, type);
		
		cache4 = createAndInitialize(blockSize, cacheSize, type);
		//printf("About to run accessCache\n");
		accessCache(cache4, 18);
//		printCache(cache4);
		accessCache(cache4, 18);
//		printCache(cache4);
		accessCache(cache4, 390);
//		printCache(cache4);
		accessCache(cache4, 9099);
//		printCache(cache4);
		accessCache(cache4, 18);
//		printCache(cache4);
		accessCache(cache4, 42);
//		printCache(cache4);
		accessCache(cache4, 42);
//		printCache(cache4);
		accessCache(cache4, 42);
//		printCache(cache4);
		accessCache(cache4, 234);
//		printCache(cache4);
		accessCache(cache4, 106);
//		printCache(cache4);
		accessCache(cache4, 42);
//		printCache(cache4);
		accessCache(cache4, 234);
//		printCache(cache4);
		accessCache(cache4, 106);
//		printCache(cache4);
		accessCache(cache4, 42);
//		printCache(cache4);
		accessCache(cache4, 234);
//		printCache(cache4);
		accessCache(cache4, 106);
//		printCache(cache4);
		accessCache(cache4, 3);
//		printCache(cache4);
		accessCache(cache4, 4);
//		printCache(cache4);
		accessCache(cache4, 5);
//		printCache(cache4);
		accessCache(cache4, 6);
//		printCache(cache4);
		accessCache(cache4, 7);
//		printCache(cache4);
		accessCache(cache4, 8);
//		printCache(cache4);
	
		//Debugging our access methods.
		x = missesSoFar(cache4);
		y = accessesSoFar(cache4);
		printf("Hits: %d, Misses: %d\n", y, x);
		printf("Total Access Time: %d\n\n", totalAccessTime(cache4));
	}
}
