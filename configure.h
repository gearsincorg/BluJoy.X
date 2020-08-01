// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIGURE_HEADER_H
#define	CONFIGURE_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  

void    SetMasterTXRX(void);
void    SetSlaveTXRX(void);
void    SetSlaveTXMasterRx(void);

void    turnPowerOn();
void    turnPowerOff();

void    setBlutoothBaud(uint16_t baud);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* CONFIGURE_HEADER_H */

