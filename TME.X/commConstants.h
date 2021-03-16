#ifndef COMMCONSTANTS_H
#define	COMMCONSTANTS_H

#include <xc.h> 
#include "common.h"

enum CmdType{
    CMD_NOOP =      128,
};

enum TlmType{
    TLM_ACK =       0,
    TLM_NAK =       1,
    TLM_SYSTSTAT =  2,
    TLM_EVR =       3,
    TLM_SYSTERR =   4,
    TLM_COMMERR =   5,
};

enum EVRID {
    EVR_RESERVED =      0x00,
    EVR_STARTSEQ =      0x01,
    EVR_SWVER =         0x02,
};

enum StartupSeqLocation {
    SUS_INITIAL =       0x00,
    SUS_INITDONE =      0x07,
};

#endif

