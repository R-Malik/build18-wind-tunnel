// MPXV7002DP Pressure Sensor Library Wrapper (Analog)

#include <Arduino.h>

#include "press.h"

static uint8_t pressAnalogPin;
static float adcRef = 3.3f;
static float rTop = 3300.0f;
static float rBottom = 10000.0f;
static float adcMax = 1023.0f;				// adjust if analogReadResolution changes

static float sensorOffsetV = 2.5f;			// Vout at 0 kPa (Vs * 0.5)
static float sensorVPerKPa = 1.0f;			// slope (Vs * 0.2)
static const float sensorSupplyV = 5.0f;

// Init pressure sensor parameters and pins
bool pressBegin(uint8_t analogPin) {
	pressAnalogPin = analogPin;
	sensorOffsetV = sensorSupplyV * 0.5f;
	sensorVPerKPa = sensorSupplyV * 0.2f;
	return true;
}

// Read ADC voltage from pressure sensor pin
float pressReadVoltage() {
	int raw = analogRead(pressAnalogPin);
	return (static_cast<float>(raw) / adcMax) * adcRef;
}

// Calculate sensor output voltage after resistor divider
float pressReadSensorVoltage() {
	float adcV = pressReadVoltage();
	return adcV * ((rTop + rBottom) / rBottom);
}

// Convert sensor voltage to pressure in kPa
float pressReadKPa() {
	float sensorV = pressReadSensorVoltage();
	return (sensorV - sensorOffsetV) / sensorVPerKPa;
}

// Return pressure in PSI
float pressReadPsi() {
	return pressReadKPa() * 0.1450377f;
}

// Set calibration offsets for pressure sensor
void pressSetCalibration(float vOffset, float vPerKPa) {
	sensorOffsetV = vOffset;
	sensorVPerKPa = vPerKPa;
}

// Calibrate sensor using known voltage and corresponding kPa
void pressCalibrate(float vKnown, float kpaKnown) {
	if (kpaKnown == 0.0f) {
		sensorOffsetV = vKnown;
	} else {
		sensorVPerKPa = (vKnown - sensorOffsetV) / kpaKnown;
	}
}

// Reset pressure calibration values to defaults
void pressReset() {
	sensorOffsetV = sensorSupplyV * 0.5f;
	sensorVPerKPa = sensorSupplyV * 0.2f;
}
