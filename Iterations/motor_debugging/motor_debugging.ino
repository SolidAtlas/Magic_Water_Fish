
#include <XBOXRECV.h>   //library for XBOX Receiver

                        // Satisfy IDE, which only needs to see the include statment in the ino.
                        //the following  libraries are referenced but go unused
                        //bogus include is a trick to compile more quickly
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>        //library for Serial Peripheral Interface - allows for communication to the usb host chip
#endif        

USB Usb;                // There can more more than one USB constant
XBOXRECV Xbox(&Usb);
int i = 0;
int J = 4;    
/*
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
//const int Power_2 = 44;
//motor Three
int enA_2 = 23;
int in1_2 = 25;
int in2_2 = 27;
// motor Four
int enB_2 = 29;
int in3_2 = 31;
int in4_2 = 33;

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
//digitalWrite(Power_2,HIGH);
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
 // pinMode(Power_2,OUTPUT);
}




void loop() 
{
  Usb.Task();
  if (Xbox.XboxReceiverConnected)
  {
    for (uint8_t i = 0; i < 4; i++) // This receiver allows up to four XBOX controllers - first controller always connects as controller one, so far for this project this statement is not necessary
    {
//        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i))   //This is for the triggers
//        {          
//           if (Xbox.getButtonPress(R2, i) == 255 && (millis()-LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY)
//           {
//           Serial.print("\tR2: ");
//           Serial.println(Xbox.getButtonPress(R2, i));
//           J++;
//           LAST_DEBOUNCE_TIME = millis();
//           }
//           else if (Xbox.getButtonPress(L2, i) == 255 && (millis()-LAST_DEBOUNCE_TIME2) > DEBOUNCE_DELAY)
//           {
//           Serial.print("L2: ");
//           Serial.println(Xbox.getButtonPress(L2, i));
//           J--;
//           LAST_DEBOUNCE_TIME2 = millis();
//           }
//        }
//           if (J == 1 && PREVIOUS_J != J)
//           {
//           Xbox.setLedOn(LED1, i);
//           Serial.print("J (gear): ");
//           Serial.println(J);
//           PREVIOUS_J = 1;
//           }
//           else if (J == 2 && PREVIOUS_J != J)
//           {
//           //Xbox.setLedOn(LED1, i);  // I want the controller ring to light up around the ring as power is increased but this function is a bitmask to all LEDs
//           Xbox.setLedOn(LED2, i);
//           Serial.print("J (gear): ");
//           Serial.println(J);
//           PREVIOUS_J = 2;
//           }
//           else if (J == 3 && PREVIOUS_J != J)
//           {
//           //Xbox.setLedOn(LED1, i);
//           //Xbox.setLedOn(LED2, i);
//           Xbox.setLedOn(LED3, i);
//           Serial.print("J (gear): ");
//           Serial.println(J);
//           PREVIOUS_J = 3;
//           }
//           else if (J == 4 && PREVIOUS_J != J)
//           {
//           //Xbox.setLedOn(LED1, i);
//           //Xbox.setLedOn(LED2, i);
//           //Xbox.setLedOn(LED3, i);
//           Xbox.setLedOn(LED4, i);
//           Serial.print("J (gear): ");
//           Serial.println(J);
//           PREVIOUS_J = 4;
//          }
//          else if (J > 4)
//          {
//          J = 4;
//          PREVIOUS_J = 5;
//          }
//          else if (J == 0 && PREVIOUS_J != J)
//          {
//          Xbox.setLedOff(i);
//          Serial.print("J (gear): ");
//          Serial.println(J);
//          PREVIOUS_J = 0; 
//          }
//          else if (J < 0)
//          {
//          J = 1;
//          Xbox.setLedOn(LED1,i);
//          PREVIOUS_J = -1;
//          }

        if (Xbox.getAnalogHat(LeftHatX, i) > 9000 || Xbox.getAnalogHat(LeftHatX, i) < -9000 || Xbox.getAnalogHat(LeftHatY, i) > 9000 || Xbox.getAnalogHat(LeftHatY, i) < -9000 || Xbox.getAnalogHat(RightHatX, i) > 9000 || Xbox.getAnalogHat(RightHatX, i) < -9000 || Xbox.getAnalogHat(RightHatY, i) > 9000 || Xbox.getAnalogHat(RightHatY, i) < -9000) 
         {
          motorMOVE();
          motorMoveRight();
         }
//          if (Xbox.getAnalogHat(LeftHatX, i) > 9000 || Xbox.getAnalogHat(LeftHatX, i) < -9000) //Left joystick x axis
//          {
//            Serial.print(F("LeftHatX: "));
//            Serial.print(Xbox.getAnalogHat(LeftHatX, i));
//            Serial.print("\t");
//          }
//          if (Xbox.getAnalogHat(LeftHatY, i) > 9000 || Xbox.getAnalogHat(LeftHatY, i) < -9000) //Left Joystick y axis
//          {
//            Serial.print(F("LeftHatY: "));
//            Serial.print(Xbox.getAnalogHat(LeftHatY, i));
//            Serial.print("\t");
//          }
//          if (Xbox.getAnalogHat(RightHatX, i) > 9000 || Xbox.getAnalogHat(RightHatX, i) < -9000) //Right Joystick x axis
//          {
//            Serial.print(F("RightHatX: "));
//            Serial.print(Xbox.getAnalogHat(RightHatX, i));
//            Serial.print("\t");
//          }
//          if (Xbox.getAnalogHat(RightHatY, i) > 9000 || Xbox.getAnalogHat(RightHatY, i) < -9000) //Right Joystick y axis
//          {
//            Serial.print(F("RightHatY: "));
//            Serial.print(Xbox.getAnalogHat(RightHatY, i));
//          }
//          Serial.println();
//        }

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
//        if (Xbox.getButtonClick(Y, i))
//        {
//           if (Xbox.getButtonPress(R2, i) == 255 && (millis()-LAST_DEBOUNCE_TIME) > DEBOUNCE_DELAY)        // needs to be changed for Y button 
//           {
//           Serial.print("\tR2: ");
//           Serial.println(Xbox.getButtonPress(R2, i));
//           J++;
//           LAST_DEBOUNCE_TIME = millis();
//           }
//           else if (Xbox.getButtonPress(L2, i) == 255 && (millis()-LAST_DEBOUNCE_TIME2) > DEBOUNCE_DELAY)
//           {
//           Serial.print("L2: ");
//           Serial.println(Xbox.getButtonPress(L2, i));
//           J--;
//           LAST_DEBOUNCE_TIME2 = millis();
//           }
//          Serial.println(F("Y"));
//        }
      }
    }
  }



