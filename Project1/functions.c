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
    
    pc =  0;//This is to set the program counter to the initial instruction.
    
    //printf("Total instructions: %d", index);//Debug
    return index-1;
}

/* fetch
 *
 * This fetches the next instruction and updates the program counter.
 * "fetching" means filling in the inst field of the instruction.
 */
void fetch(InstInfo *instruction)
{  
    instruction->inst = instmem[pc];
    pc++;
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
    imm = val & 0xffff;	//Take right 16 bits in case immediate is used.
    instruction->fields.op = op;
    instruction->fields.func = func;
    instruction->fields.rd = rd;
    instruction->fields.rs = rs;
    instruction->fields.rt = rt;
    instruction->fields.imm = imm;
    
    // now fill in the signals

    ////This is a massive if section. Gross, but I don't think there is an alternative.
    
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
        }
        //Or - TODO: Please Verify and delete this if correct---------------------------!!!!!!
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
        }
        //SLT - TODO: Please Verify and delete this if correct---------------------------!!!!!!
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
        }
        //XOR - TODO: Please Verify and delete this if correct---------------------------!!!!!!
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
    }
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
    }
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
        //Format as either "lw $x, $y" or "lw $x, z($y)" depending on immediate value
        sprintf(instruction->string,"lw $%d, %d ($%d)",instruction->fields.rt, 
			instruction->fields.imm, instruction->fields.rs);
        instruction->destreg = instruction->fields.rt;		
    }	
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
        //Format as either "sw $x, $y" or "sw $x, z($y)" depending on immediate value
		sprintf(instruction->string,"sw $%d, %d ($%d)",instruction->fields.rt, 
			instruction->fields.imm, instruction->fields.rs);
        //TODO: Incomplete. Need to update assignment statement
        instruction->destdata = instruction->fields.rt;	//destdata = Mem[rs+imm]	
    }	
    //Operation with op code 100111: bge 
    if (instruction->fields.op == 39){
        instruction->signals.aluop = 5; //SUB. Verification needed.
        instruction->signals.mw = 0;
        instruction->signals.mr = 0;
        instruction->signals.mtr = -1;
        instruction->signals.asrc = 0;
        instruction->signals.btype = 2;
        instruction->signals.rdst = -1;
        instruction->signals.rw = 0;
        //TODO: bge's last argument should be a label. Need to verify label format
		sprintf(instruction->string,"bge $%d, $%d, %d",instruction->fields.rs,
			instruction->fields.rt, instruction->fields.imm);
    }
    
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
    }
    
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
    ////This is where the other operations will go.

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
