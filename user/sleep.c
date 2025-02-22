#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "usages: sleep [ticks num]\n");
        exit(1);
    }
    // atoi sys call guarantees return an integer
    exit(sleep(atoi(argv[1])));
}