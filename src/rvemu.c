#include "rvemu.h"

int main(int argc, const char **argv) {
    machine_t m;
    machine_load_program(&m, argv[1]);
    return 0;
}

