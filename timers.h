/* 
 * Author: Phil Malone
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMERS_HEADER_H
#define	TIMERS_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/tmr2.h"

void        initTimers(void);
void        timeKeeper(void);
void        sleep(uint32_t delay);
uint32_t    timeSincelLastReply(void);
void        resetBTTimer(void);
uint32_t    getTicks(void);
int32_t     getTicksSince(uint32_t from);



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* TIMERS_HEADER_H */

