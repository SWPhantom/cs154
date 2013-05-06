#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

////=========================Global Variables==========================================
// register file
int regfile[32];
// instruction memory
int instmem[100];  // only support 100 static instruction
// data memory
int datamem[1000];
// program counter
int pc;
//Registers in queue to be written
int registerQueue[100];
//Number of registers in registerQueue
int regQueueCount;
////=========================Global Variables END======================================


////=========================Function Implementation===================================
/* load
*
* Given the filename, which is a text file that
* contains the instruction stored as integers
*
* You will need to load it into your global structure that
* stores all of the instruction.
*
* The return value is the maxpc - the number of instruction
* in the file
*/

int load(char *filename)
{
	//printf("About to open the fileee.\n");
	FILE *program;
	//Opens the specified file.
	program = fopen(filename, "r");
	if (program == NULL) {//error if file can't be opened.
	fprintf(stderr, "Can't open file!\n");
	exit(1);
	}

	//Add the integer commands to the array.
	//printf("About to scan the file!\n");
	int index = 0;
	while(fscanf(program, "%d", &instmem[index]) != EOF){	//While the next line is not EOF, read in to the array.
		//printf("Data in position %d: %d\n", index, instmem[index]);//DEBUG---------------------
		++index;
	}
	//Sets the non-used addresses to 0
	int i;
	for(i = index; i < 100; ++i){
		instmem[i] = 0;
		registerQueue[i] = 0;
	}

	pc = 0;//This is to set the program counter to the initial instruction.
	regQueueCount = 0;
	//printf("Total instruction: %d\n", index);//Debug---------------------------
	return index-1; //Adjust index for off-by-one error from while loop
}


/* fetch
*
* This fetches the next instruction and updates the program counter.
* "fetching" means filling in the inst field of the instruction.
*/
void fetch(InstInfo *instruction)
{
	instruction->inst = instmem[pc];
	//printf("ADDRESS OF instmem[%d]: %d\n", pc, instruction->inst);
	//pipelineInsts[0] = instruction;
	++pc; //Increment the PC counter
}

/* decode
*
* This decodes an instruction.  It looks at the inst field of the
* instruction.  Then it decodes the fields into the fields data
* member.  The first one is given to you.
*
* Then it checks the op code.  Depending on what the opcode is, it
* fills in all of the signals for that instruction.
*/
void decode(InstInfo *instruction)
{
	// fill in the signals and fields
	int val = instruction->inst;
	int op, func, rs, rd, rt, imm; //Added rs, rd, rt, imm
	op = (val >> 26) & 0x03f;	//Take the first 6 bits.
	func = val & 0x3f;	//Take the last 6 bits.
	rs = (val >> 21) & 0x1f;	//Take corresponding bits.
	rt = (val >> 16) & 0x1f;	//"
	rd = (val >> 11) & 0x1f;	//"
	imm = (val >> 0) & 0xffff;	//Take right 16 bits in case immediate is used.

	//if (imm > 32767){ //If imm is very large, it needs to be complemented
		//Magic that performs two's complement
		//See: http://forums.devshed.com/c-programming-42/converting-a-number-in-two-s-complement-223374.html
		imm = (imm << 16) >> 16;
	//}

	instruction->fields.op = op;
	instruction->fields.func = func;
	instruction->fields.rd = rd;
	instruction->fields.rs = rs;
	instruction->fields.rt = rt;
	instruction->fields.imm = imm;
	int i;
	////IF/ELSE loop that assigns the fields and signals.
	//Things with the 110000 op code.
	if(instruction->fields.op == 48){
		//Add
		if(instruction->fields.func == 10){	//Check to see if the func is 001010
			instruction->signals.aluop = 1;
			instruction->signals.mw = 0;
			instruction->signals.mr = 0;
			instruction->signals.mtr = 0;
			instruction->signals.asrc = 0;
			instruction->signals.btype = 0;
			instruction->signals.rdst = 1;
			instruction->signals.rw = 1;
			sprintf(instruction->string,"add $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs,
			instruction->fields.rt);
			instruction->destreg = instruction->fields.rd;
		}else
		//Or
		if(instruction->fields.func == 48){	//Check to see if the func is 110000
			instruction->signals.aluop = 4;
			instruction->signals.mw = 0;
			instruction->signals.mr = 0;
			instruction->signals.mtr = 0;
			instruction->signals.asrc = 0;
			instruction->signals.btype = 0;
			instruction->signals.rdst = 1;
			instruction->signals.rw = 1;
			sprintf(instruction->string,"or $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs,
			instruction->fields.rt);
			instruction->destreg = instruction->fields.rd;
		}else
		//SLT
		if(instruction->fields.func == 15){	//Check to see if the func is 001111
			instruction->signals.aluop = 6;
			instruction->signals.mw = 0;
			instruction->signals.mr = 0;
			instruction->signals.mtr = 0;
			instruction->signals.asrc = 0;
			instruction->signals.btype = 0;
			instruction->signals.rdst = 1;
			instruction->signals.rw = 1;
			sprintf(instruction->string,"slt $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs,
			instruction->fields.rt);
			instruction->destreg = instruction->fields.rd;
		}else
		//XOR
		if(instruction->fields.func == 20){	//Check to see if the func is 010100
			instruction->signals.aluop = 3;
			instruction->signals.mw = 0;
			instruction->signals.mr = 0;
			instruction->signals.mtr = 0;
			instruction->signals.asrc = 0;
			instruction->signals.btype = 0;
			instruction->signals.rdst = 1;
			instruction->signals.rw = 1;
			sprintf(instruction->string,"xor $%d, $%d, $%d",
			instruction->fields.rd, instruction->fields.rs,
			instruction->fields.rt);
			instruction->destreg = instruction->fields.rd;
		}
	}else
	//Operation with op code 011100: subi
	if (instruction->fields.op == 28){
		instruction->signals.aluop = 5;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = 0;
		instruction->signals.asrc = 1;
		instruction->signals.btype = 0;
		instruction->signals.rdst = 0;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"subi $%d, $%d, %d",instruction->fields.rt, instruction->fields.rs, instruction->fields.imm);
		instruction->destreg = instruction->fields.rt;
	}else
	//Operation with op code 000110: lw
	if (instruction->fields.op == 6){
		instruction->signals.aluop = 1; //LW should use the Add ALUop
		instruction->signals.mw = 0;
		instruction->signals.mr = 1;
		instruction->signals.mtr = 1;
		instruction->signals.asrc = 1;
		instruction->signals.btype = 0;
		instruction->signals.rdst = 0;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"lw $%d, %d ($%d)",instruction->fields.rt, instruction->fields.imm, instruction->fields.rs);
		instruction->destreg = instruction->fields.rt;
	}else
	//Operation with op code 000110: sw
	if (instruction->fields.op == 2){
		instruction->signals.aluop = 1; //SW should use the Add ALUop
		instruction->signals.mw = 1;
		instruction->signals.mr = 0;
		instruction->signals.mtr = -1;
		instruction->signals.asrc = 1;
		instruction->signals.btype = 0;
		instruction->signals.rdst = -1;
		instruction->signals.rw = 0;
		sprintf(instruction->string,"sw $%d, %d ($%d)",instruction->fields.rt, instruction->fields.imm, instruction->fields.rs);
		instruction->destdata = instruction->fields.rt;	//destdata = Mem[rs+imm]
	}else
	//Operation with op code 100111: bge
	if (instruction->fields.op == 39){
		instruction->signals.aluop = 5;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = -1;
		instruction->signals.asrc = 0;
		instruction->signals.btype = 2;
		instruction->signals.rdst = -1;
		instruction->signals.rw = 0;
		sprintf(instruction->string,"bge $%d, $%d, %d",instruction->fields.rs, instruction->fields.rt, instruction->fields.imm);
	}else
	//Operation with op code 100100: j
	if (instruction->fields.op == 36){
		instruction->signals.aluop = -1;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = -1;
		instruction->signals.asrc = -1;
		instruction->signals.btype = 1;
		instruction->signals.rdst = -1;
		instruction->signals.rw = 0;
		sprintf(instruction->string,"j %d", instruction->fields.imm);
		pc = instruction->fields.imm; //POSSIBLY THE WRONG PLACE D:
	}else
	//Operation with op code 100010: jal
	if (instruction->fields.op == 34){
		instruction->signals.aluop = -1;
		instruction->signals.mw = 0;
		instruction->signals.mr = 0;
		instruction->signals.mtr = -1;
		instruction->signals.asrc = -1;
		instruction->signals.btype = 1;
		instruction->signals.rdst = -1;
		instruction->signals.rw = 1;
		sprintf(instruction->string,"jal %d", instruction->fields.imm);
		//Need to write to $ra = register 31
		instruction->destreg = 31;
	}
}

/* execute
*
* This fills in the aluout value into the instruction and destdata
*/

void execute(InstInfo *instruction)
{
	int rsCpy = regfile[instruction->fields.rs];
	int rtCpy = regfile[instruction->fields.rt];

	int* rs = &rsCpy;
	int* rt = &rtCpy;
	
	int x = aluMux(rs, rt);
	if (x == 1 || x == 2){
		//printf("rs: %d, rt: %d\n", *rs, *rt);
	}
	
	if(instruction->fields.op == 48){
		//Add
		if(instruction->fields.func == 10){	//Check to see if the func is 001010
			//"add $rd, $rs, $rt"
			instruction->aluout = *rs + *rt;
		}else
		//Or
		if(instruction->fields.func == 48){	//Check to see if the func is 110000
			//"or $rd, $rs, $rt"
			instruction->aluout = *rs | *rt;
		}else
		//SLT
		if(instruction->fields.func == 15){	//Check to see if the func is 001111
			//"slt $rd, $rs, $rt"
			if(*rs - *rt < 0){
				instruction->aluout = 1;
			}else{
				instruction->aluout = 0;
			}
		}else
		//XOR
		if(instruction->fields.func == 20){	//Check to see if the func is 010100
			//"xor $rd, $rs, $rt"
			instruction->aluout = *rs ^ *rt;
		}
	}else
	//Operation with op code 011100: subi
	if (instruction->fields.op == 28){
		//"subi $rt, $rs, imm"
		instruction->aluout = *rs - instruction->fields.imm;
	}else
	//Operation with op code 000110: lw
	if (instruction->fields.op == 6){
		//"lw $rt, imm ($rs)"
		instruction->aluout = *rs + instruction->fields.imm;
	}else
	//Operation with op code 000110: sw
	if (instruction->fields.op == 2){
		//"sw $rt, imm ($rs)"
		instruction->aluout = *rs + instruction->fields.imm;
	}else
	//Operation with op code 100111: bge
	if (instruction->fields.op == 39){
		//"bge $rs, $rt, imm"
		instruction->aluout = *rs - *rt;
		//printf("rs = %d\nrt = %d\n\n",instruction->fields.rs, instruction->fields.rt); DEBUG
		if(instruction->aluout >= 0){
			pc += (instruction->fields.imm);
		}
	}
}

/* memory
*
* If this is a load or a store, perform the memory operation
*/
void memory(InstInfo *instruction)
{
	//Operation with op code 000110: lw
	if (instruction->fields.op == 6){
		//"lw $rt, imm ($rs)"
		instruction->memout = datamem[(instruction->aluout)>>2];
	}else
	//Operation with op code 000110: sw
	if (instruction->fields.op == 2){
		//"sw $rt, imm ($rs)"
		datamem[(instruction->aluout)>>2] = regfile[instruction->fields.rt];
	}
}

/* writeback
*
* If a register file is supposed to be written, write to it now
*/
void writeback(InstInfo *instruction)
{
	int i;
	if(instruction->fields.op == 48){
		//Add
		if(instruction->fields.func == 10){	//Check to see if the func is 001010
			//"add $rd, $rs, $rt"
			regfile[instruction->fields.rd] = instruction->aluout;
		}else
		//Or
		if(instruction->fields.func == 48){	//Check to see if the func is 110000
			//"or $rd, $rs, $rt"
			regfile[instruction->fields.rd] = instruction->aluout;
		}else
		//SLT
		if(instruction->fields.func == 15){	//Check to see if the func is 001111
			//"slt $rd, $rs, $rt"
			regfile[instruction->fields.rd] = instruction->aluout;
		}else
		//XOR
		if(instruction->fields.func == 20){	//Check to see if the func is 010100
			//"xor $rd, $rs, $rt"
			regfile[instruction->fields.rd] = instruction->aluout;			
		}
	}else
	//Operation with op code 011100: subi
	if (instruction->fields.op == 28){
		//"subi $rt, $rs, imm"
		regfile[instruction->fields.rt] = instruction->aluout;
	}else
	//Operation with op code 000110: lw
	if (instruction->fields.op == 6){
		//"lw $rt, imm ($rs)"
		regfile[instruction->fields.rt] = instruction->memout;
	}else
	//Operation with op code 100010: jal
	if (instruction->fields.op == 34){
		//"jal imm"
		regfile[31] = pc;
		pc = instruction->fields.imm;
	}
}
////=========================Function Implementation END===============================
