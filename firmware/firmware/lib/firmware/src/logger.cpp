#include <SD.h>

static void log(String text);
static void initLog(void);

static const char FileName[] = "log.txt";
static File logFile;
static String logBuffer;
static const unsigned int LogBufferSize = 200;
static bool isInitialized = false;

void logMessage(String text){
	log("Message: " + text);
}

void logWarning(String text){
	log("Warning: " + text);
}

void logError(String text){
	log("Error: " + text);
}

static void log(String text){
	if(!isInitialized){
		initLog();
	}
	const String LogBufferFullText = "";
	const String LineTerminator = "\r\n";
	String timeString = String(millis());
	logBuffer += timeString + " " + text + LineTerminator;
	unsigned int logBufferLength = logBuffer.length();

	if(logBufferLength >= LogBufferSize*0.75){
		logBuffer += timeString + " Warning: running out of log-buffer " + LineTerminator;
		logBufferLength = logBuffer.length();
	}
	
	unsigned int writableBytes = logFile.availableForWrite();
	if(logBufferLength > 0){
		unsigned int bytesToWrite = min(logBufferLength, writableBytes);
		logFile.write(logBuffer.c_str(), bytesToWrite);
		logBuffer.remove(0, bytesToWrite);
	}
}

static void initLog(void){
	const int ChipSelectPin = 4;
	SD.begin(ChipSelectPin);
	logFile = SD.open(FileName, FILE_WRITE);
	logBuffer.reserve(LogBufferSize);
	isInitialized = true;
}
