/*
INFO FOR WHOEVER WORKS ON THIS NEXT.

I made a reasonable loop, I think. The problem now is that the object
that the pointers in pipelineInsts is the same. I am having an issue
in my helper function movePipeline, probably. 

Maybe moving pointers by making an = sign makes EVERY POITER THE SAME
THING... Which would be a problem.
Maybe instead of assigning pointers to other pointers we can create
and copy a new object every time... though I'm not sure if that is 
easy, much less correct.


*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

InstInfo* pipelineInsts[5];
InstInfo curInsts[100];

int main(int argc, char *argv[])
{
	InstInfo instruction;
	InstInfo *instPtr = &instruction;
	
	InstInfo nullInst;
	InstInfo *nullPtr = &nullInst;
	nullPtr->inst = 0;

	int instnum = 0;
	int maxpc;
	FILE *program;
	if (argc != 2)
	{
		printf("Usage: sim filename\n");
		exit(0);
	}

	maxpc = load(argv[1]);
	printLoad(maxpc);
	
	int j;
	for(j = 0; j < 5; ++j){
		pipelineInsts[j] = nullPtr;
	}
	
	int count = 0;
	while (pc <= maxpc+4)
	{
		movePipeline(curInsts[pc]);
		fetch(pipelineInsts[0]);
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[1]->inst != 0){
			decode(pipelineInsts[1]); //Now needs to be run non-sequentially. 
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[2]->inst != 0){
			execute(pipelineInsts[2]);
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[3]->inst != 0){
			memory(pipelineInsts[3]);
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[4]->inst != 0){
			writeback(pipelineInsts[4]);
		}
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		++count;
	}
	
	// put in your own variables
	printf("Cycles: %d\n", count );
	//printf("Instructions Executed: %d\n", );
	exit(0);
}



/*
 * print out the loaded instructions.  This is to verify your loader
 * works.
 */
void printLoad(int max)
{
	int i;
	for(i=0;i<=max;i++)
		printf("%d\n",instmem[i]);
}


void printP2(InstInfo *inst0, InstInfo *inst1, InstInfo *inst2, InstInfo *inst3, InstInfo *inst4,  int count)
{
	int i, j;
	printf("Cycle %d:\n",count);
	if(inst0->inst != 0)
		printf("Fetch instruction: %d\n", inst0->inst);
	else
		printf("Fetch instruction: \n");
	if(inst1->inst != 0)
		printf("Decode instruction: %s\n", inst1->string);
	else
		printf("Decode instruction: \n");
	if(inst2->inst !=0)
		printf("Execute instruction: %s\n", inst2->string);
	else
		printf("Execute instruction: \n");

	if(inst3->inst !=0)
		printf("Memory instruction: %s\n", inst3->string);
	else
		printf("Memory instruction: \n");

	if(inst4->inst !=0)
		printf("Writeback instruction: %s\n", inst4->string);
	else
		printf("Writeback instruction: \n");



	for(i=0;i<8;i++)
	{
		for(j=0;j<32;j+=8)
			printf("$%d: %4d ",i+j,regfile[i+j]);
		printf("\n");
	}
	printf("\n");
}

/*
 *Moves the pipeline along
 */
void movePipeline(InstInfo newInst){
	
	int i;
	for(i = 4; i >= 1; --i){
		pipelineInsts[i] = pipelineInsts[i-1];
	}
	if(instmem[pc] == 0){//If the next instruction is gone
		pipelineInsts[0] = &newInst;
	}else{
		pipelineInsts[0] = &newInst;
	}
}

