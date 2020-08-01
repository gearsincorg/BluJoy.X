/* Timer.c
 * This system runs on the mSec interrupt, so ticks are milli-Sec
 */
#include "timers.h"

uint32_t    systemTime;

void        initTimers(void){
    systemTime = 0;
    TMR2_SetInterruptHandler(timeKeeper);
}

void        timeKeeper(void){
    systemTime++;
}

void        sleep(uint32_t delay){
    uint32_t start = getTicks();
    while (getTicksSince(start) < delay);
}

uint32_t    getTicks(void){
    return systemTime;
}

int32_t    getTicksSince(uint32_t from){
    return (systemTime - from);
}

