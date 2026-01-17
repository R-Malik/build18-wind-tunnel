#ifndef PRESS_H
#define PRESS_H

#include <Arduino.h>

bool pressBegin(uint8_t analogPin);
float pressReadKPa();

#endif
