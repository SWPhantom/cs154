#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores
//Project Source: http://www.cs.ucsb.edu/~franklin/154/projects/Project3/

////=========================Function Implementations==================================

void *createAndInitialize(int blocksize, int cachesize, int type){
	Cache newCache;
	newCache.blockSize = blocksize;
	newCache.cacheSize = cachesize;
	newCache.type = type;
	newCache.misses = 0;
	newCache.accesses = 0;
	Cache *outputPointer = (Cache*) malloc(sizeof(Cache));
	*outputPointer = newCache;
	
	printf("Created and Initialized.\n");
	return outputPointer;
}

/*
	I am assuming when we access a cache and there is a miss, we must insert the requested
	data into the cache.
*/
int accessCache(void *cache, int address){
	Cache *inCache = cache;
	
	//Direct-mapped cache
	if(inCache->type == 0){
		printf("Found type to be direct-mapped cache\n");
		
		//Look in cacheBlock and see if address exists here. 
		
		
	}else
	//Pseudo-associative cache
	if(inCache->type == 1){
		printf("Found type to be pseudo-associative cache\n");
	}else
	//4-way set associative cache
	if(inCache->type == 2){
		printf("Found type to be 4-way set associative cache\n");
		printf("misses: %d, type: %d\n", inCache->misses, inCache->type);
	}else{//Something went wrong and the type was not set correctly.
		return -1;
	}
	
}

int missesSoFar(void *cache){
	//The following code doesn't work. Can we find out why? It does not return
	//misses, but instead some address. We have inconsistent access problems
	//when we call similar code in other functions.
	/*
	Cache *inCache = cache;
	return inCache->misses;
	*/

	Cache *inCache = cache;
	if (inCache->type == 2){
		printf("Found 2\n");
	}
	if (inCache->misses == 4){
		printf("Found 4\n");
	}
	
	//This code works, but looks uglier.
	return ((Cache*)cache)->misses;
}

int accessesSoFar(void *cache){
	Cache *inCache = cache;
	return inCache->accesses;
}

int totalAccessTime(void *cache){
	Cache *inCache = cache;
	
	if(inCache->type == 0){
		int temp = (inCache->misses)*100+(inCache->accesses);
		return temp;
	}else
	
	if(inCache->type == 1){
		int temp = (inCache->misses)*100+(inCache->accesses);
		return temp;
	}else
	
	if(inCache->type == 2){
		int temp = (inCache->misses)*100+(inCache->accesses)*3;
		return temp;
	}else{
		return -1;
	}
}
////=========================Function Implementations End==============================
