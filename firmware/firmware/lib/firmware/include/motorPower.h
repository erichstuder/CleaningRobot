#ifndef MOTOR_POWER_H
#define MOTOR_POWER_H

void setMotorPower_A(float ratio);
void setMotorPower_B(float ratio);

void initMotorPower(float samplingtimeSeconds);

float getMotorPower_gainP(void);
void setMotorPower_gainP(float value);
float getMotorPower_gainI(void);
void setMotorPower_gainI(float value);

#endif
