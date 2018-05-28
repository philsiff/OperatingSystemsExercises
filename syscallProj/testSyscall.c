/*
 * Test the phillip Syscall. For output do dmesg | tail
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

#define SYS_phillip 333

int main(int argc, char **argv){
    printf("TIME TO PRAY\n");
    long res = syscall(SYS_phillip);
    printf("RETURNED: %ld.\n", res);
    return res;
}
