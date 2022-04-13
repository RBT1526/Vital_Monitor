//Code for radio nrf24l01 esp8266
//
//
//
//Gerardo Fregoso Code based on RadioHead library
//
//For module nrf24l01
//
//-----------------------------------------------------------------------------------
//import the libraries
#include <SPI.h>
#include <RH_NRF24.h>
#include <Wire.h>
#include <HTU2xD_SHT2x_Si70xx.h>


long actual_time;
bool Button_mark = false;


float Temperature = 0;
float Humidity = 0;




RH_NRF24 nrf24(2, 0);
HTU2xD_SHT2x_SI70xx ht2x(HTU2xD_SENSOR, HUMD_12BIT_TEMP_14BIT);
const byte button = 10;

void init_radio () {
 while (!nrf24.init()){
    Serial.println("init failed");
    delay(5000);
 }
    
  while (!nrf24.setChannel(1))
  {
      Serial.println("setChannel failed");
      delay(5000);
  }
    
  while (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)){
      Serial.println("setRF failed");
      delay(5000); 
  }
    

Serial.println("RADIO OK");
}


void init_htu () {
    while (ht2x.begin() != true) //reset sensor, set heater off, set resolution, check power (sensor doesn't operate correctly if VDD < +2.25v)
  {
    Serial.println("HTU21D not connected"); //(F()) save string to flash & keeps dynamic memory free

    delay(5000);
  }

  Serial.println("HTU21D OK");
}








bool check_for_button(){
    if((millis()-actual_time) >= 3000){
        actual_time = millis();
       Button_mark= false;
        return true;
    }
    else{
    if(digitalRead(10) && !Button_mark){
        actual_time = millis();
        Button_mark = true;
        
    }
    else if(!digitalRead(10)){
        actual_time = millis();
        Button_mark = false;
        
    }
    return false;
    }
}

void get_temp_hum () {
 
 Temperature = ht2x.readTemperature();  
 Humidity = ht2x.getCompensatedHumidity(Temperature);       
}
String get_radio_msj(){
    String final_msj = "";
    if (nrf24.available())
  {

    // Now wait for a reply
    // Should be a message for us now
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];//create the storage of 32 bytes for the message incoming
    uint8_t len = sizeof(buf);// lenght of the buffer.
    ///////////////////////////////////////////////////////////////////////////////7/
    // Should be a reply message for us now
    if (nrf24.recv(buf, &len))
    {
      //      NRF24::printBuffer("request: ", buf, len);
      String data_received =  String((char*)buf);
        data_received.trim();
        final_msj = data_received;
    }
    else
    {
      return final_msj;
    }
    ///////////////////////////////////////////////////////////////////////////////7/
  }
  return final_msj;
}

void setup()
{
  Serial.begin(115200); //start comunication
  init_htu();
 init_radio();
 pinMode(button,INPUT);
Serial.println("Reset button OK");
 
}
//-----------------------------------------------------------------------------------
//Main Loop
void loop()
{
  bool check = check_for_button();
  
  get_temp_hum();
  String msj = get_radio_msj();
  Serial.print("Temperature = ");
  Serial.print(Temperature);
  Serial.print("C Humidity = ");
  Serial.print(Humidity);
  Serial.print("% Message = '");
  Serial.print(msj);
  Serial.print("' Button = ");
  if(check)
  Serial.println("PRESSED");
  else
  Serial.println("");




  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//-----------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
