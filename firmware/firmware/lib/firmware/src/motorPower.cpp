//board: Arduino Leonardo and Motor Shield
#include <Arduino.h>
#include "motorPower.h"
#include "motorDriverCurrent.h"
#include "motorInterface.h"

typedef struct {
	float (*getMotorDriverCurrent)(void);
	float controllerIntegralPart;
} Motor;

static inline float limitPwmRatio(Motor* motor, float ratio);

static Motor motorA;
static Motor motorB;
static float gainP;
static float gainI;
static float samplingtime;

void initMotorPower(float samplingtimeSeconds){
	samplingtime = samplingtimeSeconds;

	motorA.getMotorDriverCurrent = getMotorDriverCurrent_A;
	motorA.controllerIntegralPart = 0;

	motorB.getMotorDriverCurrent = getMotorDriverCurrent_B;
	motorB.controllerIntegralPart = 0;

	gainP = 0;
	gainI = 0;

	initMotorInterface();
}

void setMotorPower_A(float ratio){
	float ratioLimited = limitPwmRatio(&motorA, ratio);
	setMotorInterface_A(ratioLimited);
}

void setMotorPower_B(float ratio){
	float ratioLimited = limitPwmRatio(&motorB, ratio);
	setMotorInterface_B(ratioLimited);
}

static inline float limitPwmRatio(Motor* motor, float ratio){
	//TODO: if current is too high: set pwm to zero for 10 seconds and log that event

	const float MaxCurrent = 0.1f;

	float current = motor->getMotorDriverCurrent();
	float err = MaxCurrent - current;
	motor->controllerIntegralPart += gainI * err * samplingtime;
	float ratioControlled = gainP * err + motor->controllerIntegralPart;

	ratio = abs(ratio);
	ratio = min(ratio, ratioControlled);
	ratio = min(ratio, 1);
	ratio = max(ratio, 0);

	motor->controllerIntegralPart = ratio - gainP * err;

	return ratio;
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
