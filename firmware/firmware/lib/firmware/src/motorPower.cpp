//board: Arduino Leonardo and Motor Shield
#include <Arduino.h>
#include "motorPower.h"
#include "motorDriverCurrent.h"

typedef struct {
	int brakePin;
	int directionPin;
	int pwmPin;
	float (*getMotorDriverCurrent)(void);
	float controllerIntegralPart;
} Motor;

static inline void setMotor(Motor* motor, float ratio);
static inline void disengageBrake(Motor* motor);
static inline void setDirection(Motor* motor, float ratio);
static inline void setPwm(Motor* motor, float ratio);

static Motor motorA;
static Motor motorB;
static float gainP;
static float gainI;
static float samplingtime;

void initMotorPower(float samplingtimeSeconds){
	samplingtime = samplingtimeSeconds;

	motorA.brakePin = 9;
	motorA.directionPin = 12;
	motorA.pwmPin = 3;
	motorA.getMotorDriverCurrent = getMotorDriverCurrent_A;
	motorA.controllerIntegralPart = 0;

	motorB.brakePin = 8;
	motorB.directionPin = 13;
	motorB.pwmPin = 11;
	motorB.getMotorDriverCurrent = getMotorDriverCurrent_B;
	motorB.controllerIntegralPart = 0;

	gainP = 0;
	gainI = 0;
}

void setMotorPower_A(float ratio){
	setMotor(&motorA, ratio);
}

void setMotorPower_B(float ratio){
	setMotor(&motorB, ratio);
}

static inline void setMotor(Motor* motor, float ratio){
	disengageBrake(motor);
	setDirection(motor, ratio);
	setPwm(motor, ratio);
}

static inline void disengageBrake(Motor* motor){
	int pin = motor->brakePin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

static inline void setDirection(Motor* motor, float ratio){
	int pin = motor->directionPin;
	pinMode(pin, OUTPUT);
	if(ratio > 0) {
		digitalWrite(pin, LOW);
	}
	else {
		digitalWrite(pin, HIGH);
	}
}

static inline void setPwm(Motor* motor, float ratio){
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

	int pwmPin = motor->pwmPin;
	pinMode(pwmPin, OUTPUT);
	analogWrite(pwmPin, int(ratio*255.0f));
}

float getMotorPower_GainP(void){
	return gainP;
}

void setMotorPower_GainP(float value){
	gainP = value;
}

float getMotorPower_GainI(void){
	return gainI;
}

void setMotorPower_GainI(float value){
	gainI = value;
}
