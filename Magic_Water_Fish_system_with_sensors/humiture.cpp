//-------------------- INCLUDES ---------------------//
#include "humiture.h"
#include "Arduino.h"
#include <dht.h>    // This is the library for the humidity and temperature sensor
                    //This library allows the sensor to receive and transmit data over one wire.
                    
dht DHT;            //This line initializes the temperature/humidity sensor as an object
dht DHT1;           // another object                    
//-------------------- VARIABLES ---------------------//          
#define DHT_Pin 9            //This is a temperature/humidity sensor
#define DHT_Pin2 10          //This is a temperature/humidity sensor

long LAST_DEBOUNCE_DHT = 0;        //Both sensors need their own debounce time so they can recover and give good readings
long LAST_DEBOUNCE_DHT2 = 0;
long DEBOUNCE_DELAY2 = 4000;      //Temperature/Humidity sensors to be pinged no more than once every 2 seconds (this is 4 seconds,it is better this way)
                                  //Leak detection is handled by water sensors, they react faster. humidity is to measure long term effect of use of electronics inside drone.

float TEMP_AVERAGE_SYSTEM = 0;    //The DHT_11 temperature and Humidity sensor gives readings of float type
float HUMIDITY_AVERAGE_SYSTEM = 0;

float TEM_1 = 0;                  //this is the temperature read by the first sensor
float TEM_2 = 0;                  //this is the temperature read by the second sensor

float HUM_1 = 0;                 //
float HUM_2 = 0;                 //

//-------------------- CLASS ---------------------//
humitureclass::humitureclass(){
}
//-------------------- SETUP ---------------------//
void humitureclass::SETUP()
{
 // dht.begin();
}
//-------------------- FUNTIONS ---------------------//
void humitureclass::HumidityTemperature_Check()
{
if ((millis()- LAST_DEBOUNCE_DHT) > DEBOUNCE_DELAY2)  
{
   Serial.print("DHT11, \t");
  //read the value returned from sensor
  int chk = DHT.read11(DHT_Pin);
  switch (chk)
  {
    case DHTLIB_OK:  
    Serial.println("OK,\t");
    TEM_1 = DHT.temperature;
    HUM_1 = DHT.humidity;
    Serial.print("TEM_1  ");
    Serial.println(TEM_1);
    Serial.print("HUM_1  ");
    Serial.println(HUM_1);
    LAST_DEBOUNCE_DHT = millis();
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error,\t"); 
    LAST_DEBOUNCE_DHT = millis();
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error,\t");
    LAST_DEBOUNCE_DHT = millis(); 
    break;
    default: 
    Serial.println("Unknown error,\t");
    LAST_DEBOUNCE_DHT = millis(); 
    break;
  }
}

if ((millis()- LAST_DEBOUNCE_DHT2) > DEBOUNCE_DELAY2)
{
Serial.print("DHT11_2, \t");
  //read the value returned from sensor
  int chk2 = DHT1.read11(DHT_Pin2);
  switch (chk2)
  {
    case DHTLIB_OK:  
   Serial.println("OK,\t");
   TEM_2 = DHT1.temperature;
   HUM_2 = DHT1.humidity;
   Serial.print("TEM_2  ");
   Serial.println(TEM_2);
   Serial.print("HUM_2 ");
   Serial.println(HUM_2);
   LAST_DEBOUNCE_DHT2 = millis();
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error,\t"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error,\t"); 
    break;
    default: 
    Serial.println("Unknown error,\t"); 
    break;
  }
} 

float tempaverageholder = (TEM_1+TEM_2)/2;
float tempaveragesystem = 1.8*tempaverageholder+32;//this converts the centigrade value to degrees fahrenheit for the user
TEMP_AVERAGE_SYSTEM = tempaveragesystem;

float humidityaveragesystem = (HUM_1 + HUM_2)/2;
HUMIDITY_AVERAGE_SYSTEM = humidityaveragesystem;
}

//-------------------- AKA ---------------------//
humitureclass humiture = humitureclass();


