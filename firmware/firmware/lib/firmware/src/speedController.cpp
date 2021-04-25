#include <Arduino.h>
#include "motorSpeed.h"
#include "motorPower.h"

//TODO: damit diese Struktur auch "Motor" heissen darf, mal die Namespaces und Klassen anschauen.
typedef struct {
	float (*getAngularSpeed)(void);
	void (*setMotorPower)(float ratio);
	float controllerIntegralPart;
	float angularSpeedSet;
} MotorSpeedControlled;

static inline void controlSpeed(MotorSpeedControlled* motor);

static MotorSpeedControlled motorA, motorB;
static float gainP = 0.2;
static float gainI = 0.3;
static float samplingtime;

void initSpeedController(float samplingtimeSeconds){
	samplingtime = samplingtimeSeconds;

	motorA.getAngularSpeed = getMotorAngularSpeed_1;
	motorA.setMotorPower = setMotorPower_A;
	motorA.controllerIntegralPart = 0;
	motorA.angularSpeedSet = 0;

	motorB.getAngularSpeed = getMotorAngularSpeed_2;
	motorB.setMotorPower = setMotorPower_B;
	motorB.controllerIntegralPart = 0;
	motorB.angularSpeedSet = 0;

	initMotorAngularSpeed(samplingtimeSeconds);
	initMotorPower(samplingtimeSeconds);
}

void speedControllerTick(void){
	motorAngularSpeedTick();
	controlSpeed(&motorA);
	controlSpeed(&motorB);
	motorPowerTick();
}

static inline void controlSpeed(MotorSpeedControlled* motor){
	float error = motor->angularSpeedSet - motor->getAngularSpeed();

	float iPart = motor->controllerIntegralPart + gainI * error * samplingtime;

	float ratio = iPart + gainP * error;
	ratio = min(ratio, 1);
	ratio = max(ratio, -1);

	iPart = min(iPart, 1);
	iPart = max(iPart, -1);
	motor->controllerIntegralPart = iPart;

	motor->setMotorPower(ratio);
}

void setAngularSpeed_A(float value){
	motorA.angularSpeedSet = value;
}

void setAngularSpeed_B(float value){
	motorB.angularSpeedSet = value;
}

void setSpeedController_gainP(float value){
	gainP = value;
}

float getSpeedController_gainP(void){
	return gainP;
}

void setSpeedController_gainI(float value){
	gainI = value;
}

float getSpeedController_gainI(void){
	return gainI;
}
