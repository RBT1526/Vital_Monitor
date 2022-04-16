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
//-----------------------------------------------------------------------------------
// Singleton instance of the radio driver
RH_NRF24 nrf24(2, 0);
//-----------------------------------------------------------------------------------
//Setup
void setup()
{
  Serial.begin(115200); //start comunication
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))//set channel
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}
//-----------------------------------------------------------------------------------
//Main Loop
void loop()
{
  if(Serial.available()){
    String xd = Serial.readString();
   // Serial.println(xd);
    xd.trim();
    if(xd == "a"){
      uint8_t data[] = "A";
      //Serial.println("entre a a");
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      Serial.println("Sended");
      xd = "b";
    }
     if(xd == "c"){
      uint8_t data[] = "M";
      //Serial.println("entre a c");
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      xd = "b";
    }
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //wait for a message
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
      Serial.print("got request: ");
      Serial.println(data_received);
    }
    else
    {
      Serial.println("recv failed");
    }
    ///////////////////////////////////////////////////////////////////////////////7/
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//-----------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
