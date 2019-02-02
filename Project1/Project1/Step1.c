# include <stdio.h>
# include <stdlib.h>

int main() {
	// function prototypes
	void printMemory(int PC, int memory[], int length);
	void StoreMem(int AC, int IR, int memory[]);
	int LoadMem(int IR, int memory[]);
	void printDevice(int device[], int device2[]);
	void Interrupt(int PC, int IO);

	// make memory
	int memory[0x1000];
	int i;
	int dev5[2] = { 2, 13 };
	int dev6[2] = { NULL, NULL };
	srand(time(NULL));

	// load memory with all 0 then populate with some values
	for (i = 0; i < 0x1000; i++) {
		memory[i] = 0;
	}
	memory[0x300] = 0x1940, memory[0x301] = 0x5941, memory[0x302] = 0x2941;
	memory[0x940] = 3, memory[0x941] = 2;

	int PC = 0x300;
	int IR = memory[PC];
	int AC = 0;

	printf("BEGIN SIMULATION STEP 1 \n");
	printf("\n");
	printMemory(PC, memory, 5);

	//Print "Fetch memory from memory location 300” 
	printf("Step 1 \n");
	printf("Fetch memory from memory location 300. \n");
	printf("PC = %x. \nAC = %d. \nIR = %x. \n", PC, AC, IR);
	printf("\n");

	//Print "Execute the memory and increment the PC: load AC from memory location 940”
	printf("Step 2 \n");
	printf("Execute the memory and increment the PC: load AC from memory location 940. \n");
	AC = LoadMem(IR, memory);
	PC++;
	printf("PC = %x. \nAC = %d. \nIR = %x. \n", PC, AC, IR);
	printf("\n");

	//Print "Fetch memory from memory location 301”
	printf("Step 3 \n");
	printf("Fetch memory from memory location 301. \n");
	IR = memory[PC];
	printf("PC = %x. \nAC = %d. \nIR = %x. \n", PC, AC, IR);
	printf("\n");

	//Print "Execute the memory and increment the PC: Add AC from memory location 941”
	printf("Step 4 \n");
	printf("Execute the memory and increment the PC: Add to AC from memory location 941. \n");
	AC = AC + LoadMem(IR, memory);
	PC++;
	printf("PC = %x. \nAC = %d. \nIR = %x. \n", PC, AC, IR);
	printf("\n");

	//Print "Fetch memory from memory location 302”
	printf("Step 5 \n");
	printf("Fetch memory from memory location 302. \n");
	IR = memory[PC];
	printf("PC = %x. \nAC = %d. \nIR = %x. \n", PC, AC, IR);
	printf("\n");

	//Print "Execute the memory and increment the PC: Store AC from memory location 941”
	printf("Step 6 \n");
	printf("Execute the memory and increment the PC: Store AC to memory location 941. \n");
	StoreMem(AC, IR, memory);
	PC++;
	printf("PC = %x. \nAC = %d. \nIR = %x. \n", PC, AC, IR);
	printf("\n");
	PC = 0x300;
	printMemory(PC, memory, 5);
	printf("END SIMULATION STEP 1\n");
	
	printf("\nBEGIN SIMULATION STEP 2\n");

	printMemory(PC, memory, 10);
	
	printf("\n");

	printDevice(dev5, dev6);

	printf("\n");

	//change memory array values to those given
	for (i = 0; i < 0x1000; i++) {
		memory[i] = 0;
	}
	memory[0x300] = 0x3005, memory[0x301] = 0x5940, memory[0x302] = 0x9941,
	memory[0x303] = 0x7006, memory[0x304] = 0x2942, memory[0x305] = 0x3005,
	memory[0x306] = 0x8942, memory[0x307] = 0x6943, memory[0x308] = 0x2944,
	memory[0x309] = 0x7006;

	memory[0x940] = 3, memory[0x941] = 5, memory[0x943] = 10;

	//1. Load AC from device 5.
	int r = rand() % 3;
	int IO = PC + r;
	Interrupt(PC, IO);
	//printf("\nINTERRUPT STEP 1\n");
	IR = memory[PC];
	AC = dev5[0];
	PC++;
	
	//2. Add AC with contents of memory location 940.
	Interrupt(PC, IO);
	IR = memory[PC];
	AC += LoadMem(IR, memory);
	PC++;

	//3. Divide AC by contents of memory location 941.
	Interrupt(PC, IO);
	IR = memory[PC];
	AC /= LoadMem(IR, memory);
	PC++;

	//4. Store AC to device 6.
	r = rand() % 3;
	IO = PC + r;
	Interrupt(PC, IO);
	//printf("\nINTERRUPT STEP 4\n");
	IR = memory[PC];
	dev6[0] = AC;
	PC++;

	//5. Store AC to memory location 942.
	Interrupt(PC, IO);
	IR = memory[PC];
	StoreMem(AC, IR, memory);
	PC++;

	//6. Load AC from device 5.
	Interrupt(PC, IO);
	r = rand() % 3;
	IO = PC + r;
	Interrupt(PC, IO);
	//printf("\nINTERRUPT STEP 6\n");
	IR = memory[PC];
	AC= dev5[1];
	PC++;

	//7. Multiply AC with contents of memory location 942.
	Interrupt(PC, IO);
	IR = memory[PC];
	AC *=LoadMem(IR, memory);
	PC++;

	//8. Subtract contents of memory location 943 from the AC.
	Interrupt(PC, IO);
	IR = memory[PC];
	AC -= LoadMem(IR, memory);
	PC++;

	//9. Store AC to memory location 944.
	IR = memory[PC];
	StoreMem(AC, IR, memory);
	PC++;

	//10. Store AC to device 6.
	Interrupt(PC, IO);
	//printf("\nINTERRUPT STEP 10\n");
	IR = memory[PC];
	dev6[1] = AC;
	PC++;

	PC = 0x300;

	printf("\n");

	printMemory(PC, memory, 10);

	printDevice(dev5, dev6);

	printf("\n");

	printf("\nEND SIMULATION STEP 2\n");

}
void printMemory(int PC, int memory[], int length) {
	//Display memory contents – memory and data
	int i;
	int dataPtr = 0x940;
	printf("Instruction Memory \n");
	for (i = PC; i < PC + length; i++) {
		printf("%x %x \t", i, memory[i]);
	}
	printf("\n");
	printf("Data Memory \n");
	for (i = dataPtr; i < dataPtr + length; i++) {
		printf("%x %x \t", i, memory[i]);
	}
	printf("\n");
	printf("\n");
}

void StoreMem(int AC, int IR, int memory[]) {
	int opcode = IR / 0x1000;
	int address = IR - 0x1000 * opcode;
	memory[address] = AC;
}

int LoadMem(int IR, int memory[]) {
	int opcode = IR / 0x1000;
	int address = IR - 0x1000 * opcode;
	return memory[address];
}

void printDevice(int device[], int device2[]) {
	printf("\nDevice 5: ");
	int i = 0;
	for (i; i < 2; i++) {
		printf("%d\t", device[i]);
	}
	printf("\nDevice 6: ");
	i = 0;
	for (i; i < 2; i++) {
		printf("%d\t", device2[i]);
	}

}

void Interrupt(int PC, int IO) {
	if (PC == IO)
		printf("\nINTERRUPT STEP %d\n", PC - 0x2ff);
	else if (PC == 0x309)
		printf("\nINTERRUPT STEP %d\n", PC - 0x2ff);
}
