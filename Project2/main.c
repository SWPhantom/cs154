/*
INFO FOR WHOEVER WORKS ON THIS NEXT.
The basics work now.
TODO: Data forwarding!
Test with "test1"
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
	
	int j;
	for(j = 0; j < 100; ++j){
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
	//printLoad(maxpc);
	
	for(j = 0; j < 5; ++j){
		pipelineInsts[j] = &curInsts[j];
	}
	
	int count = 0;
	int executions = 0;
	while (pc <= maxpc+4)
	{
		int stall = moveObjPipeline();

		//Returns 0 if there is no stall, 1 if there is a stall.
		//int stall = checkDependencies(pipelineInsts[1]); 
		if (stall != 0){
			maxpc++; //Increase maxPC to account for a stall
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[4]->inst != 0){
			writeback(pipelineInsts[4]);
			++executions;
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[3]->inst != 0){
			memory(pipelineInsts[3]);
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		if(pipelineInsts[2]->inst != 0){
			execute(pipelineInsts[2]);
		}
		//printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		//If a stall was detected, don't try to decode the instruction again.
		if(pipelineInsts[1]->inst != 0){
			decode(pipelineInsts[1]); //Now needs to be run non-sequentially. 
		}		
		if ( stall == 0){
			fetch(pipelineInsts[0]);
		}
		printP2(pipelineInsts[0],pipelineInsts[1],pipelineInsts[2],pipelineInsts[3],pipelineInsts[4], count);
		++count;
	}
	
	// put in your own variables
	printf("Cycles: %d\n", count );
	printf("Instructions Executed: %d\n", executions);
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
	
	/*
	printf("RegisterQueue: ");
	for (i=0; i<100; i++){
		if (registerQueue[i] != 0){
			printf("$%d, ", registerQueue[i]);
		}
	}
	printf("\n\n");
	*/
}

/*
 *Moves the pipeline along
 */
void movePipeline(InstInfo newInst){
	
	int i;
	for(i = 4; i >= 1; --i){
		//printf("		Before at %d: %d\n",i, pipelineInsts[i]);
		pipelineInsts[i] = pipelineInsts[i-1];
	}
	for(i = 4; i >= 1; --i){
		//printf("				After at %d: %d\n",i, pipelineInsts[i]);
	}
	if(instmem[pc] == 0){//If the next instruction is gone
		pipelineInsts[0] = &newInst;
	}else{
		pipelineInsts[0] = &newInst;
	}
}

int moveObjPipeline(){
		
	int i;
	
	//===========================================================
	//Stalls must be detected in the DECODE stage in the following code. 
	//If a stall is detected, do not move the data forward to the execute stage.
	//When a stall is detected in the decode stage, the FETCH stage must also 
	//be stalled.
	//Execute Stage: i = 2
	//Decode Stage:  i = 1
	//Fetch Stage:   i = 0
	//===========================================================
	
	int stall = 0;
	for(i = 4; i >= 1; --i){
		//printf("		Before at %d: %d\n",i, pipelineInsts[i]);
		
		
		//When in the execute stage, check the decode stage to see if data is eligible
		//to be passed up.
		if (i==2){
			stall = checkDependencies(pipelineInsts[i-1]);
		}
		
		//If no stall had been detected previously, move pipeline along.
		if (stall == 0){	
			pipelineInsts[i]->inst = pipelineInsts[i-1]->inst;
			
			pipelineInsts[i]->signals.aluop = pipelineInsts[i-1]->signals.aluop;
			pipelineInsts[i]->signals.mw = pipelineInsts[i-1]->signals.mw;
			pipelineInsts[i]->signals.mr = pipelineInsts[i-1]->signals.mr;
			pipelineInsts[i]->signals.mtr = pipelineInsts[i-1]->signals.mtr;
			pipelineInsts[i]->signals.asrc = pipelineInsts[i-1]->signals.asrc;
			pipelineInsts[i]->signals.btype = pipelineInsts[i-1]->signals.btype;
			pipelineInsts[i]->signals.rdst = pipelineInsts[i-1]->signals.rdst;
			pipelineInsts[i]->signals.rw = pipelineInsts[i-1]->signals.rw;
			
			pipelineInsts[i]->fields.rd = pipelineInsts[i-1]->fields.rd;
			pipelineInsts[i]->fields.rs = pipelineInsts[i-1]->fields.rs;
			pipelineInsts[i]->fields.rt = pipelineInsts[i-1]->fields.rt;
			pipelineInsts[i]->fields.imm = pipelineInsts[i-1]->fields.imm;
			pipelineInsts[i]->fields.op = pipelineInsts[i-1]->fields.op;
			pipelineInsts[i]->fields.func = pipelineInsts[i-1]->fields.func;
			
			pipelineInsts[i]->pc = pipelineInsts[i-1]->pc;
			pipelineInsts[i]->aluout = pipelineInsts[i-1]->aluout;
			pipelineInsts[i]->memout = pipelineInsts[i-1]->memout;
			pipelineInsts[i]->sourcereg = pipelineInsts[i-1]->sourcereg;
			pipelineInsts[i]->targetreg = pipelineInsts[i-1]->targetreg;
			pipelineInsts[i]->destreg = pipelineInsts[i-1]->destreg;
			pipelineInsts[i]->destdata = pipelineInsts[i-1]->destdata;
			int j;
			for(j = 0; j < 30; ++j){
				pipelineInsts[i]->string[j] = pipelineInsts[i-1]->string[j];
			}
			pipelineInsts[i]->s1data = pipelineInsts[i-1]->s1data;
			pipelineInsts[i]->s2data = pipelineInsts[i-1]->s2data;
			pipelineInsts[i]->input1 = pipelineInsts[i-1]->input1;
			pipelineInsts[i]->input2 = pipelineInsts[i-1]->input2;
		
		}
		else{
			pipelineInsts[2]->inst = 0; //Insert a nop bubble
		}
	}
	if(instmem[pc] == 0){//If the next instruction is gone
		pipelineInsts[0]->inst = 0;
	}
	return stall;
}

//This function takes in an instruction in the decode stage and looks for dependencies 
//in the later stages (execute, memory, writeback). 
//Returns: 0 = No Dependencies Found 
//         1 = Dependency Found
int checkDependencies(InstInfo* decodeInst){
	int dependencyDetected = 0;

	//If the instruction in the decode stage is R-formatted and there is no nop bubble in execute..
	if (decodeInst->fields.op == 48 && pipelineInsts[2]->inst != 0){
		//If the instruction in the execute stage is a lw or sw...
		if (pipelineInsts[2]->fields.op == 6 || pipelineInsts[2]->fields.op == 2){		
			//Check to see if there is a conflict between these operations
			if (pipelineInsts[2]->fields.rt == decodeInst->fields.rs || 
				pipelineInsts[2]->fields.rt == decodeInst->fields.rt)
			{
				dependencyDetected=1;
			}		
		}
	}
	return dependencyDetected;
}

int decodeMux(int input){
	InstInfo* decodeInst = pipelineInsts[1];
	InstInfo* executeInst = pipelineInsts[3];
	int output = 0;
	if (input == 1){
		printf("Decode mux switched on\n");
		//Both Execute and Decode are R-format
		if (decodeInst->fields.op == 48 && executeInst->fields.op == 48){
			if (executeInst->fields.rd == decodeInst->fields.rs){
				decodeInst->fields.rs = executeInst->aluout;
				output = 1;
			}
			if (executeInst->fields.rd == decodeInst->fields.rt){
				decodeInst->fields.rt = executeInst->aluout;
				output = 2;
			}
		}
		//Execute is R-format, Decode is I format
		else if (executeInst->fields.op == 48){
			if (executeInst->fields.rd == decodeInst->fields.rs){
				decodeInst->fields.rs = executeInst->aluout;
				output=1;
			}
		}
		//Execute is I-format, Decode is R frmat
		else if (decodeInst->fields.op == 48){
			if (executeInst->fields.rt == decodeInst->fields.rs){
				decodeInst->fields.rs = executeInst->aluout;
				output=1;
				//printf("Assigned aluout; aluout: %d\n", executeInst->aluout);
			}
			if (executeInst->fields.rt == decodeInst->fields.rt){
				decodeInst->fields.rt = executeInst->aluout;
				output=2;
				//printf("Assigned aluout; aluout: %d\n", executeInst->aluout);
			}
		}
		//Both Execute and Decode are I-format
		else{
			if (executeInst->fields.rt == decodeInst->fields.rt){
				decodeInst->fields.rt = executeInst->aluout;
				output=2;
			}
		}	
	}
	return output;
}

int aluMux(int* rs, int* rt){
	InstInfo* decodeInst = pipelineInsts[2];
	InstInfo* executeInst = pipelineInsts[3];
	int output = 0;
	if (executeInst->inst != 0){ //Check to make sure this is not a nop bubble
		//Both Execute and Decode are R-format
		if (decodeInst->fields.op == 48 && executeInst->fields.op == 48){
			if (executeInst->fields.rd == decodeInst->fields.rs){
				*rs = executeInst->aluout;
				output = 1;
			}
			if (executeInst->fields.rd == decodeInst->fields.rt){
				*rt = executeInst->aluout;
				output = 2;
			}
		}
		//Execute is R-format, Decode is I format
		else if (executeInst->fields.op == 48){
			if (executeInst->fields.rd == decodeInst->fields.rs){
				*rs = executeInst->aluout;
				output=1;
			}
		}
		//Execute is I-format, Decode is R frmat
		else if (decodeInst->fields.op == 48){
			if (executeInst->fields.rt == decodeInst->fields.rs){
				*rs = executeInst->aluout;
				output=1;
			}
			if (executeInst->fields.rt == decodeInst->fields.rt){
				*rt = executeInst->aluout;
				output=2;
			}
		}
		//Both Execute and Decode are I-format
		else{
			if (executeInst->fields.rt == decodeInst->fields.rt){
				*rt = executeInst->aluout;
				output=2;
			}
		}	
	}
	return output;
}