// AS5600 Magnetic Angle Sensor Wrapper (I2C)

#include <Wire.h>
#include <Adafruit_AS5600.h>

#include "angle.h"

static Adafruit_AS5600 angle;
static uint16_t angleZeroOffset = 0;

// Init angle sensor with given I2C address
bool angleBegin(uint8_t i2cAddr) {
	return angle.begin(i2cAddr);
}

// Read raw angle value (0..4095) adjusted by zero offset
uint16_t angleReadRaw() {
	uint16_t raw = angle.getRawAngle();
	uint16_t adjusted = (raw + 4096 - angleZeroOffset) % 4096;
	return adjusted;
}

// Return current angle in deg
float angleReadDegrees() {
	return static_cast<float>(angleReadRaw()) * (360.0f / 4096.0f);
}

// Set zero offset for angle sensor
void angleSetZero(uint16_t rawZero) {
	angleZeroOffset = rawZero % 4096;
}

// Calibrate and set zero offset by averaging number of samples
void angleCalibrateZero(uint8_t samples) {
	if (samples == 0) {
		samples = 1;
	}
	uint32_t sum = 0;
	for (uint8_t i = 0; i < samples; ++i) {
			sum += angle.getRawAngle();
		delay(2);
	}
	uint16_t avg = static_cast<uint16_t>(sum / samples);
	angleSetZero(avg);
}

// Reset angle zero offset to default (0)
void angleReset() {
	angleZeroOffset = 0;
}
