#ifndef MOTOR_SPEED_H
#define MOTOR_SPEED_H

void initMotorAngularSpeed(float samplingtimeSeconds);
void motorAngularSpeedTick(void);
float getMotorAngularSpeed_1(void);
float getMotorAngularSpeed_2(void);

#endif