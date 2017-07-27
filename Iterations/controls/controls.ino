/***************************************************************************************************
 * name: Magic Water Fish Movement Controls, sensor data aquisition, Automated Environment Detector
 * function:
 * 1. The Joystick gives values from 0-1023 on two axes (x and y)
 *  A. the x values are mapped to 0-255 range with (full left) 0 driving only the right motor and (full right) driving only the left motor.
 *  
 * updated October 11th, 2016
 * Using the joystick in the 
 * Written by Justin Garcia contact info: Justin.d.garcia@gmail.com
 * 
 ***************************************************************************************************/


/*********************************************
 * Josystick PS2
 * GND GND
 * VCC 5v
 * Switch PIN8
 * y A0
 * x A1
 **********************************************/
 const int yPin = A0; 
 const int xPin = A1;
 const int swPin = 3;
 /*********************************************
  * H-Bridge
  *********************************************/
//motor one
int enA = 10;
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

void joystickREAD()
{
  Serial.print("X: "); //print "X: "
  Serial.print(analogRead(xPin),DEC); //read the value of A0 and print it in decimal
  Serial.print("|Y: "); //print "|Y: "
  Serial.print(analogRead(yPin),DEC); //read the value of A1 and print it in decimal
  Serial.print("|Z: "); //print "|Z: "
  Serial.println(digitalRead(swPin)); //read the value of pin8 and print it

}

void motorMOVE()
{
  //turn off motors
  int motorENABLEleft = analogRead(xPin)/4;
  int motorENABLEright = 255 - motorENABLEleft;
  //return motorENABLEleft;
  //return motorENABLEright;
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  if (digitalRead(swPin) == 0)
  {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  analogWrite(enA, motorENABLEleft);
   
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
  analogWrite(enB, motorENABLEright);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(swPin,INPUT_PULLUP);
 // digitalWrite(swPin,HIGH);

  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop()
{
  joystickREAD();
  motorMOVE();
}

