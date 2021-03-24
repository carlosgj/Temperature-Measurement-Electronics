#include <xc.h>
#include "HDLC.h"

void HDLCInit(void){

}

void sendBufLE(unsigned char *buf, unsigned int count, enum TlmType commandByte){
    unsigned int i = 0;
    
    //Write start byte
    RS422_TxByte(HDLC_START);
    
    //Write command byte
    if(IS_CONTROL(commandByte)){
        RS422_TxByte(HDLC_ESCAPE);
        RS422_TxByte((unsigned char)(commandByte ^ 0b00100000));
    }
    else{
        RS422_TxByte(commandByte);
    }
    
    //Perform byte-stuffing & write message
    for(i=0; i<count; i++){
        if( IS_CONTROL(buf[i]) ){
            RS422_TxByte(HDLC_ESCAPE);
            RS422_TxByte((unsigned char)(buf[i] ^ 0b00100000));
        }
        else{
            RS422_TxByte(buf[i]);
        }
    }

    RS422_TxByte(HDLC_STOP);
    RS422_StartTx();
}

void sendBufBE(unsigned char *buf, unsigned int count, enum TlmType commandByte){
    unsigned int i = 0;
    
    //Write start byte
    RS422_TxByte(HDLC_START);
    
    //Write command byte
    if(IS_CONTROL(commandByte)){
        RS422_TxByte(HDLC_ESCAPE);
        RS422_TxByte((unsigned char)(commandByte ^ 0b00100000));
    }
    else{
        RS422_TxByte(commandByte);
    }
    
    //Perform byte-stuffing & write message
    for(i=count; i>0; i--){
        if( IS_CONTROL(buf[i-1]) ){
            RS422_TxByte(HDLC_ESCAPE);
            RS422_TxByte((unsigned char)(buf[i-1] ^ 0b00100000));
        }
        else{
            RS422_TxByte(buf[i-1]);
        }
    }

    RS422_TxByte(HDLC_STOP);
    RS422_StartTx();
}

void implementRx(void){
    static unsigned char escapeNextByte = FALSE;
    static unsigned char isInMessage = FALSE;
    //__delay_ms(5);
    
    //Check if there are unprocessed bytes in rx queue
    unsigned char used = (unsigned char)(rxbufwrite - rxbufread);
    
    //Check HWM
    if(used > rxHighWaterMark){
        rxHighWaterMark = used;
    }
    
    for(; used > 0; used--){
        unsigned char newByte = rxbuf[rxbufread++];
        
        //Check if escape byte
        if(newByte == HDLC_ESCAPE){
            escapeNextByte = TRUE;
            continue;
        }

        //Not an escape byte
        if(escapeNextByte){
            if((newByte & 0b00100000) != 0){
                //We got an escape byte and then a byte without bit 5 clear
                //Abort reception
                isInMessage = FALSE;
                escapeNextByte = FALSE;
                framePtr = 0;
                commErrors.byteStuff++;
                continue;
            }
            else{
                frameBuf[framePtr++] = (unsigned char)(newByte ^ 0b00100000);
                escapeNextByte = FALSE;
            }
        }
        else{
            frameBuf[framePtr++] = newByte;
        }
        
        
        if(newByte == HDLC_STOP){
            if(isInMessage){
                //End of message.
                isInMessage = FALSE;
                validateFrame(framePtr);
                framePtr = 0;
                continue;
            }
            else{
                //We got a stop byte when a message wasn't in progress
                isInMessage = FALSE;
                framePtr = 0;
                commErrors.framing++;
            }
        }
        
        if(newByte == HDLC_START){
            if(isInMessage){
                //We got a start byte while a message was in progress
                isInMessage = FALSE;
                framePtr = 0;
                commErrors.framing++;
            }
            else{
                isInMessage = TRUE;
            }
        }
        
        if(framePtr == FRAMEBUF_SIZE){
            //Buffer overrun
            framePtr = 0;
            isInMessage = FALSE;
            commErrors.oversizeFrame++;
        }
    }
}

void validateFrame(unsigned char len){
    //A frame must have a start byte, a message type, and an end byte
    if(len < 3){
        return;
    }
    
    if(frameBuf[0] != HDLC_START){
        return;
    }
    
    if(frameBuf[(unsigned char)(len-1)] != HDLC_STOP){
        return;
    }
    
    //Copy everything but the boundary bytes
    memcpy(&messageBuf[msgRxPtr], frameBuf+1, (unsigned char)(len-2));
    messageBuf[msgRxPtr++].payloadLen = (unsigned char)(len-3);
    if(msgRxPtr == RX_MSG_QUEUE){
        msgRxPtr = 0;
    }
}