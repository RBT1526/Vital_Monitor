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
unsigned long tempo = 0; //variable for time comparation
bool tempos = false;
String temperature = "tOf";//declare the temperature variable
const int start = 2;  //declare the variable for the start button
const int memory = 3;  //start is a cable on pin 10 to a transistor conected to the buttons, same with memory
String data = "";  //variable to save the data of the serial communication
int HR= 0; // variable to save the bpm
int diastolic = 0;//variable to save the diastolic
int systolic= 0;//variable to save the systolic
int flag = 0; //a flag
String pressure_dats = "";
String pressurefull = "";
//------------------------------------------------------------------------------
//SETUP
void setup() {
  Serial.begin(115200); //Start serial comunication at 115200 bps
  pinMode(LED, OUTPUT);//declare led as an output
  delay(3000); //wait 3 seconds to start the program
  Wire.begin();//wire start
  Wire.setClock(400000);//select frequency of the wire clock
  pinMode(start, OUTPUT); // declare start and memory pins
  pinMode(memory, OUTPUT);
  button_function(200, 0, 1); // Start the start button for 200 miliseconds
    delay(500);
    button_function(200, 0, 0); // start the memory button
    delay(500);
    button_function(5500, 1, 0); // start the two buttons to reset the memory data
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
  if (millis() - tiempo > 100) { //simple millis comparation
    tiempo = millis();
    if(pressurefull.indexOf("c") != -1){
    Send_Data(datos+temperature);
    }
    else{
    Send_Data(datos+temperature+pressure_dats);
    }
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

temperature=temp.temperature;//put the value in the string
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
    datos = "ObOs";// if the sensor didn't get a finger change string
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
      datos = beatAvg;
      datos += "b";
      datos += SPO2f;
      datos += "s";
      

    }
    else {//ELSE CHANGE THE DATA STRING TO ANNOUNCE THAT IS TAKING THE SAMPLES
      datos = "AbAs";
    }
    ///////////////////////////////////////////////////////////////////////////////
  }//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

}
void Send_Data(String datas){
nrf24.setModeTx();
  const char *datos = datas.c_str();
  if(!nrf24.send((uint8_t *)datos, strlen(datos)))Serial.println("Something happened in the message send!");
  //Serial.println("It was sended!");
  nrf24.waitPacketSent();
  nrf24.setModeRx();
  receive();
}
void receive(){
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];//create the storage of 32 bytes for the message incoming
  uint8_t len = sizeof(buf);// lenght of the buffer.
  if (nrf24.available())
  {
  if (nrf24.recv(buf, &len)) // if we receive an answer
    {
     String data_received =  String((char*)buf);
     data_received.trim();
      //Serial.print("got reply: ");// print that it got an answer
      Serial.println(data_received);//print the message
      if (data_received[0] == 'M') {
      pressurefull = "c";
      } 
      if (data_received[0] == 'A') {
    Serial.println("Recivido pa ");
    data_received = "without data";
    if(millis()-tempo > 10000 && tempos == true || tempo == 0){
    get_data();// call the get_data function
    
    if (!particleSensor.begin(0x57)) //start the max30102 sensor on his i2c address
  {
    Serial.println("!SENSOR ERROR! IT DIDN'T START CORRECTLY");// if max30102 wasn't found.
  }
  particleSensor.setup(); // max30102 setup
  shows_counter = 0;
    }
    
  }
    }
    else
    {
      Serial.println("recv failed");// if something failed on the incoming message
    }
  }
}
void button_function(int delaytime, int mode, int individualmode ) { //request a 3 variable (delaytime,mode,individualmode)
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  if (mode) { // if the mode variable is true so it click the two buttons
    digitalWrite(start, HIGH);//activate the start button
    digitalWrite(memory, HIGH); //activate the memory button
    delay(delaytime);// delay time of the delaytime variable
    digitalWrite(memory, LOW); // turn off the memory button
    delay(100);// 100 miliseconds to simulate that first i leave the memory button
    digitalWrite(start, LOW);// turn off the memory button

  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  else {
    //////////////////////////////////////////////////////////////////////////////////
    if (individualmode) { // When the mode is 0 and the individualmode is 1 activate only start button
      digitalWrite(start, HIGH);// activate start
      delay(delaytime); // delay of the delaytime variable
      digitalWrite(start, LOW);// turn off the start button
    }
    //////////////////////////////////////////////////////////////////////////////////
    else {//when mode is 0 and individualmode is 0 activate only memory button
      digitalWrite(memory, HIGH);// activate memory button
      delay(delaytime);// delay time of the delaytime variable
      digitalWrite(memory, LOW);// turn off the memory button
    }
    //////////////////////////////////////////////////////////////////////////////////
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//---------------------------------------------------------------------------
//function to get data
void get_data() { 
  button_function(200, 0, 1); // Start the start button for 200 miliseconds
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  while (data.substring(9, 12) != "err") { // wait until the proccess finis
    if (Serial.available()) { // check the serial data
      data = Serial.readStringUntil('\n');// in the variable data we store the data
    }
  }
  Serial.println(data.substring(13,14));
  if(data.substring(13, 14) != "0"){
    Serial.println("Entre a la cochinada esta");
    pressure_dats ="yOdOhOZ";
    data = "";
flag = 0;
        delay(100);
        button_function(200, 0, 1); // Start the start button for 200 miliseconds
    }
  if(data.substring(13, 14) == "0"){
      Serial.println("Entre a la else");
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  delay(1700); // 100 miliseconds delay
  button_function(200, 0, 0); // start the memory button
  Serial.println("ya pase el function");
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  while (flag == 0) { // while the flag is empty
    if (Serial.available()) { // if there is data on the serial readings
      data = Serial.readStringUntil('\n'); // its stored on the data variable
      //////////////////////////////////////////////////////////////////////////////////
      if (data.substring(0, 4) == " sys") { // search for the systolic un the string
        systolic = data.substring(7, data.length()).toInt(); // store the value on a separate variable
      }
      //////////////////////////////////////////////////////////////////////////////////
      if (data.substring(0, 4) == " dia") { // search for the diastolic un the string
        diastolic = data.substring(7, data.length()).toInt();  // store the value on a separate variable
      }
      //////////////////////////////////////////////////////////////////////////////////
      if (data.substring(0, 4) == " pul") { //search for the pulse un the string
        HR = data.substring(9, data.length()).toInt();  // store the value on a separate variable
        flag = 1;
      }
      //////////////////////////////////////////////////////////////////////////////////
    }
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  button_function(5500, 1, 0); // start the two buttons to reset the memory data
pressure_dats ="y";
pressure_dats += systolic;
pressure_dats += "d";
pressure_dats += diastolic;
pressure_dats += "h";
pressure_dats += HR;
pressure_dats += "Z";
data = "";
flag = 0;

  }
  pressurefull = "a";
  tempo = millis();
  tempos = true;
  
  }

//------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
