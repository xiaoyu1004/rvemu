#include "rvemu.h"

void mmu_load_elf(mmu_t *mmu, int fd) {
    uint8_t buf[sizeof(elf64_ehdr_t)];
    FILE *file = fdopen(fd, "rb");

    if (fread(buf, 1, sizeof(elf64_ehdr_t), file) != sizeof(elf64_ehdr_t)) {
        fatal_msg("file too small");
    }

    elf64_ehdr_t *ehdr = (elf64_ehdr_t *)buf;
}