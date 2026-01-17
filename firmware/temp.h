#ifndef TEMP_H
#define TEMP_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

bool tempBegin(uint8_t i2cAddr);
float tempReadC();
float tempReadHumidityPercent();

#endif
