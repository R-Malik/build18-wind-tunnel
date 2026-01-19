#ifndef ANGLE_H
#define ANGLE_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS5600.h>

bool angleBegin(uint8_t i2cAddr = 0x36);
float angleReadDegrees();

#endif
