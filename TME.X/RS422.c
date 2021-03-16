#include <xc.h>
#include "RS422.h"

void RS422_Init(void){
    
    //Setup UART pins
    ANSELBbits.ANSELB0 = FALSE;
    TRISBbits.TRISB0 = OUTPUT;
    ANSELCbits.ANSELC7 = FALSE;
    TRISCbits.TRISC7 = INPUT;
    RX1PPS = 0b00010111; //RC7
    RB0PPS = 0x09;
    
    //Setup BRG
    BAUD1CONbits.BRG16 = TRUE;
    TX1STAbits.BRGH = TRUE;
    SP1BRGH = 0;
    SP1BRGL = 138;
    
    //Setup transmitter
    TX1STAbits.TXEN = TRUE;

    //Setup receiver
    RC1STAbits.CREN = TRUE;

    //Setup interrupts
#ifndef UNBUFFERED_SER

#endif
    
    //Turn on port
    RC1STAbits.SPEN = TRUE;
}

void RS422_TxByte(unsigned char theByte){
#ifdef UNBUFFERED_SER
    TX1REG = theByte;
    while(!TX1STAbits.TRMT){
        
    }
#else
    if(TXBUF_FREE > 0){
        //There's room in the buffer
        txbuf[txbufwrite++] = theByte;
    }
    else{
        commErrors.txBuffOvf++;
    }
#endif
}

inline void RS422_StartTx(void){
#ifndef UNBUFFERED_SER
    //Check if we're already transmitting
    if(PIE3bits.TX1IE){
        return;
    }
    
    //Check if there's data in the buffer
    if(txbufread == txbufwrite){
        return;
    }
    
    TXREG = txbuf[txbufread++];
    PIE3bits.TX1IE = TRUE;
    
#endif
}

inline void RS422TXISR(void){
#ifndef UNBUFFERED_SER
    //If there's no more data in the buffer, disable interrupt
    if(txbufread == txbufwrite){
        PIE3bits.TX1IE = FALSE;
        return;
    }
    //Transmit the next character
    TXREG = txbuf[txbufread++];

#endif
}

inline void RS422RXISR(void){
    if(RXBUF_FREE > 0){
        //Transfer into buffer
        rxbuf[rxbufwrite++] = RCREG;
    }
    else{
        volatile unsigned char foo = RCREG; //Throw byte away
        commErrors.rxBuffOvf++;
    }
    if(RCSTAbits.FERR || RCSTAbits.OERR){
        RCSTAbits.CREN = FALSE;
        RCSTAbits.CREN = TRUE;
        //TODO
    }
    
}