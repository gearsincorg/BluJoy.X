#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "configure.h"
#include "serial.h"
#include "timers.h"
#include "ui.h"

uint8_t RX_Buffer[32];
uint8_t charsRead;
uint8_t slaveMAC[MAC_LENGTH]  = {0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t masterMAC[MAC_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0};
bool    powerOn = false;

void    SetSlaveTXRX(void){
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x00;

    RC4PPS = 0x0F;     //RC4->EUSART1:TX1;    
    RX1DTPPS = 0x15;   //RC5->EUSART1:RX1;    

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x01;
    sleep(100);
}

void    SetMasterTXRX(void){
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x00;

    RC6PPS = 0x0F;     //RC6->EUSART1:TX1;    
    RX1DTPPS = 0x13;   //RC3->EUSART1:RX1;    

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x01;
    sleep(100);
}

void    SetSlaveTXMasterRx(void){
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x00;

    RC4PPS = 0x0F;     //RC4->EUSART1:TX1;    
    RX1DTPPS = 0x13;   //RC3->EUSART1:RX1;    

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0x01;
    sleep(20);
}

void    turnPowerOn(){
    POWER_EN_SetLow();
    powerOn = true;
}

void    turnPowerOff(){
    POWER_EN_SetHigh();
    powerOn = true;
}

bool    powerIsOn() {
    return powerOn;
}

void    setBlutoothBaud(uint16_t baud){
    if (baud == 38400) {
        // 38400
        SP1BRGL = 0x33;
        SP1BRGH = 0x00;
    } else {
        // 9600
        SP1BRGL = 0xCF;
        SP1BRGH = 0x00;
    }
    sleep(200);
}

void    pairBluetoothDevices(void){

    SetMasterTXRX();
    sleep(1000);
    getBTAddress(masterMAC);   
       
    // Get two Mac Addresses
    SetSlaveTXRX();
    sleep(1000);
    getBTAddress(slaveMAC);    

    SetMasterTXRX();
    

 
    
    return;
        
    // now setup the default connection  Master First
    pulseLEDColor( COLOR_WHITE, 100, 400);
    setBTConnection(slaveMAC, true);    
    
   
    // now setup the default connection  Slave Second
    pulseLEDColor( COLOR_WHITE, 100, 4000);
    SetSlaveTXRX();
    setBTConnection(masterMAC, false);    

    // test end/end communications.
    pulseLEDColor( COLOR_WHITE, 100, 1000);
    SetSlaveTXMasterRx();
        
    while(1) {
        sendBTString("HUGS\n");
        if (receiveBTBuffer(RX_Buffer, 8, 100) == 5){
            pulseLEDColor( COLOR_MAGENTA, 250, 100);
            if (strstr((char *)RX_Buffer, "HUGS\n"))
                pulseLEDColor( COLOR_GREEN, 250, 100);
            else
                pulseLEDColor( COLOR_RED, 250, 100);
        } else {
            pulseLEDColor( COLOR_RED, 750, 250);
        }
    }
}

bool    getBTAddress(uint8_t * MAC) {

    // Force the baud rate back to 9600 (just in case it is 38400)
    pulseLEDColor( COLOR_CYAN, 100, 400);
    setBlutoothBaud(38400);
    sendBTString("AT");
    sleep(100);
    sendBTString("AT+BAUD0");
    sleep(500);
    sendBTString("AT+RESET");  // expect OK+RESET
    sleep(500);
    setBlutoothBaud(9600);
    
    pulseLEDColor( COLOR_MAGENTA, 100, 200);
    sendBTString("AT");
    charsRead = receiveBTBuffer(RX_Buffer, 2, 5000);
    // blinkLEDColor(COLOR_BLUE, charsRead);
    pulseLEDColor((strstr(RX_Buffer, "OK") != NULL) ? COLOR_GREEN : COLOR_YELLOW, 400, 100);
    
    // get the MAC address  Expect reply:   OK+ADDR:xxxxxxxxxxxx
    sendBTString("AT+ADDR?");
    charsRead = receiveBTBuffer(RX_Buffer, 20, 5000);
    pulseLEDColor((charsRead == 20) ? COLOR_GREEN : COLOR_YELLOW, 400, 100);
    if (charsRead == 20) {
        memcpy(MAC, RX_Buffer + 8, 12);
        return true;
    } else {
        return false;
    }
}

void    setBTConnection(uint8_t * MAC, bool isMaster){
    flushBTRXbuffer();
    sendBTString("AT");
    charsRead = receiveBTBuffer(RX_Buffer, 4, 100);
    
    sendBTString(isMaster ? "AT+ROLE1" : "AT+ROLE0"); // expect OK+Get:0 or 1
    charsRead = receiveBTBuffer(RX_Buffer, 10, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 400, 100);
    
    sendBTString("AT+CON");
    sendBTBuffer(MAC, MAC_LENGTH, true);
    charsRead = receiveBTBuffer(RX_Buffer, 9, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 400,100);
    
    // Set the baud rate to 38400
    sendBTString("AT+BAUD2"); // expect OK+Get:0
    charsRead = receiveBTBuffer(RX_Buffer, 10, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 400, 100);
    
    sendBTString("AT+RESET");  // expect OK+RESET
    charsRead = receiveBTBuffer(RX_Buffer, 10, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 400, 100);
    
}    
