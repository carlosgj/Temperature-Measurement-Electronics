#ifndef HDLC_H
#define	HDLC_H

#include <xc.h>
#include <string.h>
#include "common.h"
#include "UART.h"
#include "commConstants.h"

#define HDLC_START      (0x7f)
#define HDLC_STOP       (0x7e)
#define HDLC_ESCAPE     (0x7d)
#define HDLC_TS_ARM     (0x7c)
#define HDLC_TS_ABORT   (0x7b)

#define MAX_RX_SIZE     (16)
#define RX_MSG_QUEUE    (6)
#define FRAMEBUF_SIZE   (MAX_RX_SIZE+2)

//Checks if a byte is a control byte
#define IS_CONTROL(x)   ((x & 0b11111000) == 0b01111000)

struct rx_message_t{
    enum CmdType type;
    unsigned char payload[MAX_RX_SIZE];
    unsigned char payloadLen;
};

struct rx_message_t messageBuf[RX_MSG_QUEUE];
unsigned char msgRxPtr = 0;
unsigned char msgProcessPtr = 0;

unsigned char frameBuf[FRAMEBUF_SIZE];
unsigned char framePtr = 0;

#define MSGBUF_WAITING ((unsigned char)(msgRxPtr-msgProcessPtr))

void HDLCInit(void);

void sendBufBE(unsigned char *buf, unsigned int count, enum TlmType commandByte);
void sendBufLE(unsigned char *buf, unsigned int count, enum TlmType commandByte);

//This function is responsible for taking bytes out of the UART Rx queue and 
// chunking them up into HDLC frames.
void implementRx(void);

//This function checks the contents of frameBuf to make sure it's a valid HDLC frame, 
// and if so, extracts it into a message
void validateFrame(unsigned char len);
#endif

