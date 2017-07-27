//-------------------- INCLUDES ---------------------//
#include "Arduino.h"
#include "Lights.h"
//-------------------- VARIABLES ---------------------//
int LEDBULBS = 46;
//-------------------- CLASS ---------------------//
lightsclass::lightsclass()
{
}
//-------------------- SETUP ---------------------//
void lightsclass::SETUP()
{
  pinMode(LEDBULBS, OUTPUT);
}
//-------------------- AKA ---------------------//
lightsclass lights = lightsclass();




