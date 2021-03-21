#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

float doSpeedControl(float speedSetValue, float speedActualValue, float deltaTime);
void setSpeedControl_I(float value);
float getSpeedControl_I(void);

#endif