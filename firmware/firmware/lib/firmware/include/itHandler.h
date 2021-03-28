#ifndef IT_HANDLER_H
#define IT_HANDLER_H

extern "C"{
	#include "it.h"
	#include "itCommand.h"
}

void itHandlerInit(GetTimestamp_t getTimeStamp, ItSignal_t* itSignals, const unsigned char ItSignalCount);
void itHandlerTick(void);

#endif