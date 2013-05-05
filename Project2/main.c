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
		moveObjPipeline();

		int stall = checkDependencies(pipelineInsts[1]);
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
		if(pipelineInsts[1]->inst != 0 && stall==0){
			decode(pipelineInsts[1]); //Now needs to be run non-sequentially. 
		}		
		if (stall == 0){
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

void moveObjPipeline(){
		
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
			pipelineInsts[2]->inst = 0;
		}
	}
	if(instmem[pc] == 0){//If the next instruction is gone
		pipelineInsts[0]->inst = 0;
	}
}

//This function takes in an instruction in the decode stage and looks for dependencies 
//in the later stages (execute, memory, writeback). 
//Returns: 0 = No Dependencies Found 
//         1 = Dependency Found
int checkDependencies(InstInfo* decodeInst){
	int dependencyDetected = 0;
	//regRead1 and regRead2 represent registers that need to be checked ahead to make sure there is no 
	//conflict with another instruction writing to the same register. A value of -1 means that this 
	//variable is not in use (in I-format instructions, only one is used)
	int regRead1 = -1; 
	int regRead2 = -1;
	
	//Fill in regRead1 and regRead2. For R-format instructions, they are equal to rs and rt.
	//For I-format instructions, only rs is compared.
	if (decodeInst->fields.op == 48){
		regRead1 = decodeInst->fields.rs;
		regRead2 = decodeInst->fields.rt;
	}
	else{
		switch (decodeInst->fields.op){
			case 28: regRead1 = decodeInst->fields.rs; break; //subi
			case 6: regRead1 = decodeInst->fields.rs; break; //lw
			case 2: regRead1 = decodeInst->fields.rs; break; //sw
			default: break;
		}
	}
	
	//$0 is not a variable that needs to be checked, so reset it back to -1.
	if (regRead1 == 0) regRead1 = -1;
	if (regRead2 == 0) regRead2 = -1;
	
	//Compare regRead1/2 to values in registerQueue. If there is a match, then a conflict 
	//exists and there needs to be a stall. Break out of loop to prevent unnecessary calculations.
	int i=0;
	for (i=0; i<100; i++){
		if (regRead1 == registerQueue[i] || regRead1 == registerQueue[i]){
			//printf("STALL; regRead1: %d; regRead2: %d; regQueue[%d]: %d\n", regRead1,regRead2, i, registerQueue[i]);
			dependencyDetected=1;
			break;
		}
	}
	return dependencyDetected;
}

