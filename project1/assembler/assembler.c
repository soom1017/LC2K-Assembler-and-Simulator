/* Assembler code fragment for LC-2K */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000
#define MAX_LABEL_COUNT 1000
#define LABEL_KEY_LENGTH 6
#define INSTR_LENGTH 32

#define MAX_16BIT 32767
#define MIN_16BIT -32768

struct label {
	char key[LABEL_KEY_LENGTH];
	int value;
};
int label_count;

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);
int toInt(char *);
int toNumReg(char*);
uint16_t toOffsetField(struct label *, int, char *, int);
void storeLabel(struct label *, char *, int);
int findLabelValue(struct label *, char *);
void convertAndWrite(FILE *, int, struct label *, char *, char *, char *, char *, char *);

int main(int argc, char *argv[]) 
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], 
			 arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
				argv[0]);
		exit(1);
	}

	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}
	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	/* Phase-1 label calculation */

	struct label labels[MAX_LABEL_COUNT];

	for(int i=0; ; i++) {
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
			/* reached end of file */
			break;
		}
		if(label[0] != '\0') {
			/* label value is address */
			storeLabel(labels, label, i);
		}
	}
	
	rewind(inFilePtr);

	/* Phase-2 generate machine codes to outfile */

	for(int i=0; ; i++) {
		if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)) {
			/* reached end of file */
			break;
		}
		convertAndWrite(outFilePtr, i, labels, label, opcode, arg0, arg1, arg2);
	}

	if (inFilePtr) {
		fclose(inFilePtr);
	}
	if (outFilePtr) {
		fclose(outFilePtr);
	}
	return(0);
}

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
		char *arg1, char *arg2)
{
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
		/* reached end of file */
		return(0);
	}

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label)) {
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);
	}

	/*
	 * Parse the rest of the line.  Would be nice to have real regular
	 * expressions, but scanf will suffice.
	 */
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%"
			"[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	return(1);
}

int isNumber(char *string)
{
	/* return 1 if string is a number */
	int i;
	return( (sscanf(string, "%d", &i)) == 1);
}

int toInt(char *string)
{
	char *str = string;

	/* Error check 5. Non-integer register arguments */
	if(*string == '\0')
		exit(1);
	
	if(*string == '+' || *string == '-')
		++string;
	while(*string != '\0') {
		if(*string < '0' || *string > '9') {
			printf("error: non-integer register arguments");
			exit(1);
		}
		++string;
	}

	return atoi(str);
}

int toNumReg(char* arg) 
{
	int numReg = toInt(arg);
	if(numReg < 0 || numReg > 7) {
		/* Error check 6. Registers outside the range [0, 7] */
		printf("error: registers outside the range [0, 7]");
		exit(1);
	}
	return numReg;
}

uint16_t toOffsetField(struct label *labels, int lineAddr, char *arg, int isBranch)
{
	int value;
	
	if(isNumber(arg)) {
		value = toInt(arg);
	}
	else {
		value = findLabelValue(labels, arg);
		if(isBranch) {
			/* branch target = offset + PC + 1, 
		 		so offset = branch target - (PC + 1) */
		 	value -= lineAddr + 1;
		}
	}

	if(value > MAX_16BIT || value < MIN_16BIT) {
		/* Error check 3. OffsetFields that don't fit in 16 bits */
		printf("error: offsetFields that don't fit in 16 bits");
		exit(1);
	}
	return (uint16_t)value;
}

void storeLabel(struct label *labels, char *_key, int _value) 
{
	/* Error check 2. Duplicate definition of labels */
	for(int i=0;i<label_count;i++)
		if(!strncmp(labels[i].key, _key, LABEL_KEY_LENGTH)) {
			printf("error: duplicate definition of labels");
			exit(1);
		}

	/* store new label key-value pair in labels array */
	strncpy(labels[label_count].key, _key, LABEL_KEY_LENGTH);
	labels[label_count].value = _value;
	label_count++;
}

int findLabelValue(struct label *labels, char *_key)
{
	for(int i=0;i<label_count;i++)
		if(!strncmp(labels[i].key, _key, LABEL_KEY_LENGTH))
			return labels[i].value;

	/* Error check 1. Use of undefined labels */
	printf("error: use of undefined labels");
	exit(1);
}

void convertAndWrite(FILE *outFilePtr, int lineAddr, struct label *labels, 
		char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
	/* .fill instruction uses only one field */
	if (!strcmp(opcode, ".fill")) {
		int value;
		if (isNumber(arg0))
			value = toInt(arg0);
		else
			value = findLabelValue(labels, arg0);
		fprintf(outFilePtr, "%d\n", value);
		return;
	}

	/* O-type has no field */
	if(!strcmp(opcode, "halt")) {
		fprintf(outFilePtr, "%d\n", 6 << 22);
		return;
	}
	if(!strcmp(opcode, "noop")) {
		fprintf(outFilePtr, "%d\n", 7 << 22);
		return;
	}

	/* R, I, J types */
	int result = 0;

	char* opcodes[6] = {"add", "nor", "lw", "sw", "beq", "jalr"};
	int i;
	for(i=0;i<6;i++) {
		if(!strcmp(opcode, opcodes[i])) {
			result += i << 22;
			break;
		}
	}
	if(i == 6) {
		/* Error check 4. Unrecognized opcodes */
		printf("error: unrecognized opcodes");
		exit(1);
	}

	result += toNumReg(arg0) << 19;
	result += toNumReg(arg1) << 16;

	if(i == 0 || i == 1) {
		/* process R-type destReg */
		result += toNumReg(arg2);
	}
	if(i == 2 || i == 3) {
		/* process I-type offsetField */
		result += toOffsetField(labels, lineAddr, arg2, 0);
	}
	if(i == 4) {
		/* process I-type(beq) offsetField */
		result += toOffsetField(labels, lineAddr, arg2, 1);
	}

	fprintf(outFilePtr, "%d\n", result);
}