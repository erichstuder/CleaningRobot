#ifndef QUADRATURE_ENCODER_H
#define QUADRATURE_ENCODER_H

void quadratureEncoder_init(void);
unsigned long quadratureEncoder_getCounts_1(void);
unsigned long quadratureEncoder_getCounts_2(void);

#endif