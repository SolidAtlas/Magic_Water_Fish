
#include "Arduino.h"
#include "Lights.h"

int LEDBULBS = 46;

lightsclass::lightsclass(){
  
}

void lightsclass::SETUP(){
  pinMode(LEDBULBS,OUTPUT);
}

lightsclass lights = lightsclass();




