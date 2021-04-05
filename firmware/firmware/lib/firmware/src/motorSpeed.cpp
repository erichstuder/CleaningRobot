//board: Arduino Leonardo
#include <Arduino.h>
#include "quadratureEncoder.h"

//TODO diesen Datentyp in Motor umbenennen (namespace, class, ...)
typedef struct{
	unsigned long (*getCounts)(void);
	unsigned long oldCounts;
	float radPerSecond;
} MotorAngularSpeed;

static inline void calculateAngularSpeed(MotorAngularSpeed* motor);

static MotorAngularSpeed motor_1, motor_2;
static float samplingtime;

void initMotorAngularSpeed(float samplingtimeSeconds){
	samplingtime = samplingtimeSeconds;

	quadratureEncoder_init();

	motor_1.getCounts = quadratureEncoder_getCounts_1;
	motor_1.oldCounts = motor_1.getCounts();
	motor_1.radPerSecond = 1;

	motor_2.getCounts = quadratureEncoder_getCounts_2;
	motor_2.oldCounts = motor_2.getCounts();
	motor_2.radPerSecond = 0;
}

void motorAngularSpeedTick(void){
	calculateAngularSpeed(&motor_1);
	calculateAngularSpeed(&motor_2);
}

float getMotorAngularSpeed_1(void){
	return motor_1.radPerSecond;
}

float getMotorAngularSpeed_2(void){
	return motor_2.radPerSecond;
}

static inline void calculateAngularSpeed(MotorAngularSpeed* motor){
	unsigned long counts = motor->getCounts();
	float deltaCounts = (float)((long)(counts - motor->oldCounts));
	motor->oldCounts = counts;
	motor->radPerSecond= deltaCounts / (float)quadratureEncoder_getCountsPerRevolution() * (float)TWO_PI / samplingtime;
}
