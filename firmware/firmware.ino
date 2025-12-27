// Main Firmware for Build18 Wind Tunnel

/* TODO:
	- Refactor
*/

#include <Arduino.h>
#include <Wire.h>

#include "angle.h"
#include "load.h"
#include "press.h"
#include "temp.h"

const uint8_t LOAD_DATA = 20;
const uint8_t LOAD_CLK = 21;
const uint8_t PRESS_AOUT = A5;

const uint8_t TEMP_ADDR = 0x76;
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

void setup() {
	Serial.begin(115200);
	while (!Serial && millis() < 2000) {
		// await host
	}

	Wire.begin();
	analogReadResolution(10);	// matches g_adcMax in pressure wrapper

	bool okTemp = tempBegin(TEMP_ADDR);
	if (!okTemp) {
		Serial.println("Temp sensor (BME280) init failed; check I2C address/wiring.");
	}

	bool okAngle = angleBegin(ANGLE_ADDR);
	if (!okAngle) {
		Serial.println("Angle sensor (AS5600) init failed; check I2C address/wiring.");
	}

	bool okPress = pressBegin(PRESS_AOUT);
	if (!okPress) {
		Serial.println("Pressure sensor init failed; check analog pin/wiring.");
	}

	bool okLoad = loadBegin(LOAD_DATA, LOAD_CLK);
	if (!okLoad) {
		Serial.println("Load cell DAC (HX711) init failed; check DATA/CLK pins and wiring.");
	}

	if (okAngle) {
		angleCalibrateZero(32);
	}
	if (okLoad) {
		loadTare(10);
	}
}

void loop() {
	static unsigned long lastPoll = 0;
	unsigned long now = millis();
	if (now - lastPoll >= SENSOR_POLL_MS) {
		lastPoll = now;
		SensorData data;
		readAllSensors(data);
		printSensorData(data);
	}
}

void readAllSensors(SensorData &out) {
	out.timestampMs = millis();
	out.temperatureC = tempReadC();
	out.pressureKPa = pressReadKPa();
	out.humidity = tempReadHumidity();
	out.angleDeg = angleReadDegrees();
	out.loadWeight = loadReadWeight(5);
}

void printSensorData(const SensorData &d) {
	Serial.print(d.timestampMs);
	Serial.print(',');
	Serial.print(d.temperatureC, 2);
	Serial.print(',');
	Serial.print(d.pressureKPa, 3);
	Serial.print(',');
	Serial.print(d.humidity, 2);
	Serial.print(',');
	Serial.print(d.angleDeg, 2);
	Serial.print(',');
	Serial.println(d.loadWeight, 3);
}
