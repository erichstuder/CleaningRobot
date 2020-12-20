//board: Arduino Leonardo
#include <Arduino.h>
#include "quadratureEncoder.h"

static unsigned long oldCounts_1;
static unsigned long oldCounts_2;

static inline float calculateAngularSpeed(unsigned long counts, unsigned long oldCounts, float deltaTime);

void initMotorAngularSpeed(void){
	oldCounts_1 = quadratureEncoder_getCounts_1();
	oldCounts_2 = quadratureEncoder_getCounts_2();
}

float getMotorAngularSpeed_1(float deltaTime){
	unsigned long counts = quadratureEncoder_getCounts_1();
	float angularSpeed = calculateAngularSpeed(counts, oldCounts_1,  deltaTime);
	oldCounts_1 = counts;
	return angularSpeed;
}

float getMotorAngularSpeed_2(float deltaTime){
	unsigned long counts = quadratureEncoder_getCounts_2();
	float angularSpeed = calculateAngularSpeed(counts, oldCounts_2, deltaTime);
	oldCounts_2 = counts;
	return angularSpeed;
}

static inline float calculateAngularSpeed(unsigned long counts, unsigned long oldCounts, float deltaTime){
	float deltaCounts = (float)((long)(counts - oldCounts));
	float radPerSecond = deltaCounts / (float)quadratureEncoder_getCountsPerRevolution() * (float)TWO_PI / deltaTime;
	return radPerSecond;
}


