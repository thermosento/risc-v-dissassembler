#ifndef LOADER_H_
#define LOADER_H_

#include <stdint.h>

#define MAGIC_WORD 0xCAFEF00D

// Section types
#define SECTION_CODE 0xC0DE
#define SECTION_DATA 0xDADA
#define SECTION_SYMBOL 0xC3B7
#define SECTION_FILENAME 0xF17E
#define SECTION_LINENUMBER 0x715E

// Memory sizes
#define DATA_MEM_SIZE 65536
// Code memory is 2^16 bytes, each instruction is 4 bytes, so 16384 instructions
#define CODE_MEM_SIZE (65536 / 4)

// Function prototype
int load_object_file(const char* filename,
                     uint8_t data_mem[],
                     uint32_t code_mem[]);

#endif  // LOADER_H_