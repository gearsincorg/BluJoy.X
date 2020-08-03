#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "configure.h"
#include "serial.h"
#include "timers.h"
#include "ui.h"

uint8_t RX_Buffer[32];
uint8_t charsRead;
uint8_t slaveMAC[MAC_LENGTH];
uint8_t masterMAC[MAC_LENGTH];

void    SetSlaveTXRX(void){
    RC5PPS   = 0x0F;   //RC5->EUSART1:TX1;    
    RX1DTPPS = 0x14;   //RC4->EUSART1:RX1;   
    sleep(20);
}

void    SetMasterTXRX(void){
    RC3PPS   = 0x0F;   //RC3->EUSART1:TX1;    
    RX1DTPPS = 0x16;   //RC6->EUSART1:RX1;    
    sleep(20);
}

void    SetSlaveTXMasterRx(void){
    RC5PPS   = 0x0F;   //RC5->EUSART1:TX1;    
    RX1DTPPS = 0x16;   //RC6->EUSART1:RX1;    
    sleep(20);
}

void    turnPowerOn(){
    POWER_EN_SetLow();
}

void    turnPowerOff(){
    POWER_EN_SetHigh();
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
    sleep(20);
}

void    pairBluetoothDevices(void){

    // Get two Mac Addresses
    SetSlaveTXRX();
    getBTAddress(slaveMAC);    
            
    SetMasterTXRX();
    getBTAddress(masterMAC);    
    
    // now setup the default connection  Master First
    pulseLEDColor( COLOR_WHITE, 100, 1000);
    setBTConnection(slaveMAC, true);    
    
    // now setup the default connection  Slave Second
    pulseLEDColor( COLOR_WHITE, 100, 1000);
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
    setBlutoothBaud(9600);
    flushBTRXbuffer();
    
    for (int8_t i = 0; i < 2; i++) {
        flushBTRXbuffer();
        sendBTString("AT");
        charsRead = receiveBTBuffer(RX_Buffer, 4, 100);
    }
    pulseLEDColor((charsRead == 2) ? COLOR_GREEN : COLOR_RED, 800, 200);

    // Set the baud rate to 38400 and set the role
    sendBTString("AT+BAUD2"); // expect OK+Get:0
    charsRead = receiveBTBuffer(RX_Buffer, 10, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 800, 200);
    
    sendBTString("AT+RESET");  // expect OK+RESET
    charsRead = receiveBTBuffer(RX_Buffer, 10, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 800, 200);
    sleep(500);

    // Switch to the higher baud rate now we have done reset
    setBlutoothBaud(38400);
    sendBTString("AT");
    charsRead = receiveBTBuffer(RX_Buffer, 4, 100);
    flushBTRXbuffer();

    // get the MAC address  Expect reply:   OK+ADDR:xxxxxxxxxxxx
    sendBTString("AT+ADDR?");
    charsRead = receiveBTBuffer(RX_Buffer, 22, 100);
    pulseLEDColor((charsRead == 20) ? COLOR_BLUE : COLOR_YELLOW, 800, 200);
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
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 800, 200);
    
    sendBTString("AT+CON");
    sendBTBuffer(MAC, MAC_LENGTH, true);
    
    charsRead = receiveBTBuffer(RX_Buffer, 9, 100);
    pulseLEDColor((charsRead == 8) ? COLOR_GREEN : COLOR_RED, 800,200);
}    
