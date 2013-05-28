#ifndef CACHESIM_H
#define CACHESIM_H

////=========================Declarations==============================================
typedef struct _cache{
	int cacheSize;
	int blockSize;
	int type;
	int slots;
	int offsetSize;
	int byteOffset;
	
	int *cacheBlock;
	int *validBlock;
	
	int misses;
	int accesses;
	int totalAccessTime;
	int pseudoAccesses;
} Cache;

////=======================Function Prototypes=========================================

/*
 *Allocate a table with numEntries entries, each initialized to the not-taken
 *state. numEntries will be a power of 2.
 */
void *createAndInitialize(int numEntries);

/*
 *Given the PC of a branch, report whether this branch is predicted as taken
 *or not taken. (1 = taken, 0 = not taken)
 */
int accessBranchPredictor(void *bp, int PC);


/*
 *Given the PC of the branch and the actual result (1 = taken, 0 = not taken),
 *update the branch predictor for that entry in the table.
 */
void updateBranchPredictor(void *bp, int PC, int result);

/*
 *Report the total number of accesses
 */
int numAccesses(void *bp);

/*
 *Report the total number of branches that were incorrectly predicted
 */
int numMispredictions(void *bp);
////=========================Function Definitions End==================================

#endif
