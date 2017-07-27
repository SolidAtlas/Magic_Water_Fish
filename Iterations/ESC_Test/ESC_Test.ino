/*
Coded by Marjan Olesch
Sketch from Insctructables.com
Open source - do what you want with this code!
*/
#include <Servo.h>


int value = 0; // set values you need to zero

Servo firstESC; //secondESC, thirdESC, fourthESC; //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time

void setup() {

  firstESC.attach(8);    // attached to pin 9 I just do this with 1 Servo
//  secondESC.attach(25);    // attached to pin 9 I just do this with 1 Servo
//  thirdESC.attach(27);    // attached to pin 9 I just do this with 1 Servo
//  fourthESC.attach(29);    // attached to pin 9 I just do this with 1 Servo
  Serial.begin(9600);    // start serial at 9600 baud

}

void loop() {

//First connect your ESC WITHOUT Arming. Then Open Serial and follo Instructions
 
  firstESC.writeMicroseconds(value);
//   secondESC.writeMicroseconds(value);
//    thirdESC.writeMicroseconds(value);
//     fourthESC.writeMicroseconds(value);
  
 
if(Serial.available()) 
    value = Serial.parseInt();    // Parse an Integer from Serial
        if (Serial.available() > 0) {
                // read the incoming byte:
                //value = Serial.read();
                value = Serial.parseInt(); 

                // say what you got:
                Serial.print("I received: ");
                Serial.println(value, DEC);
        }
}

