//board: Arduino Leonardo
#include <Arduino.h>

static inline float readMotorCurrent(uint8_t adcPin);
static inline float countsToVolt(int adcCounts);
static inline float voltToMotorCurent(float volt);

float getMotorCurrent_1(void){
	return readMotorCurrent(A0);
}

float getMotorCurrent_2(void){
	return readMotorCurrent(A1);
}

static inline float readMotorCurrent(uint8_t adcPin){
	int adcCounts = analogRead(adcPin);
	float volt = countsToVolt(adcCounts);
	float motorCurrent = voltToMotorCurent(volt);
	return motorCurrent;
}

static inline float countsToVolt(int adcCounts){
	return ((float)adcCounts) * 5.0f / 1024.0f;
}

static inline float voltToMotorCurent(float volt){
	const float Amplification = 11.0f;
	const float Rsens = 0.15f;
	return volt / Amplification / Rsens;
}
