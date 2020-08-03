// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef UI_HEADER_H
#define	UI_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define COLOR_RED     0xF00
#define COLOR_GREEN   0x0F0
#define COLOR_BLUE    0x00F
#define COLOR_YELLOW  0xFF0
#define COLOR_AMBER   0xF70
#define COLOR_MAGENTA 0xF0F
#define COLOR_CYAN    0x0FF
#define COLOR_WHITE   0xFFF
#define COLOR_OFF     0x000

// TODO Insert appropriate #include <>

uint8_t getUISpeedMode();
void    initUI(void);
void    pulseLEDColor(uint16_t RGB, uint16_t onMS, uint16_t offMS);
void    blinkLEDColor(uint16_t RGB, uint8_t blinks);
void    runUI(void);
void    setLEDColor(uint16_t RGB);
void    setUISpeedMode(uint8_t mode);
void    setUIBreakMode(uint8_t mode);
void    showStartup(void);
void    UI_PWM_handler(void);
bool    USER1_pressed(void);
bool    USER2_pressed(void);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* UI_HEADER_H */

