#ifndef LOAD_H
#define LOAD_H

#include <Arduino.h>
#include <HX711.h>

bool loadBegin(uint8_t doutPin, uint8_t clkPin);
float loadReadWeight(uint8_t samples);

#endif
