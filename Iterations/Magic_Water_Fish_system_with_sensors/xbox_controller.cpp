//-------------------- INCLUDES ---------------------//
#include "Arduino.h"
#include "xbox_controller.h"
#include <XBOXRECV.h>

//Satisfy IDE, which only needs to see the include statment in the ino.
//the following  libraries are referenced but go unused
//bogus include is a trick to compile more quickly
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>        //library for Serial Peripheral Interface - allows for communication to the usb host chip
#endif

USB Usb;                // There can more more than one USB constant
XBOXRECV Xbox(&Usb);
#include <Servo.h>


//-------------------- VARIABLES ---------------------//
Servo RightESC, LeftESC, UpESC, SecondUpESC; //Create as much as Servo objects you want. You can control 2 or more Servos at the same time
uint8_t i = 0;
int J = 1; // This is for toggling powerlevel (gears)
int K;
int M;
int N;
int O;


// OUTPUTS
int     LMotMixL;           // Motor (left)  mixed output           (-32767..32767)
int     LMotMixR;           // Motor (right) mixed output           (-32767..32767)

//int     RMotMixL;           // Motor (left)  mixed output           (-32767..32767)
//int     RMotMixR;           // Motor (right) mixed output           (-32767..32767)

int     UpMotMixBoth;


// TEMP VARIABLES
float   LMotPremixL;    // Motor (left)  premixed output        (-32767..32767)
float   LMotPremixR;    // Motor (right) premixed output        (-32767..32767)

//float   RMotPremixL;    // Motor (left)  premixed output        (-32767..32767)
//float   RMotPremixR;    // Motor (right) premixed output        (-32767..32767)

float   UpMotPremixBoth;

int     nPivSpeed;      // Pivot Speed                          (-128..+127)
float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )
float   fPivYLimit = 9000.0;




int PREVIOUS_J = 0;     // To help contain serial printing
int LL = 0;
extern int LEDBULBS;

long LAST_DEBOUNCE_TIME = 0;       //This is the debounce time for all buttons so that one button press is registered
long LAST_DEBOUNCE_TIME2 = 0;


long DEBOUNCE_DELAY = 150;        //Typical time a button is pressed by the user in milliseconds


//-------------------- CLASS ---------------------//
controlsclass::controlsclass() {
}

//-------------------- SETUP ---------------------//
void controlsclass::SETUP() {

  RightESC.attach(23);    LeftESC.attach(27);    UpESC.attach(31);    SecondUpESC.attach(35); //like them all in one row these are pins

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



//-------------------- FUNCTIONS ---------------------//
void controlsclass::Controls()
{
  Usb.Task();
  if (Xbox.XboxReceiverConnected)
  {
    for (uint8_t i = 0; i < 4; i++) // This receiver allows up to four XBOX controllers - first controller always connects as controller one, so far for this project this statement is not necessary
    {

      if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i))   //This is for the triggers
      {
        if (Xbox.getButtonPress(R2, i) == 255 && (millis() - LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY)
        {
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          J++;
          LAST_DEBOUNCE_TIME = millis();
        }
        else if (Xbox.getButtonPress(L2, i) == 255 && (millis() - LAST_DEBOUNCE_TIME2) > DEBOUNCE_DELAY)
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
        Xbox.setLedOn(LED1, i);
        PREVIOUS_J = -1;
      }

//      if (Xbox.getAnalogHat(LeftHatX, i) < 9000 && Xbox.getAnalogHat(LeftHatX, i) > -9000 || Xbox.getAnalogHat(LeftHatY, i) < 9000 && Xbox.getAnalogHat(LeftHatY, i) > -9000)
//      {
//       RightESC.writeMicroseconds(1100);//change to 1500 for blue robotics motors
//       LeftESC.writeMicroseconds(1100); 
//       Serial.print("Stall On Left Stick");
//      }
//      if (Xbox.getAnalogHat(RightHatX, i) < 9000 && Xbox.getAnalogHat(RightHatX, i) > -9000 || Xbox.getAnalogHat(RightHatY, i) < 9000 && Xbox.getAnalogHat(RightHatY, i) > -9000)
//      {
//       UpESC.writeMicroseconds(1100);
//       SecondUpESC.writeMicroseconds(1100); 
//       Serial.print("Stall On Right Stick");
//      }                                       // Does not like these statements!!
      
      if (Xbox.getAnalogHat(LeftHatX, i) > 9000 || Xbox.getAnalogHat(LeftHatX, i) < -9000 || Xbox.getAnalogHat(LeftHatY, i) > 9000 || Xbox.getAnalogHat(LeftHatY, i) < -9000 || Xbox.getAnalogHat(RightHatX, i) > 9000 || Xbox.getAnalogHat(RightHatX, i) < -9000 || Xbox.getAnalogHat(RightHatY, i) > 9000 || Xbox.getAnalogHat(RightHatY, i) < -9000)
      {
        K = Xbox.getAnalogHat(LeftHatX, i);
        Serial.print(F("LeftHatX: "));
        Serial.print(Xbox.getAnalogHat(LeftHatX, i));
        Serial.print("\t");

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
        Serial.println(Xbox.getAnalogHat(RightHatY, i));
        // Calculate Drive Turn output due to Joystick X input
        
        if (M >= 0)
        {
          // Forward
          LMotPremixL = (K >= 0) ? 32767.0 : (32767 + K);
          LMotPremixR = (K >= 0) ? (32767.0 - K) : 32767.0;
          // Scale Drive output due to Joystick Y input (throttle)
          LMotPremixL = LMotPremixL * M / 32767.0;
          LMotPremixR = LMotPremixR * M / 32767.0;
          // Now calculate pivot amount
          // - Strength of pivot (nPivSpeed) based on Joystick X input
          // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
          nPivSpeed = K;
          //nPivSpeed = map(nPivSpeed,0,32767,1485,1900);
          fPivScale = (abs(M)>fPivYLimit)? 0.0 : (1.0 - abs(M)/fPivYLimit);
          nPivSpeed = map(nPivSpeed,0,32767,1485,1900);
          // Calculate final mix of Drive and Pivot
          LMotMixL = (1.0-fPivScale)*LMotPremixL + fPivScale*( nPivSpeed);
          LMotMixR = (1.0-fPivScale)*LMotPremixR + fPivScale*(-nPivSpeed);
          LMotMixL = map(LMotPremixL, 0, 32767, 1500, 1900);
          LMotMixR = map(LMotPremixR, 0, 32767, 1500, 1900);        
          RightESC.writeMicroseconds(LMotMixL);
          LeftESC.writeMicroseconds(LMotMixR);
          Serial.println();
          Serial.print("\t");
          Serial.print("Left Enable is ");
          Serial.print(LMotMixL);
          Serial.print("\t");
          Serial.print("Right Enable is ");
          Serial.println(LMotMixR);
          
// Uncomment/Comment to turn on the equation checker in the Serial Monitor
          Serial.println();
          Serial.print("Equation Checker");
          Serial.print("Pivot Scale is ");
          Serial.print(fPivScale);
          Serial.print("\t");
          Serial.print("Pivot Speed is ");
          Serial.print(nPivSpeed);
          Serial.print("\t");
          Serial.print("L motor Premix ");
          Serial.print(LMotPremixL);
          Serial.print("\t");
          Serial.print("R motor Premix ");
          Serial.print(LMotPremixR);
        //  Serial.println();
// Uncomment/Comment to turn on the equation checker in the Serial Monitor^^^^^
        }
        else if (M < 0)
        {
          // Reverse
          LMotPremixL = (K >= 0) ? (-32767.0 - K) : -32767.0;
          LMotPremixR = (K >= 0) ? -32767.0 : (-32767.0 + K);
          // Scale Drive output due to Joystick Y input (throttle)
          LMotPremixL = LMotPremixL * M / -32767.0;
          LMotPremixR = LMotPremixR * M / -32767.0;
          // Now calculate pivot amount
          // - Strength of pivot (nPivSpeed) based on Joystick X input
          // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
          nPivSpeed = K;
          fPivScale = (abs(M)>fPivYLimit)? 0.0 : (1.0 - abs(M)/fPivYLimit);
          // Calculate final mix of Drive and Pivot
          LMotMixL = (1.0-fPivScale)*LMotPremixL + fPivScale*( nPivSpeed);
          LMotMixR = (1.0-fPivScale)*LMotPremixR + fPivScale*(-nPivSpeed);
          LMotMixL = map(LMotPremixL, 0, -32767, 1500, 1100);
          LMotMixR = map(LMotPremixR, 0, -32767, 1500, 1100);
          RightESC.writeMicroseconds(LMotMixL);
          LeftESC.writeMicroseconds(LMotMixR);
          Serial.println();
          Serial.print("\t");
          Serial.print("Left Enable is ");
          Serial.print(LMotMixL);
          Serial.print("\t");
          Serial.print("Right Enable is ");
          Serial.print(LMotMixR);
// Uncomment/Comment to turn on the equation checker in the Serial Monitor
          Serial.println();
          Serial.print("Equation Checker");
          Serial.print("Pivot Scale is ");
          Serial.print(fPivScale);
          Serial.print("\t");
          Serial.print("Pivot Speed is ");
          Serial.print(nPivSpeed);
          Serial.print("\t");
          Serial.print("L motor Premix ");
          Serial.print(LMotPremixL);
          Serial.print("\t");
          Serial.print("R motor Premix ");
          Serial.print(LMotPremixR);
// Uncomment/Comment to turn on the equation checker in the Serial Monitor^^^^^
          
         // Serial.println();
        }
        if (O >= 0)
        {
          // Forward
          UpMotPremixBoth = (N >= 0) ? 32767.0 : (32767 + N);
          // Scale Drive output due to Joystick Y input (throttle)
          UpMotPremixBoth = UpMotPremixBoth * O / 32767.0;
          UpMotMixBoth = map(UpMotPremixBoth, 0, 32767, 1485, 1900);
          UpESC.writeMicroseconds(UpMotMixBoth);
          SecondUpESC.writeMicroseconds(UpMotMixBoth);
          Serial.println();
          Serial.print("\t");
          Serial.print("Up Enable is ");
          Serial.println(UpMotMixBoth);
          Serial.println();
        }
        else //if (O < 0)
        {
          // Reverse
          UpMotPremixBoth = (N >= 0) ? -32767.0 : (-32767 + N);
          // Scale Drive output due to Joystick Y input (throttle)
          UpMotPremixBoth = UpMotPremixBoth * O / -32767.0;
          UpMotMixBoth = map(UpMotPremixBoth, 0, -32767, 1515, 1100);
          UpESC.writeMicroseconds(UpMotMixBoth);
          SecondUpESC.writeMicroseconds(UpMotMixBoth);
          Serial.println();
          Serial.print("\t");
          Serial.print("Up Enable is ");
          Serial.println(UpMotMixBoth);
          Serial.println();
        }
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
//        RightESC.writeMicroseconds(1500);
//        LeftESC.writeMicroseconds(1500);
//        UpESC.writeMicroseconds(1500);
//        SecondUpESC.writeMicroseconds(1500);
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
      if (Xbox.getButtonClick(Y, i) && (millis() - LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY)
      {

        LL++;
        LAST_DEBOUNCE_TIME = millis();
        if (LL == 1)
        {
          digitalWrite(LEDBULBS, HIGH);
        }
        if (LL > 1)
        {
          LL = 0;
          digitalWrite(LEDBULBS, LOW);
        }

        Serial.println(F("Y"));
      }
    }
  }
}

//-------------------- AKA  ---------------------//
controlsclass controls = controlsclass();   //this was missing before july 13th 2017: unknown consequences.





