#include "LMT01.h"

void initSensors(void) {
    //Bank A
    TRISBbits.TRISB1 = INPUT; //A1
    TRISAbits.TRISA3 = INPUT; //A2
    TRISBbits.TRISB2 = INPUT; //A3
    TRISAbits.TRISA2 = INPUT; //A4
    TRISBbits.TRISB4 = INPUT; //A5
    TRISAbits.TRISA1 = INPUT; //A6
    TRISBbits.TRISB5 = INPUT; //A7
    TRISAbits.TRISA0 = INPUT; //A8
    LATBbits.LATB1 = TRUE; //A1
    LATAbits.LATA3 = TRUE; //A2
    LATBbits.LATB2 = TRUE; //A3
    LATAbits.LATA2 = TRUE; //A4
    LATBbits.LATB4 = TRUE; //A5
    LATAbits.LATA1 = TRUE; //A6
    LATBbits.LATB5 = TRUE; //A7
    LATAbits.LATA0 = TRUE; //A8
    
    //Bank B
    TRISDbits.TRISD4 = INPUT; //B1
    TRISAbits.TRISA6 = INPUT; //B2
    TRISDbits.TRISD5 = INPUT; //B3
    TRISAbits.TRISA7 = INPUT; //B4
    TRISDbits.TRISD6 = INPUT; //B5
    TRISDbits.TRISD7 = INPUT; //B6
    TRISEbits.TRISE2 = INPUT; //B7
    TRISEbits.TRISE1 = INPUT; //B8
    LATDbits.LATD4 = TRUE; //B1
    LATAbits.LATA6 = TRUE; //B2
    LATDbits.LATD5 = TRUE; //B3
    LATAbits.LATA7 = TRUE; //B4
    LATDbits.LATD6 = TRUE; //B5
    LATDbits.LATD7 = TRUE; //B6
    LATEbits.LATE2 = TRUE; //B7
    LATEbits.LATE1 = TRUE; //B8

    //Common A
    //TMR3 for bank A
    TRISBbits.TRISB3 = INPUT;
    WPUBbits.WPUB3 = TRUE;
    T3CKIPPS = 0b00001011;
    T3CONbits.NOT_SYNC = TRUE;
    T3CLKbits.CS = 0; //Source is T3CKIPPS
    
    //Common B
    //TMR1 for bank B
    //TODO
    
    //Common C
    //TMR5 for bank C
    //TODO
    
}

void measureSensors(void) {
    switch (sensorState) {
        case IDLE:
            //Setup sensors
            sensorIndex++;
            sensorIndex &= 7;
            //Setup power
            switch (sensorIndex) {
                case 0:
                    TRISBbits.TRISB1 = OUTPUT;
                    TRISDbits.TRISD4 = OUTPUT;
                    TRISDbits.TRISD2 = OUTPUT;
                    break;
                case 1:
                    TRISAbits.TRISA3 = OUTPUT;
                    TRISAbits.TRISA6 = OUTPUT;
                    TRISDbits.TRISD3 = OUTPUT;
                    break;
                case 2:
                    TRISBbits.TRISB2 = OUTPUT;
                    TRISDbits.TRISD5 = OUTPUT;
                    TRISCbits.TRISC4 = OUTPUT;
                    break;
                case 3:
                    TRISAbits.TRISA2 = OUTPUT;
                    TRISAbits.TRISA7 = OUTPUT;
                    TRISCbits.TRISC5 = OUTPUT;
                    break;
                case 4:
                    TRISBbits.TRISB4 = OUTPUT;
                    TRISDbits.TRISD6 = OUTPUT;
                    TRISDbits.TRISD1 = OUTPUT;
                    break;
                case 5:
                    TRISAbits.TRISA1 = OUTPUT;
                    TRISDbits.TRISD7 = OUTPUT;
                    TRISDbits.TRISD0 = OUTPUT;
                    break;
                case 6:
                    TRISBbits.TRISB5 = OUTPUT;
                    TRISEbits.TRISE2 = OUTPUT;
                    TRISCbits.TRISC3 = OUTPUT;
                    break;
                case 7:
                    TRISAbits.TRISA0 = OUTPUT;
                    TRISEbits.TRISE1 = OUTPUT;
                    TRISCbits.TRISC2 = OUTPUT;
                    break;
            }
            sensorState = CONV;
            lastTransitionTime = msCount;
            break;
            
        case CONV:
            //If CONV_TIME hasn't elapsed yet, return
            if((msCount-lastTransitionTime) < CONV_TIME){
                return;
            }
            //If it has, setup counters
            TMR3L = 0;
            TMR3H = 0;
            T3CONbits.ON = TRUE;
            TMR1L = 0;
            TMR1H = 0;
            T1CONbits.ON = TRUE;
            TMR5L = 0;
            TMR5H = 0;
            T5CONbits.ON = TRUE;
            
            sensorState = READOUT;
            lastTransitionTime = msCount;
            break;

        case READOUT:
            if((msCount-lastTransitionTime) < READOUT_TIME){
                return;
            }
            
            //Shut down counters
            T3CONbits.ON = FALSE;
            T1CONbits.ON = FALSE;
            T5CONbits.ON = FALSE;

            //Collect new values from counters
            bankA.readings[sensorIndex] = (((unsigned int)(TMR3H))<<8)+TMR3L;
            bankB.readings[sensorIndex] = (((unsigned int)(TMR1H))<<8)+TMR1L;
            bankC.readings[sensorIndex] = (((unsigned int)(TMR5H))<<8)+TMR5L;
            
            
            
            //Turn off power
            switch (sensorIndex) {
                case 0:
                    TRISBbits.TRISB1 = INPUT;
                    TRISDbits.TRISD4 = INPUT;
                    TRISDbits.TRISD2 = INPUT;
                    break;
                case 1:
                    TRISAbits.TRISA3 = INPUT;
                    TRISAbits.TRISA6 = INPUT;
                    TRISDbits.TRISD3 = INPUT;
                    break;
                case 2:
                    TRISBbits.TRISB2 = INPUT;
                    TRISDbits.TRISD5 = INPUT;
                    TRISCbits.TRISC4 = INPUT;
                    break;
                case 3:
                    TRISAbits.TRISA2 = INPUT;
                    TRISAbits.TRISA7 = INPUT;
                    TRISCbits.TRISC5 = INPUT;
                    break;
                case 4:
                    TRISBbits.TRISB4 = INPUT;
                    TRISDbits.TRISD6 = INPUT;
                    TRISDbits.TRISD1 = INPUT;
                    break;
                case 5:
                    TRISAbits.TRISA1 = INPUT;
                    TRISDbits.TRISD7 = INPUT;
                    TRISDbits.TRISD0 = INPUT;
                    break;
                case 6:
                    TRISBbits.TRISB5 = INPUT;
                    TRISEbits.TRISE2 = INPUT;
                    TRISCbits.TRISC3 = INPUT;
                    break;
                case 7:
                    TRISAbits.TRISA0 = INPUT;
                    TRISEbits.TRISE1 = INPUT;
                    TRISCbits.TRISC2 = INPUT;
                    break;
            }
            
            sensorState = IDLE;
            lastTransitionTime = msCount;
            break;
    }
}
