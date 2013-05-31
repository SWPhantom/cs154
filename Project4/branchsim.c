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
	++inCache->accesses;

	//Calculate offset, which ignores the two bits that never change on the right
	//and mask it with cacheSize-1 (all 1s)
	int offset = ((PC >> 2) & (inCache->cacheSize - 1));

	//Return 1 if the value at cacheBlock[offset] is 2 or 3. 0 otherwise.
	if(inCache->cacheBlock[offset] >= 2){
		return 1;
	}else{
		return 0;
	}
}

void updateBranchPredictor(void *bp, int PC, int result){
	Cache *inCache = bp;

	//Calculate offset, which ignores the two bits that never change on the right
	//and mask it with cacheSize-1 (all 1s)
	int offset = ((PC >> 2) & (inCache->cacheSize - 1));

	if(result == 0){//If the actual input says not taken.
		if(inCache->cacheBlock[offset] < 3){
			//Increment the misprediction counter if there has been one.
			if(inCache->cacheBlock[offset] == 2){
				++inCache->mispredictions;
			}
			inCache->cacheBlock[offset] = 0;
		}else{
			inCache->cacheBlock[offset] = 2;
			++inCache->mispredictions;
		}
	}else{//If the actual input says taken.
		if(inCache->cacheBlock[offset] > 0){
			//Increment the misprediction counter if there has been one.
			if(inCache->cacheBlock[offset] == 1){
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
////=========================Function Implementations End==============================
