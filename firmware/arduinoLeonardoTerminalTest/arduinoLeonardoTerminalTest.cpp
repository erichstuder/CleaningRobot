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
static void expectMotorA(int pwmValue);
static void expectMotorB(int pwmValue);
static void expectMotor(int pwmPin, int pwmValue);
//static void expectReadAndSendAdcA0(void);
static void appLoopN(int nrOfLoops);

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

void analogWrite(uint8_t pin, uint8_t value) {
	mock_c()->actualCall("analogWrite")
		->withIntParameters("pin", pin)
		->withIntParameters("value", value);
}

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
	const char inputString[] = "setLedOn";	
	expectInputString(inputString);
	expectLedOn();
	expectNoInputLeft();

	appLoopN(strlen(inputString) + 2);
}

TEST(ArduinoLeonardoTerminalTest, turnLedOff) {
	const char inputString[] = "setLedOff";	
	expectInputString(inputString);
	expectLedOff();
	expectNoInputLeft();

	appLoopN(strlen(inputString) + 2);
}

/*TEST(ArduinoLeonardoTerminalTest, getAdcA0) {
	expectInputString("getAdc_A0");
	expectReadAndSendAdcA0();
	expectNoInputLeft();

	for (int n = 0; n < 12; n++) {
		appLoop();
	}
}*/

TEST(ArduinoLeonardoTerminalTest, setMotorA) {
	const char inputString[] = "setMotorA 0.42";	
	expectInputString(inputString);
	expectMotorA(107); //0.42*255 about 107
	expectNoInputLeft();

	appLoopN(strlen(inputString) + 2);
}

TEST(ArduinoLeonardoTerminalTest, setMotorB) {
	const char inputString[] = "setMotorB -0.98";	
	expectInputString(inputString);
	expectMotorB(249); //-0.98*255 about 249
	expectNoInputLeft();

	appLoopN(strlen(inputString) + 2);
}

static void expectInputString(const char* str) {
	for (int i = 0; i < (int)strlen(str); i++) {
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

static void expectMotorA(int pwmValue) {
	expectMotor(3, pwmValue);
}	


static void expectMotorB(int pwmValue) {
	expectMotor(11, pwmValue);
}	

static void expectMotor(int pwmPin, int pwmValue) {
	switch(pwmPin) {
		case 3:
			mock().expectOneCall("pinMode")
				.withIntParameter("pin", 9)
				.withIntParameter("value", 1);
			mock().expectOneCall("digitalWrite")
				.withIntParameter("pin", 9)
				.withIntParameter("value", 0);
			
			mock().expectOneCall("pinMode")
				.withIntParameter("pin", 12)
				.withIntParameter("value", 1);
			mock().expectOneCall("digitalWrite")
				.withIntParameter("pin", 12)
				.withIntParameter("value", 1);

			break;
		case 11:
			mock().expectOneCall("pinMode")
				.withIntParameter("pin", 8)
				.withIntParameter("value", 1);
			mock().expectOneCall("digitalWrite")
				.withIntParameter("pin", 8)
				.withIntParameter("value", 0);
			
			mock().expectOneCall("pinMode")
				.withIntParameter("pin", 13)
				.withIntParameter("value", 1);
			mock().expectOneCall("digitalWrite")
				.withIntParameter("pin", 13)
				.withIntParameter("value", 0);

			break;
		default:
			FAIL("undefined pwmPin");
			break;
	}
	mock().expectOneCall("pinMode")
		.withIntParameter("pin", pwmPin)
		.withIntParameter("value", 1);
	mock().expectOneCall("analogWrite")
		.withIntParameter("pin", pwmPin)
		.withIntParameter("value", pwmValue);
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

static void appLoopN(int nrOfLoops) {
	for (int n = 0; n < nrOfLoops; n++) {
		appLoop();
	}
}
