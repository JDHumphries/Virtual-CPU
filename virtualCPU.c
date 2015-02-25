/*
 * CENG606 Lab3
 * Jordan Humphries
 * Simple Virtual CPU with Command Line User Interface
 * Memory = 16KB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 16384
#define MAX_INPUT 128

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
	
}

//memoryModify will allow the user to change the value at a specific memory location
void memoryModify(void *memory){
	int location = 0, value = 0;

	printf("\nmemoryModify will load a location and let you change the HEX value in it\n");
	printf("Enter a location in memory: ");
	scanf (" %d", &location);

	value = *memory[location];
	printf("\n%d	%x	> ", location, value);
}

void goRun(){
	printf("Go is not implemented yet\n\n");
}

void displayRegisters(){
	printf("Display Registers is not implemented yet\n\n");
}

void trace(){
	printf("Trace is not implemented yet\n\n");
}

void reset(){
	printf("Reset is not implemented yet\n\n");
}