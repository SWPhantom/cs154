#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
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
	
	
	//Allocate space for cacheBlock
	switch (type){
		//Direct-Mapped
		case 0:
			//Calculate number of slots in cache and allocate them
			newCache.slots = cachesize/blocksize;
			newCache.cacheBlock = malloc(sizeof(int) * newCache.slots);
			
			//Initialize cache as array of -1.
			int i;
			for (i=0; i<newCache.slots; i++){
				newCache.cacheBlock[i] = -1;
			}
			newCache.offsetSize = cachesize/blocksize-1;
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
	}
	
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
		//Get offset and tag bits from address
		int offset = address & (inCache->offsetSize);
		int tag = (address >> inCache->offsetSize) & (32-inCache->offsetSize);
		//printf("Tag: %d, Offset: %d\n", tag, offset);
		
		//Check slot at offset for tag
		//Hit
		if ( (inCache->cacheBlock[offset]) == tag){
			(inCache->accesses)++; //Hit; increment accesses
			return 1;
		}
		//Miss
		else{
			//Overwrite slot with tag
			(inCache->cacheBlock[offset]) = tag;
			(inCache->misses)++; //Miss; increment misses;
			return 0;
		}
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

	//Cache *inCache = cache;
	
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

void printCache(void *cache){
	Cache *inCache = cache;
	printf("Cache slots:\n");
	int i;
	for (i=0; i<inCache->slots; i++){
		printf("[%d]: %d\n", i, *(inCache->cacheBlock+i));
	}
}

/*
int bitLog(int input){
	int temp = input;
	int output = -1;
	while(temp != 0){
		++output;
		temp >>= 1;
	}
}
*/
////=========================Function Implementations End==============================
