/*
 * joystick.c :  Reads switch inputs and generates axis motions.
 * Implements acceleration limits
 * Assumes constant 50mS cycle time from Timer 1 (20 updates per second)
 * 
 * To enable small variation using integers, speeds will be left shifted SHIFT_BITS places
 * Integers holding shifted values will have FP (Fixed Point) suffix.
 * 
 */

#include <math.h>
#include "joystick.h"
#include "serial.h"

#define     SHIFT_BITS      2
#define     AXIAL_ACC_LIMIT 600      //  mm/s/s
#define     YAW_ACC_LIMIT   90       //  deg/s/s
#define     SWEEP_ACC_LIMIT 30       //  deg/s/s   
#define     CYCLE_PER_SEC   20       

#define     TOP_AXIAL_SPEED 500
#define     TOP_YAW_SPEED   90
#define     TOP_SWEEP_SPEED 60

int16_t     targetAxialFP = 0;
int16_t     targetYawFP   = 0;

int16_t     limitedAxialFP = 0;
int16_t     limitedYawFP   = 0;

int16_t     accelAxialFP = 0;
int16_t     accelYawFP   = 0;

int16_t     accelLimitAxialFP   = (AXIAL_ACC_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitYawFP     = (YAW_ACC_LIMIT   << SHIFT_BITS) / CYCLE_PER_SEC ;
int16_t     accelLimitSweepFP   = (SWEEP_ACC_LIMIT << SHIFT_BITS) / CYCLE_PER_SEC ;

int16_t     topAxialSpeedFP   = (TOP_AXIAL_SPEED << SHIFT_BITS) ;
int16_t     topYawSpeedFP     = (TOP_YAW_SPEED   << SHIFT_BITS) ;
int16_t     topSweepSpeedFP   = (TOP_SWEEP_SPEED << SHIFT_BITS) ;


void    initJoystick(void) {
    TMR1_SetInterruptHandler(readJoystick);
    targetAxialFP = 0;
    targetYawFP   = 0;
    limitedAxialFP = 0;
    limitedYawFP   = 0;
}

void    readJoystick(void) {
    
    accelAxialFP = accelLimitAxialFP;
    accelYawFP   = accelLimitYawFP;
            
    if (JSUP_GetValue() == 0)
        targetAxialFP =  topAxialSpeedFP;
    else if (JSDO_GetValue() == 0)
        targetAxialFP = -topAxialSpeedFP;
    else
        targetAxialFP =   0;
        
    if (JSRI_GetValue() == 0){
        if (targetAxialFP == 0)
            targetYawFP =  topYawSpeedFP;
        else {
            targetYawFP =  topSweepSpeedFP;
            accelYawFP   = accelLimitSweepFP;
        }
    }
    else if (JSLE_GetValue() == 0){
        if (targetAxialFP == 0)
            targetYawFP =  -topYawSpeedFP;
        else {
            targetYawFP =  -topSweepSpeedFP;
            accelYawFP   = accelLimitSweepFP;
        }
    }
    else
        targetYawFP =   0;
    
    // calculate motion profile and send to serial port.
    calculateMotion();
    sendSpeedCmd(limitedAxialFP >> SHIFT_BITS, limitedYawFP >> SHIFT_BITS, false);
}
    
void    calculateMotion(void) {
    // use target speeds and acceleration limits to generate limited speeds.
    limitedAxialFP = limitSpeed(targetAxialFP, limitedAxialFP, accelAxialFP);
    limitedYawFP   = limitSpeed(targetYawFP,   limitedYawFP,   accelYawFP);
}

int16_t limitSpeed(int16_t targetFP, int16_t limitedFP, int16_t accelFP) {

    int16_t diffFP  = targetFP - limitedFP;
    int16_t speedFP = targetFP;
    
    // boost deceleration rate if we are stopping
    if (targetFP == 0)
        accelFP *= 2;
    
    // Ensure acceleration limit is not too small.
    if (accelFP == 0)
        accelFP = 1;
    
    // Limit acceleration (speed change) if required.
    if (diffFP > accelFP){
        speedFP = limitedFP + accelFP;
    } else if (diffFP < -accelFP){
        speedFP = limitedFP - accelFP;
    }
    
    return (speedFP);
}
