static float integralStorage = 0;
static float iPart = 0.01;

float doSpeedControl(float speedSetValue, float speedActualValue, float deltaTime){
	float error = speedSetValue - speedActualValue;
	integralStorage += iPart * error;

	if(integralStorage > 1){
		integralStorage = 1;
	}
	else if(integralStorage < -1){
		integralStorage = -1;
	}

	return integralStorage;
}

void setSpeedControl_I(float value){
	iPart = value;
}

float getSpeedControl_I(void){
	return iPart;
}
