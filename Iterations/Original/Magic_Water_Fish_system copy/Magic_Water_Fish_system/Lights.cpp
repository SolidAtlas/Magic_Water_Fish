
#include "Arduino.h"
#include "Lights.h"

int LEDBULBS = 44;

lightsclass::lightsclass(){
  
}

void lightsclass::SETUP(){
  pinMode(LEDBULBS,OUTPUT);
}

lightsclass lights = lightsclass();




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
