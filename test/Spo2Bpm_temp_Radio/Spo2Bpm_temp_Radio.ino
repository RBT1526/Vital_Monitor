//ARDUINO UNO CODE GET SPO2,BPM and Temperature
//
//
//
//Gerardo Fregoso Jiménez Code based on the "j.n.magee max30102 tinypulseppg proyect" and adafruit library
//
//Libraries used: j.n.magee max30102 library and Sparkfun drivers, adafruit library for htu31
//
//
//------------------------------------------------------------------------------
//IMPORT LIBRARIES
#include <Wire.h>
#include "MAX30102.h"
#include "Pulse.h"
#include "heartRate.h"
#include "Adafruit_HTU31D.h"//adafruit library
#include <SPI.h>
#include <RH_NRF24.h>
//------------------------------------------------------------------------------
//Declare the libraries variable names
MAX30102 particleSensor;
Pulse pulseIR;
Pulse pulseRed;
MAFilter bpm;
#define LED LED_BUILTIN // define led
Adafruit_HTU31D htu = Adafruit_HTU31D();//declare the name of the temperature sensor
RH_NRF24 nrf24;
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
String datos = "bOsO"; //final spo2 and bpm data string
unsigned long tiempo = 0; //variable for time comparation
String temperature = "tOf";//declare the temperature variable
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
  //starting the temperature sensor
  if (!htu.begin(0x40)) {//i2c communication begins
    Serial.println("Couldn't find sensor!");//if it dosen't find the sensor
    while (1);
  }
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1)) //set channel
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}
//------------------------------------------------------------------------------
//MAIN LOOP
void loop()  {
  GetOxi();//call the function to get the spo2 and bpm
  GetTemp();//call the function to get the temperature
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //EVERY 500 MILISECONDS IT PRINTS THE DATA STRING
  if (millis() - tiempo > 500) { //simple millis comparation
    tiempo = millis();
    Send_Data(datos+temperature);
    //Serial.println(datos+temperature);//combine the 2 strings
  }
  // NOTE= IT CAN'T BE USE A DELAY BECAUSE WE WANT TO RUN SIMULTANEUS
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

}
//------------------------------------------------------------------------------
//GET TEMPERATURE FUNCTION
void GetTemp(){
  sensors_event_t humidity, temp;//Declare name for sensor event
  htu.getEvent(&humidity, &temp);// populate temp objects with fresh data
temperature="t";//put a t to identify when the value start
temperature+=temp.temperature;//put the value in the string
temperature+="f";// to finish the string

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
    datos = "bOsO";// if the sensor didn't get a finger change string
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
      datos = "b";
      datos += beatAvg;
      datos += "s";
      datos += SPO2f;

    }
    else {//ELSE CHANGE THE DATA STRING TO ANNOUNCE THAT IS TAKING THE SAMPLES
      datos = "bAsA";
    }
    ///////////////////////////////////////////////////////////////////////////////
  }//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

}
void Send_Data(String datas){
  const char *datos = datas.c_str();
  if(!nrf24.send((uint8_t *)datos, strlen(datos)))Serial.println("Something happened in the message send!");
  //Serial.println("It was sended!");
  nrf24.waitPacketSent();
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];//create the storage of 32 bytes for the message incoming
  uint8_t len = sizeof(buf);// lenght of the buffer.
  if (nrf24.recv(buf, &len)) // if we receive an answer
    {
      Serial.print("got reply: ");// print that it got an answer
      Serial.println((char*)buf);//print the message
    }
    else
    {
      Serial.println("recv failed");// if something failed on the incoming message
    }
}

//------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022