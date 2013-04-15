
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "functions.h"

// these are the structures used in this simulator


// global variables
// register file
int regfile[32];
// instruction memory
int instmem[100];  // only support 100 static instructions
// data memory
int datamem[1000];
// program counter
int pc;

/* load
 *
 * Given the filename, which is a text file that 
 * contains the instructions stored as integers 
 *
 * You will need to load it into your global structure that 
 * stores all of the instructions.
 *
 * The return value is the maxpc - the number of instructions
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
	while(fscanf(program, "%d", &instmem[index]) != EOF){	//While the next line
																			//is not EOF, read in
																			//to the array.
		//printf("Data in position %d: %d\n", index, instmem[index]);//DEBUG!
		++index;
	}
	//Sets the non-used addresses to 0
	int i;
	for(i = index; i < 100; ++i){
		instmem[i] = 0;
	}
	//printf("Total instructions: %d", index);//Debug
	return index;
}

/* fetch
 *
 * This fetches the next instruction and updates the program counter.
 * "fetching" means filling in the inst field of the instruction.
 */
void fetch(InstInfo *instruction)
{
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
	int op, func;
	instruction->fields.op = (val >> 26) & 0x03f;
	// fill in the rest of the fields here

	// now fill in the signals

	// if it is an add
	//if(instruction->fields.op == 0x1ADB0){	//Check to see if the
															//opcode is 110000
		//if(instruction->fields.func == 0x3F2){	//Check to see if the
																//funccode is 001010
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
		//}
	//}

	// fill in s1data and input2
}

/* execute
 *
 * This fills in the aluout value into the instruction and destdata
 */

void execute(InstInfo *instruction)
{

}

/* memory
 *
 * If this is a load or a store, perform the memory operation
 */
void memory(InstInfo *instruction)
{

}

/* writeback
 *
 * If a register file is supposed to be written, write to it now
 */
void writeback(InstInfo *instruction)
{
}

