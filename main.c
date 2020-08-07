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

#define BT_TIMEOUT  60000

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    initTimers();   
    initSerial();   
    initUI();   
 
    turnPowerOn();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    initConfiguration();
    sleep(1000);
           
    while (1) {
        sleep(50);
        
        // check to see if we should initialize the master and slave devices
        if (USER1_pressed()){
            // Configure Master and Slave devices
            pairBluetoothDevices();
        }
                
        if (USER2_pressed()){
            // Do a factory reset
            doFactoryReset();
        }
    }
    
    // check to see if we should initialize the master and slave devices
    if (USER1_pressed() && USER1_pressed()){
        // Configure Master and Slave devices
        pairBluetoothDevices();
        
        // Also init any NV memory
        setUISpeedMode(0);
        setUIBreakMode(0);
    } else {
        // if we get here, it's time to play
        initJoystick();   

        while(1)
        {
            // keep an eye out loss of bluetooth
            if (timeSincelLastReply() > BT_TIMEOUT){
                // power down the BT and Joystick and wait for wakeup
                turnPowerOff();
                SLEEP();
                
                while (!powerIsOn());
                
            }
            runUI();
            sleep(50);
        }
    }
    
    // Disable the Global Interrupts
    INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptDisable();
}

/**
 End of File
*/