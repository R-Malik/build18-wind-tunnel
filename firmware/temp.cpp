// BME280 Temperature/Pressure/Humidity Sensor Wrapper

#include <Wire.h>
#include <Adafruit_BME280.h>
#include "temp.h"

static Adafruit_BME280 temp;

bool tempBegin(uint8_t i2cAddr) {
	if (temp.begin(i2cAddr)) {
		Serial.println("Temp sensor (BME280) init succeeded.");
		return true;
	} else {
		Serial.println("Temp sensor (BME280) init failed: not connected.");
		return false;
	}
}

float tempReadC() {
	return temp.readTemperature();
}

float tempReadPressurePa() {
	return temp.readPressure();
}

float tempReadHumidityPercent() {
	return temp.readHumidity();
}
