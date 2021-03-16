#ifndef COMM_H
#define	COMM_H

#include <xc.h> 
#include <string.h>
#include "common.h"
#include "commConstants.h"
#include "HDLC.h"

#define PRINT(x) sendBufBE((unsigned char *)x, sizeof(x), TLM_TEXT)
//#define PRINT(x) RS422_SendBytes((unsigned char *)x, sizeof(x))

#define COMM_ERR_LEN (8)
#define TLM_PERIOD_TAB_LEN (3)

union commError_t {
    unsigned char all[COMM_ERR_LEN];
    struct {
        unsigned char txBuffOvf;
        unsigned char rxBuffOvf;
        unsigned char frameBufOvf;
        unsigned char msgBufOvf;
        unsigned char byteStuff;
        unsigned char oversizeFrame;
        unsigned char framing;
        unsigned char unkOpcode;
    };
} commErrors;

union TlmPeriodTab_t {
    unsigned char all[TLM_PERIOD_TAB_LEN];
    struct{
        unsigned char SystStat;
        unsigned char SystErr;
        unsigned char CommErr;
    };
} TlmPeriodTab, TlmElapsed;

const union TlmPeriodTab_t defaultTlmPeriods = {50, 100, 100};

void commInit(void);
void sendTlm(void);
void sendCommErrorTlm(void);
void sendSystStatTlm(void);
void sendSystErrorTlm(void);
void sendSUSEVR(enum StartupSeqLocation loc);
void sendSwVerEVR(void);

#endif

