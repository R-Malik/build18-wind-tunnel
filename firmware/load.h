// Header for load.cpp
#ifndef LOAD_H
#define LOAD_H

#include <Arduino.h>
#include <HX711.h>

bool loadBegin(uint8_t doutPin, uint8_t clkPin);
void loadTare(uint8_t times);
void loadSetScale(float scale);
void loadSetOffset(long offset);
long loadReadRaw();
float loadReadWeight(uint8_t samples);
void loadCalibrate(float knownMass, uint8_t samples);
void loadReset();

#endif // LOAD_H
