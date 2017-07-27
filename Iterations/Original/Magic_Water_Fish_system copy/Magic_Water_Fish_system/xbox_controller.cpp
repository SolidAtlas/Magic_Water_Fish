
#include "Arduino.h"
#include "xbox_controller.h"
#include "motors_esc.h"
#include <XBOXRECV.h>

                        // Satisfy IDE, which only needs to see the include statment in the ino.
                        //the following  libraries are referenced but go unused
                        //bogus include is a trick to compile more quickly
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>        //library for Serial Peripheral Interface - allows for communication to the usb host chip
#endif    

USB Usb;                // There can more more than one USB constant
XBOXRECV Xbox(&Usb);

uint8_t i = 0;
int J = 1; // This is for toggling powerlevel (gears)
int K = 0;
int M;
int N;
int O;

int PREVIOUS_J = 0;     // To help contain serial printing
int LL = 0;
extern int LEDBULBS;

long LAST_DEBOUNCE_TIME = 0;       //This is the debounce time for all buttons so that one button press is registered
long LAST_DEBOUNCE_TIME2 = 0;


long DEBOUNCE_DELAY = 150;        //Typical time a button is pressed by the user in milliseconds



controlsclass::controlsclass(){
}

void controlsclass::SETUP(){
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
  Serial.println(" ");
}




void controlsclass::Controls()
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
          K = Xbox.getAnalogHat(LeftHatX, i);
              Serial.print(F("LeftHatX: "));
              Serial.print(Xbox.getAnalogHat(LeftHatX, i));
              Serial.print("\t");
              Serial.print("K");
              Serial.print(K);
              
           M = Xbox.getAnalogHat(LeftHatY, i);
              Serial.print(F("LeftHatY: "));
              Serial.print(Xbox.getAnalogHat(LeftHatY, i));
              Serial.print("\t");

           N = Xbox.getAnalogHat(RightHatX, i);
              Serial.print(F("RightHatX: "));
              Serial.print(Xbox.getAnalogHat(RightHatX, i));
              Serial.print("\t");
           O = Xbox.getAnalogHat(RightHatY, i);
              Serial.print(F("RightHatY: "));
              Serial.print(Xbox.getAnalogHat(RightHatY, i));
              
         motors.motorMOVE(J,K,M,N,O);
         motors.motorMoveLATERAL(J,K,M,N,O);
         motors.motorMoveUP(J,K,M,N,O);
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
}



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
