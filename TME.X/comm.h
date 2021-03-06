#ifndef COMM_H
#define	COMM_H

#include <xc.h> 
#include <string.h>
#include "common.h"
#include "commConstants.h"
#include "HDLC.h"
#include "LMT01.h"

#define PRINT(x) sendBufBE((unsigned char *)x, sizeof(x), TLM_TEXT)
//#define PRINT(x) RS422_SendBytes((unsigned char *)x, sizeof(x))


#define TLM_PERIOD_TAB_LEN (6)

#define COMM_ERR_LEN (10)
union commError_t {
    unsigned char all[COMM_ERR_LEN];
    struct {
        unsigned char txBuffHWM;
        unsigned char rxBuffHWM;
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
        unsigned char TempA;
        unsigned char TempB;
        unsigned char TempC;
    };
} TlmPeriodTab, TlmElapsed;

const union TlmPeriodTab_t defaultTlmPeriods = {100, 100, 100, 100, 100, 100};

void commInit(void);
void sendTlm(void);
void sendCommErrorTlm(void);
void sendSystStatTlm(void);
void sendSystErrorTlm(void);
void sendSUSEVR(enum StartupSeqLocation loc);
void sendSwVerEVR(void);
void sendTempAReadings(void);
void sendTempBReadings(void);
void sendTempCReadings(void);
#endif

