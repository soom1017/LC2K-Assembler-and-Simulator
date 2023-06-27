# LC2K-Assembler-and-Simulator
Assembler translates an LC-2K(Little Computer 2000) assembly-language program into machine language, and Simulator executes the converted machine code.</br>
## LC-2K Instruction-Set Architecture
The LC-2K is one of 32-bit RISC architectures which includes
- 8 x 32-bit registers: reg01 to reg7
- Each address is 32-bits and it stores a word
- 65536 words of memory (addressed by word index, not byte)

## Project1
### Project1 includes
- assembler
  - `assembler.c`: assembler source code
  - `spec.as`: An example LC-2K assembly file, that counts down from 5, stopping when it hits 0.
  - `mult.as`: An example LC-2K assembly file for multiplication.
  - Test cases that validates the assembler. The assembler catches the following errors in the assembly-language program.
    1. Use of undefined labels
    2. Duplicate definition of labels
    3. *offsetFields* that don’t fit in 16 bits
    4. Unrecognized opcodes
    5. Non-integer register arguments
    6. Registers outside the range [0, 7]

- simulator
  - `simulator.c`: simulator source code
  - Test cases that are short, valid assembly-language programs. (executing less than 40 instructions)

### How to simulate
1. Under the assembler and simulator directory, run `make`.
2. Assemble the program into machine code.
```
assembler program.as program.mc
```
3. Serve the machine code as an input to simulator.
```
simulate program.mc > output
```
