#ifndef TIME_H
#define	TIME_H

#include <xc.h> 
#include "common.h"

#define MS_TMR_VAL (5)

void timerInit(void);
inline void TMR0ISR(void);

#endif


