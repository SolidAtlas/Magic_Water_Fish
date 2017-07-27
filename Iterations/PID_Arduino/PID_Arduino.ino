/*
 * This is a code that implements a function that performs discretized digital filter. 
 * The function of this program is to reduce the noise present in the signals of sensors.
 * The Funtion is defined as : Error_filt = Error_filt * alpha + Error * (1-alpha)
 * this is the backward difference method which uses the a portion of the previously sensed value 
 * to wash out the high frequency noise through constant averaging.
 * 
 * so far this program is written so that if a potentiometer is turned to say: i want to be at 5 meters
 * then the actual position is read: I am at two meters
 * The error between the two is calculated: I need to move three meters
 * Then the controller moves as fast a possible to get to five meters
 * if it overshoots it corrects.
 * if the correction undershoots it corrects, etc, until the error is zero
 * 
 * The future itration of this program will take both a gyroscopic reading and accelerometer reading to determine the drones angle of pitch and use the motors to correct it.
 * This program can also be used to help control the depth of the drone and keep it there. 
 */

int ControlPin = 1; //Set the output pin

double PosRef = 0; //position reference
double Pos = 0;    // actual position
double Error = 0;  // error
double Error_dot = 0;  // time-derivative of the error
double Error_int = 0;  // Integral of the error
double Control_U = 0;  // Controller output
double Control_U_max = 255;  // Maximum output, in digital, 255 for Arduino digital pin output
double Control_U_min = 0;    // Minimum output
                             // these min/max controls can be changed, not suggested

double KP = 0;   // This is the proportional gain
double KI = 0;   // This is the Integral gain
double KD = 0;   // This is the derivative gain
                 // Any of the K values can be commented out to make certian controller types.
double Delta_t = 0; // discrete time jump

double Error_filt = 0;  
double Error_filt_prev = 0; 
double alpha = 0.9;  // alpha may be changed but is usually kept close to unity

unsigned long time_cur = 0;
unsigned long time_prev = 0;

double Poten2Meter = (5.0/1023.0)*0.01;
double Sense2Meter = (5.0/1023.0)*0.015;

void setup() {
  Serial.begin(9600);
  delay(1000);
  time_prev = millis();
}

void loop() {
   
   delay(5);                             // For slowing down the sampling rate if it is too fast 
                                         // For our drone this delay will be implemented using the millis function
                                         // And a boolean statement
                                         // A delay() command stops all processing on the arduino and would render the drone useless
                                         // During that time.
   
   PosRef = analogRead(ControlPin)*Poten2Meter;   // Read reference command from the potentiometer and convert it to meters
   Pos = analogRead(2)*Sense2Meter;      // Read sensor output and convert it to meters
 
   Error = PosRef-Pos;                  // Feedback Error
   
   Error_filt = Error_filt * alpha + Error * (1-alpha);  // Filter the error for derivative calculation if the signal is noisy

   time_cur = millis();
   Delta_t = time_cur - time_prev;     // Calculate Delta_t
   
   Error_dot = (Error_filt-Error_filt_prev)/Delta_t;     // Calculate Error derivative
   
   Error_int = Error_int + Error*Delta_t;                // Calculate Error integral

   Control_U = KP*Error + KI*Error_int + KD*Error_dot;   // Compute the control signal

   if (Control_U > Control_U_max)
   { 
      Control_U = Control_U_max;              // Control saturation
      Error_int = Error_int - Error*Delta_t;  // Integrtor anti-windup (prevent error integrator from winding up)
   }
   
   if (Control_U < Control_U_min)
   { 
      Control_U = Control_U_min;              // Control saturation
      Error_int = Error_int - Error*Delta_t;  // Integrtor anti-windup (prevent error integrator from winding up)
   }
  
   analogWrite(ControlPin,floor(Control_U));   // Apply the control signal to actuator through PWM at pin#1
                                      

   Error_filt_prev = Error_filt;
   time_prev = time_cur;

   Serial.print(time_cur/1000);   // Prints time in seconds
   Serial.print("\t");            // Prints a tab

   Serial.print(PosRef);
   Serial.print("\t");

   Serial.print(Pos);
   Serial.print("\t");

   Serial.println(Control_U);
   
}
