/*
 * This program is the controller for the Magic Water Fish ROV System
 * last updated 12/5/16  
 * For any questions please contact Justin.d.garcia@gmail.com
 * 
 * This program draws the following code and accompanying library:
 * 
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 *
 *The Purpose of this program is to recognize inputs from an XBOX 360 and use those inputs to 
 *control the underwater ROV
 *
 *The Inputs work as follows:
 *
Left Bumper             - rotate counter-clockwise around center axis (not implemented, but felt this would be the best button for such a control)
Left Trigger            - decrease maximum drive - 100%, 75%, 50%, 25%(default), 0%
Right Bumper            - rotate clockwise around the center axis (see note about Left Bumper)
Right Trigger           - increase maximum drive - 25% (default), 50%, 75%, 100%
Left Stick              - Left stick
                          Pure Y tilt (activates forward/backward thrusters(AFT L&R)) - amount of tilt is proportional to amount of thrust 
                          Y + X tilt  (activates port/starboard + Aft L&R thrusters)  - amount of tilt is proportional to amount of thrust
                          Pure X tilt (activates lateral thruster)                    - amount of tilt is proportional to amount of thrust
                          Sticks are [ -32 K --> + 32 K ] 
                          
Right Stick             - Pure Y tilt (activates vertical thruster)                   - amount of tilt is proportional to amount of thrust
Left Thumbstick Button  - no function - ALL no functions have been commented out.
Right Thumbstick Button - no function
Y Button                - toggle lights
B Button                - toggle Right Stick Camera Gimbal Control
A Button                - no function
X Button                - no function
D-pad Buttons           - no function
Back Button             - no function
Start Button            - no function
XBOX Button             - battery indicator
*
*
*/



/*
TEST FOR THE POSSIBLE BUILDUP OF CONDENSATION IN AN UNDERWATER ROV (MAGIC WATER FISH)

Written by Justin Garcia
for questions please contact @ justin.d.garcia@gmail.com
Our experiment is meant to simulate the conditions that our senior project underwater drone will be in, primarily having a hot temperature inside its case and a cold outer temperperature.
The possibility of condensation forming on the inside is a threat to our internal electronics and this investigation will tell us if insulation will be needed in our design.

The conditions:
Temperature Range: 68-93 in degrees F
                   20-34 in degrees C
Humidity Range:    77-88 % humidity

Sensors: 2 X DHT_11 (does both temperature and humidity)
Water Level Sensor  (measure voltage change to determine the level of water) In this project we are useing it only to detect the presence of water and do not care about its height.


/****************************************************/
/* 
 * connection:
 * I2C LCD1602                     SunFounder Uno R3
 * VCC                              5V
 * GND                              GND
 * SDA                              A4
 * SCL                              A5
 * Water Level Sensor                  SunFounder Uno R3
 * -                                    GND
 * +                                    VCC
 * S                                    A0
 * DHT SENSOR 1
 * 
  */
 /*
/****************************************************/



//Libraries
/*************************************************************************************************/

#include <XBOXRECV.h>   //library for XBOX Receiver

                        // Satisfy IDE, which only needs to see the include statment in the ino.
                        //the following  libraries are referenced but go unused
                        //bogus include is a trick to compile more quickly
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>        //library for Serial Peripheral Interface - allows for communication to the usb host chip
#endif    

#include <dht.h>    // This is the library for the humidity and temperature sensor
                    //This library allows the sensor to receive and transmit data over one wire.
                    
dht DHT;            //This line initializes the temperature/humidity sensor as an object
dht DHT1;           // another object

#include <Wire.h>   // This Arduino included library sets A4 and A5 for the I2C chip
                    // A4 is SDA(data line) and A5 is SCL (clock line) and handles communications to the I2C on the LCD screen
 

/***************************************************/


//Constants
/*************************************************************************************************/

USB Usb;                // There can more more than one USB constant
XBOXRECV Xbox(&Usb);
int i = 0;
int J = 1;              // This is for toggling powerlevel (gears)
int PREVIOUS_J = 0;      // To help contain serial printing
int LL = 0;
int LEDBULBS = 44;


/************************************************************************************************/
//Constants for Sensors

#define DHT_Pin 46            //This is a temperature/humidity sensor
#define DHT_Pin2 48          //This is a temperature/humidity sensor

const int waterSensor = 0;
int waterValue = 0;

/**************************************************/
//variables

long LAST_DEBOUNCE_TIME = 0;       //This is the debounce time for all buttons so that one button press is registered
long LAST_DEBOUNCE_TIME2 = 0;
long LAST_DEBOUNCE_DHT = 0;        //Both sensors need their own debounce time so they can recover and give good readings
long LAST_DEBOUNCE_DHT2 = 0;
long DEBOUNCE_DELAY = 150;        //Typical time a button is pressed by the user in milliseconds
long DEBOUNCE_DELAY2 = 4000;      //Temperature/Humidity sensors to be pinged no more than once every 2 seconds

float TEMP_AVERAGE_SYSTEM = 0;    //The DHT_11 temperature and Humidity sensor gives readings of float type
float HUMIDITY_AVERAGE_SYSTEM = 0;

float TEM_1 = 0;                  //this is the temperature read by the first sensor
float TEM_2 = 0;                  //this is the temperature read by the second sensor
//long LM_VAL = 0;                //the temperature reading of the LM35 sensor, a long value, to be converted to degrees C.
float HUM_1 = 35;                 //Started with initial value so humidifier isnt on, value of the first sensor
float HUM_2 = 35;                 //Ensures average humidity is started at 35% since values of HUM_1 and HUM-2 are averaged, value of the second sensor.
//float TEM_3 = 0;                //would be the calculated value of the LM35 sensor


/*********************************************
 * H-Bridge
 *********************************************/
//motor one
int enA = 22;
int in1 = 24;
int in2 = 26;
// motor two
int enB = 28;
int in3 = 30;
int in4 = 32;

/*********************************************
 * H-Bridge 2
 *********************************************/
//motor Three
int enA_2 = 23;
int in1_2 = 25;
int in2_2 = 27;
// motor Four
int enB_2 = 29;
int in3_2 = 31;
int in4_2 = 33;

//Voids
/*************************************************************************************************/
void motorMOVE()
{
int  K = Xbox.getAnalogHat(LeftHatX, i);
              Serial.print(F("LeftHatX: "));
              Serial.print(Xbox.getAnalogHat(LeftHatX, i));
              Serial.print("\t");
int  M = Xbox.getAnalogHat(LeftHatY, i);
              Serial.print(F("LeftHatY: "));
              Serial.print(Xbox.getAnalogHat(LeftHatY, i));
              Serial.print("\t");

Serial.println();

if (M > 9000 && K <= 9000 && K >= -9000)  //if y+
  {
  int motorENABLE = (M/516)*J;
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  analogWrite(enA, motorENABLE);
   
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enB, motorENABLE);
  }
else if (M < -9000 && K <= 9000 && K >= -9000)  //if y-
  {
  int motorENABLE = (M/516)*J*-1;
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  analogWrite(enA, motorENABLE);
   
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  analogWrite(enB, motorENABLE);
  }
else if (K > 10000 && M > 10000 )  //if y+ and x+  //else if
  {
  int motorENABLEleft = ((K/516)*J);
  int motorENABLEright = 64*J - motorENABLEleft;
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  analogWrite(enA, motorENABLEleft);
   
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enB, motorENABLEright);
  }
  else if (K < -10000 && M > 10000)  //if y+ and x-
  {
  int motorENABLEright = -1*(K/516)*J;
  int motorENABLEleft = 64*J - motorENABLEright;
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  analogWrite(enA, motorENABLEleft);
   
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enB, motorENABLEright);
  }
  else if (K < -10000 && M < -10000)  //if y- and x-
  {
  int motorENABLEright = (-1*(K/516)*J);
  int motorENABLEleft = 64*J - motorENABLEright;
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  analogWrite(enA, motorENABLEleft);
   
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  analogWrite(enB, motorENABLEright);
  }
  else if (K > 10000 && M < -10000)  //if y- and x+
  {
  int motorENABLEleft = (K/516)*J;
  int motorENABLEright = 64*J - motorENABLEleft;
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); 
  analogWrite(enA, motorENABLEleft);
   
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  analogWrite(enB, motorENABLEright);
  }
  else
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
}

void motorMoveRight ()
{
int  N = Xbox.getAnalogHat(RightHatX, i);
              Serial.print(F("RightHatX: "));
              Serial.print(Xbox.getAnalogHat(RightHatX, i));
              Serial.print("\t");
int  O = Xbox.getAnalogHat(RightHatY, i);
              Serial.print(F("RightHatY: "));
              Serial.print(Xbox.getAnalogHat(RightHatY, i));

  /***************************************
 * Lateral Motor
 */
 if (N > 9000 && O <= 9000 && O >= -9000)  //if x+
  {
  int motorENABLE = (N/(516))*J;
  digitalWrite(in1_2, HIGH);
  digitalWrite(in2_2, LOW); 
  analogWrite(enB_2, motorENABLE);
//digitalWrite(Power_2,HIGH);
  Serial.print(F("Why u no work"));

   
//  digitalWrite(in3_2, HIGH);
//  digitalWrite(in4_2, LOW); 
//  analogWrite(enB_2, motorENABLE);
  }
else if (N < -9000 && O <= 9000 && O >= -9000)  //if x-
  {
  int motorENABLE = (N/(516))*J*-1;
  digitalWrite(in1_2, LOW);
  digitalWrite(in2_2, HIGH); 
  analogWrite(enA_2, motorENABLE);
   
//  digitalWrite(in3_2, LOW);
//  digitalWrite(in4_2, HIGH); 
//  analogWrite(enB_2, motorENABLE);
  }
    else
{
  digitalWrite(in1_2, LOW);
  digitalWrite(in2_2, LOW);  
  digitalWrite(in3_2, LOW);
  digitalWrite(in4_2, LOW);
}
}

void setup() 
{
Serial.begin(115200);
#if !defined(__MIPSEL__)
while (!Serial); // Wait for serial port to connect
#endif
  if (Usb.Init() == -1) 
    {
    Serial.println(F("\r\nOSC did not start"));
    while (1); //halt
    }
  Serial.println(F("\r\nXbox Wireless Receiver Library Started"));
  Serial.println("                                                                                                    Sensor Status     Water Level     Tem_1     Hum_1     TempAverage System     HumidityAverage System     ");
pinMode(LEDBULBS,OUTPUT);
}


void loop() 
{
  Usb.Task();
  if (Xbox.XboxReceiverConnected)
  {
    for (uint8_t i = 0; i < 4; i++) // This receiver allows up to four XBOX controllers - first controller always connects as controller one, so far for this project this statement is not necessary
    {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i))   //This is for the triggers
        {          
           if (Xbox.getButtonPress(R2, i) == 255 && (millis()-LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY)
           {
           Serial.print("\tR2: ");
           Serial.println(Xbox.getButtonPress(R2, i));
           J++;
           LAST_DEBOUNCE_TIME = millis();
           }
           else if (Xbox.getButtonPress(L2, i) == 255 && (millis()-LAST_DEBOUNCE_TIME2) > DEBOUNCE_DELAY)
           {
           Serial.print("L2: ");
           Serial.println(Xbox.getButtonPress(L2, i));
           J--;
           LAST_DEBOUNCE_TIME2 = millis();
           }
        }
           if (J == 1 && PREVIOUS_J != J)
           {
           Xbox.setLedOn(LED1, i);
           Serial.print("J (gear): ");
           Serial.println(J);
           PREVIOUS_J = 1;
           }
           else if (J == 2 && PREVIOUS_J != J)
           {
           //Xbox.setLedOn(LED1, i);  // I want the controller ring to light up around the ring as power is increased but this function is a bitmask to all LEDs
           Xbox.setLedOn(LED2, i);
           Serial.print("J (gear): ");
           Serial.println(J);
           PREVIOUS_J = 2;
           }
           else if (J == 3 && PREVIOUS_J != J)
           {
           //Xbox.setLedOn(LED1, i);
           //Xbox.setLedOn(LED2, i);
           Xbox.setLedOn(LED3, i);
           Serial.print("J (gear): ");
           Serial.println(J);
           PREVIOUS_J = 3;
           }
           else if (J == 4 && PREVIOUS_J != J)
           {
           //Xbox.setLedOn(LED1, i);
           //Xbox.setLedOn(LED2, i);
           //Xbox.setLedOn(LED3, i);
           Xbox.setLedOn(LED4, i);
           Serial.print("J (gear): ");
           Serial.println(J);
           PREVIOUS_J = 4;
          }
          else if (J > 4)
          {
          J = 4;
          PREVIOUS_J = 5;
          }
          else if (J == 0 && PREVIOUS_J != J)
          {
          Xbox.setLedOff(i);
          Serial.print("J (gear): ");
          Serial.println(J);
          PREVIOUS_J = 0; 
          }
          else if (J < 0)
          {
          J = 1;
          Xbox.setLedOn(LED1,i);
          PREVIOUS_J = -1;
          }

        if (Xbox.getAnalogHat(LeftHatX, i) > 9000 || Xbox.getAnalogHat(LeftHatX, i) < -9000 || Xbox.getAnalogHat(LeftHatY, i) > 9000 || Xbox.getAnalogHat(LeftHatY, i) < -9000 || Xbox.getAnalogHat(RightHatX, i) > 9000 || Xbox.getAnalogHat(RightHatX, i) < -9000 || Xbox.getAnalogHat(RightHatY, i) > 9000 || Xbox.getAnalogHat(RightHatY, i) < -9000) 
         {
          motorMOVE();
          motorMoveRight();
        }

        if (Xbox.getButtonClick(UP, i)) // UP buton
        {
          Serial.println(F("Up"));
        }
        if (Xbox.getButtonClick(DOWN, i)) //Down Button
        {
          Serial.println(F("Down"));
        }
        if (Xbox.getButtonClick(LEFT, i)) //Left Button
        {
          Serial.println(F("Left"));
        }
        if (Xbox.getButtonClick(RIGHT, i)) //Right Button
        {
          Serial.println(F("Right"));
        }

        if (Xbox.getButtonClick(START, i)) //Start Button
        {
          Xbox.setLedMode(ALTERNATING, i);
          Serial.println(F("Start"));
        }
        if (Xbox.getButtonClick(BACK, i)) //Back Button
        {
          Xbox.setLedBlink(ALL, i);
          Serial.println(F("Back"));
        }
        if (Xbox.getButtonClick(L3, i))
          Serial.println(F("L3"));
        if (Xbox.getButtonClick(R3, i))
          Serial.println(F("R3"));

        if (Xbox.getButtonClick(L1, i))
          Serial.println(F("L1"));
        if (Xbox.getButtonClick(R1, i))
          Serial.println(F("R1"));
        if (Xbox.getButtonClick(XBOX, i)) 
        {
          Xbox.setLedMode(ROTATING, i);
          Serial.print(F("Xbox (Battery: "));
          Serial.print(Xbox.getBatteryLevel(i)); // The battery level in the range 0-3
          Serial.println(F(")"));
        }
        if (Xbox.getButtonClick(SYNC, i))
        {
          Serial.println(F("Sync"));
          Xbox.disconnect(i);
        }

        if (Xbox.getButtonClick(A, i))  //colored buttons 
          Serial.println(F("A"));
        if (Xbox.getButtonClick(B, i))
          Serial.println(F("B"));
        if (Xbox.getButtonClick(X, i))
          Serial.println(F("X"));
        if (Xbox.getButtonClick(Y, i) && (millis()-LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY)
        {
           
           LL++;
           LAST_DEBOUNCE_TIME = millis();
              if (LL == 1)
              {
                digitalWrite(LEDBULBS,HIGH);
              }
              if (LL>1)
              {
                LL = 0;
                digitalWrite(LEDBULBS, LOW);
              }

          Serial.println(F("Y"));
        }
     }
    }

/*********************    
 *     The sensors do not yet play nice with the control system
 */

 
//    float tempaverageholder = (TEM_1+TEM_2)/2;
//float tempaveragesystem = 1.8*tempaverageholder+32;//this converts the centigrade value to degrees fahrenheit for the user
//TEMP_AVERAGE_SYSTEM = tempaveragesystem;
//float humidityaveragesystem = (HUM_1 + HUM_2)/2;
//HUMIDITY_AVERAGE_SYSTEM = humidityaveragesystem;
// 
//
//int waterValue = analogRead(waterSensor);
//  
//if ((millis()- LAST_DEBOUNCE_DHT) > DEBOUNCE_DELAY2)  
//{
//   Serial.print("                                                                                                                             DHT11");
//  //read the value returned from sensor
//  int chk = DHT.read11(DHT_Pin);
//  switch (chk)
//  {
//    case DHTLIB_OK:  
//    Serial.print("OK,\t");
//    TEM_1 = DHT.temperature;
//    HUM_1 = DHT.humidity;
//    Serial.print(waterValue);
//    Serial.print("                 ");
//    Serial.print(TEM_1);
//    Serial.print("     ");
//    Serial.print(HUM_1);
//    Serial.print("     ");
//    Serial.print(TEMP_AVERAGE_SYSTEM);
//    Serial.print("                    ");
//    Serial.print(HUMIDITY_AVERAGE_SYSTEM);
//    Serial.print("     ");
//    Serial.println();
//    LAST_DEBOUNCE_DHT = millis();
//    break;
//    case DHTLIB_ERROR_CHECKSUM: 
//    Serial.println("Checksum error,\t"); 
//    LAST_DEBOUNCE_DHT = millis();
//    break;
//    case DHTLIB_ERROR_TIMEOUT: 
//    Serial.println("Time out error,\t");
//    LAST_DEBOUNCE_DHT = millis(); 
//    break;
//    default: 
//    Serial.println("Unknown error,\t");
//    LAST_DEBOUNCE_DHT = millis(); 
//    break;
//  }
//}
//
//if ((millis()- LAST_DEBOUNCE_DHT2) > DEBOUNCE_DELAY2)
//{
//Serial.print("DHT12");
//  //read the value returned from sensor
//  int chk2 = DHT1.read11(DHT_Pin2);
//  switch (chk2)
//  {
//    case DHTLIB_OK:  
//   Serial.print("OK,\t");
//   TEM_2 = DHT1.temperature;
//   HUM_2 = DHT1.humidity;
//      Serial.print(waterValue);
//      Serial.print("                 ");
//    Serial.print(TEM_2);
//    Serial.print("     ");
//    Serial.print(HUM_2);
//    Serial.print("     ");
//    Serial.print(TEMP_AVERAGE_SYSTEM);
//    Serial.print("                    ");
//    Serial.print(HUMIDITY_AVERAGE_SYSTEM);
//    Serial.print("     ");
//    Serial.println();
//   LAST_DEBOUNCE_DHT2 = millis();
//    break;
//    case DHTLIB_ERROR_CHECKSUM: 
//    Serial.println("Checksum error,\t"); 
//    break;
//    case DHTLIB_ERROR_TIMEOUT: 
//    Serial.println("Time out error,\t"); 
//    break;
//    default: 
//    Serial.println("Unknown error,\t"); 
//    break;
// }
//} 
  }

