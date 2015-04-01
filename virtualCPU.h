#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 	0x4000
#define MAX_INPUT 	128
#define MAX32		0xFFFFFFFF
#define SUB_VALUE	0xFFFFFFFF
#define STACK_POINTER 	registers[0xD]
#define LINK_REGISTER 	registers[0xE]
#define PROG_COUNTER 	registers[0xF]
#define NUM_REG 	0x10

#define FORMAT (unsigned)currentInstruction >> 13
#define DATA_PROCESS 	FORMAT & 0	//00000000
#define LOAD_STORE	FORMAT & 1	//00100000
#define IMMEDIATE	FORMAT & 2 | 3	//01X00000
#define CON_BRANCH	FORMAT & 4	//10000000
#define PUSH_PULL	FORMAT & 5	//10100000
#define BRANCH		FORMAT & 6	//11000000
#define STOP		FORMAT & 7	//11100000

#define OPERATION	((currentInstruction >> 8) & 15)
#define RN	((currentInstruction >> 4) & 15)
#define RD	currentInstruction & 15
#define OPCODE	((currentInstruction >> 12) & 3)
#define IMM_VALUE	((currentInstruction >> 4) & 255)

//Data Processing codes
#define AND_DATA	0 & OPERATION
#define EOR_DATA	1 & OPERATION
#define SUB_DATA	2 & OPERATION
#define SXB_DATA	3 & OPERATION
#define ADD_DATA	4 & OPERATION
#define ADC_DATA	5 & OPERATION
#define LSR_DATA	6 & OPERATION
#define LSL_DATA	7 & OPERATION
#define TST_DATA	8 & OPERATION
#define TEQ_DATA	9 & OPERATION
#define CMP_DATA	10 & OPERATION
#define ROR_DATA	11 & OPERATION
#define ORR_DATA	12 & OPERATION
#define MOV_DATA	13 & OPERATION
#define BIC_DATA	14 & OPERATION
#define MVN_DATA	15 & OPERATION

//Immediate codes
#define MOV	0 & OPCODE	//00
#define CMP	1 & OPCODE	//01
#define ADD	2 & OPCODE	//10
#define SUB	3 & OPCODE	//11

unsigned long mar; 			//Memory Address Register
unsigned long mbr; 			//Memory Buffer Register
unsigned long instrR; 			//Instruction Register
unsigned long ccr; 			//Condition Codes Register
unsigned long registers[NUM_REG]; 	//Registers
unsigned instrFlag = 0; 		//Instruction Register Flag
unsigned stopFlag = 0; 			//Stop Flag
unsigned SIGN_FLAG = 4; 	//Sign flag for ccr
unsigned ZERO_FLAG = 2; 	//Zero flag for ccr
unsigned CARRY_FLAG = 1;	//Carry flag for ccr
unsigned currentInstruction = 0;	//Current Instruction Register

void writeFile(void *memory);
int loadFile(void *memory, unsigned int max);
void printHelp();
void dumpMemory(void *memory, unsigned offset, unsigned length);
void memoryModify(void *memory);
void goRun();
void displayRegisters();
void trace(void *memory);
void reset();

unsigned getIR0(unsigned long instrR);
unsigned getIR1(unsigned long instrR);
void fetch(void *memory);
unsigned char memory[MAX_MEMORY] = {45, 67, 89};

void instructionCycles(void *memory);
void execute(void *memory);
