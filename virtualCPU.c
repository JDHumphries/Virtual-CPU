/*
 * CENG606 Lab3
 * Jordan Humphries
 * Simple Virtual CPU with Command Line User Interface
 * Memory = 16KB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 16384 //4000 in hex
#define MAX_INPUT 128
#define STACK_POINTER 13 //Stack pointer, D in hex
#define LINK_REGISTER 14 //Link register, E in hex
#define PROG_COUNTER 15 //Program Counter, F in hex
#define NUM_REG 16 //Number of Registers, 10 in hex
#define SIGN_FLAG 4 //Sign flag for ccr
#define ZERO_FLAG 2 //Zero flag for ccr
#define CARRY_FLAG 1 //Carry flag for ccr

static unsigned long mar; //Memory Address Register
static unsigned long mbr; //Memory Buffer Register
static unsigned long intrR; //Instruction Register
static unsigned long ccr; //Condition Codes Register
static unsigned long registers[NUM_REG]; //Registers

static bool instrFlag = 0; //Instruction Register Flag
static bool stopFlag = 0; //Stop Flag

//Implemented Functions
void writeFile(void *memory);
int loadFile(void *memory, unsigned int max);
void printHelp();
void dumpMemory(void *memory, unsigned offset, unsigned length);
void memoryModify(void *memory);
//Unimplemented Functions
void goRun();
void displayRegisters();
void trace();
void reset();

int main(int argc, char **argv) {
	unsigned char memory[MAX_MEMORY];
	char cmd;
	int offset = 0, length = 0;
	printHelp();

	do {
		printf("Enter a command: ");
		scanf(" %c", &cmd);

		switch (cmd) {
		case 'd':
		case 'D':
			printf("Enter the offset to begin at: ");
			scanf(" %d", &offset);
			printf("Enter how many bytes to view: ");
			scanf(" %d", &length);
			dumpMemory(&memory, offset, length);
			break;
		case 'g':
		case 'G':
			goRun();
			break;
		case 'l':
		case 'L':
			loadFile(&memory, MAX_MEMORY);
			break;
		case 'm':
		case 'M':
			memoryModify(&memory);
			break;
		case 'q':
		case 'Q':
			return 0;
			break;
		case 'r':
		case 'R':
			displayRegisters();
			break;
		case 't':
		case 'T':
			trace();
			break;
		case 'w':
		case 'W':
			writeFile(&memory);
			break;
		case 'z':
		case 'Z':
			reset();
			break;
		case '?':
		case 'h':
		case 'H':
			printHelp();
			break;
		default:
			printf("Error - Command Doesn't Exist\n\n");
			break;
		}
	}while(1);

	return 0;
}

//printHelp prints out the list of commands
void printHelp(){
	printf("\nd	dump memory");
	printf("\ng	go - run the entire program");
	printf("\nl	load a file into memory");
	printf("\nm	memory modify");
	printf("\nq	quit");
	printf("\nr	display registers");
	printf("\nt	trace - execute one instruction");
	printf("\nw	write file");
	printf("\nz	reset all registers to zero");
	printf("\n?,h	display list of commands\n\n");
}

//loadFile opens a file, reads it into memory, and closes the file
int loadFile(void *memory, unsigned int max) {
	char filename[MAX_INPUT], *position;
	int size;
	FILE *fp;

	printf("\nloadFile will open a file and read it into the memory\n");
	printf("Enter the name of the file to load: ");
	getchar();
	fgets(filename, MAX_INPUT, stdin);
	if ((position = strchr(filename, '\n')) != NULL)
		*position = '\0';
	fp = fopen(filename, "rb");
	size = fread(&memory, 1, max, fp);
	fclose(fp);
	printf("The file is %d bytes in decimal, and %x bytes in hex\n", size, size);
}

//writeFile will open/create a file, write the memory to the file and close the file
void writeFile(void *memory) {
	char filename[MAX_INPUT], *position;
	int size;
	FILE *fp;

	printf("\nwriteFile will open or create a file and write the data in memory to the file\n");
	printf("Be careful - it will overwrite files without warning\n");
	printf("Enter the name of a file to write to: ");
	getchar();
	fgets(filename, MAX_INPUT, stdin);
	if ((position = strchr(filename, '\n')) != NULL)
		*position = '\0';
	printf("Enter the number of decimal bytes to write: ");
	scanf(" %d", &size);
	fp = fopen(filename, "wb");
	fwrite(&memory, 1, size, fp);
	fclose(fp);
}

//dumpMemory will display the memory contents starting at offset for a number of bytes equal to length
void dumpMemory(void *memory, unsigned offset, unsigned length){
	unsigned int x, z;
	unsigned int rowLength = 0x10;

	length--; //Decrease length to display the entered value

	for(x = offset; x < (offset+length); x += rowLength) {
		printf("%4X\t", x);
		for(z = x; z < (x + rowLength); z++) {
			printf("%2X ", *((char *) memory + z));
			if(z == (offset+length))
				break;
		}

		printf("\n\t");
	
		for(z = x; z < (x+rowLength); z++) {
			if(isprint((int) *((char *) memory + z)))
				printf(" %c ", *((char *) memory + z)); //print value at position in memory
			else
				printf(" . "); //if the position is blank, print .

			if(z == (offset + length))
				break; //Exit if z = end of entered length
		}

		printf("\n");
		if(z == length)
			break;
	}
}

//memoryModify will allow the user to change the value at a specific memory location
void memoryModify(void *memory){
	unsigned offset;
	char value;
	
	printf("\nmemoryModify will load a location and let you change the HEX value in it");
	printf("\nA period ('.') will end it");
	printf("\nEnter an offset: "); //Get an offset to start the modifying at
	scanf("%x", &offset);
	printf("Enter a value: "); //Enter a value for the offset
	
	while(1){
		printf("%4X > ", offset);
		while(getchar() != '\n');
		
		if ((value = getchar()) == '.') //Exit if value = .
			break;
		
		*((char *) memory+offset) = value; //Put the value in the offset
		
		if(++offset == MAX_MEMORY) //Check if the next offset position is the end of the memory
			break;
	}
}

void goRun(){
	printf("Go is not implemented yet\n\n");
}

void displayRegisters(){
	//Display named registers
	printf("Memory Address Register: %x\n" mar);
	printf("Memory Buffer Register: %x\n" mbr);
	printf("Instruction Register: %x\n\n" ir);
	
	//Display CCR and its flags
	printf("Condition Code Register:\n");
	printf("\tSign: %d\n", SIGN_FLAG);
	printf("\tZero: %d\n", ZERO_FLAG);
	printf("\tCarry: %d\n\n", CARRY_FLAG);
	
	//Display flags
	printf("Instruction Flag: %d\n", instrFlag);
	printf("Stop Flag: %d\n\n", stopFlag);
	
	//Display list of registers
	printf("Generic Register List:\n")
	for (int z = 0; z < NUM_REG; z++){
		if(z == STACK_POINTER)
			break;
			
		printf("R%d: %x\n", z, registers[z]);
	}
	
	//Display special registers
	printf("Stack Pointer: %x\n", registers[STACK_POINTER]);
	printf("Link Register: %x\n", registers[LINK_POINTER]);
	printf("Program Counter: %x\n\n", registers[PROG_COUNTER]);
}

void trace(){
	printf("Trace is not implemented yet\n\n");
}

void reset(){
	//Zero the registers
	mar = 0;
	mbr = 0;
	ccr = 0;
	intrR = 0;
	instrFlag = 0;
	stopFlag = 0;
	
	//Reset the registers
	for (int z = 0; z < NUM_REG; z++)
		registers[z] = 0;
}
