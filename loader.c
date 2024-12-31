#include "loader.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int read_word(FILE* f, uint32_t* w) {
  uint32_t buf;
  if (fread(&buf, sizeof(uint32_t), 1, f) != 1) {
    return -1;
  }
  *w = ntohl(buf);
  return 0;
}

static int read_half(FILE* f, uint16_t* h) {
  uint16_t buf;
  if (fread(&buf, sizeof(uint16_t), 1, f) != 1) {
    return -1;
  }
  *h = ntohs(buf);
  return 0;
}

static int skip_bytes(FILE* f, uint32_t count) {
  if (fseek(f, (long)count, SEEK_CUR) != 0) {
    return -1;
  }
  return 0;
}

static int process_section(uint16_t sec_type,
                           FILE* file,
                           uint8_t data_mem[],
                           uint32_t code_mem[]) {
  uint32_t addr, length;

  switch (sec_type) {
    case SECTION_CODE:
      if (read_word(file, &addr) || read_word(file, &length)) {
        return -1;
      }
      for (uint32_t i = 0; i < length; i++) {
        uint32_t val;
        if (read_word(file, &val)) {
          return -1;
        }
        uint32_t offset = ((addr - 0x10000000) / 4) + i;
        if (offset < CODE_MEM_SIZE) {
          code_mem[offset] = val;
        }
      }
      break;

    case SECTION_DATA:
      if (read_word(file, &addr) || read_word(file, &length)) {
        return -1;
      }
      for (uint32_t i = 0; i < length; i++) {
        uint8_t val;
        if (fread(&val, sizeof(uint8_t), 1, file) != 1) {
          return -1;
        }
        // Convert address to data_mem index
        uint32_t offset = (addr - 0x20000000) + i;
        if (offset < DATA_MEM_SIZE) {
          data_mem[offset] = val;
        }
      }
      break;

    case SECTION_SYMBOL: {
      uint32_t sym_addr, nbytes;
      if (read_word(file, &sym_addr) || read_word(file, &nbytes)) {
        return -1;
      }
      if (skip_bytes(file, nbytes)) {
        return -1;
      }
      break;
    }

    case SECTION_FILENAME: {
      uint32_t name_len;
      if (read_word(file, &name_len)) {
        return -1;
      }
      if (skip_bytes(file, name_len)) {
        return -1;
      }
      break;
    }

    case SECTION_LINENUMBER: {
      uint32_t l_addr, line, f_idx;
      if (read_word(file, &l_addr) || read_word(file, &line) ||
          read_word(file, &f_idx)) {
        return -1;
      }
      break;
    }

    default:
      return -1;
  }

  return 0;
}

int load_object_file(const char* filename,
                     uint8_t data_mem[],
                     uint32_t code_mem[]) {
  FILE* f = fopen(filename, "rb");
  if (!f) {
    fprintf(stderr, "Error: Could not open file %s\n", filename);
    return -1;
  }

  uint32_t magic_val;
  if (read_word(f, &magic_val) || magic_val != MAGIC_WORD) {
    fprintf(stderr, "Error: Invalid magic word in file %s\n", filename);
    fclose(f);
    return -1;
  }

  while (1) {
    uint16_t section_type;
    int sec_read = read_half(f, &section_type);
    if (sec_read != 0) {
      break;
    }

    if (process_section(section_type, f, data_mem, code_mem) != 0) {
      fprintf(stderr, "Error: Corrupted or unknown section in %s\n", filename);
      fclose(f);
      return -1;
    }
  }

  fclose(f);
  return 0;
}