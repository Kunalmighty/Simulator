# Simulator
A program in C that simulates the contents of a file containing a 256 byte SCRAM program. The extended SCRAM instructions are simulated. 

For each instruction fed to the simulator, the address is oputput as:
0x followed by a two digit hexadecimal number representing the address, 
then two spaces, then the opcode (LDA, LDI, etc). 

If the instruction has no argument, there are five spaces. 
Otherwise, a single space followed by 0x and a two digit hexadecimal number representing the argument. 

Finally, the line ends with four spaces followed by ACC=0x and the accumulator contents 
(after execution of the instruction) as a two digit hexadecimal number.

Note that if the executed instruction is immediately preceeded by an EXT instruction, 
the argument of the current instruction reflects this. 
e.g. LDA 0 that is immediately preceeded by EXT 1 should display as LDA 0x10. 

Many instructions are not affected by EXT, and those that are,
are only affected if the EXT instruction is the immediately executed instruction. 
That is, the effect of EXT expires after one instruction.

The program exits with a zero status when an HLT is encountered. 
If the input data is more than 256 bytes, anything beyond the 256th byte is ignored and discarded. 
If the input data is less than 256 bytes, simulation occurs as normal, but an error is generated 
if the program accesses data or instructions beyond the length of the input data.
