//board: Arduino Leonardo
#include <Arduino.h>

static inline float readCurrent(uint8_t adcPin);
static inline float countsToVolt(int adcCounts);
static inline float voltToCurrent(float volt);

float getMotorDriverCurrent_A(void){
	return readCurrent(A0);
}

float getMotorDriverCurrent_B(void){
	return readCurrent(A1);
}

static inline float readCurrent(uint8_t adcPin){
	int adcCounts = analogRead(adcPin);
	float volt = countsToVolt(adcCounts);
	float motorCurrent = voltToCurrent(volt);
	return motorCurrent;
}

static inline float countsToVolt(int adcCounts){
	return ((float)adcCounts) * 5.0f / 1024.0f;
}

static inline float voltToCurrent(float volt){
	const float Amplification = 11.0f;
	const float Rsens = 0.15f;
	return volt / Amplification / Rsens;
}
