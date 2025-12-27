// Header for press.cpp
#ifndef PRESS_H
#define PRESS_H

#include <Arduino.h>

bool pressBegin(uint8_t analogPin);
float pressReadVoltage();
float pressReadSensorVoltage();
float pressReadKPa();
float pressReadPsi();
void pressSetCalibration(float vOffset, float vPerKPa);
void pressCalibrate(float vKnown, float kpaKnown);
void pressReset();

#endif // PRESS_H
