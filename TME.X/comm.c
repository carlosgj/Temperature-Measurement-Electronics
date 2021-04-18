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
    TlmElapsed.TempA = 120;
    TlmElapsed.TempB = 160;
    TlmElapsed.TempC = 200;
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
    
    if(TlmElapsed.TempA == 0){
        sendTempAReadings();
        TlmElapsed.TempA = TlmPeriodTab.TempA;
    }
    else{
        TlmElapsed.TempA--;
    }
    
    if(TlmElapsed.TempB == 0){
        sendTempBReadings();
        TlmElapsed.TempB = TlmPeriodTab.TempB;
    }
    else{
        TlmElapsed.TempB--;
    }
    
    if(TlmElapsed.TempC == 0){
        sendTempCReadings();
        TlmElapsed.TempC = TlmPeriodTab.TempC;
    }
    else{
        TlmElapsed.TempC--;
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

void sendTempAReadings(void){
#ifdef TLM_TEST_PATTERN
    bankA.readings[0] = 0x0A01;
    bankA.readings[1] = 0x0A02;
    bankA.readings[2] = 0x0A03;
    bankA.readings[3] = 0x0A04;
    bankA.readings[4] = 0x0A05;
    bankA.readings[5] = 0x0A06;
    bankA.readings[6] = 0x0A07;
    bankA.readings[7] = 0x0A08;
#endif
    sendBufLE(bankA.bytes, 16, TLM_TEMPA);
}

void sendTempBReadings(void){
#ifdef TLM_TEST_PATTERN
    bankB.readings[0] = 0x0B01;
    bankB.readings[1] = 0x0B02;
    bankB.readings[2] = 0x0B03;
    bankB.readings[3] = 0x0B04;
    bankB.readings[4] = 0x0B05;
    bankB.readings[5] = 0x0B06;
    bankB.readings[6] = 0x0B07;
    bankB.readings[7] = 0x0B08;
#endif
    sendBufLE(bankB.bytes, 16, TLM_TEMPB);

}

void sendTempCReadings(void){
#ifdef TLM_TEST_PATTERN
    bankC.readings[0] = 0x0C01;
    bankC.readings[1] = 0x0C02;
    bankC.readings[2] = 0x0C03;
    bankC.readings[3] = 0x0C04;
    bankC.readings[4] = 0x0C05;
    bankC.readings[5] = 0x0C06;
    bankC.readings[6] = 0x0C07;
    bankC.readings[7] = 0x0C08;
#endif
    sendBufLE(bankC.bytes, 16, TLM_TEMPC);

}