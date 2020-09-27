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
	int incomingInt = Serial.read();
	char incomingChar = (char)incomingInt;
	if(incomingChar == Terminator) {
		appendToInputBuffer('\0');
		handleInput();
	}
	else if(incomingInt != -1) {
		appendToInputBuffer(incomingChar);
	}
}

static inline void handleInput(void) {
	if(strcmp(inputBuffer, "setLedOn") == 0){
		setLedOn();
	}
	else if(strcmp(inputBuffer, "setLedOff") == 0){
		setLedOff();
	}
	/*else if(strcmp(inputBuffer, "getAdc_A0") == 0){
		char numberString[4+sizeof('\0')];
		sprintf(numberString, "%d", analogRead(A0)); //TODO: sprintf hat einen Rückgabewert!
		Serial.write(numberString);
		Serial.write('\n');
	}*/
	inputBufferIndex = 0;
}

static inline void setLedOn(void) {
	initLed();
	digitalWrite(LED_BUILTIN, HIGH);
}

static inline void setLedOff(void) {
	initLed();
	digitalWrite(LED_BUILTIN, LOW);
}

static inline void initLed(void) {
	pinMode(LED_BUILTIN, OUTPUT);
}

static inline void appendToInputBuffer(char ch) {
	inputBuffer[inputBufferIndex] = ch;
	inputBufferIndex++;
}
