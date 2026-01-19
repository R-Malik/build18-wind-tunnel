// AS5600 Magnetic Angle Sensor Wrapper
// UNUSED IN FINAL DEMONSTRATION - SENSOR NOT INTEGRATED

#include <Wire.h>
#include <Adafruit_AS5600.h>
#include "angle.h"

static Adafruit_AS5600 angle;
static uint16_t angleZeroOffset = 0;

bool angleBegin(uint8_t i2cAddr) {
	if (angle.begin(i2cAddr)) {
		Serial.println("Angle sensor (AS5600) init succeeded.");
		// calibrate
		uint32_t sum = 0;
		for (uint8_t i = 0; i < 32; ++i) {
				sum += angle.getRawAngle();
			delay(2);
		}
		uint16_t avg = static_cast<uint16_t>(sum / 32);
		// tare
		angleZeroOffset = avg % 4096;
		Serial.println("Angle sensor (AS5600) tare succeeded.");
		return true;
	} else {
		Serial.println("Angle sensor (AS5600) init failed: not connected.");
		return false;
	}
}

float angleReadDegrees() {
	uint16_t raw = angle.getRawAngle();
	uint16_t adjusted = (raw + 4096 - angleZeroOffset) % 4096;
	return static_cast<float>(adjusted) * (360.0f / 4096.0f);
}