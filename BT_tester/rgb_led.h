// rgb_led.h
// definitions to use rgb leds on Manito Security System
// Brian Gravelle
// March 14 2015

 
#ifndef _RGB_LED_h
#define _RGB_LED_h

#include <Arduino.h>

#define LED_R 9
#define LED_G 6
#define LED_B 3
#define OFF   0
#define ON    5

void init_rgb_led();

void turn_on_red();

void turn_on_green();

void turn_on_blue();

#endif

