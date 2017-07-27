
#include "Arduino.h"
#include "water_sensor.h"


const int waterSensor = 0;
int waterValue = 0;

waterclass::waterclass()
{
  
}

void waterclass::SETUP()
{
  
}

waterclass water = waterclass();
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
