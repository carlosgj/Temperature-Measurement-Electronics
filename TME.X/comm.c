#include <xc.h>
#include "comm.h"

void commInit(void){
    memset(commErrors.all, 0, COMM_ERR_LEN);
    
    memcpy(TlmPeriodTab.all, defaultTlmPeriods.all, TLM_PERIOD_TAB_LEN);
    
    //Setup tlm times
    //memcpy(TlmElapsed.all, TlmPeriodTab.all, TLM_PERIOD_TAB_LEN);
    TlmElapsed.CommErr = 0;
    TlmElapsed.SystErr = 40;
    TlmElapsed.SystStat = 80;
}

void sendTlm(){
    if(TlmElapsed.SystStat == 0){
        sendSystStatTlm();
        TlmElapsed.SystStat = TlmPeriodTab.SystStat;
    }
    else{
        TlmElapsed.SystStat--;
    }
    
    if(TlmElapsed.SystErr == 0){
        sendSystErrorTlm();
        TlmElapsed.SystErr = TlmPeriodTab.SystErr;
    }
    else{
        TlmElapsed.SystErr--;
    }
    
    if(TlmElapsed.CommErr == 0){
        sendCommErrorTlm();
        TlmElapsed.CommErr = TlmPeriodTab.CommErr;
    }
    else{
        TlmElapsed.CommErr--;
    }
}

void sendSUSEVR(enum StartupSeqLocation loc){
    unsigned char buf[2];
    buf[0] = EVR_STARTSEQ;
    buf[1] = loc;
    sendBufLE(buf, 2, TLM_EVR);
}

void sendSwVerEVR(void){
    unsigned char buf[sizeof(SW_VER_STR)+1];
    const unsigned char *ver = SW_VER_STR;
    buf[0] = EVR_SWVER;
    memcpy(buf+1, ver, sizeof(SW_VER_STR));
    sendBufLE(buf, sizeof(SW_VER_STR)+1, TLM_EVR);
}

void sendCommErrorTlm(void){
#ifdef TLM_TEST_PATTERN
    commErrors.txBuffOvf = 1;
    commErrors.rxBuffOvf = 2;
    commErrors.frameBufOvf = 3;
    commErrors.msgBufOvf = 4;
    commErrors.byteStuff = 5;
    commErrors.oversizeFrame = 6;
    commErrors.framing = 7;
    commErrors.unkOpcode = 8;
    commErrors.rxBuffHWM = 0x55;
    commErrors.txBuffHWM = 0xAA;
#endif
    sendBufBE(commErrors.all, COMM_ERR_LEN, TLM_COMMERR);
}

void sendSystStatTlm(void){
#ifdef TLM_TEST_PATTERN
    systStat.inputVoltage = 0x0201;
    systStat.TMC429Stat = 1;
    systStat.motEn = 1;
#endif
    sendBufBE(systStat.all, SYST_STAT_LEN, TLM_SYSTSTAT);
}

void sendSystErrorTlm(void){
#ifdef TLM_TEST_PATTERN
    systErr.unhandledInt = 0x0f;
#endif
    sendBufBE(systErr.all, SYST_ERR_LEN, TLM_SYSTERR);
}