#ifndef MOTOR_POWER_H
#define MOTOR_POWER_H

void initMotorPower(float samplingtimeSeconds);

void motorPowerTick(void);

void setMotorPower_A(float ratio);
void setMotorPower_B(float ratio);

float getMotorPower_gainP(void);
void setMotorPower_gainP(float value);
float getMotorPower_gainI(void);
void setMotorPower_gainI(float value);

#endif
