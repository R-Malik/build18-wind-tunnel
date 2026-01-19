// Load Cell DAC (HX711) Library Wrapper

#include <HX711.h>
#include "load.h"

static HX711 load;
static float loadScale = -85343.417f;	// use spring scale with known mass
static long loadOffset = -1274351.709f;	// tare to 0 with no sample, tunnel on

bool loadBegin(uint8_t doutPin, uint8_t clkPin) {
	load.begin(doutPin, clkPin);
	// calibrate
	load.set_scale(loadScale);
	load.set_offset(loadOffset);

	// check connectivity
	if (load.wait_ready_timeout(10000)) {
		Serial.println("Load cell DAC (HX711) init succeeded.");

		// tare with 10 samples
		load.tare(10);
		loadOffset = load.get_offset();
		Serial.println("Load cell DAC (HX711) tare succeeded.");
		return true;
	} else {
		Serial.println("Load cell DAC (HX711) init failed: not connected.");
		return false;
	}
}

// calibrated for Newtons
float loadReadWeight(uint8_t samples) {
	if (load.wait_ready_timeout(100)) {
		return load.get_units(samples) * -1;
	} else {
		Serial.println("Load cell DAC (HX711) read failed: not connected.");
		return 0.0f;
	}
}