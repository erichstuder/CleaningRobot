#include <Arduino.h>

#include "itHandler.h"
#include "motorPower.h"
#include "motorInterface.h"
#include "motorDriverCurrent.h"

static inline unsigned long getMicros(void);

//TODO: was passiert wenn beim setzen der Getter NULL ist?
static ItSignal_t itSignals[] = {
	{
		"directRatioMotorA",
		ItValueType_Float,
		(void (*)(void)) getMotorInterface_ratioA,
		(void (*)(void)) setMotorInterface_ratioA
	},
	{
		"directRatioMotorB",
		ItValueType_Float,
		(void (*)(void)) getMotorInterface_ratioB,
		(void (*)(void)) setMotorInterface_ratioB
	},
	{
		"controlledRatioMotorA",
		ItValueType_Float,
		(void (*)(void)) getMotorInterface_ratioA,
		(void (*)(void)) setMotorPower_A
	},
	{
		"controlledRatioMotorB",
		ItValueType_Float,
		(void (*)(void)) getMotorInterface_ratioB,
		(void (*)(void)) setMotorPower_B
	},
	{
		"driverCurrentA",
		ItValueType_Float,
		(void (*)(void)) getMotorDriverCurrent_A,
		NULL
	},
	{
		"driverCurrentB",
		ItValueType_Float,
		(void (*)(void)) getMotorDriverCurrent_B,
		NULL
	},
	{
		"p",
		ItValueType_Float,
		(void (*)(void)) getMotorPower_gainP,
		(void (*)(void)) setMotorPower_gainP
	},
	{
		"i",
		ItValueType_Float,
		(void (*)(void)) getMotorPower_gainI,
		(void (*)(void)) setMotorPower_gainI
	}
};

static const unsigned char ItSignalCount = sizeof(itSignals) / sizeof(itSignals[0]);

static unsigned long currentMicros;
static unsigned long lastMicros;
static const unsigned long SamplingTimeMicros = 0.01e6;

void setup(void) {
	itHandlerInit(getMicros, itSignals, ItSignalCount);
	initMotorPower(((float)SamplingTimeMicros)/1e6);
	lastMicros = micros();
}

void loop(void) {
	currentMicros = micros();
	if(currentMicros - lastMicros < SamplingTimeMicros){
		return;
	}
	lastMicros = currentMicros;
	motorDriverCurrentTick();
	motorPowerTick();
	itHandlerTick();
}

static unsigned long getMicros(void){
	return currentMicros;
}
