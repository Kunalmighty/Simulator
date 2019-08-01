/*

Name: Kunal Baran Maiti, JHED ID: kmaiti1, CSF Spring 2019, Prog Assignment 3 (Simulator)

sim.c - Simulator for a file containing a 256 byte SCRAM program.

*/

#include <unistd.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * Max file size in bytes.
 */
#define SIZE 256


/*
 * Return upper four bits of byte b.
 */
static unsigned char upper(unsigned char b) {
    
    return (b >> 4);    

}

/*
 * Return lower four bits of byte b.
 */
static unsigned char lower(unsigned char b) {

    return (b & 0x0f);

}

/*
 * Run the SCRAM simulator on the input 256 (or less) character array.
 *
 * We stop if the program tries to execute an illegal opcode, 
 * an HLT instruction, or if we reach the maximum number of instructions we're
 * supposed to run.
 */
static int run(unsigned char binary[], int count2)
{
	unsigned char acc = 0;
	unsigned char pc = 0;
	unsigned char ir = 0;
	unsigned char extadd = 0 & 0xf;  
	unsigned char code, address, t, carry, foradl, foradl2;	
	unsigned short foradds;	
	bool hadext = false; 

	for (int count = 0; count < 9999; count++) {	


		ir = binary[pc];
		code = upper(ir);
		address = lower(ir);
		if (hadext == true) {
			address = (extadd << 4) | (address & 0xF);			
		}
		hadext = false; 

		//error checks for reading, writing and executing beyond length of input
		if (address > count2) {
			if (code == 1 || code == 2 || code == 5 || code == 6 || code == 9 || code == 10
				|| code == 11 || code == 13 || code == 14) {		   		
           		return 5;
       		}
		}	
		if (address > count2) {
			if (code == 3 || code == 4) {		   		
           		return 6;
       		}
		}	
		if (address > count2) {		   
           return 4;
		}	

		//switch statement where each case is a different opcode 
		switch (code) {
		case 0://HLT 
			if (code == 0 && address == 0) {
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "HLT", acc);
				return 1;
			}
			//EXT	
			if (code == 0 && address != 0) {
				extadd = address;
				hadext = true; 
				printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "EXT", address, acc); 
				break;	
			}		
			break; 	
		case 1://LDA
			acc = binary[address];
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "LDA", address, acc); 
			break;
		case 2://LDI
			t = binary[address] & 0xf;
			acc = binary[t];
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "LDI", address, acc); 
			break;
		case 3://STA
			binary[address] = acc;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "STA", address, acc); 
			break;
		case 4://STI
			t = binary[address] & 0xf;
			binary[t] = acc;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "STI", address, acc); 
			break;
		case 5://ADD			
			foradds = acc + binary[address];
			acc = foradds & 0x00FF;
			carry = (foradds & 0x0100) >> 8;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "ADD", address, acc); 
			break;
		case 6://SUB			
			foradds = acc - binary[address];
			acc = foradds & 0x00FF;
			carry = (foradds & 0x0100) >> 8;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "SUB", address, acc); 
			break;
		case 7://JMP
			pc = address-1;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "JMP", address, acc); 
			break;
		case 8://JMZ
			if (acc == 0) {
				pc = address-1;
			}
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "JMZ", address, acc); 
			break;
		case 9://AND
			acc = acc & binary[address];
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "AND", address, acc); 
			break;
		case 10://IOR
			acc = acc | binary[address];
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "IOR", address, acc); 
			break;
		case 11://XOR
			acc = acc ^ binary[address];
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "XOR", address, acc); 
			break;
		case 12://ADL
			if (address > 15) {
				foradds = acc + address;
				acc = foradds & 0x00FF;
				carry = (foradds & 0x0100) >> 8;
				printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "ADL", address, acc); 
				break;
			}
			foradl2 = address >> 3;
			if (foradl2 == 1) {
				foradl = address | 0xF0;
			}
			else {
				foradl = address & 0xFF;
			}			
			foradds = acc + foradl;
			acc = foradds & 0x00FF;
			carry = (foradds & 0x0100) >> 8;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "ADL", address, acc); 
			break;
		case 13://ADC A ← A + M[b] + C			
			foradds = acc + binary[address] + carry;
			acc = (foradds & 0x00FF);
			carry = (foradds & 0x0100) >> 8;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "ADC", address, acc); 
			break;
		case 14://SBB A ← A + /M[b] + C
			foradds = acc + (~(binary[address])) + carry;
			acc = (foradds & 0x00FF);
			carry = (foradds & 0x0100) >> 8;
			printf("0x%02X  %s 0x%02X    ACC=0x%02X\n", pc, "SBB", address, acc); 
			break;
		case 15:// 0) NEG A ← /A + 1
			if (address == 0) {
				acc = ~acc + 1;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "NEG", acc);
				break;
			}
			//1) COM A ← /A
			if (address == 1) {
				acc = ~acc;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "COM", acc); 
				break;
			}
			//2) CLR A ← 0
			if (address == 2) {
				acc = 0;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "CLR", acc); 
				break;
			}
			//3) SET A ← 0xFF
			if (address == 3) {
				acc = 0xFF;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "SET", acc); 
			}
			//4) RTL A ← ((A<<1)∨(A>>7))∧0xFF
			if (address == 4) {
				acc = ((acc << 1) | (acc > 7)) & 0xFF;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "RTL", acc); 
				break;
			}
			//5) RTR A ← ((A<<7)∨(A>>1))∧0xFF
			if (address == 5) {
				acc = ((acc << 7) | (acc >> 1)) & 0xFF;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "RTR", acc); 
				break;
			}
			//6) LSL A ← (A<<1)∧0xFE
			if (address == 6) {
				acc = (acc << 1) & 0xFE;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "LSL", acc); 
				break;
			}
			//7) LSR A ← (A>>1)∧0x7F
			if (address == 7) {
				acc = (acc >> 1) & 0x7F;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "LSR", acc); 
				break;
			}
			//8) ASR A ← ((A>>1)∧0x7F) ∨ (A∧0x80))
			if (address == 8) {
				acc = ((acc >> 1) & 0x7F) | (acc & 0x80);
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "ASR", acc); 
				break;
			}
			//9) TST A ← 0x01 if A not equal to 0
			if (address == 9) {
				if (acc != 0) {
					acc = 0x01; 
				}				
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "TST", acc); 
				break;
			}
			//10) CLC C ← 0
			if (address == 10) {
				carry = 0; 
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "CLC", acc); 
				break;
			}
			//11) SEC C ← 1
			if (address == 11) {
				carry = 1; 
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "SEC", acc); 
				break;
			}
			//12) TCA A ← C
			if (address == 12) {
				acc = carry;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "TCA", acc); 
				break;
			}
			//13) TVA A ← C ⊕ N
			if (address == 13) {
				unsigned char n = acc >> 7; 
				acc = carry ^ n;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "TVA", acc); 
				break;
			}
			//14) JAL PC ← A and A ← PC + 1
			if (address == 14) {
				acc = pc + 1;
				pc = acc-1;
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "JAL", acc); 
				break;
			}
			//15) NOP
			if (address == 15) {				
				printf("0x%02X  %s         ACC=0x%02X\n", pc, "NOP", acc); 
				break;
			}
			break;
		}

		pc = (pc + 1);
		
	}

	return 1; 
}

/*
 * Print state (registers and memory) of the SCRAM to standard output.
 */
static void dump(unsigned char input[], FILE *fo, int c)
{	
	fwrite(input, 1, c, fo);	
}


int main(int argc, char * argv []) {	

    if (argc > 3) {
        fprintf (stderr, "Error: Too many arguments.\n");
        return 1;
    }

    FILE *fp = argc > 1 ? fopen (argv[1], "r") : stdin;

    if (!fp) {  /* check file */
        fprintf (stderr, "Error: File Open Failed '%s'.\n", argv[1]);
        return 2;
    }    

	//Initializing array of 256 unsigned chars
	unsigned char binary[SIZE]; 
    int i = 0; 
    unsigned char c;
    int count2 = 0; 
    

    //populate the 256 byte char array from stdin
	while (count2 < SIZE) {
		c=getc(fp);
		binary[i] = c;
		i++;
        count2++;             		
    }	        

    /**
    Run the simulator on the 256 byte array.
    This will: 
    1) Print out the simulation 
    and 2) Modify the array itself
     */    
    int forerrors = run(binary, count2);   
    if (forerrors == 6) {
    	fprintf (stderr, "Error: Beyond Input File.\n");
    	return 6;
    }
    if (forerrors == 5) {
    	fprintf (stderr, "Error: Beyond Input File.\n");
    	return 5;
    } 
    if (forerrors == 4) {
    	fprintf (stderr, "Error: Beyond Input File.\n");
    	return 4;
    }


    //dump the 256 char array in its current state, if need be
    if (argc == 3) {
    	FILE *fp2 = fopen (argv[2], "w"); 
    	if (!fp2) {  // check file 
        	fprintf (stderr, "Error: File Open Failed '%s'.\n", argv[2]);
        	return 2;
   		}
    	dump(binary, fp2, count2);
    	fclose (fp2); 
    }
    

	if (fp != stdin) fclose (fp);   /* close file if not stdin */    

	return 0;

}