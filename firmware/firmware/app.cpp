//board: Arduino Leonardo
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#include "src/IT_Server/IT_Server/it.h"
#include "quadratureEncoder.h"
#include "motorPower.h"
#include "motorSpeed.h"
#include "speedController.h"



// static inline void handleInput(void);
// static inline bool charArrayBeginsWith(const char* array, const char* begin);
// static inline void setLedOn(void);
// static inline void setLedOff(void);
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

static inline uint8_t getExternalMode(void);
static inline void setExternalMode(uint8_t value);

static inline float getBothPwm(void);
static inline void setBothPwm(float value);

static inline float getSpeed_1(void);
static inline float getSpeed_2(void);

static inline void setDesiredSpeed(float value);




static bool timerEvent = false;
static unsigned long sampleTimeMicros;
static unsigned long tickMicros = 0;
static unsigned long oldTickMicros = 0;
static float deltaTime;

static const unsigned char OnBoardLedPin = 13;

static bool externalMode = false;

static float bothPwm;
static float speed_1;
static float speed_2;

static float speedSetValue = 0;

// static const char Terminator = '\r';
// static char inputBuffer[100];
// static unsigned char inputBufferIndex;

static ItSignal_t itSignals[] = {
	{
		"extMode",
		ItValueType_Uint8,
		[]()->uint8_t{return (uint8_t)externalMode;},
		(void (*)(void)) setExternalMode,
	},
	{
		"bothPwm",
		ItValueType_Float,
		[]()->float{return bothPwm;},
		(void (*)(void)) setBothPwm,
	},
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
		"speed1",
		ItValueType_Float,
		[]()->float{return speed_1;},
		[](float value)->float{speedSetValue = value;},
	},
	{
		"speed2",
		ItValueType_Float,
		[]()->float{return speed_2;},
		NULL,
	},
	{
		"i",
		ItValueType_Float,
		(void (*)(void)) getSpeedControl_I,
		(void (*)(void)) setSpeedControl_I,
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

	sampleTimeMicros = 0.01e6;
	timerSetup(sampleTimeMicros);

	//inputBufferIndex = 0;
	quadratureEncoder_init();
	initMotorPower(((float)sampleTimeMicros)/1e6f);
	initMotorAngularSpeed();

	speed_1 = 0;
	speed_2 = 0;
}

void appLoop(void) {
	if(!timerEvent){
		//setBuiltinLedOff();
		return;
	}
	//setBuiltinLedOn();
	timerEvent=false;

	speed_1 = getMotorAngularSpeed_1(deltaTime);
	speed_2 = getMotorAngularSpeed_2(deltaTime);

	if(externalMode){
		//nothing to do
	}
	else{
		unsigned long tickTemp = getTickMicros();
		deltaTime = (float)(tickTemp - oldTickMicros) / 1.0e6f;
		oldTickMicros = tickTemp;

		float controlValue = doSpeedControl(speedSetValue, speed_1, deltaTime);
		///setPowerMotorA(controlValue);
	}

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

static inline void setExternalMode(uint8_t value){
	externalMode = (bool)value;
	if(externalMode){
		///setPowerMotorA(0);
		///setPowerMotorB(0);
	}
}

static inline void setBothPwm(float value){
	///setPowerMotorA(value);
	///setPowerMotorB(value);
	bothPwm = value;
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

// static inline void initLed(void) {
// 	pinMode(LED_BUILTIN, OUTPUT);
// }

// static inline void appendToInputBuffer(char ch) {
// 	inputBuffer[inputBufferIndex] = ch;
// 	inputBufferIndex++;
// }
