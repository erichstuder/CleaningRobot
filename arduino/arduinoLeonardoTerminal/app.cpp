//board: Arduino Leonardo
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define LED_BUILTIN 13

typedef enum {
  Forward,
  Backward
} Direction_t;

static const char Terminator = '\r';
static char inputBuffer[100];
static unsigned char inputBufferIndex;

static inline void handleInput(void);
static inline bool charArrayBeginsWith(const char* array, const char* begin);
static inline void setLedOn(void);
static inline void setLedOff(void);
static inline void setMotorA(float value);
static inline void setMotorB(float value);
static inline void disengageBreakOnPin(int pin);
static inline void setDirectionOnPin(int pin, Direction_t direction);
static inline void setPwmOnPin(int pin, float ratio);
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
	else if(charArrayBeginsWith(inputBuffer, "setMotorA ")){
		float ratio = atof(strstr(inputBuffer, " ") + 1);
		setMotorA(ratio);
	}
	else if(charArrayBeginsWith(inputBuffer, "setMotorB ")){
		float ratio = atof(strstr(inputBuffer, " ") + 1);
		setMotorB(ratio);
	}
	/*else if(strcmp(inputBuffer, "getAdc_A0") == 0){
		char numberString[4+sizeof('\0')];
		sprintf(numberString, "%d", analogRead(A0)); //TODO: sprintf hat einen Rückgabewert!
		Serial.write(numberString);
		Serial.write('\n');
	}*/
	inputBufferIndex = 0;
}

static inline bool charArrayBeginsWith(const char* array, const char* begin) {
	return strstr(array, begin) == array;
}

static inline void setLedOn(void) {
	initLed();
	digitalWrite(LED_BUILTIN, HIGH);
}

static inline void setLedOff(void) {
	initLed();
	digitalWrite(LED_BUILTIN, LOW);
}

static inline void setMotorA(float value) {
	disengageBreakOnPin(9);
	const int DirectionPin = 12;
	if(value > 0) {
		setDirectionOnPin(DirectionPin, Forward);  
	}
	else {
		setDirectionOnPin(DirectionPin, Backward);
	}
	setPwmOnPin(3, abs(value));
}

static inline void setMotorB(float value) {
	disengageBreakOnPin(8);
	const int DirectionPin = 13;
	if(value > 0) {
		setDirectionOnPin(DirectionPin, Forward);  
	}
	else {
		setDirectionOnPin(DirectionPin, Backward);
	}
	setPwmOnPin(11, abs(value));
}

static inline void disengageBreakOnPin(int pin) {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

static inline void setDirectionOnPin(int pin, Direction_t direction) {
	pinMode(pin, OUTPUT);
	if(direction == Forward) {
		digitalWrite(pin, HIGH);
	}
	else {
		digitalWrite(pin, LOW);
	}
}

static inline void setPwmOnPin(int pin, float ratio) {
	pinMode(pin, OUTPUT);
	analogWrite(pin, int(ratio*255.0f));
}

static inline void initLed(void) {
	pinMode(LED_BUILTIN, OUTPUT);
}

static inline void appendToInputBuffer(char ch) {
	inputBuffer[inputBufferIndex] = ch;
	inputBufferIndex++;
}
