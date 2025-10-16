#ifndef HARDWARE_H
#define HARDWARE_H

#include <Tone.h>
#include <LiquidCrystal_I2C.h> 
#include <Keypad.h>

extern LiquidCrystal_I2C lcd;
extern Keypad kpd;
extern Tone speaker;

void initializeHardware();

#endif

