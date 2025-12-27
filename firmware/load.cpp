// Load Cell and HX711 DAC Library Wrapper

#include <HX711.h>

#include "load.h"

static HX711 load;
static float loadScale = 1.0f;
static long loadOffset = 0;

// Init load cell with DATA and CLK pins
bool loadBegin(uint8_t doutPin, uint8_t clkPin) {
	load.begin(doutPin, clkPin);
	load.set_scale(loadScale);
	load.set_offset(loadOffset);
	return true;
}

// Tare load cell, optionally repeating multiple times
void loadTare(uint8_t times) {
	load.tare(times);
	loadOffset = load.get_offset();
}

// Set scale factor used to convert raw to weight units
void loadSetScale(float scale) {
	loadScale = scale;
	load.set_scale(loadScale);
}

// Set a raw offset for load cell readings
void loadSetOffset(long offset) {
	loadOffset = offset;
	load.set_offset(loadOffset);
}

// Read averaged raw value from load cell
long loadReadRaw() {
	return load.read_average(5);
}

// Read weight in units using number of samples
float loadReadWeight(uint8_t samples) {
	return load.get_units(samples);
}

// Calibrate scale using known mass and sample count
void loadCalibrate(float knownMass, uint8_t samples) {
	if (knownMass == 0.0f) {
		return;
	}
	load.tare(samples);
	long raw = load.get_value(samples);
	float scale = static_cast<float>(raw) / knownMass;
	loadSetScale(scale);
	loadOffset = load.get_offset();
}

// Reset load cell calibration to defaults
void loadReset() {
	loadScale = 1.0f;
	loadOffset = 0;
	load.set_scale(loadScale);
	load.set_offset(loadOffset);
}
