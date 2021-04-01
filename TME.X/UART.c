#include <xc.h>
#include "UART.h"

void RS422_Init(void){
    
    //Setup UART pins
    ANSELBbits.ANSELB0 = FALSE;
    TRISBbits.TRISB0 = OUTPUT;
    ANSELCbits.ANSELC7 = FALSE;
    TRISCbits.TRISC7 = INPUT;
    U1RXPPS = 0b00010111; //RC7
    RB0PPS = 0x20;
    
    //Setup BRG
    U1CON0bits.BRGS = TRUE;
    U1BRGH = 0;
    U1BRGL = 138;
    
    //Setup transmitter
    U1CON0bits.TXEN = TRUE;

    //Setup receiver
    U1CON0bits.RXEN = TRUE;

    //Setup interrupts
#ifndef UNBUFFERED_SER

#endif
    
    //Turn on port
    U1CON1bits.ON = TRUE;
}

void RS422_TxByte(unsigned char theByte){
#ifdef UNBUFFERED_SER
    U1TXB = theByte;
    while(!U1ERRIRbits.TXMTIF){
        
    }
#else
    unsigned char used = (unsigned char)(txbufwrite - txbufread);
    unsigned char free = (unsigned char)((TX_BUF_SIZE-1)-used);
    if(free != 0){
        //There's room in the buffer
        txbuf[txbufwrite++] = theByte;
        
        //If the fullness of the buffer was equal to HWM *before* the increment,
        //now it's one greater
        if(used == txHighWaterMark){
            txHighWaterMark++;
        }
    }
    else{
        commErrors.txBuffOvf++;
    }
#endif
}

inline void RS422_StartTx(void){
#ifndef UNBUFFERED_SER
    //Check if we're already transmitting
    if(PIE4bits.U1TXIE){
        return;
    }
    
    //Check if there's data in the buffer
    if(txbufread == txbufwrite){
        return;
    }
    
    U1TXB = txbuf[txbufread++];
    PIE4bits.U1TXIE = TRUE;
    
#endif
}

void __interrupt(irq(U1TX),high_priority) RS422TXISR(void){
#ifndef UNBUFFERED_SER
    //If there's no more data in the buffer, disable interrupt
    if(txbufread == txbufwrite){
        PIE4bits.U1TXIE = FALSE;
        return;
    }
    //Transmit the next character
    U1TXB = txbuf[txbufread++];

#endif
}

void __interrupt(irq(U1RX),high_priority) RS422RXISR(void){
    if(RXBUF_FREE > 0){
        //Transfer into buffer
        rxbuf[rxbufwrite++] = U1RXB;
    }
    //So as not to slow down the interrupt, HWM checking is done in implementRx
    else{
        volatile unsigned char foo = U1RXB; //Throw byte away
        commErrors.rxBuffOvf++;
    }
//    if(RCSTAbits.FERR || RCSTAbits.OERR){
//        U1CON0bits.RXEN = FALSE;
//        U1CON0bits.RXEN = TRUE;
//        //TODO
//    }
    
}