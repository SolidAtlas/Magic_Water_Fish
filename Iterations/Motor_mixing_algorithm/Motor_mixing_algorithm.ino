// Differential Steering Joystick Algorithm
// ========================================
//   by Justin Garcia
//   justin.d.garcia@gmail.com
//
// Converts a single dual-axis joystick into a differential
// drive motor control, for a system of two motors (think tank drive) with support for both drive and turning
// 
//

// INPUTS
int     K = Xbox.getAnalogHat(LeftHatX, i);              // Joystick X input                     (-32767..32767)
int     M = Xbox.getAnalogHat(LeftHatY, i);              // Joystick Y input                     (-32767..32767)
int     N = Xbox.getAnalogHat(RightHatX, i);             // Joystick X input                     (-32767..32767)
int     O = Xbox.getAnalogHat(RightHatY, i);             // Joystick Y input                     (-32767..32767)
// OUTPUTS
int     LMotMixL;           // Motor (left)  mixed output           (-32767..32767)
int     LMotMixR;           // Motor (right) mixed output           (--32767..32767)

int     RMotMixL;           // Motor (left)  mixed output           (-32767..32767)
int     RMotMixR;           // Motor (right) mixed output           (--32767..32767)

int     UpMotMixBoth;

      
// TEMP VARIABLES
float   LMotPremixL;    // Motor (left)  premixed output        (-32767..32767)
float   LMotPremixR;    // Motor (right) premixed output        (-32767..32767)

float   RMotPremixL;    // Motor (left)  premixed output        (-32767..32767)
float   RMotPremixR;    // Motor (right) premixed output        (-32767..32767)

float   UpMotPremixBoth;



// Calculate Drive Turn output due to Joystick X input
if (M >= 0) 
{
  // Forward
  LMotPremixL = (K>=0)? 32767.0 : (32767 + K);
  LMotPremixR = (K>=0)? (32767.0 - K) : 32767.0;
  // Scale Drive output due to Joystick Y input (throttle)
  LMotPremixL = LMotPremixL * M/32767.0; 
  LMotPremixR = LMotPremixR * M/32767.0;
  LmotMixL = map(LMotPremixL,0,32767,1500,1900)
  LmotMixR = map(LMotPremixR,0,32767,1500,1900)
  RightESC.writeMicroseconds(LMotMixL);
  LeftESC.writeMicroseconds(RMotMixR);
} 
else if (M < 0) 
{
  // Reverse
  LMotPremixL = (K>=0)? (32767.0 - K) : 32767.0;
  LMotPremixR = (K>=0)? 32767.0 : (32767.0 + K);
  // Scale Drive output due to Joystick Y input (throttle)
  LMotPremixL = LMotPremixL * M/32767.0;
  LMotPremixR = LMotPremixR * M/32767.0;
  LmotMixL = map(LMotPremixL,0,32767,1500,1100)
  LmotMixR = map(LMotPremixR,0,32767,1500,1100)
  RightESC.writeMicroseconds(LMotMixL);
  LeftESC.writeMicroseconds(RMotMixR);
}
else if (O >= 0) 
{
  // Forward
  UpMotPremixBoth = (N>=0)? 32767.0 : (32767 + N);
  // Scale Drive output due to Joystick Y input (throttle)
  UpMotPremixBoth = UpMotPremixBoth * O/32767.0; 
  UpMotMixBoth = map(UPMotPremixBoth,0,32767,1500,1900)
  UpESC.writeMicroseconds(UpMotMixBoth);
  Up2ESC.writeMicroseconds(UpMotMixBoth);
} 
else 
{
  // Reverse
  UpMotPremixBoth = (N>=0)? 32767.0 : (32767 + N);
  // Scale Drive output due to Joystick Y input (throttle)
  UpMotPremixBoth = UpMotPremixBoth * O/32767.0;
  UpMotMixBoth = map(UPMotPremixBoth,0,32767,1500,1100)
  UpESC.writeMicroseconds(UpMotMixBoth);
  Up2ESC.writeMicroseconds(UpMotMixBoth);
}




    
