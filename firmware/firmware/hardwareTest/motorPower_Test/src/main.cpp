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
		NULL,
		(void (*)(void)) setMotorInterface_A
	},
	{
		"directRatioMotorB",
		ItValueType_Float,
		NULL,
		(void (*)(void)) setMotorInterface_B
	},
	{
		"controlledRatioMotorA",
		ItValueType_Float,
		NULL,
		(void (*)(void)) setMotorPower_B
	},
	{
		"controlledRatioMotorB",
		ItValueType_Float,
		NULL,
		(void (*)(void)) setMotorPower_B
	}
	{
		"driverCurrentA",
		ItValueType_Float,
		NULL,
		(void (*)(void)) setMotorPower_B
	}
};

static const unsigned char ItSignalCount = sizeof(itSignals) / sizeof(itSignals[0]);

static unsigned long currentMicros;
static unsigned long lastMicros;

void setup(void) {
	lastMicros = micros();
	itHandlerInit(getMicros, itSignals, ItSignalCount);
}

void loop(void) {
	currentMicros = micros();
	if(currentMicros - lastMicros < 0.01e6){
		return;
	}
	lastMicros = currentMicros;
	itHandlerTick();
}

static unsigned long getMicros(void){
	return currentMicros;
}
