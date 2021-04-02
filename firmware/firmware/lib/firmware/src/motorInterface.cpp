#include <Arduino.h>

typedef struct {
	int brakePin;
	int directionPin;
	int pwmPin;
	float ratio;
} MotorInterface;

static inline void setMotor(MotorInterface* motor);
static inline void disengageBrake(MotorInterface* motor);
static inline void setDirection(MotorInterface* motor);
static inline void setPwm(MotorInterface* motor);

static MotorInterface motorA;
static MotorInterface motorB;

void initMotorInterface(void){

	motorA.brakePin = 9;
	motorA.directionPin = 12;
	motorA.pwmPin = 3;
	motorA.ratio = 0;

	motorB.brakePin = 8;
	motorB.directionPin = 13;
	motorB.pwmPin = 11;
	motorB.ratio = 0;
}

float getMotorInterface_ratioA(void){
	return motorA.ratio;
}

float getMotorInterface_ratioB(void){
	return motorB.ratio;
}

void setMotorInterface_ratioA(float ratio){
	motorA.ratio = ratio;
	setMotor(&motorA);
}

void setMotorInterface_ratioB(float ratio){
	motorB.ratio = ratio;
	setMotor(&motorB);
}

static inline void setMotor(MotorInterface* motor){
	disengageBrake(motor);
	setDirection(motor);
	setPwm(motor);
}

static inline void disengageBrake(MotorInterface* motor){
	int pin = motor->brakePin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

static inline void setDirection(MotorInterface* motor){
	int pin = motor->directionPin;
	pinMode(pin, OUTPUT);
	if(motor->ratio > 0) {
		digitalWrite(pin, LOW);
	}
	else {
		digitalWrite(pin, HIGH);
	}
}

static inline void setPwm(MotorInterface* motor){
	float ratio = abs(motor->ratio);
	ratio = min(ratio, 1);


	int pwmPin = motor->pwmPin;
	pinMode(pwmPin, OUTPUT);
	analogWrite(pwmPin, int(ratio*255.0f));
}
