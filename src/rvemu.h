#ifndef RVEMU_H
#define RVEMU_H

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include "elfdef.h"

#define fatal_fmt(fmt, ...)                                                         \
    {                                                                               \
        fprintf(stderr, "fatal: %s:%d " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
        exit(1);                                                                    \
    }

#define fatal_msg(msg) fatal_fmt("%s", msg)

/**
 * mmu.c
 */
typedef struct {
    uint64_t entry;
} mmu_t;

void mmu_load_elf(mmu_t *mmu, int fd);

typedef struct {
    uint64_t gp_regs[32];
    uint64_t pc;
} state_t;

typedef struct {
    state_t state;
    mmu_t mmu;
} machine_t;

void machine_load_program(machine_t *m, const char *prog);

#endif