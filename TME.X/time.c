#include <xc.h>
#include "time.h"

void timerInit(void){
    
#ifdef MSOUT
    TRISAbits.TRISA4 = OUTPUT;
    LATAbits.LATA4 = FALSE;
#endif
    
}

//Implement ms timer
inline void TMR0ISR(void){
    msCount++;
#ifdef MSOUT
    LATAbits.LATA4 = !LATAbits.LATA4;
#endif
}