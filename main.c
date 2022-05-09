#include "utils.h"

int32_t main(void) {
    extern void memAbort();
    extern void readErr();
    extern void timeLimit();
    
    signal(SIGUSR1, memAbort);
    signal(SIGUSR2, readErr);
    signal(SIGUSR3, timeLimit);

    parser();
    return 0;
}