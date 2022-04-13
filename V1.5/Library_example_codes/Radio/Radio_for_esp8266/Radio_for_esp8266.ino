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
RH_NRF24 nrf24(2, 4);
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
      Serial.print("got request: ");
      Serial.println((char*)buf);

      // Send a reply
      uint8_t data[] = "And hello back to you";
      nrf24.send(data, sizeof(data));
      nrf24.waitPacketSent();
      Serial.println("Sent a reply");
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
