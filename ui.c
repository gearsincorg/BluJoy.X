/*
 * ui.c :  Reads switch inputs and generates axis motions.
 * Implements acceleration limits
 * Assumes constant 50mS cycle time from Timer 1 (20 updates per second)
 * 
 * To enable small variation using integers, speeds will be left shifted SHIFT_BITS places
 * Integers holding shifted values will have FP (Fixed Point) suffix.
 * 
 */

#include "mcc_generated_files/mcc.h"
#include "configure.h"
#include "timers.h"
#include "ui.h"

__eeprom uint8_t  EEPROM_uiSpeedMode ;
__eeprom uint8_t  EEPROM_uiBreakMode ;

uint8_t  uiSpeedMode ;
uint8_t  uiBreakMode ;

uint8_t  R_LED ;
uint8_t  G_LED ;
uint8_t  B_LED ;
uint8_t  dutyCycle ;
uint8_t  uiState ;
uint32_t uiStateTime;

#define UI_IDLE 0
#define UI_USER1_HOLD 1
#define UI_USER2_HOLD 2

#define UI_SPEED_MODES 3
#define UI_BREAK_MODES 2
#define UI_MIN_HOLD  150

void    initUI(void) {
    R_LED = 0 ;
    G_LED = 0 ;
    B_LED = 0 ;
    dutyCycle = 0;
    uiState = UI_IDLE;
    uiStateTime = getTicks();
    
    RED_SetHigh();
    GREEN_SetHigh();
    BLUE_SetHigh();
    
    uiSpeedMode = EEPROM_uiSpeedMode;
    uiBreakMode = EEPROM_uiBreakMode;
    
    TMR3_SetInterruptHandler(UI_PWM_handler);
    //IOCCF2_SetInterruptHandler(turnPowerOn);
    
}

void    runUI(void) {
    switch (uiState) {
        case UI_IDLE:
            if (USER1_pressed()) {
                uiStateTime = getTicks();
                uiState = UI_USER1_HOLD; 
            } else if (USER2_pressed()) {
                uiStateTime = getTicks();
                uiState = UI_USER2_HOLD; 
            }
            break;

        case UI_USER1_HOLD:
            if (!USER1_pressed() && (getTicksSince(uiStateTime) > UI_MIN_HOLD)) {
                setUISpeedMode((uiSpeedMode +1) % UI_SPEED_MODES);
                blinkLEDColor(COLOR_YELLOW, uiSpeedMode + 1);
                uiState = UI_IDLE;
            }
            break;
            
        case UI_USER2_HOLD:
            if (!USER1_pressed() && (getTicksSince(uiStateTime) > UI_MIN_HOLD)) {
                setUIBreakMode((uiBreakMode +1) % UI_BREAK_MODES);
                blinkLEDColor(COLOR_CYAN, uiBreakMode + 1);
                uiState = UI_IDLE;
            }
            break;
            
        default:
            break;
    }
}

void    setUISpeedMode(uint8_t mode){
    uiSpeedMode = mode;
    EEPROM_uiSpeedMode = uiSpeedMode;
}

uint8_t getUISpeedMode(){
    return (uiSpeedMode);
}

void    setUIBreakMode(uint8_t mode){
    uiBreakMode = mode;
    EEPROM_uiBreakMode = uiBreakMode;
}

uint8_t getUIBreakMode(){
    return (uiBreakMode);
}

void    showStartupx(void){
    R_LED = 0x0;
    sleep(1000);
    R_LED = 0x1;
    sleep(1000);
    R_LED = 0x8;
    sleep(1000);
    R_LED = 0x10;
    sleep(1000);
}


void    showStartup(void){
    int8_t ramp = 0;
    
    for (ramp = 0 ; ramp < 16; ramp++) {
        R_LED = ramp;
        sleep(10);
    }
    for (ramp = 14 ; ramp >= 0; ramp--) {
        R_LED = ramp;
        sleep(10);
    }
    for (ramp = 0 ; ramp < 16; ramp++) {
        G_LED = ramp;
        sleep(10);
    }
    for (ramp = 14 ; ramp >= 0; ramp--) {
        G_LED = ramp;
        sleep(10);
    }
    for (ramp = 0 ; ramp < 16; ramp++) {
        B_LED = ramp;
        sleep(10);
    }
    for (ramp = 14 ; ramp >= 0; ramp--) {
        B_LED = ramp;
        sleep(10);
    }
    
}

/**
 * 
 * @param RGB  16 bit unsigned number where bottom 12 LSB save vales for RG and B as 4 bit values, 
 * packed into number as 0x0RGB
 */
void    setLEDColor(uint16_t RGB) {
    B_LED = RGB & 0x0F;
    G_LED = (RGB >> 4) & 0x0F;
    R_LED = (RGB >> 8) & 0x0F;
}

void    pulseLEDColor(uint16_t RGB, uint16_t onTimeMS, uint16_t offTimeMS){
    setLEDColor(RGB);
    sleep(onTimeMS);
    setLEDColor(COLOR_OFF);
    sleep(offTimeMS);
}

void    blinkLEDColor(uint16_t RGB, uint8_t blinks){
    uint8_t blink;
    
    for(blink=0; blink < blinks; blink++){
        pulseLEDColor(RGB, 100, 400);
    }
}

void    UI_PWM_handlerx(void){
    dutyCycle++;
    dutyCycle = dutyCycle % 16;
    if (dutyCycle == 0){
        if (R_LED > 0) 
            RED_SetLow();
        else
            RED_SetHigh();
    } else {
        if (dutyCycle >= R_LED) 
            RED_SetHigh();
    }
}

void    UI_PWM_handler(void){
    // Manage a 16 step PWM Duty cycle for the 3 LEDs that form the RGB LED
    // Set the output low to turn on each cycle at dutyCycle = 0 (if required)
    // Set the output High when the required dutyCycle is reached for each LED
    dutyCycle++;
    dutyCycle = dutyCycle % 16;  //
    if (dutyCycle == 0){
        if (R_LED > 0) 
            RED_SetLow();
        else
            RED_SetHigh();
            
        if (G_LED > 0) 
            GREEN_SetLow();
        else
            GREEN_SetHigh();
            
        if (B_LED > 0) 
            BLUE_SetLow();
        else
            BLUE_SetHigh();
    } else {
        if (dutyCycle >= R_LED) 
            RED_SetHigh();
        if (dutyCycle >= G_LED) 
            GREEN_SetHigh();
        if (dutyCycle >= B_LED) 
            BLUE_SetHigh();
    }
    
}

bool    USER1_pressed(void) {
    return USER1_GetValue() == 0;
}

bool    USER2_pressed(void) {
    return USER2_GetValue() == 0;
}


