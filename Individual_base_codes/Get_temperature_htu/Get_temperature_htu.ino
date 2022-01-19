//Get temperature with htu31 sensor
//
//
//Gerardo Fregoso Jiménez Code based on adafruit library
//
// 
//
//--------------------------------------------------------
//include libraries
#include <Wire.h>//wire for i2c
#include "Adafruit_HTU31D.h"//adafruit library
//--------------------------------------------------------
Adafruit_HTU31D htu = Adafruit_HTU31D();//declare the name of the sensor
float temperature = 0;//declare the temperature variable
//--------------------------------------------------------
//Setup
void setup() {
  Serial.begin(115200);//start Serial communication at 15200 bps
  Serial.println("htu temperature sensor");
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//starting the sensor
  if (!htu.begin(0x40)) {//i2c communication begins
    Serial.println("Couldn't find sensor!");//if it dosen't find the sensor
    while (1);
  }
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
}
//--------------------------------------------------------
//Main loop
void loop() {
  sensors_event_t humidity, temp;//Declare name for sensor event
  htu.getEvent(&humidity, &temp);// populate temp objects with fresh data
temperature=temp.temperature;//put the value in another variable
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
  //print the temperature in celsius
  Serial.print("Temp: "); 
  Serial.print(temperature);
  Serial.println(" C");
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
  delay(500);//delay for the data rate
}
//--------------------------------------------------------
//Gerardo Fregoso Jimenez 2022
