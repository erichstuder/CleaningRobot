//board: Arduino Leonardo and Motor Shield
#include <Arduino.h>
#include "motorPower.h"
#include "motorDriverCurrent.h"
#include "motorInterface.h"

typedef struct {
	float (*getMotorDriverCurrent)(void);
	float controllerIntegralPart;
	float ratioSet;
	float ratioActual;
} Motor;

static inline float limitPwmRatio(Motor* motor);
static inline float controlRatio(Motor* motor, float current, const float MaxCurrent);
static inline float limitRatioChange(Motor* motor);

static Motor motorA;
static Motor motorB;
static float gainP;
static float gainI;
static float samplingtime;

void initMotorPower(float samplingtimeSeconds){
	samplingtime = samplingtimeSeconds;

	motorA.getMotorDriverCurrent = getMotorDriverCurrent_A;
	motorA.controllerIntegralPart = 0;
	motorA.ratioSet = 0;
	motorA.ratioActual = 0;

	motorB.getMotorDriverCurrent = getMotorDriverCurrent_B;
	motorB.controllerIntegralPart = 0;
	motorB.ratioSet = 0;
	motorB.ratioActual = 0;

	gainP = 0.1;
	gainI = 4;

	initMotorInterface();
}

void motorPowerTick(void){
	motorDriverCurrentTick();

	float ratioLimited = limitPwmRatio(&motorA);
	setMotorInterface_ratioA(ratioLimited);

	ratioLimited = limitPwmRatio(&motorB);
	setMotorInterface_ratioB(ratioLimited);
}

static inline float limitPwmRatio(Motor* motor){
	//The actual 9V powersupply has 0.8A maximum output current.
	//This results in 0.4A maximum current per motor.
	const float MaxCurrent = 0.4;
	const float CurrentTooHigh = MaxCurrent + 0.2;

	float current = motor->getMotorDriverCurrent();
	if(current < CurrentTooHigh){
		motor->ratioActual = controlRatio(motor, current, MaxCurrent);
	}
	else{
		//TODO: The current was too high, so log that event
		motor->ratioActual = 0;
	}

	return motor->ratioActual;
}

static inline float controlRatio(Motor* motor, float current, const float MaxCurrent){
	float ratio = limitRatioChange(motor);

	float ratioAbs = abs(ratio);
	float ratioSign = ratio >= 0 ? 1 : -1;

	float err = MaxCurrent - current;
	motor->controllerIntegralPart += gainI * err * samplingtime;
	float ratioControlled = gainP * err + motor->controllerIntegralPart;

	ratioControlled = min(ratioAbs, ratioControlled);
	ratioControlled = min(ratioControlled, 1);
	ratioControlled = max(ratioControlled, 0);

	motor->controllerIntegralPart = ratioControlled - gainP * err;

	return ratioSign * ratioControlled;
}

static inline float limitRatioChange(Motor* motor){
	const float MaxChange = 0.5;
	float ratioLimited = motor->ratioSet;
	float ratioActual = motor->ratioActual;

	// Idea:
	// - The change of the ratio is limited to prevent high currents.
	// - A change towards zero can be as big as it wants, as it doesn't crate additional current.
	// - All other changes are limited.
	if(ratioLimited > 0 && ratioActual >= 0){
		if(ratioLimited < ratioActual){
			//do nothing
		}
		else if(ratioLimited > ratioActual + MaxChange){
			ratioLimited = ratioActual + MaxChange;
		}
		else{
			//do nothing
		}
	}
	else if(ratioLimited < 0 && ratioActual <= 0){
		if(ratioLimited > ratioActual){
			//do nothing
		}
		else if(ratioLimited < ratioActual - MaxChange){
			ratioLimited= ratioActual - MaxChange;
		}
		else{
			//do nothing
		}
	}
	else if(ratioLimited < 0 && ratioActual > 0){
		ratioLimited = 0;
	}
	else if(ratioLimited > 0 && ratioActual < 0){
		ratioLimited = 0;
	}

	return ratioLimited;
}

void setMotorPower_A(float ratio){
	motorA.ratioSet = ratio;
}

void setMotorPower_B(float ratio){
	motorB.ratioSet = ratio;
}

float getMotorPower_gainP(void){
	return gainP;
}

void setMotorPower_gainP(float value){
	gainP = value;
}

float getMotorPower_gainI(void){
	return gainI;
}

void setMotorPower_gainI(float value){
	gainI = value;
}
