#include "main.h"

void main(void) {
    init();
    while(TRUE){
        run();
    }
}

void init(void){
    INTDIS;
    //Setup pins
    
    //Setup oscillator
    
    INTCON0bits.IPEN = TRUE;
    INTCON0bits.GIEL = TRUE;
#ifdef LOOPOUT
    TRISAbits.TRISA4 = OUTPUT;
#endif
    INTEN;
    
    //All pins to digital
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    
    memset(systErr.all, 0, SYST_ERR_LEN); //Initialize system error counters
    
    timerInit();
    __delay_ms(400);
    
    RS422_Init();
    HDLCInit();
    commInit();
    sendSUSEVR(SUS_INITIAL);
    sendSwVerEVR();
    initSensors();
    __delay_ms(10);
    sendSUSEVR(SUS_INITDONE);
}

void run(void){
    implementRx();
    processCommand();
    measureSensors();
    sendTlm();
}

void processCommand(void){
    
}