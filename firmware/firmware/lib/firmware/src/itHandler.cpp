#include <Arduino.h>

#include "itHandler.h"

static inline bool byteFromUartAvailable(void);
static inline ItError_t readByteFromUart(char* const data);
static inline ItError_t writeByteToUart(const unsigned char data);
static inline void sendBufferToUart(void);

static char itInputBuffer[30];
static unsigned char outBuffer[1024];
static unsigned short outBufferIndex = 0;

void itHandlerInit(GetTimestamp_t getTimeStamp, ItSignal_t* itSignals, const unsigned char ItSignalCount){
	ItCallbacks_t itCallbacks;
	itCallbacks.byteFromClientAvailable = byteFromUartAvailable;
	itCallbacks.readByteFromClient = readByteFromUart;
	itCallbacks.writeByteToClient = writeByteToUart;
	itCallbacks.getTimestamp = getTimeStamp;
	ItParameters_t itParameters;
	itParameters.itInputBuffer = itInputBuffer;
	itParameters.itInputBufferSize = sizeof(itInputBuffer)/sizeof(itInputBuffer[0]);
	itParameters.itSignals = itSignals;
	itParameters.itSignalCount = ItSignalCount;
	itInit(&itParameters, &itCallbacks);
}

void itHandlerTick(void){
	itTick();
	sendBufferToUart();
}

static inline bool byteFromUartAvailable(void){
	return Serial.available() > 0;
}

static inline ItError_t readByteFromUart(char* const data){
	int incomingByte = Serial.read();
	if(incomingByte == -1){
		return ItError_NoDataAvailable;
	}else{
		*data = (char)incomingByte;
	}
	return ItError_NoError;
}

static inline ItError_t writeByteToUart(const unsigned char data){
	if(outBufferIndex >= sizeof(outBuffer)){
		sendBufferToUart();
		if(outBufferIndex >= sizeof(outBuffer)){
			return ItError_ClientWriteError;
		}
	}

	outBuffer[outBufferIndex++] = data;
	return ItError_NoError;
}

static inline void sendBufferToUart(void){
	outBufferIndex -= Serial.write(outBuffer, outBufferIndex);
}