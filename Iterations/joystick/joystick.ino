/*********************************************
* name:Joystick PS2
* function:push the joystick and the coordinates of X and Y axes displayed on Serial Monitor will change accordingly; 
* press down the joystick, and the coordinate of Z=0 will also be displayed.
* connection:
* Joystick PS2 SunFounder Uno R3
* GND GND
* VCC 5V
* Bt 8
* x A0
* y A1
***********************************************/
//
//
const int xPin = A0; //X attach to A0
const int yPin = A1; //Y attach to A1
const int swPin = 8; //Bt attach to digital 8
void setup()
{
  pinMode(swPin,INPUT); //set btpin as INPUT 
  digitalWrite(swPin, HIGH); //and HIGH
  Serial.begin(9600); //initialize serial monitor
}
void loop()
{
  Serial.print("X: "); //print "X: "
  Serial.print(analogRead(xPin),DEC); //read the value of A0 and print it in decimal
  Serial.print("|Y: "); //print "|Y: "
  Serial.print(analogRead(yPin),DEC); //read the value of A1 and print it in decimal
  Serial.print("|Z: "); //print "|Z: "
  Serial.println(digitalRead(swPin)); //read the value of pin8 and print it
  delay(40); //delay 500ms
}
