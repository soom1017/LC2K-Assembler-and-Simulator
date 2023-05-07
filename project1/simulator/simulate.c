/* LC-2K Instruction-level simulator */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000 
typedef struct stateStruct {
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);
int readAndParse(int, int *, int *, int *, int *);
void execInstruction(stateType *, int, int, int, int);

int main(int argc, char *argv[])
{
    char line[MAXLINELENGTH];
    stateType state;
    FILE *filePtr;

    if (argc != 2) {
        printf("error: usage: %s <machine-code file>\n", argv[0]);
        exit(1);
    }

    filePtr = fopen(argv[1], "r");
    if (filePtr == NULL) {
        printf("error: can't open file %s", argv[1]);
        perror("fopen");
        exit(1);
    }

    /* read in the entire machine-code file into memory */
    for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL;
            state.numMemory++) {

        if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
            printf("error in reading address %d\n", state.numMemory);
            exit(1);
        }
        printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
    }

    int opcode, arg0, arg1, arg2;

    int i;
    for(i=0;;i++) {
        printState(&state);
        
        if (!readAndParse(state.mem[state.pc], &opcode, &arg0, &arg1, &arg2)) {
			/* machine halted */
            state.pc++;
			break;
		}
        // state.pc++;
        /* TODO: */
        execInstruction(&state, opcode, arg0, arg1, arg2);
    }
    printf("machine halted\ntotal of %d instructions executed\nfinal state of machine:\n", i+1);
    printState(&state);
    return(0);
}

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++) {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++) {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

int convertNum(int num)
{
	/* convert a 16-bit number into a 32-bit Linux integer */
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return (num);
}

int readAndParse(int mem, int *opcode, int *arg0, int *arg1, int *arg2)
{
    /* delete prior values */
	*opcode = *arg0 = *arg1 = *arg2 = 0;
    
    /* read and parse a machine-code file */
    *opcode = mem >> 22;
    mem -= *opcode << 22;

    *arg0 = mem >> 19;
    mem -= *arg0 << 19;
    *arg1 = mem >> 16;
    mem -= *arg1 << 16;
    *arg2 = mem;

    if(*opcode == 6) {
        /* execute halt instruction */
        return(0);
    }

    return(1);
}

void execInstruction(stateType *statePtr, int opcode, int arg0, int arg1, int arg2)
{
    switch(opcode) {
        case 0:
            /* add: */
            statePtr->reg[arg2] = statePtr->reg[arg0] + statePtr->reg[arg1];
            statePtr->pc += 1;
            break;

        case 1:
            /* nor: */
            statePtr->reg[arg2] = ~ (statePtr->reg[arg0] | statePtr->reg[arg1]);
            statePtr->pc += 1;
            break;

        case 2:
            /* lw: */
            arg2 = convertNum(arg2);
            statePtr->reg[arg1] = statePtr->mem[statePtr->reg[arg0] + arg2]; 
            statePtr->pc += 1;
            break;

        case 3:
            /* sw: */
            arg2 = convertNum(arg2);
            statePtr->mem[statePtr->reg[arg0] + arg2] = statePtr->reg[arg1];
            statePtr->pc += 1;
            break;

        case 4:
            /* beq: */
            arg2 = convertNum(arg2);
            if(statePtr->reg[arg0] == statePtr->reg[arg1])
                statePtr->pc += arg2;
            statePtr->pc += 1;
            break;


        case 5:
            /* jalr: */
            statePtr->reg[arg1] = statePtr->pc + 1;
            statePtr->pc = statePtr->reg[arg0];
            break;

        case 7:
            /* noop: Do nothing */
            statePtr->pc += 1;
            break;

        default:
            break;
    }
    
}