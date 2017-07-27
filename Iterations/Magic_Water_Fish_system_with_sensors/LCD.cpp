// this code makes use of the LCD library written by Fmalpartida called new liquid crystal, easily obtained on GitHub
//       Example commands given by the library.
//       lcd.setCursor(2, 0); // set cursor in (column, row)
//       lcd.print("");       // put words in the ""
//       lcd.print(value, 3); // 3 designates decimal places


//-------------------- INCLUDES ---------------------//
#include "Arduino.h"
#include "LCD.h"

#include <Wire.h>
// This Arduino included library sets A4 and A5 for the I2C chip
// A4 is SDA(data line) and A5 is SCL (clock line) and handles communications to the I2C on the LCD screen

#include <LiquidCrystal_I2C.h>

//Initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// This is the address for a specific lcd screen you wil need to run a seperate piece of code to see the address of your LCD
// The POSITIVE term in the above command turns on the backlight in the LCD

//-------------------- VARIABLES ---------------------//
// all the variables created in sensor tabs
extern int WaterLevel1;
extern int WaterLevel2;
//extern float pitch;
//extern float roll;
//extern float TEMP_AVERAGE_SYSTEM;
//extern float HUMIDITY_AVERAGE_SYSTEM;



//-------------------- CLASS ---------------------//
lcdclass::lcdclass()   // You will see the same call out in every tab to the right of this. It is essential
{
}


//-------------------- SETUP ---------------------//
void lcdclass::SETUP() // all fuctions written in a certain tab need the class identifier before the function name.
{
  lcd.begin(16, 2);      // This is specifying the number of columns (16) and rows (2) on the particular screen. This is the usable area. Change if necessary
}

//-------------------- OTHER FUNCTIONS ---------------------//

void lcdclass::lcdDisplay ()
{
  if (WaterLevel1 >= 50)          // front end sensor
  {
    Serial.println("DRONE HAS WATER!");
   
    lcd.setCursor(0, 0);
    lcd.print("DRONE HAS WATER");
  }
  else if (WaterLevel2 >= 50)    //back end sensor
  {
   
    lcd.setCursor(0, 0);
    lcd.print("DRONE HAS WATER");
  }
  else
  {
 
  // first row data
  lcd.setCursor(0,0); lcd.print("OK?");
  lcd.setCursor(4,0); lcd.print("T:");
//  lcd.setCursor(6,0); lcd.print(TEMP_AVERAGE_SYSTEM,0);//the zero in these lines makes sure that the lcd only prints the whole number value of the float
  lcd.setCursor(8,0); lcd.print("F");
  lcd.setCursor(10,0); lcd.print("H:");
//  lcd.setCursor(12,0); lcd.print(HUMIDITY_AVERAGE_SYSTEM,0);
  lcd.setCursor(15,1); lcd.print("%");
  // second row data
  lcd.setCursor(0,1); lcd.print("Pich");
//  lcd.setCursor(4,1); lcd.print(pitch,0);
  lcd.setCursor(7,1); lcd.print("!");
  lcd.setCursor(8,1); lcd.print("Rol");
//  lcd.setCursor(11,1); lcd.print(roll,0);
  lcd.setCursor(15,1); lcd.print("!");
  }
}

//-------------------- AKA ---------------------//
lcdclass screen = lcdclass();

