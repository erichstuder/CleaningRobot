#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupport_c.h"
#include "Arduino.h"
#include <string.h>

#include "app.h"

HardwareSerial Serial;

static void expectInputString(const char* str);
static void expectNoInputLeft(void);

static void expectLedOn(void);
static void expectLedOff(void);
static void expectLedInit(void);
//static void expectReadAndSendAdcA0(void);

void pinMode(uint8_t pin, uint8_t value) {
	mock_c()->actualCall("pinMode")
		->withIntParameters("pin", pin)
		->withIntParameters("value", value);
}

void digitalWrite(uint8_t pin, uint8_t value) {
	mock_c()->actualCall("digitalWrite")
		->withIntParameters("pin", pin)
		->withIntParameters("value", value);
}

/*int analogRead(uint8_t pin) {
	mock_c()->actualCall("analogRead")
		->withIntParameters("pin", pin);
	return mock_c()->returnValue().value.intValue;
}*/

TEST_GROUP(ArduinoLeonardoTerminalTest) {
	void setup() {
		mock().strictOrder();
		appSetup();
	}

	void teardown() {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(ArduinoLeonardoTerminalTest, turnLedOn) {
	expectInputString("setLedOn");
	expectLedOn();
	expectNoInputLeft();

	for (int n = 0; n < 11; n++) {
		appLoop();
	}
	
}

TEST(ArduinoLeonardoTerminalTest, turnLedOff) {
	expectInputString("setLedOff");
	expectLedOff();
	expectNoInputLeft();

	for (int n = 0; n < 12; n++) {
		appLoop();
	}
}

/*TEST(ArduinoLeonardoTerminalTest, getAdcA0) {
	expectInputString("getAdc_A0");
	expectReadAndSendAdcA0();
	expectNoInputLeft();

	for (int n = 0; n < 12; n++) {
		appLoop();
	}
}*/

static void expectInputString(const char* str) {
	for (int i = 0; i < (int)strlen(str) + 1; i++) {
		mock().expectOneCall("read")
			.andReturnValue(str[i]);
	}
	mock().expectOneCall("read")
		.andReturnValue('\r');
}

static void expectNoInputLeft(void) {
	mock().expectOneCall("read")
		.andReturnValue(-1);
}

static void expectLedOn(void) {
	expectLedInit();

	mock().expectOneCall("digitalWrite")
		.withIntParameter("pin", 13)
		.withIntParameter("value", 1);
}

static void expectLedOff(void) {
	expectLedInit();

	mock().expectOneCall("digitalWrite")
		.withIntParameter("pin", 13)
		.withIntParameter("value", 0);
}

static void expectLedInit(void) {
	mock().expectOneCall("pinMode")
		.withIntParameter("pin", 13)
		.withIntParameter("value", 1);
}

/*static void expectReadAndSendAdcA0(void) {
	mock().expectOneCall("analogRead")
		.withIntParameter("pin", A0)
		.andReturnValue(222);

	const char* str = "222";
	mock().expectOneCall("write")
		.withStringParameter("str", str)
	    .andReturnValue((int)strlen(str));

	mock().expectOneCall("write")
		.withIntParameter("val", '\n')
		.andReturnValue(1);
}*/
