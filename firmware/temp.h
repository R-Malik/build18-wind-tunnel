// Header for temp.cpp
#ifndef TEMP_H
#define TEMP_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

bool tempBegin(uint8_t i2cAddr);
float tempReadC();
float tempReadF();
float tempReadPressurePa();
float tempReadHumidity();
float tempReadAltitudeM(float seaLevel_hPa);

#endif // TEMP_H
