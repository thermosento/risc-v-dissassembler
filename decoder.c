#include "./decoder.h"
#include <stdio.h>  // for printf

riscv_instr decode_instr(uint32_t instr_bits) {
  riscv_instr result = {0};

  uint32_t opcode = instr_bits & 0x7F;
  uint32_t fun3 = (instr_bits >> 12) & 0x7;
  uint32_t fun7 = (instr_bits >> 25) & 0x7F;

  result.rd = (instr_bits >> 7) & 0x1F;
  result.rs1 = (instr_bits >> 15) & 0x1F;
  result.rs2 = (instr_bits >> 20) & 0x1F;

  switch (opcode) {
    // U fmt
    case 0x17:
      result.type = AUIPC;
      result.PCMuxCTL = 3;
      result.regFileWE = 1;
      result.regInputMuxCTL = 0;
      result.PCAddMuxCTL = ANY_SIGNAL;
      result.DATAWE = 0;
      result.ALUInputMuxCTL = 1;
      result.ALUCTL = 9;

      result.immediate = (instr_bits & 0xFFFFF000) >> 12;
      if (result.immediate & 0x80000) {
        result.immediate |= 0xFFF00000;
      }
      break;

    case 0x37:
      result.type = LUI;
      result.PCMuxCTL = 3;
      result.regFileWE = 1;
      result.regInputMuxCTL = 0;
      result.PCAddMuxCTL = ANY_SIGNAL;
      result.DATAWE = 0;
      result.ALUInputMuxCTL = 1;
      result.ALUCTL = 33;

      result.immediate = (instr_bits & 0xFFFFF000) >> 12;
      break;

    // I fmt
    case 0x13:
      result.immediate = (instr_bits & 0xFFF00000) >> 20;
      if (result.immediate & 0x800) {
        result.immediate |= 0xFFFFF000;
      }

      switch (fun3) {
        case 0x0:
          result.type = ADDI;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 0;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 0;
          break;

        case 0x2:
          result.type = SLTI;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 0;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 1;
          break;

        case 0x3:
          result.type = SLTIU;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 0;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 2;
          break;

        case 0x4:
          result.type = XORI;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 0;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 3;
          break;

        case 0x6:
          result.type = ORI;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 0;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 4;
          break;

        case 0x7:
          result.type = ANDI;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 0;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 5;
          break;

        case 0x1:
          if ((fun7 & 0xFE0) == 0x00) {
            result.type = SLLI;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 1;
            result.ALUCTL = 6;
          }
          break;

        case 0x5:
          if (fun7 == 0x00) {
            result.type = SRLI;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 1;
            result.ALUCTL = 7;
          } else if (fun7 == 0x20) {
            result.type = SRAI;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 1;
            result.ALUCTL = 8;
            result.immediate = (instr_bits >> 20) & 0x1F;
          }
          break;
      }
      break;

    // R fmt
    case 0x33:
      if (fun7 == 0x00) {
        switch (fun3) {
          case 0x0:
            result.type = ADD;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 9;
            break;

          case 0x1:
            result.type = SLL;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 11;
            break;

          case 0x2:
            result.type = SLT;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 12;
            break;

          case 0x3:
            result.type = SLTU;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 13;
            break;

          case 0x4:
            result.type = XOR;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 14;
            break;

          case 0x5:
            result.type = SRL;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 15;
            break;

          case 0x6:
            result.type = OR;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 17;
            break;

          case 0x7:
            result.type = AND;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 18;
            break;
        }
      } else if (fun7 == 0x20) {
        switch (fun3) {
          case 0x0:
            result.type = SUB;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 10;
            break;

          case 0x5:
            result.type = SRA;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 16;
            break;
        }
      } else if (fun7 == 0x01) {
        switch (fun3) {
          case 0x0:
            result.type = MUL;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 25;
            break;

          case 0x1:
            result.type = MULH;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 26;
            break;

          case 0x2:
            result.type = MULHSU;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 27;
            break;

          case 0x3:
            result.type = MULHU;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 28;
            break;

          case 0x4:
            result.type = DIV;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 29;
            break;

          case 0x5:
            result.type = DIVU;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 30;
            break;

          case 0x6:
            result.type = REM;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 31;
            break;

          case 0x7:
            result.type = REMU;
            result.PCMuxCTL = 3;
            result.regFileWE = 1;
            result.regInputMuxCTL = 0;
            result.PCAddMuxCTL = ANY_SIGNAL;
            result.DATAWE = 0;
            result.ALUInputMuxCTL = 0;
            result.ALUCTL = 32;
            break;
        }
      }
      break;

    // I fmt
    case 0x03:
      result.immediate = (instr_bits & 0xFFF00000) >> 20;
      if (result.immediate & 0x800) {
        result.immediate |= 0xFFFFF000;
      }
      switch (fun3) {
        case 0x0:
          result.type = LB;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 3;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 0;
          break;

        case 0x1:
          result.type = LH;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 2;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 0;
          break;

        case 0x2:
          result.type = LW;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 1;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 0;
          break;

        case 0x4:
          result.type = LBU;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 3;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 0;
          break;

        case 0x5:
          result.type = LHU;
          result.PCMuxCTL = 3;
          result.regFileWE = 1;
          result.regInputMuxCTL = 2;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 0;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 0;
          break;
      }
      break;

    // S fmt
    case 0x23:
      result.immediate =
          (((instr_bits >> 25) & 0x7F) << 5) + ((instr_bits >> 7) & 0x1F);
      if (result.immediate & 0x800) {
        result.immediate = result.immediate | 0xFFFFF000;
      }
      switch (fun3) {
        case 0x0:
          result.type = SB;
          result.PCMuxCTL = 3;
          result.regFileWE = 0;
          result.regInputMuxCTL = ANY_SIGNAL;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 1;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 36;
          break;

        case 0x1:
          result.type = SH;
          result.PCMuxCTL = 3;
          result.regFileWE = 0;
          result.regInputMuxCTL = ANY_SIGNAL;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 3;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 36;
          break;

        case 0x2:
          result.type = SW;
          result.PCMuxCTL = 3;
          result.regFileWE = 0;
          result.regInputMuxCTL = ANY_SIGNAL;
          result.PCAddMuxCTL = ANY_SIGNAL;
          result.DATAWE = 15;
          result.ALUInputMuxCTL = 1;
          result.ALUCTL = 36;

          result.rd = ANY_SIGNAL;
          break;
      }
      break;

    // other J fmt
    case 0x6F:  // JAL
      result.type = JAL;
      result.PCMuxCTL = 2;
      result.regFileWE = 1;
      result.regInputMuxCTL = 4;
      result.PCAddMuxCTL = 0;
      result.DATAWE = 0;
      result.ALUInputMuxCTL = ANY_SIGNAL;
      result.ALUCTL = ANY_SIGNAL;
      result.immediate = (((instr_bits >> 31) & 0x1) << 20) +
                         (((instr_bits >> 12) & 0xFF) << 12) +
                         (((instr_bits >> 20) & 0x1) << 11);
      result.immediate += ((instr_bits >> 21) & 0x3FF) << 1;
      if (result.immediate >> 20) {
        result.immediate = result.immediate | 0xFFE00000;
      }
      break;

    // other I fmt
    case 0x67:
      if (fun3 == 0x0) {
        result.type = JALR;
        result.PCMuxCTL = 0;
        result.regFileWE = 1;
        result.regInputMuxCTL = 4;
        result.PCAddMuxCTL = 0;
        result.DATAWE = 0;
        result.ALUInputMuxCTL = 1;
        result.ALUCTL = 34;
        result.immediate = instr_bits >> 20;
        if (result.immediate & 0x800) {
          result.immediate = result.immediate | 0xFFFFF000;
        }
      }
      break;

    // B fmt
    case 0x63:
      result.PCMuxCTL = 1;
      result.regFileWE = 0;
      result.regInputMuxCTL = ANY_SIGNAL;
      result.PCAddMuxCTL = ANY_SIGNAL;
      result.DATAWE = 0;
      result.ALUInputMuxCTL = 0;

      result.immediate = (((instr_bits >> 31) & 0x1) << 12) +
                         (((instr_bits >> 7) & 0x1) << 11);
      result.immediate =
          result.immediate + ((((instr_bits >> 25) & 0x3F) << 5) |
                              (((instr_bits >> 8) & 0xF) << 1));
      if (result.immediate & 0x1000) {
        result.immediate = result.immediate | 0xFFFFE000;
      }
      switch (fun3) {
        case 0x0:
          result.type = BEQ;
          result.ALUCTL = 19;
          break;

        case 0x1:
          result.type = BNE;
          result.ALUCTL = 20;
          break;

        case 0x4:
          result.type = BLT;
          result.ALUCTL = 21;
          break;

        case 0x5:
          result.type = BGE;
          result.ALUCTL = 22;
          break;

        case 0x6:
          result.type = BLTU;
          result.ALUCTL = 23;
          break;

        case 0x7:
          result.type = BGEU;
          result.ALUCTL = 24;
          break;
      }
      break;

    // anything else
    default:
      result.type = ILLEGAL_INSTR;
      result.PCMuxCTL = ANY_SIGNAL;
      result.regFileWE = ANY_SIGNAL;
      result.regInputMuxCTL = ANY_SIGNAL;
      result.PCAddMuxCTL = ANY_SIGNAL;
      result.DATAWE = ANY_SIGNAL;
      result.ALUInputMuxCTL = ANY_SIGNAL;
      result.ALUCTL = ANY_SIGNAL;
      break;
  }

  return result;
}

// Implementation of helper funtcion that may be useful for debugging
// You should not modify this function, nor do you need to understand how it
// works If you do want to know how this works, we left some comments..
//
// Given an INSTR_TYPE, prints out the corresponding string for that type.
// This is provided for debugging purposes, you should not modify the method
void print_type(INSTR_TYPE type) {
  // declares an array of C-strings containing the string of every
  // riscv instruction type. This array exists in global memory
  // and is generated exactly once.
  static const char* type_strings[] = {
      "ILLEGAL_INSTR",
      "LUI",
      "AUIPC",
      "ADDI",
      "SLTI",
      "SLTIU",
      "XORI",
      "ORI",
      "ANDI",
      "SLLI",
      "SRLI",
      "SRAI",
      "ADD",
      "SUB",
      "SLL",
      "SLT",
      "SLTU",
      "XOR",
      "SRL",
      "SRA",
      "OR",
      "AND",
      "LB",
      "LH",
      "LW",
      "LBU",
      "LHU",
      "SB",
      "SH",
      "SW",
      "JAL",
      "JALR",
      "BEQ",
      "BNE",
      "BLT",
      "BGE",
      "BLTU",
      "BGEU",
      "MUL",
      "MULH",
      "MULHSU",
      "MULHU",
      "DIV",
      "DIVU",
      "REM",
      "REMU",
  };

  // Note that an ENUM variable instance is internally implemented as a
  // integer variable. The value of that integer corresponds with the order in
  // which the possible enum values are declared. So ILLEGAL_INSTR is
  // represented as the integer 0 and the enum value LUI is implemented as the
  // integer 1.
  printf("%s", type_strings[type]);
}
