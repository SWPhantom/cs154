#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include "branchsim.h"

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores
//Project Source: http://www.cs.ucsb.edu/~franklin/154/projects/Project4/

////=========================Function Implementations==================================
void *createAndInitialize(int numEntries){
	//create local copy of a Cache struct.
	Cache newCache;
	newCache.cacheBlock = malloc(sizeof(int) * numEntries);
	newCache.accesses = 0;
	newCache.mispredictions = 0;
	newCache.cacheSize = numEntries;
	
	//Initialize cache as array of 0.
	int i;
	for(i = 0; i < newCache.cacheSize; ++i){
		newCache.cacheBlock[i] = 0;
	}
	
	//Allocate Cache amount of memory.
	Cache *outputPointer = (Cache*) malloc(sizeof(Cache));
	//Pass the local struct to the pointer by copy, preserving the struct
	*outputPointer = newCache;
	return outputPointer;
}

int accessBranchPredictor(void *bp, int PC){
	Cache *inCache = bp;
	
	//Calculate offset, which ignores the two bits that never change on the right
	//and masks it with as many bits as the cacheSize is (in binary).
	int offset = ((PC >> 2) & (inCache->cacheSize - 1));
	
	++inCache->accesses;
	//printf("DEBUG : (inCache->cacheSize - 1) = %d\n", (inCache->cacheSize - 1));
	//printf("DEBUG : offset = %d :: ", offset);
	
	//Return 1 if the value at cacheBlock[offset] is 2 or 3.
	if(inCache->cacheBlock[offset] >= 2){
		return 1;
	}else{
		return 0;
	}
	return inCache->cacheBlock[offset];
}

void updateBranchPredictor(void *bp, int PC, int result){
	Cache *inCache = bp;
	
	//Calculate offset, which ignores the two bits that never change on the right
	//and masks it with as many bits as the cacheSize is (in binary).
	int offset = ((PC >> 2) & (inCache->cacheSize - 1));
	int bVal = inCache->cacheBlock[offset];
	
	if(result == 0){//If the actual input says not taken.
		if(bVal < 3){
			if(bVal == 2){
				++inCache->mispredictions;
			}
			inCache->cacheBlock[offset] = 0;
		}else{
			inCache->cacheBlock[offset] = 2;
			++inCache->mispredictions;
		}
	}else{//If the actual input says taken.
		if(bVal > 0){
			if(bVal == 1){
				++inCache->mispredictions;
			}
			inCache->cacheBlock[offset] = 3;
		}else{
			inCache->cacheBlock[offset] = 1;
			++inCache->mispredictions;
		}
	}
}

int numAccesses(void *bp){
	Cache *inCache = bp;
	return inCache->accesses;
}

int numMispredictions(void *bp){
	Cache *inCache = bp;
	return inCache->mispredictions;
}

int calcLog(int input){
	//This function shifts the input value right until
	//it is 0. Every time it shifts, increments the
	//output value by 1.
	//This may have issues with integers starting with
	//a 1(in binary).
	int counter = -1;
	while(input != 0){
		input >>= 1;
		++counter;
	}
	return counter;
}
////=========================Function Implementations End==============================
