//-------------------- INCLUDES ---------------------//
#include "Arduino.h"
#include "water_sensor.h"
//-------------------- VARIABLES ---------------------//
const int WaterPin1 = A0;
const int WaterPin2 = A1;

int WaterLevel1;
int WaterLevel2;

//-------------------- CLASS ---------------------//
waterclass::waterclass()
{  
}
//-------------------- SETUP ---------------------//
void waterclass::SETUP()
{
 pinMode(WaterPin1, INPUT);
 pinMode(WaterPin2, INPUT);
}
//-------------------- FUNCTIONS ---------------------//
void waterclass::Water()
{
  WaterLevel1=  analogRead(WaterPin1);
  WaterLevel2 = analogRead(WaterPin2);
}
//-------------------- AKA ---------------------//
waterclass water = waterclass();

