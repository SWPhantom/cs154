#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

InstInfo* pipelineInsts[5];
InstInfo curInsts[10];

int main(int argc, char *argv[])
{
	InstInfo *instPtr = &curInsts[0];
	
	InstInfo nullInst;
	InstInfo *nullPtr = &nullInst;
	nullPtr->inst = 0;
	int j;
	
	for(j = 0; j < 10; ++j){
		curInsts[j].inst = 0;
	}
	
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

	//Points all of the pointers to just beyond the number of InstInfo objects.
	for(j = 0; j < maxpc; ++j){
		pipelineInsts[j] = &curInsts[j+maxpc];
	}
	for(j = 0; j < maxpc; ++j){
		curInsts[j].inst = ;
	}
	int count = 0;
	while (pc <= maxpc)
	{

		fetch(instPtr);
		movePipeline(nullPtr);
		decode(instPtr); //Now needs to be run non-sequentially. 
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		movePipeline(nullPtr);
		count++;
		execute(instPtr);
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		movePipeline(nullPtr);
		count++;
		memory(instPtr);
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		movePipeline(nullPtr);
		count++;
		writeback(instPtr);
		//print(instPtr,instnum++);
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		movePipeline(nullPtr);
		count++;
	}
	
	// put in your own variables
	//printf("Cycles: %d\n", );
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

/* print
 *
 * prints out the state of the simulator after each instruction
 */
void print(InstInfo *inst, int count)
{
	int i, j;
	printf("Instruction %d: %d\n",count,inst->inst);
	printf("%s\n\n",inst->string);
	printf("Fields:\n rd: %d\nrs: %d\nrt: %d\nimm: %d\n\n",
		inst->fields.rd, inst->fields.rs, inst->fields.rt, inst->fields.imm);
	printf("Control Bits:\nalu: %d\nmw: %d\nmr: %d\nmtr: %d\nasrc: %d\nbt: %d\nrdst: %d\nrw: %d\n\n",
		inst->signals.aluop, inst->signals.mw, inst->signals.mr, inst->signals.mtr, inst->signals.asrc,
		inst->signals.btype, inst->signals.rdst, inst->signals.rw);
	printf("ALU Result: %d\n\n",inst->aluout);
	if (inst->signals.mr == 1)
		printf("Mem Result: %d\n\n",inst->memout);
	else
		printf("Mem Result: X\n\n");
	for(i=0;i<8;i++)
	{
		for(j=0;j<32;j+=8)
			printf("$%d: %4d ",i+j,regfile[i+j]);
		printf("\n");
	}
	printf("\n");
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
	if(inst2->inst != 0)
		printf("Execute instruction: %s\n", inst2->string);
	else
		printf("Execute instruction: \n");

	if(inst3->inst != 0)
		printf("Memory instruction: %s\n", inst3->string);
	else
		printf("Memory instruction: \n");

	if(inst4->inst != 0)
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
void movePipeline(InstInfo *empty){
	
	int i;
	for(i = 4; i >= 1; --i){
		pipelineInsts[i] = pipelineInsts[i-1];
	}
	if(instmem[pc] == 0){//If the next instruction is gone
		pipelineInsts[0] = empty;
	}
}

