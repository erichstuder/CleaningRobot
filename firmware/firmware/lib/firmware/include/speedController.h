#ifndef SPEED_CONTROLLER_H
#define SPEED_CONTROLLER_H

void initSpeedController(float samplingtimeSeconds);
void speedControllerTick(void);
void setAngularSpeed_A(float value);
void setAngularSpeed_B(float value);
void setSpeedController_gainP(float value);
float getSpeedController_gainP(void);
void setSpeedController_gainI(float value);
float getSpeedController_gainI(void);

#endif