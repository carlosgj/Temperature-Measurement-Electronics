#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "common.h"
#include "RS422.h"
#include "HDLC.h"
#include "comm.h"
#include "time.h"
#include "LMT01.h"

void init(void);
void run(void);
void processCommand(void);
#endif