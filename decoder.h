#ifndef DECODER_H_
#define DECODER_H_

#include <stdint.h>  // for sized integers

/*
 * decode.h
 */

// Defines a new enumerated type 'INSTR_TYPE'
// which is used to identify the different types of instructions in RISC-V
// Each of the listed values e.g. ILLEGAL_INSTR, LUI, ADD, ... are
// possible values of the INSTR_TYPE.
typedef enum {
  ILLEGAL_INSTR,
  LUI,
  AUIPC,
  ADDI,
  SLTI,
  SLTIU,
  XORI,
  ORI,
  ANDI,
  SLLI,
  SRLI,
  SRAI,
  ADD,
  SUB,
  SLL,
  SLT,
  SLTU,
  XOR,
  SRL,
  SRA,
  OR,
  AND,
  LB,
  LH,
  LW,
  LBU,
  LHU,
  SB,
  SH,
  SW,
  JAL,
  JALR,
  BEQ,
  BNE,
  BLT,
  BGE,
  BLTU,
  BGEU,
  MUL,
  MULH,
  MULHSU,
  MULHU,
  DIV,
  DIVU,
  REM,
  REMU,
} INSTR_TYPE;

// defines a constant "variable" that can be later used
// to indicate that any signal is ok for that signal value
#define ANY_SIGNAL -1

typedef struct {
  INSTR_TYPE type;  // Type of the instruction

  // contol signals (Due this week)
  int PCMuxCTL;
  int regFileWE;
  int regInputMuxCTL;
  int PCAddMuxCTL;
  int DATAWE;
  int ALUInputMuxCTL;
  int ALUCTL;

  // Due next week
  int32_t rd, rs1, rs2;  // Register fields
  int32_t immediate;     // Immediate field of instruction
} riscv_instr;

riscv_instr decode_instr(uint32_t instr_bits);

// Given an INSTR_TYPE, prints out the corresponding string for that type.
// This is provided for debugging purposes, you should not modify the method
void print_type(INSTR_TYPE type);

#endif  // DECODER_H_
