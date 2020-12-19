//board: Arduino Leonardo
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#include "src/IT_Server/IT_Server/it.h"
#include "quadratureEncoder.h"
#include "motorCurrent.h"


// static inline void handleInput(void);
// static inline bool charArrayBeginsWith(const char* array, const char* begin);
// static inline void setLedOn(void);
// static inline void setLedOff(void);
// static inline void setMotorA(float value);
// static inline void setMotorB(float value);
// static inline void disengageBreakOnPin(int pin);
// static inline void setDirectionOnPin(int pin, Direction_t direction);
// static inline void setPwmOnPin(int pin, float ratio);
// static inline void initLed(void);
// static inline void appendToInputBuffer(char ch);
static inline unsigned long getSampleTimeMicros(void);
static inline void setSampleTimeMicros(unsigned long);
static inline void timerSetup(unsigned long sampleTime_us);
static inline unsigned long getTickMicros(void);

static inline bool byteFromUartAvailable(void);
static inline ItError_t readByteFromUart(char* const data);
static inline ItError_t writeByteToUart(const unsigned char data);
static inline void sendBufferToUart(void);

static inline void setBuiltinLedOn(void);
static inline void setBuiltinLedOff(void);

// typedef enum {
// 	Forward,
// 	Backward
// } Direction_t;

static ItSignal_t itSignals[] = {
	{
		"enc1",
		ItValueType_Ulong,
		(void (*)(void)) quadratureEncoder_getCounts_1,
		NULL,
	},
	{
		"enc2",
		ItValueType_Ulong,
		(void (*)(void)) quadratureEncoder_getCounts_2,
		NULL,
	},
	{
		"current1",
		ItValueType_Float,
		(void (*)(void)) getMotorCurrent_1,
		NULL,
	},
	{
		"current2",
		ItValueType_Float,
		(void (*)(void)) getMotorCurrent_2,
		NULL,
	},
	{
		"sampleTime_us",
		ItValueType_Ulong,
		(void (*)(void)) getSampleTimeMicros,
		(void (*)(void)) setSampleTimeMicros,
	},
};

static unsigned char outBuffer[1024];
static unsigned short outBufferIndex = 0;

static const unsigned char ItSignalCount = sizeof(itSignals) / sizeof(itSignals[0]);
static char itInputBuffer[30];

static bool timerEvent = false;
static unsigned long sampleTimeMicros;
static unsigned long tickMicros = 0;

static const unsigned char OnBoardLedPin = 13;

// static const char Terminator = '\r';
// static char inputBuffer[100];
// static unsigned char inputBufferIndex;



void appSetup(void) {
	ItCallbacks_t itCallbacks;
	itCallbacks.byteFromClientAvailable = byteFromUartAvailable;
	itCallbacks.readByteFromClient = readByteFromUart;
	itCallbacks.writeByteToClient = writeByteToUart;
	itCallbacks.getTimestamp = getTickMicros;
	ItParameters_t itParameters;
	itParameters.itInputBuffer = itInputBuffer;
	itParameters.itInputBufferSize = sizeof(itInputBuffer)/sizeof(itInputBuffer[0]);
	itParameters.itSignals = itSignals;
	itParameters.itSignalCount = ItSignalCount;
	itInit(&itParameters, &itCallbacks);

	sampleTimeMicros = 1e5;
	timerSetup(sampleTimeMicros);

	//inputBufferIndex = 0;
	quadratureEncoder_init();
}

void appLoop(void) {
	if(!timerEvent){
		setBuiltinLedOff();
		return;
	}
	setBuiltinLedOn();
	timerEvent=false;
	itTick();
	sendBufferToUart();
	
	/*int incomingInt = Serial.read();
	char incomingChar = (char)incomingInt;
	if(incomingChar == Terminator) {
		appendToInputBuffer('\0');
		handleInput();
	}
	else if(incomingInt != -1) {
		appendToInputBuffer(incomingChar);
	}*/
}

static inline unsigned long getTickMicros(void){
	return tickMicros;
}

static inline void initBuiltinLed(void){
	pinMode(OnBoardLedPin, OUTPUT);
}

static inline void setBuiltinLedOn(void){
	initBuiltinLed();
	digitalWrite(OnBoardLedPin, HIGH);
}

static inline void setBuiltinLedOff(void){
	initBuiltinLed();
	digitalWrite(OnBoardLedPin, LOW);
}

static inline unsigned long getSampleTimeMicros(void){
	return sampleTimeMicros;
}

static inline void setSampleTimeMicros(unsigned long sampleTime_us){
	sampleTimeMicros = sampleTime_us;
	timerSetup(sampleTimeMicros);
}

static inline void timerSetup(unsigned long periodMicros){
	const unsigned long CLOCK_FREQUENCY = 16e6;
	const unsigned long COUNTER_MAXIMUM = 65535;

	TCCR1A = 0; //for any reason, this must be done!!

	unsigned long counts = (CLOCK_FREQUENCY / 1e6) * periodMicros;
	if(counts <= COUNTER_MAXIMUM){
		TCCR1B = _BV(WGM12) | _BV(CS10); //match on value of OCR1A and divide clock by 1
		OCR1A = counts;
	}
	else if((counts / 8) <= COUNTER_MAXIMUM){
		TCCR1B = _BV(WGM12) | _BV(CS11); //match on value of OCR1A and divide clock by 8
		OCR1A = counts / 8;
	}
	else if((counts / 64) <= COUNTER_MAXIMUM){
		TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10); //match on value of OCR1A and divide clock by 64
		OCR1A = counts / 64;
	}
	else if((counts / 256) <= COUNTER_MAXIMUM){
		TCCR1B = _BV(WGM12) | _BV(CS12); //match on value of OCR1A and divide clock by 256
		OCR1A = counts / 256;
	}
	else if((counts / 1024) <= COUNTER_MAXIMUM){
		TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10); //match on value of OCR1A and divide clock by 1024
		OCR1A = counts / 1024;
	}
	else{ //maximum sampletime is 4.1942s
		TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10); //match on value of OCR1A and divide clock by 1024
		OCR1A = COUNTER_MAXIMUM;
    	}
  
	TIMSK1 = _BV(OCIE1A); //enable interrupt
}

ISR(TIMER1_COMPA_vect){
	tickMicros += sampleTimeMicros;
	timerEvent = true;
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

// static inline void handleInput(void) {
// 	if(strcmp(inputBuffer, "setLedOn") == 0){
// 		setLedOn();
// 	}
// 	else if(strcmp(inputBuffer, "setLedOff") == 0){
// 		setLedOff();
// 	}
// 	else if(charArrayBeginsWith(inputBuffer, "setMotorA ")){
// 		float ratio = atof(strstr(inputBuffer, " ") + 1);
// 		setMotorA(ratio);
// 	}
// 	else if(charArrayBeginsWith(inputBuffer, "setMotorB ")){
// 		float ratio = atof(strstr(inputBuffer, " ") + 1);
// 		setMotorB(ratio);
// 	}
// 	/*else if(strcmp(inputBuffer, "getAdc_A0") == 0){
// 		char numberString[4+sizeof('\0')];
// 		sprintf(numberString, "%d", analogRead(A0)); //TODO: sprintf hat einen Rückgabewert!
// 		Serial.write(numberString);
// 		Serial.write('\n');
// 	}*/
// 	inputBufferIndex = 0;
// }

//static inline bool charArrayBeginsWith(const char* array, const char* begin) {
//	return strstr(array, begin) == array;
//}

// static inline void setLedOn(void) {
// 	initLed();
// 	digitalWrite(LED_BUILTIN, HIGH);
// }

// static inline void setLedOff(void) {
// 	initLed();
// 	digitalWrite(LED_BUILTIN, LOW);
// }

// static inline void setMotorA(float value) {
// 	disengageBreakOnPin(9);
// 	const int DirectionPin = 12;
// 	if(value > 0) {
// 		setDirectionOnPin(DirectionPin, Forward);  
// 	}
// 	else {
// 		setDirectionOnPin(DirectionPin, Backward);
// 	}
// 	setPwmOnPin(3, abs(value));
// }

// static inline void setMotorB(float value) {
// 	disengageBreakOnPin(8);
// 	const int DirectionPin = 13;
// 	if(value > 0) {
// 		setDirectionOnPin(DirectionPin, Forward);  
// 	}
// 	else {
// 		setDirectionOnPin(DirectionPin, Backward);
// 	}
// 	setPwmOnPin(11, abs(value));
// }

// static inline void disengageBreakOnPin(int pin) {
// 	pinMode(pin, OUTPUT);
// 	digitalWrite(pin, LOW);
// }

// static inline void setDirectionOnPin(int pin, Direction_t direction) {
// 	pinMode(pin, OUTPUT);
// 	if(direction == Forward) {
// 		digitalWrite(pin, HIGH);
// 	}
// 	else {
// 		digitalWrite(pin, LOW);
// 	}
// }

// static inline void setPwmOnPin(int pin, float ratio) {
// 	pinMode(pin, OUTPUT);
// 	analogWrite(pin, int(ratio*255.0f));
// }

// static inline void initLed(void) {
// 	pinMode(LED_BUILTIN, OUTPUT);
// }

// static inline void appendToInputBuffer(char ch) {
// 	inputBuffer[inputBufferIndex] = ch;
// 	inputBufferIndex++;
// }
