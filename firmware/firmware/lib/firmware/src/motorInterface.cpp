#include <Arduino.h>

typedef struct {
	int brakePin;
	int directionPin;
	int pwmPin;
} MotorInterface;

static inline void setMotor(MotorInterface* motor, float ratio);
static inline void disengageBrake(MotorInterface* motor);
static inline void setDirection(MotorInterface* motor, float ratio);
static inline void setPwm(MotorInterface* motor, float ratio);

static MotorInterface motorA;
static MotorInterface motorB;

void initMotorInterface(void){

	motorA.brakePin = 9;
	motorA.directionPin = 12;
	motorA.pwmPin = 3;

	motorB.brakePin = 8;
	motorB.directionPin = 13;
	motorB.pwmPin = 11;
}

void setMotorInterface_A(float ratio){
	setMotor(&motorA, ratio);
}

void setMotorInterface_B(float ratio){
	setMotor(&motorB, ratio);
}

static inline void setMotor(MotorInterface* motor, float ratio){
	disengageBrake(motor);
	setDirection(motor, ratio);
	setPwm(motor, ratio);
}

static inline void disengageBrake(MotorInterface* motor){
	int pin = motor->brakePin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

static inline void setDirection(MotorInterface* motor, float ratio){
	int pin = motor->directionPin;
	pinMode(pin, OUTPUT);
	if(ratio > 0) {
		digitalWrite(pin, LOW);
	}
	else {
		digitalWrite(pin, HIGH);
	}
}

static inline void setPwm(MotorInterface* motor, float ratio){
	ratio = abs(ratio);
	ratio = min(ratio, 1);

	int pwmPin = motor->pwmPin;
	pinMode(pwmPin, OUTPUT);
	analogWrite(pwmPin, int(ratio*255.0f));
}
