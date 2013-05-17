#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores

int main(int argc, char *argv[])
{
	void *cache1;
	int blockSize = 0;
	int cacheSize = 0;
	int type = 0;


	//test1
	{
		blockSize = 8;
		cacheSize = 64;
		type = 0;
		printf("\n==========TEST 1==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		accessCache(cache1, 0);
		accessCache(cache1, 4);
		accessCache(cache1, 8);
		accessCache(cache1, 12);
		accessCache(cache1, 16);
		accessCache(cache1, 20);
		accessCache(cache1, 24);
		accessCache(cache1, 28);
		accessCache(cache1, 32);

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}


	//////////test2
	{
		blockSize = 64;
		cacheSize = 512;
		type = 0;
		printf("\n==========TEST 2==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 390));
		printf("Hit: %d\n",accessCache(cache1, 9099));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 3));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 5));
		printf("Hit: %d\n",accessCache(cache1, 6));
		printf("Hit: %d\n",accessCache(cache1, 7));
		printf("Hit: %d\n",accessCache(cache1, 8));

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}


	//////////test3
	{
		blockSize = 64;
		cacheSize = 512;
		type = 1;
		printf("\n==========TEST 3==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 390));
		printf("Hit: %d\n",accessCache(cache1, 9099));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 3));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 5));
		printf("Hit: %d\n",accessCache(cache1, 6));
		printf("Hit: %d\n",accessCache(cache1, 7));
		printf("Hit: %d\n",accessCache(cache1, 8));

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}


	//////////test4
	{
		blockSize = 64;
		cacheSize = 512;
		type = 2;
		printf("\n==========TEST 4==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

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

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}


	//////////test5
	{
		blockSize = 8;
		cacheSize = 1024;
		type = 0;
		printf("\n==========TEST 5==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 390));
		printf("Hit: %d\n",accessCache(cache1, 9099));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 3));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 5));
		printf("Hit: %d\n",accessCache(cache1, 6));
		printf("Hit: %d\n",accessCache(cache1, 7));
		printf("Hit: %d\n",accessCache(cache1, 8));

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}


	//////////test6
	{
		blockSize = 8;
		cacheSize = 1024;
		type = 1;
		printf("\n==========TEST 6==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 390));
		printf("Hit: %d\n",accessCache(cache1, 9099));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 3));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 5));
		printf("Hit: %d\n",accessCache(cache1, 6));
		printf("Hit: %d\n",accessCache(cache1, 7));
		printf("Hit: %d\n",accessCache(cache1, 8));

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}


	//////////test7
	{
		blockSize = 8;
		cacheSize = 1024;
		type = 2;
		printf("\n==========TEST 7==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 390));
		printf("Hit: %d\n",accessCache(cache1, 9099));
		printf("Hit: %d\n",accessCache(cache1, 18));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 42));
		printf("Hit: %d\n",accessCache(cache1, 234));
		printf("Hit: %d\n",accessCache(cache1, 106));
		printf("Hit: %d\n",accessCache(cache1, 3));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 5));
		printf("Hit: %d\n",accessCache(cache1, 6));
		printf("Hit: %d\n",accessCache(cache1, 7));
		printf("Hit: %d\n",accessCache(cache1, 8));

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}

	//////////test8
	{
		blockSize = 8;
		cacheSize = 64;
		type = 1;
		printf("\n==========TEST 3==========\nBlocksize: %d\nCacheSize: %d\nType: %d\n\n", blockSize, cacheSize, type);

		cache1 = createAndInitialize(blockSize, cacheSize, type);
		printf("Hit: %d\n",accessCache(cache1, 0));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 8));
		printf("Hit: %d\n",accessCache(cache1, 12));
		printf("Hit: %d\n",accessCache(cache1, 64));
		printf("Hit: %d\n",accessCache(cache1, 68));
		printf("Hit: %d\n",accessCache(cache1, 72));
		printf("Hit: %d\n",accessCache(cache1, 76));
		printf("Hit: %d\n",accessCache(cache1, 0));
		printf("Hit: %d\n",accessCache(cache1, 4));
		printf("Hit: %d\n",accessCache(cache1, 8));
		printf("Hit: %d\n",accessCache(cache1, 12));
		printf("Hit: %d\n",accessCache(cache1, 64));
		printf("Hit: %d\n",accessCache(cache1, 68));
		printf("Hit: %d\n",accessCache(cache1, 72));
		printf("Hit: %d\n",accessCache(cache1, 76));

		printf("Accesses: %d, Misses: %d\n", accessesSoFar(cache1), missesSoFar(cache1));
		printf("Total Access Time: %d\n\n", totalAccessTime(cache1));
	}
}
