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
	newCache.pseudoAccesses = 0;
	newCache.slots = (cachesize>>calcLog(blocksize));
	newCache.cacheBlock = malloc(sizeof(int) * newCache.slots);
	newCache.validBlock = malloc(sizeof(int) * newCache.slots);
	
	//Initialize cache as array of -1.
	int i;
	for(i=0; i<newCache.slots; i++){
		newCache.cacheBlock[i] = -1;
	}
	//Initialize all validity bits to 0
	for(i = 0; i < newCache.slots; ++i){
		newCache.validBlock[i] = 0;
	}
	
	//printf("Log of blocksize: %d.\n",calcLog(newCache.blockSize));//DEBUG
	
	//Change the offsetSize in accordance to the type of cache this is.
	switch (type){
		//Direct-Mapped
		case 0:
			newCache.offsetSize = (cachesize>>calcLog(blocksize)) - 1;
			break;
		case 1:
			newCache.offsetSize = (((cachesize>>calcLog(blocksize))) >> 1) - 1;
			break;
		case 2:
			newCache.offsetSize = (((cachesize>>calcLog(blocksize))) >> 2) - 1;
			break;
		default:
			break;
	}
	//printf("Offset Size: %d\n", newCache.offsetSize);//DEBUG
	Cache *outputPointer = (Cache*) malloc(sizeof(Cache));
	*outputPointer = newCache;
	return outputPointer;
}

int accessCache(void *cache, int address){
	Cache *inCache = cache;
	++(inCache->accesses); // Because every access will add to the access variable

	int offset = (address>>calcLog(inCache->blockSize)) & (inCache->offsetSize);
	//Direct-mapped cache
	if(inCache->type == 0){
		//Get offset and tag bits from address
		
		//printf("OFFSET = %d\n", offset);
		
		if(inCache->validBlock[offset] == 1){//Sees if the entry valid
			if (inCache->cacheBlock[offset] == (address>>calcLog(inCache->blockSize))){
				return 1;
			}
			//Miss
			else{
				//Overwrite slot with tag
				inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
				(inCache->misses)++; //Miss; increment misses;
				return 0;
			}
		}else{//If the entry is not valid: miss, validate, and update data.
			inCache->validBlock[offset] = 1;
			inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
			++(inCache->misses);
			return 0;
		}
	}else
	
	//Pseudo-associative cache
	if(inCache->type == 1){
		//Get offset and tag bits from address
		
		
		offset <<= 1; //This is to spread the offset up to make it go in the correct blocks
		//printf("OFFSET = %d\n", offset);
		
		if(inCache->validBlock[offset] == 1){//Sees if the entry valid
			if (inCache->cacheBlock[offset] == (address>>calcLog(inCache->blockSize))){
				//printf("%d already in cache!\n", (address>>calcLog(inCache->blockSize)));//debug
				return 1;
			}
			//Miss
			else{
				//printf("%d Not in first index...", (address>>calcLog(inCache->blockSize)));
				++(inCache->pseudoAccesses);	//This is for the extra accesses that may be
					//necessary for passing the text cases.
				if(inCache->cacheBlock[offset+1] == (address>>calcLog(inCache->blockSize))){
					//printf("BUT IS already in SECOND!\n");
					inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
					inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
					inCache->validBlock[offset+1] = 1;
					return 1;
				}else{//if there is no match at all
					//printf("And is NOT in the second index.\n");
					++(inCache->misses);
					inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
					inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
					inCache->validBlock[offset+1] = 1;
					return 0;
				}
			}
		}else{//If the entry is not valid: miss, validate, and update data.
			//printf("Invalid.\n");
			++(inCache->pseudoAccesses);	//This is for the extra accesses that may be
			//printf("%d Not in the cache and the first entry is not valid.\n", (address>>calcLog(inCache->blockSize)));
			inCache->validBlock[offset] = 1;
			inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
			++(inCache->misses);
			return 0;
		}
	}else
	
	//4-way set associative cache
	if(inCache->type == 2){
		//Get offset and tag bits from address
		
		
		offset <<= 2;	//This is to spread the offset up to make it go in the correct blocks
		//printf("OFFSET = %d\n", offset);
		
		if(inCache->validBlock[offset] == 1){//Sees if the entry valid
			if (inCache->cacheBlock[offset] == (address>>calcLog(inCache->blockSize))){
				return 1;
			}
			//Miss
			else{
				if(inCache->validBlock[offset+1] == 1){
					if(inCache->cacheBlock[offset+1] == (address>>calcLog(inCache->blockSize))){
						inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
						inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
						inCache->validBlock[offset+1] = 1;
						return 1;
					}else{
						if(inCache->validBlock[offset+2] == 1){
							if(inCache->cacheBlock[offset+2] == (address>>calcLog(inCache->blockSize))){
								inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
								inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
								inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
								inCache->validBlock[offset+2] = 1;
								return 1;
							}else{
								if(inCache->validBlock[offset+3] == 1){
									if(inCache->cacheBlock[offset+3] == (address>>calcLog(inCache->blockSize))){
										inCache->cacheBlock[offset+3] = inCache->cacheBlock[offset+2];
										inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
										inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
										inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
										inCache->validBlock[offset+3] = 1;
										return 1;
									}else{
										++(inCache->misses);
										inCache->cacheBlock[offset+3] = inCache->cacheBlock[offset+2];
										inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
										inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
										inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
										inCache->validBlock[offset+3] = 1;
										return 0;
									}
								}else{
									++(inCache->misses);
									inCache->cacheBlock[offset+3] = inCache->cacheBlock[offset+2];
									inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
									inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
									inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
									inCache->validBlock[offset+3] = 1;
									return 0;
								}
							}
						}else{
							++(inCache->misses);
							inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
							inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
							inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
							inCache->validBlock[offset+2] = 1;
							return 0;
						}
					}
				}else{//if there is no match at all
					++(inCache->misses);
					inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
					inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
					inCache->validBlock[offset+1] = 1;
					return 0;
				}
			}
		}else{//If the entry is not valid: miss, validate, and update data.
			//printf("Invalid.\n");
			inCache->validBlock[offset] = 1;
			inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
			++(inCache->misses);
			return 0;
		}


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
		int temp = (inCache->misses)*100+(inCache->accesses)+(inCache->pseudoAccesses);
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
	printf("Cache slots:");
	int i;
	for (i = 0; i < inCache->slots; i++){
		printf("[%d]:%d, ", i, inCache->cacheBlock[i]);
	}
	printf("\n");
}

int calcLog(int input){
	int counter = -1;
	while(input != 0){
		input >>= 1;
		++counter;
	}
	return counter;
}

////=========================Function Implementations End==============================
