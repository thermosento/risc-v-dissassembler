#include <stdio.h>
#include <stdlib.h>
#include "decoder.h"
#include "loader.h"

static void print_instruction(FILE* out, riscv_instr instr) {
  if (instr.type == ILLEGAL_INSTR) {
    fprintf(out, "INVALID INSTRUCTION");
    return;
  }

#define XR(reg) ((int)(reg))

  switch (instr.type) {
    case LUI:
      fprintf(out, "LUI x%d, %d", XR(instr.rd), instr.immediate);
      break;
    case AUIPC:
      fprintf(out, "AUIPC x%d, %d", XR(instr.rd), instr.immediate);
      break;
    case ADDI:
      fprintf(out, "ADDI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate);
      break;
    case SLTI:
      fprintf(out, "SLTI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate);
      break;
    case SLTIU:
      fprintf(out, "SLTIU x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate);
      break;
    case XORI:
      fprintf(out, "XORI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate);
      break;
    case ORI:
      fprintf(out, "ORI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate);
      break;
    case ANDI:
      fprintf(out, "ANDI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate);
      break;
    case SLLI:
      fprintf(out, "SLLI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate & 0x1F);
      break;
    case SRLI:
      fprintf(out, "SRLI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate & 0x1F);
      break;
    case SRAI:
      fprintf(out, "SRAI x%d, x%d, %d", XR(instr.rd), XR(instr.rs1),
              instr.immediate & 0x1F);
      break;
    case ADD:
      fprintf(out, "ADD x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case SUB:
      fprintf(out, "SUB x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case SLL:
      fprintf(out, "SLL x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case SLT:
      fprintf(out, "SLT x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case SLTU:
      fprintf(out, "SLTU x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case XOR:
      fprintf(out, "XOR x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case SRL:
      fprintf(out, "SRL x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case SRA:
      fprintf(out, "SRA x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case OR:
      fprintf(out, "OR x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case AND:
      fprintf(out, "AND x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case LB:
      fprintf(out, "LB x%d, %d(x%d)", XR(instr.rd), instr.immediate,
              XR(instr.rs1));
      break;
    case LH:
      fprintf(out, "LH x%d, %d(x%d)", XR(instr.rd), instr.immediate,
              XR(instr.rs1));
      break;
    case LW:
      fprintf(out, "LW x%d, %d(x%d)", XR(instr.rd), instr.immediate,
              XR(instr.rs1));
      break;
    case LBU:
      fprintf(out, "LBU x%d, %d(x%d)", XR(instr.rd), instr.immediate,
              XR(instr.rs1));
      break;
    case LHU:
      fprintf(out, "LHU x%d, %d(x%d)", XR(instr.rd), instr.immediate,
              XR(instr.rs1));
      break;
    case SB:
      fprintf(out, "SB x%d, %d(x%d)", XR(instr.rs2), instr.immediate,
              XR(instr.rs1));
      break;
    case SH:
      fprintf(out, "SH x%d, %d(x%d)", XR(instr.rs2), instr.immediate,
              XR(instr.rs1));
      break;
    case SW:
      fprintf(out, "SW x%d, %d(x%d)", XR(instr.rs2), instr.immediate,
              XR(instr.rs1));
      break;
    case JAL:
      fprintf(out, "JAL x%d, %d", XR(instr.rd), instr.immediate);
      break;
    case JALR:
      // Changed format here
      fprintf(out, "JALR x%d, %d(x%d)", XR(instr.rd), instr.immediate,
              XR(instr.rs1));
      break;
    case BEQ:
      fprintf(out, "BEQ x%d, x%d, %d", XR(instr.rs1), XR(instr.rs2),
              instr.immediate);
      break;
    case BNE:
      fprintf(out, "BNE x%d, x%d, %d", XR(instr.rs1), XR(instr.rs2),
              instr.immediate);
      break;
    case BLT:
      fprintf(out, "BLT x%d, x%d, %d", XR(instr.rs1), XR(instr.rs2),
              instr.immediate);
      break;
    case BGE:
      fprintf(out, "BGE x%d, x%d, %d", XR(instr.rs1), XR(instr.rs2),
              instr.immediate);
      break;
    case BLTU:
      fprintf(out, "BLTU x%d, x%d, %d", XR(instr.rs1), XR(instr.rs2),
              instr.immediate);
      break;
    case BGEU:
      fprintf(out, "BGEU x%d, x%d, %d", XR(instr.rs1), XR(instr.rs2),
              instr.immediate);
      break;
    case MUL:
      fprintf(out, "MUL x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case MULH:
      fprintf(out, "MULH x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case MULHSU:
      fprintf(out, "MULHSU x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case MULHU:
      fprintf(out, "MULHU x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case DIV:
      fprintf(out, "DIV x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case DIVU:
      fprintf(out, "DIVU x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case REM:
      fprintf(out, "REM x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    case REMU:
      fprintf(out, "REMU x%d, x%d, x%d", XR(instr.rd), XR(instr.rs1),
              XR(instr.rs2));
      break;
    default:
      fprintf(out, "INVALID INSTRUCTION");
      break;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s output_filename input1.o [input2.o ...]\n",
            argv[0]);
    return EXIT_FAILURE;
  }

  const char* output_filename = argv[1];
  FILE* out = fopen(output_filename, "w");
  if (!out) {
    fprintf(stderr, "Error: Could not open output file %s\n", output_filename);
    return EXIT_FAILURE;
  }

  static uint8_t data_mem[DATA_MEM_SIZE] = {0};
  static uint32_t code_mem[CODE_MEM_SIZE] = {0};

  for (int i = 2; i < argc; i++) {
    if (load_object_file(argv[i], data_mem, code_mem) != 0) {
      fprintf(stderr, "Error: Loading object file %s failed.\n", argv[i]);
      fclose(out);
      return EXIT_FAILURE;
    }
  }

  // Print code memory
  for (int i = 0; i < CODE_MEM_SIZE; i++) {
    if (code_mem[i] != 0) {
      uint32_t address = 0x10000000 + (i * 4);
      uint32_t instr_bits = code_mem[i];
      riscv_instr instr = decode_instr(instr_bits);
      fprintf(out, "%08x : %08x -> ", address, instr_bits);
      print_instruction(out, instr);
      fprintf(out, "\n");
    }
  }

  // Print data memory
  for (int i = 0; i < DATA_MEM_SIZE; i++) {
    if (data_mem[i] != 0) {
      uint32_t address = 0x20000000 + i;
      fprintf(out, "%08x : %02x\n", address, data_mem[i]);
    }
  }

  fclose(out);
  return EXIT_SUCCESS;
}