#include "mcc_generated_files/mcc.h"

void    SetSlaveTXRX(void){
    RC5PPS   = 0x0F;   //RC5->EUSART1:TX1;    
    RX1DTPPS = 0x14;   //RC4->EUSART1:RX1;   
}

void    SetMasterTXRX(void){
    RC3PPS   = 0x0F;   //RC3->EUSART1:TX1;    
    RX1DTPPS = 0x16;   //RC6->EUSART1:RX1;    
}

void    SetSlaveTXMasterRx(void){
    RC5PPS   = 0x0F;   //RC5->EUSART1:TX1;    
    RX1DTPPS = 0x16;   //RC6->EUSART1:RX1;    
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
}


