#include <Arduino.h>

#include "itHandler.h" //TODO: warum wird dieser include benötigt damit es kompiliert?
#include "logger.h"

static unsigned long oldMillis;

void setup(void) {
	oldMillis = 0;
}

void loop(void) {
	unsigned long currentMillis = millis();
	unsigned long diff = currentMillis - oldMillis;
	if(diff >= 1){
		logMessage("This is a message.");
		logWarning("This is a warning.");
		logError("This is an error.");
		oldMillis = currentMillis;
	}
}
