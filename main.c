#include "utils.h"

extern void memAbort();
extern void readErr();
extern void timeLimit();
int32_t main(void) {
    signal(SIGUSR1, memAbort);
    signal(SIGUSR2, readErr);
    signal(SIGUSR3, timeLimit);

    parser();
    return 0;
}