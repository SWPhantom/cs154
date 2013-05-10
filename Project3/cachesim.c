#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "cachesim.h"

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores
//Project Source: http://www.cs.ucsb.edu/~franklin/154/projects/Project3/

////=========================Declarations==============================================
typedef struct _cache{
	int cacheSize;
	int blockSize;
	int type;
	
	int **cacheBlock;
	
	int misses;
	int accesses;
	int totalAccessTime;
} Cache;
////=========================Declarations End==========================================

////=========================Function Implementations==================================

void *createAndInitialize(int blocksize, int cachesize, int type){
	Cache newCache;
	newCache.blockSize = blocksize;
	newCache.cacheSize = cachesize;
	newCache.type = type;
	newCache.misses = 0;
	newCache.accesses = 0;
	Cache *outputPointer = &newCache;
	
	printf("Created and Initialized.\n");
}


int accessCache(void *cache, int address){
	Cache *inCache = cache;
	//Direct-mapped cache
	if(inCache->type == 0){
		
	}else
	//Pseudo-associative cache
	if(inCache->type == 1){
		
	}else
	//4-way set associative cache
	if(inCache->type == 2){
		
	}else{//Something went wrong and the type was not set correctly.
		return -1;
	}
}

int missesSoFar(void *cache){
	Cache *inCache = cache;
	return inCache->misses;
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
