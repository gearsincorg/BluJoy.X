/* Timer.c
 * This system runs on the mSec interrupt, so ticks are milli-Sec
 */
#include "timers.h"

uint32_t    systemTime;
uint32_t    lastBTTime;
bool        oneSecBlink;

void        initTimers(void){
    systemTime = 0;
    lastBTTime = 0;
    oneSecBlink = false;
    TMR2_SetInterruptHandler(timeKeeper);
}

void        timeKeeper(void){
    systemTime++;
    
    // Look for 1024 ms interval.  0x03FF = 1023
    if ((systemTime & 0x03FF) == 0)
        oneSecBlink = true;
}

bool    oneSec(void) {
    if (oneSecBlink) {
        oneSecBlink = false;
        return true;
    } else {
        return false;
    }
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

void       resetBTTimer(void){
    lastBTTime = systemTime;
}

uint32_t   timeSinceLastReply(void){
    return (systemTime - lastBTTime);
}
