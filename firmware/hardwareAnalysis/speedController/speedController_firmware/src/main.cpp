#include <Arduino.h>

#include "itHandler.h"
#include "speedController.h"
#include "motorSpeed.h"

static inline unsigned long getMicros(void);

static ItSignal_t itSignals[] = {
	{
		"i",
		ItValueType_Float,
		(void (*)(void)) getSpeedController_gainI,
		(void (*)(void)) setSpeedController_gainI
	},
	{
		"speedA",
		ItValueType_Float,
		(void (*)(void)) getMotorAngularSpeed_1,
		(void (*)(void)) setAngularSpeed_A
	},
	{
		"speedB",
		ItValueType_Float,
		(void (*)(void)) getMotorAngularSpeed_2,
		(void (*)(void)) setAngularSpeed_B
	}
};

static const unsigned char ItSignalCount = sizeof(itSignals) / sizeof(itSignals[0]);

static unsigned long currentMicros;
static unsigned long lastMicros;
static const unsigned long SamplingTimeMicros = 0.01e6;

void setup(void){
	itHandlerInit(getMicros, itSignals, ItSignalCount);
	initSpeedController(((float)SamplingTimeMicros)/1e6);
	lastMicros = micros();
}

void loop(void){
	currentMicros = micros();
	if(currentMicros - lastMicros < SamplingTimeMicros){
		return;
	}
	lastMicros = currentMicros;
	speedControllerTick();
	itHandlerTick();
}

static unsigned long getMicros(void){
	return currentMicros;
}
