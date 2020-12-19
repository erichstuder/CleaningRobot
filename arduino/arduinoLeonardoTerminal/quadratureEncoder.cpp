//board: Arduino Leonardo
#include <Arduino.h>

typedef void (*InterruptCallback)(void);

typedef struct Encoder{
	unsigned long counts;
	int A_Pin;
	int B_Pin;
	uint8_t A_InterruptId;
	uint8_t B_InterruptId;
	InterruptCallback A_fallingClb;
	InterruptCallback A_risingClb;
	InterruptCallback B_fallingClb;
	InterruptCallback B_risingClb;
};

static Encoder encoder_1;
static Encoder encoder_2;

static inline void attachToInterrupts(Encoder* encoder);
static inline void A_falling(Encoder* encoder);
static inline void A_rising(Encoder* encoder);
static inline void B_falling(Encoder* encoder);
static inline void B_rising(Encoder* encoder);

void quadratureEncoder_init(void){
	encoder_1.counts = 0;
	encoder_1.A_Pin = 0;
	encoder_1.B_Pin = 1;
	encoder_1.A_InterruptId = digitalPinToInterrupt(encoder_1.A_Pin);
	encoder_1.B_InterruptId = digitalPinToInterrupt(encoder_1.B_Pin);
	encoder_1.A_fallingClb = []()->void{A_falling(&encoder_1);};
	encoder_1.A_risingClb = []()->void{A_rising(&encoder_1);};
	encoder_1.B_fallingClb = []()->void{B_falling(&encoder_1);};
	encoder_1.B_risingClb = []()->void{B_rising(&encoder_1);};

	encoder_2.counts = 0;
	encoder_2.A_Pin = 2;
	encoder_2.B_Pin = 7;
	encoder_2.A_InterruptId = digitalPinToInterrupt(encoder_2.A_Pin);
	encoder_2.B_InterruptId = digitalPinToInterrupt(encoder_2.B_Pin);
	encoder_2.A_fallingClb = []()->void{A_falling(&encoder_2);};
	encoder_2.A_risingClb = []()->void{A_rising(&encoder_2);};
	encoder_2.B_fallingClb = []()->void{B_falling(&encoder_2);};
	encoder_2.B_risingClb = []()->void{B_rising(&encoder_2);};

	attachToInterrupts(&encoder_1);
	attachToInterrupts(&encoder_2);
}

static inline void attachToInterrupts(Encoder* encoder){
	if(digitalRead(encoder->A_Pin) == HIGH){
		attachInterrupt(encoder->A_InterruptId, encoder->A_fallingClb, FALLING);
	}
	else{
		attachInterrupt(encoder->A_InterruptId, encoder->A_risingClb, RISING);
	}

	if(digitalRead(encoder->B_Pin) == HIGH){
		attachInterrupt(encoder->B_InterruptId, encoder->B_fallingClb, FALLING);
	}
	else{
		attachInterrupt(encoder->B_InterruptId, encoder->B_risingClb, RISING);
	}
}

static inline void A_falling(Encoder* encoder){
	detachInterrupt(encoder->A_InterruptId);
	if(digitalRead(encoder->B_Pin) == HIGH){
		encoder->counts++;
		attachInterrupt(encoder->B_InterruptId, encoder->B_fallingClb, FALLING);
	}
	else{
		encoder->counts--;
		attachInterrupt(encoder->B_InterruptId, encoder->B_risingClb, RISING);
	}
}

static inline void A_rising(Encoder* encoder){
	detachInterrupt(encoder->A_InterruptId);
	if(digitalRead(encoder->B_Pin) == HIGH){
		encoder->counts--;
		attachInterrupt(encoder->B_InterruptId, encoder->B_fallingClb, FALLING);
	}
	else{
		encoder->counts++;
		attachInterrupt(encoder->B_InterruptId, encoder->B_risingClb, RISING);
	}
}

static inline void B_falling(Encoder* encoder){
	detachInterrupt(encoder->B_InterruptId);
	if(digitalRead(encoder->A_Pin) == HIGH){
		encoder->counts--;
		attachInterrupt(encoder->A_InterruptId, encoder->A_fallingClb, FALLING);
	}
	else{
		encoder->counts++;
		attachInterrupt(encoder->A_InterruptId, encoder->A_risingClb, RISING);
	}
}

static inline void B_rising(Encoder* encoder){
	detachInterrupt(encoder->B_InterruptId);
	if(digitalRead(encoder->A_Pin) == HIGH){
		encoder->counts++;
		attachInterrupt(encoder->A_InterruptId, encoder->A_fallingClb, FALLING);
	}
	else{
		encoder->counts--;
		attachInterrupt(encoder->A_InterruptId, encoder->A_risingClb, RISING);
	}
}

unsigned long quadratureEncoder_getCounts_1(void){
	return encoder_1.counts;
}

unsigned long quadratureEncoder_getCounts_2(void){
	return encoder_2.counts;
}
