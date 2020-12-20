//board: Arduino Leonardo
#include <Arduino.h>
#include "quadratureEncoder.h"

typedef void (*InterruptCallback)(void);

typedef struct Buffer{
	bool isDataValid;
	bool isLockedByReader;
	unsigned long counts;
};

typedef enum{
	A,
	B,
} ActiveWriteBuffer;

typedef struct Encoder{
	Buffer bufferA;
	Buffer bufferB;
	ActiveWriteBuffer activeWriteBuffer;
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
static inline void oneStepForward(Encoder* encoder);
static inline void oneStepBackward(Encoder* encoder);
static inline void switchToBufferA(Encoder* encoder);
static inline void switchToBufferB(Encoder* encoder);
static unsigned long getBufferedCounts(Encoder* encoder);

void quadratureEncoder_init(void){
	encoder_1.bufferA.isDataValid = true;
	encoder_1.bufferA.isLockedByReader = false;
	encoder_1.bufferA.counts = 0;
	encoder_1.bufferB.isDataValid = false;
	encoder_1.bufferB.isLockedByReader = false;
	encoder_1.bufferB.counts = 0;
	encoder_1.activeWriteBuffer = A;
	encoder_1.A_Pin = 0;
	encoder_1.B_Pin = 1;
	encoder_1.A_InterruptId = digitalPinToInterrupt(encoder_1.A_Pin);
	encoder_1.B_InterruptId = digitalPinToInterrupt(encoder_1.B_Pin);
	encoder_1.A_fallingClb = []()->void{A_falling(&encoder_1);};
	encoder_1.A_risingClb = []()->void{A_rising(&encoder_1);};
	encoder_1.B_fallingClb = []()->void{B_falling(&encoder_1);};
	encoder_1.B_risingClb = []()->void{B_rising(&encoder_1);};

	encoder_2.bufferA.isDataValid = true;
	encoder_2.bufferA.isLockedByReader = false;
	encoder_2.bufferA.counts = 0;
	encoder_2.bufferB.isDataValid = false;
	encoder_2.bufferB.isLockedByReader = false;
	encoder_2.bufferB.counts = 0;
	encoder_2.activeWriteBuffer = A;
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
		oneStepForward(encoder);
		attachInterrupt(encoder->B_InterruptId, encoder->B_fallingClb, FALLING);
	}
	else{
		oneStepBackward(encoder);
		attachInterrupt(encoder->B_InterruptId, encoder->B_risingClb, RISING);
	}
}

static inline void A_rising(Encoder* encoder){
	detachInterrupt(encoder->A_InterruptId);
	if(digitalRead(encoder->B_Pin) == HIGH){
		oneStepBackward(encoder);
		attachInterrupt(encoder->B_InterruptId, encoder->B_fallingClb, FALLING);
	}
	else{
		oneStepForward(encoder);
		attachInterrupt(encoder->B_InterruptId, encoder->B_risingClb, RISING);
	}
}

static inline void B_falling(Encoder* encoder){
	detachInterrupt(encoder->B_InterruptId);
	if(digitalRead(encoder->A_Pin) == HIGH){
		oneStepBackward(encoder);
		attachInterrupt(encoder->A_InterruptId, encoder->A_fallingClb, FALLING);
	}
	else{
		oneStepForward(encoder);
		attachInterrupt(encoder->A_InterruptId, encoder->A_risingClb, RISING);
	}
}

static inline void B_rising(Encoder* encoder){
	detachInterrupt(encoder->B_InterruptId);
	if(digitalRead(encoder->A_Pin) == HIGH){
		oneStepForward(encoder);
		attachInterrupt(encoder->A_InterruptId, encoder->A_fallingClb, FALLING);
	}
	else{
		oneStepBackward(encoder);
		attachInterrupt(encoder->A_InterruptId, encoder->A_risingClb, RISING);
	}
}

static inline void oneStepForward(Encoder* encoder){
	if(encoder->activeWriteBuffer == A){
		if(encoder->bufferA.isLockedByReader){
			switchToBufferB(encoder);
			encoder->bufferB.counts++;
		}
		else{
			encoder->bufferA.counts++;
		}
	}
	else{
		if(encoder->bufferB.isLockedByReader){
			switchToBufferA(encoder);
			encoder->bufferA.counts++;
		}
		else{
			encoder->bufferB.counts++;
		}
	}
}

static inline void oneStepBackward(Encoder* encoder){
	if(encoder->activeWriteBuffer == A){
		if(encoder->bufferA.isLockedByReader){
			switchToBufferB(encoder);
			encoder->bufferB.counts--;
		}
		else{
			encoder->bufferA.counts--;
		}
	}
	else{
		if(encoder->bufferB.isLockedByReader){
			switchToBufferA(encoder);
			encoder->bufferA.counts--;
		}
		else{
			encoder->bufferB.counts--;
		}
	}
}

static inline void switchToBufferA(Encoder* encoder){
	encoder->bufferA.counts = encoder->bufferB.counts;
	encoder->activeWriteBuffer = A;
	encoder->bufferA.isDataValid = true;
	encoder->bufferB.isDataValid == false;
}

static inline void switchToBufferB(Encoder* encoder){
	encoder->bufferB.counts = encoder->bufferA.counts;
	encoder->activeWriteBuffer = B;
	encoder->bufferA.isDataValid == false;
	encoder->bufferB.isDataValid = true;
}

unsigned long quadratureEncoder_getCountsPerRevolution(void){
	return 720;
}

unsigned long quadratureEncoder_getCounts_1(void){
	return getBufferedCounts(&encoder_1);
}

unsigned long quadratureEncoder_getCounts_2(void){
	return getBufferedCounts(&encoder_2);
}

static unsigned long getBufferedCounts(Encoder* encoder){
	unsigned long counts;
	if(encoder->bufferA.isDataValid){
		encoder->bufferA.isLockedByReader = true;
		counts = encoder->bufferA.counts;
		encoder->bufferA.isLockedByReader = false;
	}else{
		encoder->bufferB.isLockedByReader = true;
		counts = encoder->bufferB.counts;
		encoder->bufferB.isLockedByReader = false;
	}
	return counts;
}
