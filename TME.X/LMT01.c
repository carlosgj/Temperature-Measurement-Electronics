#include "LMT01.h"

void initSensors(void) {
    //TRIS
    //Bank A
    TRISBbits.TRISB1 = INPUT; //A1
    TRISAbits.TRISA3 = INPUT; //A2
    TRISBbits.TRISB2 = INPUT; //A3
    TRISAbits.TRISA2 = INPUT; //A4
    TRISBbits.TRISB4 = INPUT; //A5
    TRISAbits.TRISA1 = INPUT; //A6
    TRISBbits.TRISB5 = INPUT; //A7
    TRISAbits.TRISA0 = INPUT; //A8

    //LAT
    //Bank A
    LATBbits.LATB1 = TRUE; //A1
    LATAbits.LATA3 = TRUE; //A2
    LATBbits.LATB2 = TRUE; //A3
    LATAbits.LATA2 = TRUE; //A4
    LATBbits.LATB4 = TRUE; //A5
    LATAbits.LATA1 = TRUE; //A6
    LATBbits.LATB5 = TRUE; //A7
    LATAbits.LATA0 = TRUE; //A8

    //Counters
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
                default:
                    //TODO: error handling
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
            //TODO
            sensorState = READOUT;
            lastTransitionTime = msCount;
            break;

        case READOUT:
            if((msCount-lastTransitionTime) < READOUT_TIME){
                return;
            }
            //Collect new values from counters
            //TODO
            
            sensorState = IDLE;
            lastTransitionTime = msCount;
            break;
    }
}
