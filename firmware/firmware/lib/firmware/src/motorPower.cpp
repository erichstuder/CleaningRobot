//board: Arduino Leonardo and Motor Shield
#include <Arduino.h>
#include "motorPower.h"
#include "motorDriverCurrent.h"
#include "motorInterface.h"

typedef struct {
	float (*getMotorDriverCurrent)(void);
	//float oldCurrent[10];
	float controllerIntegralPart;
	float ratioSet;
	float ratioActual;
} Motor;

static inline float limitPwmRatio(Motor* motor);

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
	float ratioLimited = limitPwmRatio(&motorA);
	setMotorInterface_ratioA(ratioLimited);

	ratioLimited = limitPwmRatio(&motorB);
	setMotorInterface_ratioB(ratioLimited);
}

static inline float limitPwmRatio(Motor* motor){
	//TODO: if current is too high: set pwm to zero for 10 seconds and log that event

	//The actual 9V powersupply has 0.8A maximum output current.
	//This results in 0.4A maximum current per motor.
	const float MaxCurrent = 0.4;

	float ratio = motor->ratioSet;

	const float MaxChange = 0.5;
	float ratioActual = motor->ratioActual;
	if(ratio > 0 && ratioActual >= 0){
		if(ratio < ratioActual){
			//do nothing
		}
		else if(ratio > ratioActual + MaxChange){
			ratio = ratioActual + MaxChange;
		}
		else{
			//do nothing
		}
	}
	else if(ratio < 0 && ratioActual <= 0){
		if(ratio > ratioActual){
			//do nothing
		}
		else if(ratio < ratioActual - MaxChange){
			ratio= ratioActual - MaxChange;
		}
		else{
			//do nothing
		}
	}
	else if(ratio < 0 && ratioActual > 0){
		ratio = 0;
	}
	else if(ratio > 0 && ratioActual < 0){
		ratio = 0;
	}



	float ratioAbs = abs(ratio);
	float ratioSign;
	if(ratio >= 0){
		ratioSign = 1;
	}
	else{
		ratioSign = -1;
	}

	float current = motor->getMotorDriverCurrent();
	/*float currentFiltered = current;

	//find maximum current of last n measurements
	unsigned char arrayLength = sizeof(motor->oldCurrent)/sizeof(motor->oldCurrent[0]);
	for(unsigned char n=1; n<arrayLength; n++){
		float currentTemp = motor->oldCurrent[n];
		if(currentFiltered < currentTemp){
			currentFiltered = currentTemp;
		}
		motor->oldCurrent[n-1] = currentTemp;
	}
	motor->oldCurrent[arrayLength-1] = current;*/

	//float err = MaxCurrent - currentFiltered;
	float err = MaxCurrent - current;
	motor->controllerIntegralPart += gainI * err * samplingtime;
	float ratioControlled = gainP * err + motor->controllerIntegralPart;

	ratioControlled = min(ratioAbs, ratioControlled);
	ratioControlled = min(ratioControlled, 1);
	ratioControlled = max(ratioControlled, 0);

	motor->controllerIntegralPart = ratioControlled - gainP * err;

	motor->ratioActual = ratioSign * ratioControlled;

	return motor->ratioActual;
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
