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
	//create local copy of a Cache struct.
	Cache newCache;
	newCache.blockSize = blocksize;
	newCache.cacheSize = cachesize;
	newCache.type = type;
	newCache.misses = 0;
	newCache.accesses = 0;
	newCache.pseudoAccesses = 0;
	newCache.slots = (cachesize>>calcLog(blocksize));
	newCache.byteOffset = calcLog(blocksize);
	newCache.cacheBlock = malloc(sizeof(int) * newCache.slots);
	newCache.validBlock = malloc(sizeof(int) * newCache.slots);
	
	//Initialize cache as array of -1.
	int i;
	for(i = 0; i < newCache.slots; ++i){
		newCache.cacheBlock[i] = -1;
	}
	//Initialize all validity bits to 0
	for(i = 0; i < newCache.slots; ++i){
		newCache.validBlock[i] = 0;
	}

	//Change the offsetSize in accordance to the type of cache this is.
	//The offset size acts as a mask to get the index bits from the
	//address.
	//XXX: This mask starts right after the byte offset (which is cal-
	//culated by >>calcLog(blocksize).
	switch (type){
		//Direct-Mapped
		case 0:
			newCache.offsetSize = (newCache.slots) - 1;
			break;
		case 1:
			//Has to be shifted by 1 because we are indexing this into /2 of
			//the direct mapped cache.
			newCache.offsetSize = (newCache.slots >> 1) - 1;
			break;
		case 2:
			//Has to be shifted by 2 because we are indexing this into /4 of
			//the direct mapped cache.
			newCache.offsetSize = (newCache.slots >> 2) - 1;
			break;
		default:
			break;
	}

	//Allocate Cache amount of memory.
	Cache *outputPointer = (Cache*) malloc(sizeof(Cache));
	//Pass the local struct to the pointer by copy, preserving the struct
	*outputPointer = newCache;
	return outputPointer;
}

int accessCache(void *cache, int address){
	Cache *inCache = cache;
	++(inCache->accesses); // Because every access will add to the access variable

	//Create the index value, shifted by the byteOffset and masked by the
	//previously computed offsetSize.
	int offset = (address>>calcLog(inCache->blockSize)) & (inCache->offsetSize);

	//Direct-mapped cache block
	if(inCache->type == 0){

		//Check to see if the valid bit at the index is valid.
		if(inCache->validBlock[offset] == 1){
			//If the address in entry is already correct, do nothing!
			if (inCache->cacheBlock[offset] == (address>>calcLog(inCache->blockSize))){
				return 1;
			}
			//Miss!
			else{
				//Overwrite slot with tag and increment miss variable
				inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
				(inCache->misses)++;
				return 0;
			}
		}else{//If the entry is not valid: miss, validate, and update data.
			inCache->validBlock[offset] = 1;
			inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
			++(inCache->misses);
			return 0;
		}
	}

	//Pseudo-associative cache
	else if(inCache->type == 1){
		offset <<= 1; //This is to spread the offset out to make it go in the correct blocks

		//Check to see if the valid bit at the index is valid.
		if(inCache->validBlock[offset] == 1){
			//If the address in entry is already correct, do nothing!
			if (inCache->cacheBlock[offset] == (address>>calcLog(inCache->blockSize))){
				return 1;
			}
			//Miss
			else{
				++(inCache->pseudoAccesses);	//Extra access for pseudo-associative case.

				//Check the second block for the correct address
				if(inCache->cacheBlock[offset+1] == (address>>calcLog(inCache->blockSize))){
					inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
					inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
					inCache->validBlock[offset+1] = 1;
					return 1;
				}else{//if there is no match at all
					++(inCache->misses);
					inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
					inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
					inCache->validBlock[offset+1] = 1;
					return 0;
				}
			}
		}else{//If the entry is not valid: miss, validate, and update data.
			++(inCache->pseudoAccesses);	//Extra access for pseudo-associative case.
			inCache->validBlock[offset] = 1;
			inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
			++(inCache->misses);
			return 0;
		}
	}

	//4-way set associative cache
	else if(inCache->type == 2){
		offset <<= 2;	//This is to spread the offset out to make it go in the correct blocks

		//Check to see if the valid bit at the index is valid.
		if(inCache->validBlock[offset] == 1){
			if (inCache->cacheBlock[offset] == (address>>calcLog(inCache->blockSize))){
				return 1;
			}
			//Miss
			else{
				//Check the second index validity.
				if(inCache->validBlock[offset+1] == 1){
					//Hit!
					if(inCache->cacheBlock[offset+1] == (address>>calcLog(inCache->blockSize))){
						inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
						inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
						inCache->validBlock[offset+1] = 1;
						return 1;
					}else{
						//Check the third index validity.
						if(inCache->validBlock[offset+2] == 1){
							//Hit!
							if(inCache->cacheBlock[offset+2] == (address>>calcLog(inCache->blockSize))){
								inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
								inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
								inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
								inCache->validBlock[offset+2] = 1;
								return 1;
							}else{
								//Check the fourth index validity.
								if(inCache->validBlock[offset+3] == 1){
									//Hit!
									if(inCache->cacheBlock[offset+3] == (address>>calcLog(inCache->blockSize))){
										inCache->cacheBlock[offset+3] = inCache->cacheBlock[offset+2];
										inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
										inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
										inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
										inCache->validBlock[offset+3] = 1;
										return 1;
									}else{
										//Miss in the fourth. Shift the entries!
										++(inCache->misses);
										inCache->cacheBlock[offset+3] = inCache->cacheBlock[offset+2];
										inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
										inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
										inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
										inCache->validBlock[offset+3] = 1;
										return 0;
									}
								}else{
									//Miss in the fourth. Shift the entries!
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
							//Miss in the third. Shift the entries!
							++(inCache->misses);
							inCache->cacheBlock[offset+2] = inCache->cacheBlock[offset+1];
							inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
							inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
							inCache->validBlock[offset+2] = 1;
							return 0;
						}
					}
				}else{//Miss in the second. Shift the entries!
					++(inCache->misses);
					inCache->cacheBlock[offset+1] = inCache->cacheBlock[offset];
					inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
					inCache->validBlock[offset+1] = 1;
					return 0;
				}
			}
		}else{//If the entry is not valid: miss, validate, and update data.
			inCache->validBlock[offset] = 1;
			inCache->cacheBlock[offset] = (address>>calcLog(inCache->blockSize));
			++(inCache->misses);
			return 0;
		}

	//Something went wrong and the type was not set correctly.
	}else{
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
	
	//Direct cache:
		//Access cost: 1
		//Miss cost:100
	if(inCache->type == 0){
		int temp = (inCache->misses)*100+(inCache->accesses);
		return temp;
	}else

	//Pseudo-associative cache:
		//Access cost: 1 (2 accesses for an initial miss.)
		//Miss cost:100
	if(inCache->type == 1){
		int temp = (inCache->misses)*100+(inCache->accesses)+(inCache->pseudoAccesses);
		return temp;
	}else

	//Fully-associative cache:
		//Access cost: 3
		//Miss cost:100
	if(inCache->type == 2){
		int temp = (inCache->misses)*100+(inCache->accesses)*3;
		return temp;
	}else{
		return -1;
	}
}

void printCache(void *cache){
	//Print every entry of the cache.
	Cache *inCache = cache;
	printf("Cache slots:");
	int i;
	for (i = 0; i < inCache->slots; i++){
		printf("[%d]:%d, ", i, inCache->cacheBlock[i]);
	}
	printf("\n");
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
