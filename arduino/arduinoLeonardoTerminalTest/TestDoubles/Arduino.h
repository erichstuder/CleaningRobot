//test double
#pragma once

#include "CppUTestExt/MockSupport.h"

//visual studio doesnt like sprintf any more.
//#define sprintf sprintf_s

#define LED_BUILTIN 13

#define HIGH 0x1
#define LOW  0x0

#define OUTPUT 0x1

#define A0 18

typedef unsigned char uint8_t;

class HardwareSerial {
public:
	/*int available(void) { 
		//TODO:implementation
	}*/
	
	int read(void) { 
		mock().actualCall("read");
		return mock().returnValue().getIntValue();
	}

	size_t write(uint8_t val) {
		mock().actualCall("write")
			.withIntParameter("val", val);
		return mock().returnValue().getIntValue();
	}

	size_t write(const char* str) {
		mock().actualCall("write")
			.withStringParameter("str", str);
		return mock().returnValue().getIntValue(); 
	}
	operator bool() {
		mock().actualCall("bool");
		return mock().returnValue().getBoolValue();
	}
};
extern HardwareSerial Serial;

extern void pinMode(uint8_t pin, uint8_t value);
extern void digitalWrite(uint8_t pin, uint8_t value);
extern int analogRead(uint8_t pin);
