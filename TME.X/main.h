#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "config.h"
#include "common.h"
#include "UART.h"
#include "HDLC.h"
#include "comm.h"
#include "time.h"
#include "LMT01.h"

#define MAIN_LOOP_PER (10)

unsigned int lastMainRun = 0;

void init(void);
void run(void);
void processCommand(void);
void getResetCause(void);
#endif