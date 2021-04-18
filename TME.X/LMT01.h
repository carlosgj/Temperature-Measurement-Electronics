#ifndef LMT01_H
#define	LMT01_H

#include <xc.h> 
#include "common.h"

#define LMT01_ACQ_DELAY (100)

#define MIN_CONV_TIME (40) //46 ms per datasheet, plus margin
#define CONV_TIME (MIN_CONV_TIME/2) //Time to wait after applying power, before enabling counters

#define MAX_READOUT_TIME (55) //(1/82 kHz)*4096, plus margin
#define READOUT_TIME (MAX_READOUT_TIME+(MIN_CONV_TIME-CONV_TIME)) //Time to wait after enabling counters to collect all pulses

enum SensorState_t{
    IDLE,
    CONV,
    READOUT,
};

enum SensorState_t sensorState = IDLE;

unsigned char sensorIndex = 0;
unsigned int lastTransitionTime = 0;

union TempReadings_t{
    unsigned int readings[8];
    unsigned char bytes[16];
} bankA, bankB, bankC;

void initSensors(void);
void measureSensors(void);

#endif	/* XC_HEADER_TEMPLATE_H */

