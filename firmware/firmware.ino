// Main Firmware for Build18 Wind Tunnel

#include <Arduino.h>
#include <Wire.h>
#include "angle.h"
#include "load.h"
#include "press.h"
#include "temp.h"

#define DEBUG 1

const uint8_t LOAD_DATA = 21;
const uint8_t LOAD_CLK = 22;
const uint8_t PRESS_AOUT = A6;

const uint8_t TEMP_ADDR = 0x77;
const uint8_t ANGLE_ADDR = 0x36;

const unsigned long SENSOR_POLL_MS = 200;

struct SensorData {
	unsigned long timestampMs;
	float temperatureC;
	float pressureKPa;
	float humidity;
	float angleDeg;
	float loadWeight;
};

void readAllSensors(SensorData &out);
void printSensorData(const SensorData &d);
void printSensorDataDebug(const SensorData &d);

void setup() {
	Serial.begin(115200);
	while (!Serial && millis() < 2000) { }
	Serial.println("Welcome Wind Tunnelers!");

	Wire.begin();
	analogReadResolution(10);
	
	loadBegin(LOAD_DATA, LOAD_CLK);
	pressBegin(PRESS_AOUT);
	tempBegin(TEMP_ADDR);
	angleBegin(ANGLE_ADDR);

	Serial.println("Finished setup.");
}

void loop() {
	static unsigned long lastPoll = 0;
	unsigned long now = millis();
	if (now - lastPoll >= SENSOR_POLL_MS) {
		lastPoll = now;
		SensorData data;
		readAllSensors(data);
		#if DEBUG
		printSensorDataDebug(data);
		#else
		printSensorData(data);
		#endif
	}
}

void readAllSensors(SensorData &out) {
	out.timestampMs = millis();
	out.temperatureC = tempReadC();
	out.pressureKPa = pressReadKPa();
	out.humidity = tempReadHumidityPercent();
	out.angleDeg = angleReadDegrees();
	out.loadWeight = loadReadWeight(5);
}

void printSensorData(const SensorData &d) {
	Serial.print(d.timestampMs);
	Serial.print(",");
	Serial.print(d.temperatureC, 2);
	Serial.print(",");
	Serial.print(d.pressureKPa, 3);
	Serial.print(",");
	Serial.print(d.humidity, 2);
	Serial.print(",");
	Serial.print(d.angleDeg, 2);
	Serial.print(",");
	Serial.println(d.loadWeight, 3);
}

void printSensorDataDebug(const SensorData &d) {
	Serial.print(d.timestampMs);
	Serial.print(" ms, ");
	Serial.print(d.temperatureC, 2);
	Serial.print(" temp (C), ");
	Serial.print(d.pressureKPa, 3);
	Serial.print(" kPa,");
	Serial.print(d.humidity, 2);
	Serial.print(" \% humid,");
	Serial.print(d.angleDeg, 2);
	Serial.print(" degrees, ");
	Serial.print(d.loadWeight, 3);
	Serial.println(" Netwons");
}