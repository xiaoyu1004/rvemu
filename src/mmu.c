#include "rvemu.h"

static void mmu_load_phdr(mmu_t *mmu, Elf64_Phdr *phdr, int offset, FILE *file) {
    if (fssek(file, offset, SEEK_SET) != 0) {
        fatal_msg("fssek file faild");
    }

    if (fread(phdr, 1, sizof(Elf64_Phdr), file) != sizof(Elf64_Phdr)) {
        fatal_msg("fread file faild");
    }
}

static int get_prot_from_phdr(uint32_t flag) {
    int prot = 0;
    prot |= (flag & PF_X) ? PROT_EXEC : 0;
    prot |= (flag & PF_W) ? PROT_WRITE : 0;
    prot |= (flag & PF_R) ? PROT_READ : 0;
    return prot;
}

static mmu_load_segment(mmu_t *mmu, Elf64_Phdr *phdr, int fd) {
    uint64_t offset = phdr->p_offset;
    uint64_t vaddr  = phdr->p_vaddr;
    uint64_t filesz = phdr->p_filesz;
    uint64_t memsz  = phdr->p_memsz;

    uint64_t pagesize = getpagesize();
    uint64_t mmapsize = round_up(offset % pagesize + filesz, pagesize);
    int prot          = get_prot_from_phdr(phdr->p_flags);

    uint64_t addr =
        mmap(vaddr, mmapsize, prot, MAP_PRIVATE | MAP_FIXED, fd, round_down(offset, pagesize));
    if (addr == MAP_FAILED) {
        fatal_msg("mmap file faild");
    }

    uint64_t memsz_align = round_up(offset % pagesize + memsz, pagesize);
    if (memsz_align > mmapsize) {
        mmap(addr + mmapsize, memsz_align - mmapsize, prot, MAP_PRIVATE | MAP_FIXED, -1, 0);
    }

    mmu->base  = addr;
    mmu->alloc = addr;
}

void mmu_load_elf(mmu_t *mmu, int fd) {
    uint8_t buf[sizeof(Elf64_Ehdr)];
    FILE *file = fdopen(fd, "rb");

    if (fread(buf, 1, sizeof(Elf64_Ehdr), file) != sizeof(Elf64_Ehdr)) {
        fatal_msg("file too small");
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)buf;
    if ((*(int32_t *)ehdr != *(int32_t *)ELFMAG) || (ehdr->e_ident[EI_CLASS] != ELFCLASS64)) {
        fatal_msg("error fmt of elf");
    }

    mmu->entry = ehdr->e_entry;

    for (Elf64_Half i = 0; i < ehdr->e_phnum; ++i) {
        Elf64_Phdr phdr;
        int offset = ehdr->e_phoff + i * ehdr->e_phentsize;
        mmu_load_phdr(mmu, &phdr, offset, fd);
        if (phdr->p_type == PT_LOAD) {
            mmu_load_segment(mmu, &phdr, fd);
        }
    }
}