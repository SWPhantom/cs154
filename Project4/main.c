#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "branchsim.h"

//CS154 Project 3
//Brian Ly, Yevgen Frolov, Justin Flores

int main(int argc, char *argv[])
{
	void *cache1;
	int cacheSize = 32;
	int temp = 0;
	cache1 = createAndInitialize(cacheSize);
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 1);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 1);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 1);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	printf("Prediction for %d: %d\n", temp, accessBranchPredictor(cache1, temp));
	updateBranchPredictor(cache1, temp, 0);
	temp += 4;
	
}
