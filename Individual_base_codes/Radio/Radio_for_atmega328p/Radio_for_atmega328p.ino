//Code for radio nrf24l01 atmega328p
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
RH_NRF24 nrf24;
//-----------------------------------------------------------------------------------
//SETup
void setup()
{
  Serial.begin(115200); //start comunicaction at 115200 bps
  //start radio
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1)) //set channel
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}
//-----------------------------------------------------------------------------------
//MAIN LOOP
void loop()
{
  Serial.println("Sending to nrf24_server");//print a message to announce the status
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Send a message to nrf24_server
  uint8_t data[] = "Hello World!";//uint8_t variable with the text to send
  nrf24.send(data, sizeof(data));//command to send the message
  nrf24.waitPacketSent();//wait until it send the message
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];//create the storage of 32 bytes for the message incoming
  uint8_t len = sizeof(buf);// lenght of the buffer.
  if (nrf24.waitAvailableTimeout(500)) // wait 500 miliseconds for reply
  {
    ///////////////////////////////////////////////////////////////////////////////7/
    // Should be a reply message for us now
    if (nrf24.recv(buf, &len)) // if we receive an answer
    {
      Serial.print("got reply: ");// print that it got an answer
      Serial.println((char*)buf);//print the message
    }
    else
    {
      Serial.println("recv failed");// if something failed on the incoming message
    }
    ///////////////////////////////////////////////////////////////////////////////7/
  }
  else
  {
    Serial.println("No reply, is nrf24_server running?");//if we dont receive we print this
  }
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  delay(400); //delay of 400 milisec
}
//-----------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
