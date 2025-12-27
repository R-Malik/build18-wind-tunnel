// BME280 Temperature/Pressure/Humidity Sensor Wrapper (I2C)

#include <Wire.h>
#include <Adafruit_BME280.h>

#include "temp.h"

static Adafruit_BME280 temp;

// Init temp sensor at I2C address
bool tempBegin(uint8_t i2cAddr) {
	return temp.begin(i2cAddr);
}

// Read temp in C
float tempReadC() {
	return temp.readTemperature();
}

// Read temp in F
float tempReadF() {
	return (tempReadC() * 9.0f / 5.0f) + 32.0f;
}

// Read pressure in Pa
float tempReadPressurePa() {
	return temp.readPressure();
}

// Read relative humidity %
float tempReadHumidity() {
	return temp.readHumidity();
}

// Read altitude in meters given sea level pressure in hPa
float tempReadAltitudeM(float seaLevel_hPa) {
	return temp.readAltitude(seaLevel_hPa);
}
