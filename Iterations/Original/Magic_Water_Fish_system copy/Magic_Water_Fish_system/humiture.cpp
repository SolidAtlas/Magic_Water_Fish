
#include "Arduino.h"
#include <dht.h>    // This is the library for the humidity and temperature sensor
                    //This library allows the sensor to receive and transmit data over one wire.
                    
dht DHT;            //This line initializes the temperature/humidity sensor as an object
dht DHT1;           // another object

long LAST_DEBOUNCE_DHT = 0;        //Both sensors need their own debounce time so they can recover and give good readings
long LAST_DEBOUNCE_DHT2 = 0;
long DEBOUNCE_DELAY2 = 4000;      //Temperature/Humidity sensors to be pinged no more than once every 2 seconds

float TEMP_AVERAGE_SYSTEM = 0;    //The DHT_11 temperature and Humidity sensor gives readings of float type
float HUMIDITY_AVERAGE_SYSTEM = 0;

float TEM_1 = 0;                  //this is the temperature read by the first sensor
float TEM_2 = 0;                  //this is the temperature read by the second sensor

float HUM_1 = 35;                 //Started with initial value so humidifier isnt on, value of the first sensor
float HUM_2 = 35;                 //Ensures average humidity is started at 35% since values of HUM_1 and HUM-2 are averaged, value of the second sensor.

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
