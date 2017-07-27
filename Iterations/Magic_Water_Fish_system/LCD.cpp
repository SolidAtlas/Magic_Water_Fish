
#include "Arduino.h"
#include "LCD.h"
#include <Wire.h>   // This Arduino included library sets A4 and A5 for the I2C chip
                    // A4 is SDA(data line) and A5 is SCL (clock line) and handles communications to the I2C on the LCD screen


lcdclass::lcdclass()
{
  
}

void lcdclass::SETUP(){
  
}

lcdclass lcd = lcdclass();
//const uint8_t motor_pin = 8;
//
//mclass::mclass(){
//}
//
//void mclass::SETUP(){
//  pinMode(motor_pin, OUTPUT);
//}
//
//
//void mclass::SPEED(int motor_speed){
//  
//}
//
//mclass motor = mclass();
