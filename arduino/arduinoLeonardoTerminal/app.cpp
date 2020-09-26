//board: Arduino Leonardo
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define LED_BUILTIN 13

static const char Terminator = '\r';
static char inputBuffer[100];
static unsigned char inputBufferIndex;

static inline void handleInput(void);
static inline void setLedOn(void);
static inline void setLedOff(void);
static inline void initLed(void);
static inline void appendToInputBuffer(char ch);

void appSetup(void) {
	 inputBufferIndex = 0;
}

void appLoop(void) {
	//if(Serial){
		//while(true){
			int incomingInt = Serial.read();
			char incomingChar = (char)incomingInt;
			if(incomingChar == Terminator) {
				appendToInputBuffer('\0');
				handleInput();
				//delay(10);
			}
			else if(incomingInt != -1) {
				appendToInputBuffer(incomingChar);
			}
		//}
	//}
}

static inline void handleInput(void) {
	if(strcmp(inputBuffer, "setLedOn") == 0){
		setLedOn();
	}
	else if(strcmp(inputBuffer, "setLedOff") == 0){
		setLedOff();
	}
	else if(strcmp(inputBuffer, "getAdc_A0") == 0){
		char numberString[4+sizeof('\0')];
		sprintf(numberString, "%d", analogRead(A0)); //TODO: sprintf hat einen Rückgabewert!
		Serial.write(numberString);
		Serial.write('\n');
	}
	inputBufferIndex = 0;
}

static inline void setLedOn(void) {
	initLed();
	digitalWrite(0, LOW);
	digitalWrite(1, HIGH);
	digitalWrite(2, LOW);
	digitalWrite(3, HIGH);
	digitalWrite(5, LOW);
	digitalWrite(7, HIGH);
	digitalWrite(9, LOW);
	digitalWrite(11, HIGH);
}

static inline void setLedOff(void) {
	initLed();
	digitalWrite(0, LOW);
	digitalWrite(1, LOW);
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(5, LOW);
	digitalWrite(7, LOW);
	digitalWrite(9, LOW);
	digitalWrite(11, LOW);
}

static inline void initLed(void) {
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(11, OUTPUT);
}

static inline void appendToInputBuffer(char ch) {
	inputBuffer[inputBufferIndex] = ch;
	inputBufferIndex++;
}
