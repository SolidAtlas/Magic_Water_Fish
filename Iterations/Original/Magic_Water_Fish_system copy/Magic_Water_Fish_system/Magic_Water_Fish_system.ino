
//-------------------- INCLUDES ---------------------//
#include "Arduino.h"
#include <EEPROM.h>
#include "xbox_controller.h"
#include "motors_esc.h"
#include "water_sensor.h"
#include "gyro_accel.h"
#include "humiture.h"
#include "LCD.h"
#include "Lights.h"

void setup() {
  lcd.SETUP();
  controls.SETUP();
  motors.SETUP(); 
}

void loop() {
controls.Controls();
}
