#ifndef LMT01_H
#define	LMT01_H

#include <xc.h> 
#include "common.h"

#define LMT01_ACQ_DELAY (100)

unsigned char sensorIndex = 0;
unsigned int sensorStartTime = 0;

void initSensors(void);
void measureSensors(void);

unsigned int bankA[8];
unsigned int bankB[8];
unsigned int bankC[8];

#endif	/* XC_HEADER_TEMPLATE_H */

