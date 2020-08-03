/* 
 * Author: Phil Malone
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SERIAL_HEADER_H
#define	SERIAL_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "mcc_generated_files/eusart1.h"

// TODO Insert declarations
void     initSerial(void);
bool     sendBTSpeedCmd(int16_t  axial, int16_t yaw, bool blockIfBusy) ;
void     sendBTString(char * command);
bool     sendBTBuffer(uint8_t * buffer, uint8_t length, bool blockIfBusy);
void     flushBTRXbuffer(void);
uint8_t  receiveBTBuffer(uint8_t * buffer, uint8_t maxChars, uint16_t timeoutMS);

uint16_t calcCRC(uint8_t *ptr);
void	 int16ToBytes(int16_t num, uint8_t * ndata);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* SERIAL_HEADER_H */

