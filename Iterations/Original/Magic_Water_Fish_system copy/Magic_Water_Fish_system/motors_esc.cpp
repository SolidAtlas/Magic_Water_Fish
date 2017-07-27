
#include "Arduino.h"
#include "motors_esc.h"
#include "xbox_controller.h"
//#include <XBOXRECV.h> // Satisfy IDE, which only needs to see the include statment in the ino.
//                        //the following  libraries are referenced but go unused
//                        //bogus include is a trick to compile more quickly
//#ifdef dobogusinclude
//#include <spi4teensy3.h>
//#include <SPI.h>        //library for Serial Peripheral Interface - allows for communication to the usb host chip
//#endif    
//
//USB Usb;                // There can more more than one USB constant
//XBOXRECV Xbox(&Usb);
#include <Servo.h>

//extern uint8_t i;
extern int J;
extern int K;
extern int M;
extern int N;
extern int O; 

Servo RightESC, LeftESC, UpESC, LateralESC; //Create as much as Servo objects you want. You can control 2 or more Servos at the same time

motorsclass::motorsclass()
{
}

void motorsclass::SETUP()
{

RightESC.attach(23);    
LeftESC.attach(25);    
UpESC.attach(27);    
LateralESC.attach(29); 
}  

void motorsclass::motorMOVE(int J, int K, int M, int N, int O)
{
Serial.println();
  Serial.print("K is");
  Serial.println(K);
if (M > 9000 && K <= 9000 && K >= -9000)  //if y+
  {
  int motorENABLE = (M/2)*J;
  Serial.print("Motor Enable is ");
  Serial.print(motorENABLE);
   RightESC.writeMicroseconds(motorENABLE);
   LeftESC.writeMicroseconds(motorENABLE);
  }
else if (M < -9000 && K <= 9000 && K >= -9000)  //if y-
  {
  int motorENABLE = (M)*J*-1;/// divide by 2
   RightESC.writeMicroseconds(motorENABLE);
   LeftESC.writeMicroseconds(motorENABLE);
  }
else if (K > 10000 && M > 10000 )  //if y+ and x+  //else if
  {
  int motorENABLEleft = ((K/2)*J);
  int motorENABLEright = 64*J - motorENABLEleft;
   RightESC.writeMicroseconds(motorENABLEright);
   LeftESC.writeMicroseconds(motorENABLEleft);
  }
  else if (K < -10000 && M > 10000)  //if y+ and x-
  {
  int motorENABLEright = -1*(K/2)*J;
  int motorENABLEleft = 64*J - motorENABLEright;
   RightESC.writeMicroseconds(motorENABLEright);
   LeftESC.writeMicroseconds(motorENABLEleft);
  }
  else if (K < -10000 && M < -10000)  //if y- and x-
  {
  int motorENABLEright = (-1*(K/2)*J);
  int motorENABLEleft = 64*J - motorENABLEright;
   RightESC.writeMicroseconds(motorENABLEright);
   LeftESC.writeMicroseconds(motorENABLEleft);
  }
  else if (K > 10000 && M < -10000)  //if y- and x+
  {
  int motorENABLEleft = (K/2)*J;
  int motorENABLEright = 64*J - motorENABLEleft;
   RightESC.writeMicroseconds(motorENABLEright);
   LeftESC.writeMicroseconds(motorENABLEleft);

  }
  else
{
   RightESC.writeMicroseconds(0);
   LeftESC.writeMicroseconds(0);
}
}

void motorsclass::motorMoveLATERAL (int J,int K, int M, int N, int O)
{


  /***************************************
 * Lateral Motor
 */
 if (N > 9000 && O <= 9000 && O >= -9000)  //if x+
  {
  int motorENABLE = ((N/20)*J);
   LateralESC.writeMicroseconds(motorENABLE);   
  }
  
else if (N < -9000 && O <= 9000 && O >= -9000)  //if x-
  {
  int motorENABLE = (N/(2))*J*-1;
   LateralESC.writeMicroseconds(motorENABLE);
  }
  
    else
{
   LateralESC.writeMicroseconds(0);
}
}


void motorsclass::motorMoveUP (int J, int K, int M, int N, int O)
{
  /***************************************
 * Lateral Motor
 */
 if (O > 9000 && N <= 9000 && N >= -9000)  //if y+
  {
  int motorENABLE = (O/(2))*J;
   UpESC.writeMicroseconds(motorENABLE);   
  }
  
else if (O < -9000 && N <= 9000 && N >= -9000)  //if y-
  {
  int motorENABLE = (O/(2))*J*-1;
   UpESC.writeMicroseconds(motorENABLE);
  }
  
    else
{
   LateralESC.writeMicroseconds(0);
}
}

motorsclass motors = motorsclass();
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
