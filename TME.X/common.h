#ifndef COMMON_H
#define	COMMON_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <pic18f47q84.h>

//######### Debug options ###########

//Uncomment to provide human-readable ASCII serial messages, instead of HDLC
//#define HUMAN_READABLE_SERIAL

//Uncomment AT MOST ONE of the following to send signals to the test point
//#define LOOPOUT
//#define MSOUT

//Uncomment to send one byte at a time to the UART, instead of using software buffering
//#define UNBUFFERED_SER

//Uncomment to send test patterns in packets instead of real telemetry
//#define TLM_TEST_PATTERN


#define TRUE 1
#define FALSE 0

#define INPUT 1
#define OUTPUT 0

#define _XTAL_FREQ (64000000)

#define SW_VER_STR "0.0.1"

#define INTEN (INTCON0bits.GIE = TRUE)
#define INTDIS (INTCON0bits.GIE = FALSE)

#define SYST_ERR_LEN (3)
union SystErr_t {
    unsigned char all[SYST_ERR_LEN];
    struct{
        unsigned char unhandledInt;
        unsigned char lastUnhandledInt;
        unsigned char loopOverruns;
    };
} systErr;

#define SYST_STAT_LEN (1)
union SystStatus_t{
    unsigned char all[SYST_STAT_LEN];
    struct{
        unsigned char seconds;
    };
} systStat;

enum ResetCause {
    RS_POR,
    RS_BOR,
    RS_MCLR,
    RS_WDT,
    RS_WIN,
    RS_RST,
    RS_SO,
    RS_SU,
    RS_DP,
    RS_VREG,
    RS_MEM,
    RS_INVALID
};

enum ResetCause resetCause = RS_INVALID;

unsigned int msCount = 0;

void ItoA(unsigned char val, char* dest);

#endif

