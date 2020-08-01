/**
  Generated Main Source File

  File Name:
    main.c

*/

#include "mcc_generated_files/mcc.h"
#include "timers.h"
#include "joystick.h"
#include "serial.h"
#include "configure.h"

/*
                         Main application
 */
void main(void)
{
    volatile uint8_t rxData;

    // Initialize the device
    SYSTEM_Initialize();
    initTimers();   
    initJoystick();   
    initSerial();   
    turnPowerOn();
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    while(1)
    {
        sleep(500);
    }
    
    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
}

/**
 End of File
*/