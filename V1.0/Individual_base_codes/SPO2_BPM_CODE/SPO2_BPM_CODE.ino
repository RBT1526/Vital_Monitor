//ARDUINO UNO CODE GET SPO2 AND BPM
//
//
//
//Gerardo Fregoso Jiménez Code based on the "j.n.magee max30102 tinypulseppg proyect"
//
//Libraries used: j.n.magee max30102 library and Sparkfun drivers.
//
//
//------------------------------------------------------------------------------
//IMPORT LIBRARIES
#include <Wire.h>
#include "MAX30102.h"
#include "Pulse.h"
#include "heartRate.h".
//------------------------------------------------------------------------------
//Declare the libraries variable names
MAX30102 particleSensor;
Pulse pulseIR;
Pulse pulseRed;
MAFilter bpm;
#define LED LED_BUILTIN // define led
//------------------------------------------------------------------------------
//define variables
int beatAvg, SPO2, SPO2f;  //beat avarage, spo2 and spo2 final variables
bool filter_for_graph = false; // select the filter for the ir and red data
long shows_counter = 0;  // counter of show rate
const byte RATE_SIZE = 6; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0; //to locate a rate on the array
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute = 0; //operation to get the bpm
String datos; //final data string
unsigned long tiempo = 0; //variable for time comparation
//------------------------------------------------------------------------------
//SETUP
void setup() {
  Serial.begin(115200); //Start serial comunication at 115200 bps
  pinMode(LED, OUTPUT);//declare led as an output
  delay(3000); //wait 3 seconds to start the program
  Wire.begin();//wire start
  Wire.setClock(400000);//select frequency of the wire clock
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (!particleSensor.begin(0x57)) //start the max30102 sensor on his i2c address
  {
    Serial.println("!SENSOR ERROR! IT DIDN'T START CORRECTLY");// if max30102 wasn't found.
    while (1); // infinite loop
  }
  particleSensor.setup(); // max30102 setup
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//------------------------------------------------------------------------------
//MAIN LOOP
void loop()  {
  GetOxi();//call the function to get the spo2 and bpm
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //EVERY 500 MILISECONDS IT PRINTS THE DATA STRING
  if (millis() - tiempo > 500) { //simple millis comparation
    tiempo = millis();
    Serial.println(datos);
  }
  // NOTE= IT CAN'T BE USE A DELAY BECAUSE WE WANT TO RUN SIMULTANEUS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

}
//------------------------------------------------------------------------------
//GET SPO2 AND BPM VALUES FUNCTION
void GetOxi() {
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Getting the sensors main values
  particleSensor.check(); // check if the max30102 is working
  long now = millis();   //start time of this cycle
  if (!particleSensor.available()) return;// check if the max30102 is working
  uint32_t irValue = particleSensor.getIR(); //get Ir_Led values from the sensor
  uint32_t redValue = particleSensor.getRed();//get Red_Led values from the sensor
  particleSensor.nextSample();//change sample
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Checking if there's a finger or not
  if (irValue < 5000) { 
    datos = "THERE'S NO FINGER IN THE SENSOR";// if the sensor didn't get a finger change string
    shows_counter = 0;
  }
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 //Get the spo2 and bpm values if its a finger
  else {
    shows_counter += 1; // if is a finger add 1 to  the show counter.
 ///////////////////////////////////////////////////////////////////////////////
// IR AND RED SIGNAL FILTER
    int16_t IR_signal, Red_signal;
    bool beatRed, beatIR;

    if (!filter_for_graph) {    // selection of type of filter
      IR_signal =  pulseIR.dc_filter(irValue) ;
      Red_signal = pulseRed.dc_filter(redValue);
      beatRed = pulseRed.isBeat(pulseRed.ma_filter(Red_signal));
      beatIR =  pulseIR.isBeat(pulseIR.ma_filter(IR_signal));
    } else {
      IR_signal =  pulseIR.ma_filter(pulseIR.dc_filter(irValue)) ;
      Red_signal = pulseRed.ma_filter(pulseRed.dc_filter(redValue));
      beatRed = pulseRed.isBeat(Red_signal);
      beatIR =  pulseIR.isBeat(IR_signal);
    }
  ///////////////////////////////////////////////////////////////////////////////
//BPM CALCULATE
    if (checkForBeat(irValue) == true)// CHECK IF THERE'S A BEAT
    {
      //We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0); 

      if (beatsPerMinute < 255 && beatsPerMinute > 20)// get beat avg
      {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
///////////////////////////////////////////////////////////////////////////////
   //SPO2 CALCULATE
    long numerator   = (pulseRed.avgAC() * pulseIR.avgDC()) / 256;
    long denominator = (pulseRed.avgDC() * pulseIR.avgAC()) / 256;
    int RX100 = (denominator > 0) ? (numerator * 100) / denominator : 999;

    SPO2f = (10400 - RX100 * 17 + 50) / 100; //spo2 operations
///////////////////////////////////////////////////////////////////////////////
 //SAMPLES CONDITION_ FOR STABILIZATION 
    if (shows_counter > 500) {// IF THERE'S 500 SAMPLES(AFTER DETECT A FINGER) OR MORE CHANGE DATA STRING
      datos = "BPM = ";
      datos += beatAvg;
      datos += " SPO2 ";
      datos += SPO2f;

    }
    else {//ELSE CHANGE THE DATA STRING TO ANNOUNCE THAT IS TAKING THE SAMPLES
      datos = "TAKING SPO2 AND BPM";
    }
    ///////////////////////////////////////////////////////////////////////////////
  }//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

}
//------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022