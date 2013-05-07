#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

//CS154 Project 2
//Brian Ly, Yevgen Frolov, Justin Flores

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
		//On a new cycle, look at a branch in the Decode stage (if it exists) 
		//and determine whether it should be taken.
		
		//branchTaken is the imm field of the branch instruction. It is set to 0 if
		//no branch action is taken or if a branch instruction doesn't exist.
		int branchTarget = 0; 
		branchTarget = checkBranch();
		if (branchTarget != 0){//
			//printf("pc: %d, branchTarget: %d\n", pc, branchTarget);
			pc = branchTarget; //...why does this work? 'pc += branchTarget' did not work.
		}		
		
		int stall = moveObjPipeline();
		if(pipelineInsts[4]->inst != 0){
			writeback(pipelineInsts[4]);
			++executions;
		}
		if(pipelineInsts[3]->inst != 0){
			memory(pipelineInsts[3]);
		}
		if(pipelineInsts[2]->inst != 0){
			execute(pipelineInsts[2]);
		}
		if(pipelineInsts[1]->inst != 0){
			decode(pipelineInsts[1]); //Now needs to be run non-sequentially. 
		}		
		//Don't fetch another instruction if there was a stall
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

//This function moves the pipelong.
//RETURNS: '0' if no stall is found
//         '1' if stall is found
int moveObjPipeline(){
		
	int i;
	
	//===========================================================
	//Stalls must be detected in the DECODE stage in the following code. 
	//If a stall is detected, do not move the data forward to the execute stage.
	//===========================================================
	
	int stall = 0;
	for(i = 4; i >= 1; --i){
		//When in the EXECUTE stage, check the decode stage to see if data is eligible
		//to be passed up. If a dependency is found, set a flag to stall the pipeline.
		if (i==2){
			stall = checkDependencies(pipelineInsts[i-1]);
		}
		
		//If no stall was detected, move the pipeline along.
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
		//If a stall was detected, insert a nop bubble into the execute stage
		else{
			pipelineInsts[2]->inst = 0; //Insert a nop bubble
			pipelineInsts[2]->fields.rd = 0;
			pipelineInsts[2]->fields.rs = 0;
			pipelineInsts[2]->fields.rt = 0;
			pipelineInsts[2]->fields.imm = 0;
			pipelineInsts[2]->fields.op = 0;
			pipelineInsts[2]->fields.func = 0;
		}
	}
	return stall;
}

//===========================================
//This function takes in an instruction in the decode stage and looks forward to the 
//execute stage to see if there is a data dependency between them. This solves the specific 
//case where a lw or sw is followed by an instruction containing a dependent register. 
//This specific case is tested for in the DATA HAZARDS test cases.
//Returns: 0 = No Dependencies Found 
//         1 = Dependency Found
//===========================================
int checkDependencies(InstInfo* decodeInst){
	int dependencyDetected = 0;

	//The following blocks of code will compare the decode instruction with the execute 
	//instruction. There are two cases to account for: one where the decode stage is an R-
	//format instruction and another where it is a I-format instruction. The cases are very
	//similar, the only difference being that I-format instruction does not check its rt register.
	
	//First, make sure this is not a nop bubble.
	if (pipelineInsts[2]->inst != 0){
		//CASE 1: R-Format instruction in decode stage
		if (decodeInst->fields.op == 48){
			//If the instruction in the execute stage is a lw or sw...
			if (pipelineInsts[2]->fields.op == 6 || pipelineInsts[2]->fields.op == 2){		
				//Check to see if there is a conflict between these two instructions
				if (pipelineInsts[2]->fields.rt == decodeInst->fields.rs || 
					pipelineInsts[2]->fields.rt == decodeInst->fields.rt)
				{
					dependencyDetected=1;
				}		
			}
		}
		//Case 2: I-Format instruction in decode stage
		else{
			if (decodeInst->fields.op == 28 || decodeInst->fields.op == 39){ //subi and bge
				//If the instruction in the execute stage is a lw or sw...
				if (pipelineInsts[2]->fields.op == 6 || pipelineInsts[2]->fields.op == 2){		
					//Check to see if there is a conflict between these two instructions
					if (pipelineInsts[2]->fields.rt == decodeInst->fields.rs)
					{
						dependencyDetected=1;
					}		
				}			
			}
		}
	}
	return dependencyDetected;
}

//==========================================
//Compares the previous execute function and the current execute function (the
//memory instruction still contains the aluout of the previous execute function, so 
//that represents it). If the current execute function relies on data from the past 
//execute function, then forward the data from the previous aluout to the current
//relevant register. This function solves the DATA FORWARDING test cases and is called
//at the beginning of execute().
//RETURNS: If there is no dependency found, a 0 is returned. If a dependency was found,
//         then it returns '1' if rs was dependent and '2' if rt was dependent.
//===========================================
int aluMux(int* rs, int* rt){
	InstInfo* executeInst = pipelineInsts[2];
	InstInfo* memoryInst = pipelineInsts[3];
	int output = 0;
	//The following block compares the two instructions to see if there is a data
	//dependency between them. There are 4 cases to check for based on the instruction
	//types.
	if (memoryInst->inst != 0){ //First make sure this is not a nop bubble
		//Case 1: Both Execute and Memory are R-format
		if (executeInst->fields.op == 48 && memoryInst->fields.op == 48){
			if (memoryInst->fields.rd == executeInst->fields.rs){
				*rs = memoryInst->aluout;
				output = 1;
			}
			if (memoryInst->fields.rd == executeInst->fields.rt){
				*rt = memoryInst->aluout;
				output = 2;
			}
		}
		//Case 2: Execute is R-format, Memory is I format
		else if (memoryInst->fields.op == 48){
			if (memoryInst->fields.rd == executeInst->fields.rs){
				*rs = memoryInst->aluout;
				output=1;
			}
		}
		//Case 3: Execute is I-format, Memory is R frmat
		else if (executeInst->fields.op == 48){
			if (memoryInst->fields.rt == executeInst->fields.rs){
				*rs = memoryInst->aluout;
				output=1;
			}
			if (memoryInst->fields.rt == executeInst->fields.rt){
				*rt = memoryInst->aluout;
				output=2;
			}
		}
		//Case 4: Both Execute and Memory are I-format
		else{
			if (memoryInst->fields.rt == executeInst->fields.rs){
				*rs = memoryInst->aluout;
				output=2;
			}
		}	
	}
	return output;
}

//===============================================
//This function will confirm whether a branch should be taken
//at the Decode stage. If a data dependency is detected, then forward 
//that data into the Decode stage for calculation.
//RETURNS: The instruction to be jumped to; 0 if no action
//		   
//===============================================
int checkBranch(){
	int action = 0;
	//Check if a branch instruction exists in Decode stage
	if (pipelineInsts[1]->fields.op == 39){ //bge
		InstInfo* branch = pipelineInsts[1];
		
		//Create copies of register values so we don't overwrite the real ones
		int rsCpy = regfile[branch->fields.rs];
		int rtCpy = regfile[branch->fields.rt];
		int* rs = &rsCpy;
		int* rt = &rtCpy;
		//printf("Checking branch in DECODE section\n");
		//Check for dependencies in Execute and Memory stages(Writeback should 
		//already be taken care of). If a dependency is found, forward data.
		action = checkBranchWithExecute(rs, rt);
		//If data has been forwarded from execute, no need to forward from memory
		if (action==0){
			action = checkBranchWithMemory(rs, rt);
		}
		//int rt = regfile[branch->fields.rt];
		//int rs = regfile[branch->fields.rs];
		//printf("rt: %d, rs: %d\n", rt, rs);
		if (*rs >= *rt){
			//printf("Branch action taken\n");
			action = branch->fields.imm;
			pipelineInsts[0]->inst = 0; //Insert a nop bubble
			pipelineInsts[0]->fields.rd = 0;
			pipelineInsts[0]->fields.rs = 0;
			pipelineInsts[0]->fields.rt = 0;
			pipelineInsts[0]->fields.imm = 0;
			pipelineInsts[0]->fields.op = 0;
			pipelineInsts[0]->fields.func = 0;
		}	
		action = pc + action - 1; //Subtract one because we actually do the calculation on the next cycle
	}
	//TODO: Later, if we need to store the value of jal into register 31, this block should be split up 
	//      and jal should be given its own block that stores return address at register 31.
	else if (pipelineInsts[1]->fields.op == 34 || pipelineInsts[1]->fields.op == 36){ //j and jal
		InstInfo* branch = pipelineInsts[1];
		action = branch->fields.imm;
		pipelineInsts[0]->inst = 0; //Insert a nop bubble
		pipelineInsts[0]->fields.rd = 0;
		pipelineInsts[0]->fields.rs = 0;
		pipelineInsts[0]->fields.rt = 0;
		pipelineInsts[0]->fields.imm = 0;
		pipelineInsts[0]->fields.op = 0;
		pipelineInsts[0]->fields.func = 0;	
		if (pipelineInsts[1]->fields.op == 34){
			pipelineInsts[1]->aluout = pc - 1; //If jal, write to register now.
		}
	}
	return action;
}


//===============================================
//This function will checks for a dependency between the Decode
//stage and the Execute stage. If a dependency is found, it will 
//forward the data from Execute -> Decode using aluout.
//RETURNS: The instruction to be jumped to; 0 if no action
//===============================================
int checkBranchWithExecute(int* rs, int* rt){
	InstInfo* decodeInst = pipelineInsts[1];
	InstInfo* executeInst = pipelineInsts[2];
	int output = 0;
	//The following block compares the two instructions to see if there is a data
	//dependency between them. There are 2 cases to check for based on the instruction
	//types.
	if (executeInst->inst != 0){ //First make sure this is not a nop bubble
		//Case 1: Execute is in R-format
		if (executeInst->fields.op == 48){
			if (executeInst->fields.rd == decodeInst->fields.rs){
				//printf("Branch dependency found with Execute\n");
				*rs = executeInst->aluout;
				output=decodeInst->fields.imm;
			}
			if (executeInst->fields.rd == decodeInst->fields.rt){
				//printf("Branch dependency found with Execute\n");
				*rt = executeInst->aluout;
				output=decodeInst->fields.imm;
			}
		}
		//Case 2: Execute is in I-format
		else{
			if (executeInst->fields.rt == decodeInst->fields.rt){
				*rt = executeInst->aluout;
				//printf("Branch dependency found with Execute\n");
				output=decodeInst->fields.imm;
			}
			if (executeInst->fields.rt == decodeInst->fields.rs){
				*rs = executeInst->aluout;
				//printf("Branch dependency found with Execute\n");
				output=decodeInst->fields.imm;
			}
		}	
	}
	return output;
}

//===============================================
//This function will checks for a dependency between the Decode
//stage and the Memory stage. If a dependency is found, it will 
//forward the data from Memory -> Decode using aluout.
//RETURNS: The instruction to be jumped to; 0 if no action
//===============================================
int checkBranchWithMemory(int* rs, int* rt){
	InstInfo* decodeInst = pipelineInsts[1];
	InstInfo* memoryInst = pipelineInsts[3];
	int output = 0;
	//The following block compares the two instructions to see if there is a data
	//dependency between them. There are 2 cases to check for based on the instruction
	//types.
				
	if (memoryInst->inst != 0){ //First make sure this is not a nop bubble
		//Case 1: Memory is in R-format
		if (memoryInst->fields.op == 48){
			if (memoryInst->fields.rd == decodeInst->fields.rs){
				//printf("Branch dependency found with Execute\n");
				*rs = memoryInst->aluout;
				output=decodeInst->fields.imm;
			}
			if (memoryInst->fields.rd == decodeInst->fields.rt){
				//printf("Branch dependency found with Execute\n");
				*rt = memoryInst->aluout;
				output=decodeInst->fields.imm;
			}
		}
		
		//Case 2: Memory is in I-format
		else{
			if (memoryInst->fields.rt == decodeInst->fields.rt){
				*rt = memoryInst->aluout;
				//printf("Branch dependency found with Execute\n");
				output=decodeInst->fields.imm;
			}
			if (memoryInst->fields.rt == decodeInst->fields.rs){
				*rs = memoryInst->aluout;
				//printf("Branch dependency found with Execute\n");
				output=decodeInst->fields.imm;
			}
		}
		
	}
	
	return output;
}