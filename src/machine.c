#include "rvemu.h"

void machine_load_program(machine_t *m, const char *prog) {
    int fd = open(prog, O_RDONLY);
    if (fd == -1) {
        fatal_msg(strerror(errno));
    }

    
}