#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

void initMotorInterface(void);
float getMotorInterface_ratioA(void);
float getMotorInterface_ratioB(void);
void setMotorInterface_ratioA(float ratio);
void setMotorInterface_ratioB(float ratio);

#endif