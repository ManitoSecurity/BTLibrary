// rgb_led.cpp
// implementation of rgb led driver
// Manito Security Solutions
// Brian Gravelle
// March 14 2015

#include "rgb_led.h"


void init_rgb_led() {	
   pinMode(LED_R, OUTPUT);
   pinMode(LED_G, OUTPUT);
   pinMode(LED_B, OUTPUT);
}

void turn_on_red() {
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
}

void turn_on_green() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

void turn_on_blue() {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
}
