#ifndef MOTOR_POWER_H
#define MOTOR_POWER_H

void setMotorPower_A(float ratio);
void setMotorPower_B(float ratio);

void initMotorPower(float samplingtimeSeconds);

float getMotorPower_GainP(void);
void setMotorPower_GainP(float value);
float getMotorPower_GainI(void);
void setMotorPower_GainI(float value);

#endif
