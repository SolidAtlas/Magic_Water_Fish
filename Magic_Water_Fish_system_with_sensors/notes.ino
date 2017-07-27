/*
 * This program is an update to the Magic Water Fish Rov System.
 * This is meant for the next year team to build upon.
 * hopefully there are enough notes here to explain.
 * For any questions please contact me at Justin.d.garcia@gmail.com
 */

 /*
  * This program draws the following code and accompanying library:
 
  Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
  It supports up to four controllers wirelessly
  For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
  send me an e-mail:  kristianl@tkjelectronics.com
  
  */

/*
  The Purpose of this program is to recognize inputs from an XBOX 360 and use those inputs to
  control the underwater ROV

  The Inputs work as follows:

  Left Bumper             - rotate counter-clockwise around center axis (not implemented, but felt this would be the best button for such a control)
  Left Trigger            - decrease maximum drive - 100%, 75%, 50%, 25%(default), 0% (also not implemented)
  Right Bumper            - rotate clockwise around the center axis (see note about Left Bumper)
  Right Trigger           - increase maximum drive - 25% (default), 50%, 75%, 100% (also not implemented)
  Left Stick              - Left stick
                          Pure Y tilt (activates forward/backward thrusters(AFT L&R)) - amount of tilt is proportional to amount of thrust
                          Y + X tilt  (activates port/starboard + Aft L&R thrusters)  - amount of tilt is proportional to amount of thrust
                          Pure X tilt (activates lateral thruster)                    - amount of tilt is proportional to amount of thrust
                          Sticks are [ -32 K --> + 32 K ]

  Right Stick             - Pure Y tilt (activates vertical thruster)                   - amount of tilt is proportional to amount of thrust
  Left Thumbstick Button  - no function
  Right Thumbstick Button - no function
  Y Button                - toggle lights
  B Button                - toggle Right Stick Camera Gimbal Control
  A Button                - no function
  X Button                - no function
  D-pad Buttons           - no function
  Back Button             - no function
  Start Button            - no function
  XBOX Button             - battery indicator

  not all of the tabs are fully integrated but have been left formatted so that code can be easily added in. 
*/

// all .cpp files have a similar structure:

//const uint8_t motor_pin = 8;              //integer creation
//
//mclass::mclass(){                         //Class definition    
//}
//
//void mclass::SETUP(){                     //A Setup funtion to be called in the main tab setup
//  pinMode(motor_pin, OUTPUT);             //setup code
//}
//                                          //other created functions following the setup style
//
//void mclass::SPEED(int motor_speed){      //A definiton of how the class is referred to by other tabs
//  
//}
//
//mclass motor = mclass();
