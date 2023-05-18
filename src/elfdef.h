#ifndef ELF_DEF_H
#define ELF_DEF_H

#include <stdint.h>

#define ELF_MAG "\177ELF"

typedef struct 
{
    uint64_t e_entry;
} elf64_ehdr_t;

typedef struct 
{
    uint32_t p_type;
    uint32_t p_flags;
} elf64_phdr_t;

#endif