// MPXV7002DP Pressure Sensor Library Wrapper

#include <Arduino.h>
#include "press.h"

static uint8_t pressAnalogPin;
static float adcRef = 3.3f;
static float adcMax = 1023.0f;
static float sensorOffsetV = 2.5f;
static float sensorVPerKPa = 2.0f;
static float sensorOffsetP = 0.305f;	// manually tare when fan off
static const float sensorSupplyV = 5.0f;

bool pressBegin(uint8_t analogPin) {
	pressAnalogPin = analogPin;
	sensorOffsetV = sensorSupplyV * 0.5f;
	sensorVPerKPa = sensorSupplyV * 0.2f;

	// check connectivity by sanity checking samples
	long sum = 0;
	for (int i = 0; i < 8; ++i) {
		int raw = analogRead(pressAnalogPin);
		sum += raw;
		delay(5);
	}
	float avg = static_cast<float>(sum) / 8;
	if (avg < (adcMax * 0.05f) || avg > (adcMax * 0.95f)) {
		Serial.println("Pressure sensor (MPXV7002DP) init failed: not connected.");
		return false;
	}
	Serial.println("Pressure sensor (MPXV7002DP) init succeeded.");
	return true;
}

float pressReadKPa() {
	int raw = analogRead(pressAnalogPin);
	float sensorV = (static_cast<float>(raw) / adcMax) * adcRef;
	return ((sensorV - sensorOffsetV) / sensorVPerKPa) - sensorOffsetP;
}