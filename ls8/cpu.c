#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6
#define SP 7

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

void cpu_load(struct cpu *cpu, int argc, char *argv[])
{

  char line[1024];
  int address = 0;

  if (argc < 2)
  {
    fprintf(stderr, "file does not exist \n");
    exit(1);
  }
  char *file_to_be_read = argv[1];
  printf("%s  file to be read \n", file_to_be_read);
  FILE *fp = fopen(file_to_be_read, "r");
  if (fp == NULL)
  {
    printf("No file read with the argument provided \n");
    exit(1);
  }

  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;
    unsigned char str_value;
    str_value = strtol(line, &endptr, 2);
    // printf("%c  this is the str value\n", str_value);
    cpu->ram[address] = str_value;
    address++;
  }
  fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    // TODO: implement more ALU ops
    printf("MULTIPLYING \n");
    cpu->reg[regA] *= cpu->reg[regB];
    break;
  case ALU_ADD:
    cpu->reg[regA] += cpu->reg[regB];
    break;
  case ALU_CMP:
    if (cpu->reg[regA] == cpu->reg[regB])
    {
      cpu->FLAG = 1;
    }
    break;
  }
}

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
    unsigned int num_operands = Instruction >> 6;
    unsigned int pc_to_be_added = num_operands + 1;
    // GET OPERANDS/ARGUMENTS
    // OPERANDS CANNOT EXCEED 255 bits
    // printf("NUM OPERANDS  %u \n", num_operands);
    // printf("PC NUM  %u \n", cpu->PC);
    unsigned char operandA = cpu_ram_read(cpu, (cpu->PC + 1) & 0xff);
    unsigned char operandB = cpu_ram_read(cpu, (cpu->PC + 2) & 0xff);
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (Instruction)
    {
    case HLT:
      running = 0;
      printf("END \n\n");
      break;
    case PRN:
      printf("%d \n", cpu->reg[operandA]);
      break;
    case LDI:
      cpu->reg[operandA] = operandB;
      break;
    case ADD:
      alu(cpu, ALU_ADD, operandA, operandB);
      break;
    case MUL:
      alu(cpu, ALU_MUL, operandA, operandB);
      break;
    case POP:
      cpu->reg[operandA] = cpu->ram[cpu->reg[SP]];
      cpu->reg[SP]++;
      break;
    case PUSH:
      cpu->reg[SP]--;
      cpu_ram_write(cpu, cpu->reg[SP], cpu->reg[operandA]);
      break;
    case CALL:
      //PUSH OUR SYS CALL IN THE STACK
      // PUSH THE CALL FUNCTION IN THE CURRENT STACK ADDRESS
      // TO BE EXECUTED BEFORE ANYTHING ELSE
      cpu->reg[SP]--;
      cpu_ram_write(cpu, cpu->reg[SP], cpu->PC + 1);
      // LETS EXECUTE PROCESS THE ON THE GIVEN REGISTER
      cpu->PC = cpu->reg[operandA] - 1;
      break;
    case RET:
      // RETURN TO THE SAVED WORK BY POPPING
      cpu->PC = cpu->ram[cpu->reg[SP]];
      //INCREMENT STACK POINTER
      cpu->reg[SP]++;
      break;
    case CMP:
      alu(cpu, ALU_CMP, operandA, operandB);
      break;
    case JMP:
      cpu->PC = cpu->reg[operandA];
      cpu->PC += 1;
      break;
    case JEQ:
      if (cpu->FLAG == 1)
      {
        cpu->PC = cpu->reg[operandA];
        cpu->PC -= 1;
        break;
      }
      break;
    case JNE:
      if (cpu->FLAG == 0)
      {
        cpu->PC = cpu->reg[operandA];
        cpu->PC -= 1;
        break;
      }
      break;
    default:
      break;
    }
    // 6. Move the PC to the next instruction.
    cpu->PC += pc_to_be_added;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->FLAG = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
  printf("initialized cpu \n");
}
