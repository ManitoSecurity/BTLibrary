// rgb_led.h
// definitions to use rgb leds on Manito Security System
// Brian Gravelle
// March 14 2015

 
#ifndef _RGB_LED_h
#define _RGB_LED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define LED_R 9
#define LED_G 6
#define LED_B 3

void init_rgb_led();

void turn_on_red();

void turn_on_green();

void turn_on_blue();

#endif

