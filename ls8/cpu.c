#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu->ram[address] = value;
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu->ram[address];
}

void cpu_load(struct cpu *cpu, int num_args, char *argv[])
// #define LDI 0b10000010
// #define HLT 0b00000001
// #define PRN 0b01000111
{

  // char line[1024];

  // if (num_args < 2)
  // {
  //   fprintf(stderr, "file does not exist \n");
  //   exit(1);
  // }
  // char *file_to_be_read = argv[1];
  // FILE *fp = fopen(file_to_be_read, "r");
  // if (fp == NULL)
  // {
  //   printf("No file read with the argument provided \n");
  //   exit(1);
  // }

  // while (fgets(line, 1024, fp) != NULL)
  // {
  //   char *endptr;
  //   unsigned char str_value;
  //   str_value = stroul(line, &endptr, 2);
  //   printf("%s  this is the str value\n", str_value);
  // }
}

//TODO OPEN FILE
// char data[DATA_LEN] = {
//     // From print8.ls8
//     0b10000010, // LDI R0,8
//     0b00000000,
//     0b00001000,
//     0b01000111, // PRN R0
//     0b00000000,
//     0b00000001 // HLT
// };

// int address = 0;

// for (int i = 0; i < DATA_LEN; i++)
// {
//   cpu->ram[address++] = data[i];
// }

// TODO: Replace this with something less hard-coded

/**
 * ALU
 */
// void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
// {
//   switch (op)
//   {
//   case ALU_MUL:
//     // TODO
//     break;

//     // TODO: implement more ALU ops
//   }
// }

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned int Counter;
  unsigned char Instruction;

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    Counter = cpu->PC;
    // 2. Figure out how many operands this next instruction requires
    Instruction = cpu_ram_read(cpu, Counter); // READ INSTRUCTIONS FROM RAM
    // 3. Get the appropriate value(s) of the operands following this instruction
    // GET OPERANDS/ARGUMENTS
    // 4. switch() over it to decide on a course of action.
    switch (Instruction)
    {
    case HLT:
      running = 0;
      printf("HALT!! \n\n");
      break;
    default:
      printf("Cannot read instructions");
      cpu->PC++;
    }
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  cpu->PC = 0;
  memset(cpu->ram, 0, sizeof(cpu->ram));
  memset(cpu->reg, 0, sizeof(cpu->reg));
  // TODO: Initialize the PC and other special registers
}
