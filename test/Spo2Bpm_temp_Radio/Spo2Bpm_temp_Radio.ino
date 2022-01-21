#include <SPI.h>  // incluye libreria SPI para comunicacion con el modulo
#include <RH_NRF24.h> // incluye la seccion NRF24 de la libreria RadioHead
#include <Wire.h>
#include "MAX30102.h"
#include "Pulse.h"
<<<<<<< HEAD
#include "heartRate.h"
#include "Adafruit_HTU31D.h"//adafruit library
//------------------------------------------------------------------------------
//Declare the libraries variable names
=======
#include "heartRate.h".
//INCLUDE TEMP
#include "Adafruit_HTU31D.h"

Adafruit_HTU31D htu = Adafruit_HTU31D();
float tempe = 0;
// fin
RH_NRF24 nrf24;   // crea objeto con valores por defecto para bus


>>>>>>> 31f2844a8437498c4235c47bfe4212b26f22e63d
MAX30102 particleSensor;
Pulse pulseIR;
Pulse pulseRed;
MAFilter bpm;
<<<<<<< HEAD
#define LED LED_BUILTIN // define led
Adafruit_HTU31D htu = Adafruit_HTU31D();//declare the name of the temperature sensor
//------------------------------------------------------------------------------
//define variables
int beatAvg, SPO2, SPO2f;  //beat avarage, spo2 and spo2 final variables
bool filter_for_graph = false; // select the filter for the ir and red data
long shows_counter = 0;  // counter of show rate
=======
#define LED LED_BUILTIN
int beatAvg;
int  SPO2, SPO2f;
bool filter_for_graph = true;
uint8_t pcflag = 0;
uint8_t istate = 0;
long sleep_counter = 0;
>>>>>>> 31f2844a8437498c4235c47bfe4212b26f22e63d
const byte RATE_SIZE = 6; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
String data;
String data1;
String final_msj;
String recivido = "b";
unsigned long tiempo = 0;

void SEND(int msg) {

  switch (msg) {
    case 0:
      Serial.println("error de dispositivo");
      break;
    case 1:
      final_msj = data + data1;
      send_data(final_msj);

      break;


  }
}

void setup() {
  pinMode(LED, OUTPUT);
  // IMPRIMIR(3);
  delay(3000);
  Serial.begin(115200);
  Wire.begin();//wire start
  Wire.setClock(400000);//select frequency of the wire clock

  // Initialize sensor
  if (!particleSensor.begin(0x57)) //Use default I2C port, 400kHz speed
  {
    SEND(0);
    while (1);
  }
  if (!htu.begin(0x40)) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
<<<<<<< HEAD
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
}
//------------------------------------------------------------------------------
//MAIN LOOP
=======

  particleSensor.setup();
  if (!nrf24.init()) {
    Serial.println("fallo de inicializacion");
    while (1);
  }
  if (!nrf24.setChannel(1))
  {
    Serial.println("fallo en establecer canal");
    while (1);
  }

  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
  {
    Serial.println("fallo en opciones RF");
    while (1);
  }
  Serial.println("LISYTO");
}

>>>>>>> 31f2844a8437498c4235c47bfe4212b26f22e63d
void loop()  {

  loopTemp();
  loopOxi();
  checa_info();
  if (recivido[0] == 'a') {
    Serial.println("Recivido pa ");
    recivido = "b";
  }


  if (millis() - tiempo > 500) {
    tiempo = millis();
    SEND(1);
  }

}
void loopTemp() {
  sensors_event_t humidity, temp;

  htu.getEvent(&humidity, &temp);
  tempe = temp.temperature;
  data1 = "T";
  data1 += tempe;
  data1 += "F";

}
void loopOxi() {
  particleSensor.check();
  long now = millis();   //start time of this cycle
  if (!particleSensor.available()) return;
  uint32_t irValue = particleSensor.getIR();
  uint32_t redValue = particleSensor.getRed();
  particleSensor.nextSample();
  if (irValue < 5000) {
    data = "PAOA";
    sleep_counter = 0;
  }
  else {
    sleep_counter += 1;


    int16_t IR_signal, Red_signal;
    bool beatRed, beatIR;

    if (!filter_for_graph) {
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


    if (checkForBeat(irValue) == true)
    {
      //We sensed a beat!
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      if (beatsPerMinute < 255 && beatsPerMinute > 20)
      {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
    long numerator   = (pulseRed.avgAC() * pulseIR.avgDC()) / 256;
    long denominator = (pulseRed.avgDC() * pulseIR.avgAC()) / 256;
    int RX100 = (denominator > 0) ? (numerator * 100) / denominator : 999;

    SPO2f = (10400 - RX100 * 17 + 50) / 100;


    if (sleep_counter > 500) {
      data = "P";
      data += beatAvg;
      data += "O";
      data += SPO2f;

    }
    else {
      data = "PXOX";
    }
  }

}




void send_data(String datas) {
  const char *datis = datas.c_str();
  //Serial.println(datis);
  if (!nrf24.send((uint8_t *)datis, strlen(datis)))Serial.println("erroe");
  // Serial.println("sE ENVIO");
  nrf24.waitPacketSent();
}



void checa_info() {

  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];  // buffer con longitud maxima de 32 bytes
  uint8_t len = sizeof(buf);      // obtiene longitud de la cadena
  if (nrf24.available())      // si hay informacion disponible
  {
    if (nrf24.recv(buf, &len))      // si hay informacion valida en el buffer
    {

      recivido = String((char*)buf);
      Serial.println(recivido);

      // Serial.println(recivido);
      /*
        if(recivido != ""){
        Serial.println("entre");
        if(recivido[0] =='a'){
        digitalWrite(4,HIGH);
        send_data("ok encendido");
        recivido = "";
        }
        if(recivido[0]== 'b'){
        digitalWrite(4,LOW);

        send_data("ok encendido");
        recivido = "";
        }
      */
    }
    /*
      Serial.println(recivido.length());
      if(recivido[0] == 'h')Serial.println("hola xddd");
      //Serial.print("Recibido: ");   // muestra texto
      //Serial.println();   // muestra contenido del buffer
    */
  }
  /*
    else          // si falla la recepcion
    {
    Serial.println("fallo en recepcion"); // muestra texto
    }
  */
}