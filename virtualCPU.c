/*
 * CENG606 Lab3
 * Jordan Humphries
 * Simple Virtual CPU with Command Line User Interface
 * Memory = 16KB
 */
#include "virtualCPU.h"

int main(int argc, char **argv) {
	//unsigned char memory[MAX_MEMORY];
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
			trace(&memory);
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
} //End of printHelp

//loadFile opens a file, reads it into memory, and closes the file
int loadFile(void *memory, unsigned max) {
	unsigned char filename[MAX_INPUT], buffer[MAX_INPUT], *position;
	int size;
	FILE *fp;

	printf("\nloadFile will open a file and read it into the memory\n");
	printf("Enter the name of the file to load: ");
	getchar();
	fgets(filename, MAX_INPUT, stdin);
	if ((position = strchr(filename, '\n')) != NULL)
		*position = '\0';
	fp = fopen(filename, "rb");
	
	while(fgets(buffer, MAX_INPUT, fp)){
		if(max - strlen(memory) >= strlen(buffer)){
			strcat(memory, buffer);
			size += strlen(buffer);
		}else{
			buffer[(max - strlen(memory))] = '\0';
			strcat (memory, buffer);
			size += strlen(buffer);
			printf("This file has been truncated due to memory limitations\n");
			fclose(fp);
		}
	}
	fclose(fp);
	printf("The file is %d bytes in decimal, and %x bytes in hex\n", size, size);
} //End of loadFile

//writeFile will open/create a file, write the memory to the file and close the file
void writeFile(void *memory) {
	char filename[MAX_INPUT];
	int size;
	FILE *fp;

	printf("\nwriteFile will open or create a file and write the data in memory to the file\n");
	printf("Be careful - it will overwrite files without warning\n");
	printf("\nEnter the name of a file to write to: ");
	getchar();
	fgets(filename, MAX_INPUT, stdin);
	filename[strlen(filename) -1] = '\0'; //Remove last byte
	
	printf("Enter the number of bytes to write: ");
	scanf("%d", &size); //Get size of file
	
	fp = fopen(filename, "wb");
	fwrite(memory, ((int)sizeof(char)), size, fp);
	fclose(fp);
} //End of writeFile

//dumpMemory will display the memory contents starting at offset for a number of bytes equal to length
void dumpMemory(void *memory, unsigned offset, unsigned length){
	unsigned x, z;
	unsigned rowLength = 16;

	length--; //Decrease length to display the entered value

	for(x = offset; x < (offset+length); x += rowLength) {
		printf("%04x\t", x);
		for(z = x; z < (x + rowLength); z++) {
			printf("%02x ", *((char *) memory + z));
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
} //End of dumpMemory

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
} //End of memoryModify

void goRun(){
	printf("Go is not implemented yet\n\n");
} //End of goRun

void displayRegisters(){
	unsigned z = 0;
	unsigned instrR0 = getIR0(instrR);
	unsigned instrR1 = getIR1(instrR);

	//Display list of registers
        printf("Generic Register List:\n");

	for (z = 0; z < NUM_REG; z++){
		if(z == 13) //Stack Pointer
			break;

		if(z%4 == 0)
			printf("\n");

		printf("R%d: %08X\t", z, registers[z]);
	}
		
	//Display special registers
	printf(" SP: %08X\t", STACK_POINTER);
	printf(" LR: %08X\t", LINK_REGISTER);
	printf(" PC: %08X\n\n", PROG_COUNTER);
	
	//Display named registers
	printf("MAR: %08X\tMBR: %08X\tIR0: %08X\tIR1: %08X\n\n", mar, mbr, instrR0, instrR1);
	
	//Display CCR and its flags
	printf("Condition Code Register:\n");
	printf("CCR: %08X\tSign:%d\tZero:%d\tCarry:%d\n\n", ccr, SIGN_FLAG, ZERO_FLAG, CARRY_FLAG);
	
	//Display flags
	printf("InstrFlag: %d\tStop Flag: %d\n\n", instrFlag, stopFlag);
} //End of displayRegisters

void trace(void *memory){
	instructionCycles(&memory);
	displayRegisters();
} //End of trace

void reset(){
	int z = 0;
	//Zero the registers
	mar = 0;
	mbr = 0;
	ccr = 0;
	instrR = 0;
	instrFlag = 0;
	stopFlag = 0;
	
	//Reset the registers
	for (z = 0; z < NUM_REG; z++)
		registers[z] = 0;
} //End of reset

unsigned getIR0(unsigned long instrR){
	return instrR >> 16;
} //End of getIR0

unsigned getIR1(unsigned long instrR){
	return instrR & 0x000FFFF;
} //End of getIR1

//Pull an instruction from memory using address at Program Counter
//Then increments Program Counter
void fetch(void *memory){
	unsigned cycles, registerSize = 32, z = 0;
	cycles = registerSize / ((int)sizeof(char));
	
	//Move Program Counter into MAR
	mar = registers[PROG_COUNTER];
	
	//Move memory at MAR into MBR
	for (z = 0; z < cycles; z++){
		mbr = mbr << 8;
		mbr += *((unsigned char*)memory + (mar + z));
	}
	
	//Move MBR into the Instruction Register
	instrR = mbr;
	
	//Move instruction into Program Counter
	registers[PROG_COUNTER] += registerSize;
} //End of fetch

//Instruction Cycle for determining which part of the instruction register to use
//Then calls execute function to run the command
void instructionCycles(void *memory){
	if(instrFlag == 0){
		getIR0(instrR);
		fetch(memory);
		currentInstruction = instrR;
		execute(memory);
		instrFlag = 1;
	}else{
		getIR1(instrR);
		currentInstruction = instrR;
		execute(memory);
		instrFlag = 0;
	}
	currentInstruction = 0;
} //End of instructionCycles

void execute(void *memory){
	unsigned z = 0; //For looping when storing
	if(DATA_PROCESS){
		if(AND_DATA)
			registers[RD] &= RN;
		else if(EOR_DATA)
			registers[RD] ^= RN;
		else if(SUB_DATA)
			registers[RD] -= RN;
		else if(SXB_DATA)
			registers[RD] = (signed)RN;
		else if(ADD_DATA)
			registers[RD] += RN;
		else if(ADC_DATA)
			registers[RD] += RN + CARRY_FLAG;
		else if(LSR_DATA)
			registers[RD] = registers[RD] >> RN;
		else if(LSL_DATA)
			registers[RD] = registers[RD] << RN;
		else if(TST_DATA){
			if(registers[RD] & RN){
			}
		}else if(TEQ_DATA){
			if(registers[RD] ^ RN){
			}
		}else if(CMP_DATA){
			if(registers[RD] - CMP == 0){
			}
		}else if(ROR_DATA){
		
		}else if(ORR_DATA)
			registers[RD] |= RN;
		else if(MOV_DATA)
			registers[RD] = RN;
		else if(BIC_DATA)
			registers[RD] &= !RN;
		else if(MVN_DATA)
			registers[RD] != RN;
	}else if(LOAD_STORE){
		mar = registers[RN];

		if(L_BIT){ //LOAD
			if(B_BIT){ //Load a byte
				mbr = *((unsigned char *)memory + mar);
				registers[RD] = mbr;
			}else{ //Load a double word
				for(z = 0; < 4/((int)sizeof(char)); z++, mar++){
					mbr << 8; //Shift by a byte
					mbr += *((unsigned char *)memory + mar);
				}
			}
		}else{ //STORE
			mbr = registers[RD];
			if(B_BIT){ //Store a byte
				*((unsigned char *)memory + mar) = (unsigned char)mbr & 0xFF;
			}else{ //Store a double word
				*((unsigned char*)memory + mar++) = (unsigned char)mbr >> 24 & 0xFF;
				*((unsigned char*)memory + mar++) = (unsigned char)mbr >> 16 & 0xFF;
				*((unsigned char*)memory + mar++) = (unsigned char)mbr >> 8 & 0xFF;
				*((unsigned char*)memory + mar) = (unsigned char)mbr & 0xFF;
			}
		}
	}else if(IMMEDIATE){
		//Each option include an operation involving RD
		//Then it checks the appropiate flags
		if(MOV){
			registers[RD] = IMM_VALUE;
			flagsCheck(registers[RD]);
		}else if(CMP){
			alu = ~(registers[RD] + IMM_VALUE) + 1;
			flagsCheck(alu);
			CARRY_FLAG = isCarry(registers[RD], ~IMM_VALUE, 0);
		}else if(ADD){
			alu = registers[RD] + IMM_VALUE; //Add values
			flagsCheck(alu);
			CARRY_FLAG = isCarry(registers[RD], IMM_VALUE, 0);
			registers[RD] = alu; //Move final value into RD
		}else if(SUB){
			alu = ~(registers[RD] + IMM_VALUE) + 1;
			flagsCheck(alu);
			CARRY_FLAG = isCarry(~registers[RD], ~IMM_VALUE, 1);
			registers[RD] = alu;
		}
	}else if(CON_BRANCH){
		
	}else if(PUSH_PULL){
		printf("\nPush/Pull\n");
	}else if(BRANCH){
		printf("\nBranch\n");
	}else if(STOP){
		printf("\nStop\n");
		stopFlag = 1;
	}else
		printf("\n\nBad option\n\n");
	
	
} //End of execute

//Check Zero and Sign Flags function
void flagsCheck(unsigned long alu){
	if(alu == 0)
		ZERO_FLAG = 1;
	else
		ZERO_FLAG = 0;

	SIGN_FLAG = alu & SUB_VALUE;
}//End of flagsCheck

//isCarry checks to see if a carry is generated
int isCarry(unsigned long op1, unsigned long op2, unsigned c){
	if ((op2 == MAX32) && (c == 1))
		return(1); //special case where op2 is at MAX32
	return ((op1 > (MAX32 - op2 - c))?1:0);
} //End of isCarry
