/**
  Generated Main Source File

  File Name:
    main.c

*/

#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "timers.h"
#include "joystick.h"
#include "serial.h"
#include "configure.h"
#include "ui.h"

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    initTimers();   
    initJoystick();   
    initSerial();   
    initUI();   
    
    turnPowerOn();
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    showStartup();

    // check to see if we should initialize the master and slave devices
    if (USER1_pressed() && USER1_pressed()){
        // Configure Master and Slave devices
        pairBluetoothDevices();
        
        // Also init any NV memory
        setUISpeedMode(0);
        setUIBreakMode(0);
    }
        
    while(1)
    {
        runUI();
        sleep(50);
    }
    
    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
}

/**
 End of File
*/