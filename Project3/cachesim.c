#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores
//Project Source: http://www.cs.ucsb.edu/~franklin/154/projects/Project3/

////=========================Declarations==============================================
typedef struct _cache{
	int cacheSize;
	int blockSize;
	int type;
	
	//POSSIBLY NEED ARRAY STRUCTURE OF INTS THAT CORRESPOND TO THE
	//ACTUAL CACHE TABLE. THIS COULD BE A POINTER THAT IS MALLOCed
	//WHEN THE CACHE IS CREATED. THE SIZE OF THE CACHE, THE BLOCK
	//SIZE AND THE TYPE WILL INFLUENCE HOW BIG THE SAID TABLE WILL
	//BE
	
	int misses;
	int accesses;
	int totalAccessTime;
} Cache;


////=========================Function Implementations==================================

void *createAndInitialize(int blocksize, int cachesize, int type){
	Cache newCache;
	newCache.blockSize = blocksize;
	newCache.cacheSize = cachesize;
	newCache.type = type;
	Cache *outputPointer = &newCache;
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
	return inCache->totalAccessTime;
}
////=========================Function Implementations End==============================
