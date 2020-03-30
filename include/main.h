#pragma once

#include <Arduino.h>

#include <avr/dtostrf.h>

#include "gateway.h"



#define LED_SHIELD 11

void init_canbus();


void pos_geben();

void sdoInit(CDHD_Drive& node);
void motorInit(CDHD_Drive& node);

