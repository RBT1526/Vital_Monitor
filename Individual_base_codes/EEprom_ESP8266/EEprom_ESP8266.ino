//EEprom write and read example code for esp8266
//
//
//Gerardo Fregoso Jiménez code with EEPROM library
//
//
//-------------------------------------------------------------
#include<EEPROM.h>//EEPROM library added
//-------------------------------------------------------------
String ssid = "Internet_random_ssid"; //Example ssid to write on the eeprom
String pass = "Random_password"; // Example password to wtire on the eeprom
String getssid = ""; //example getvariable to read eeprom
String getpass = ""; // example getvariable to read eeprom
//-------------------------------------------------------------
//Write data function
void send_eeprom(int addr, String a) { // it want an address and the string to write
  int len= a.length(); // get the string lenght 
  char inchar[50]; //create a char to save the string
  a.toCharArray(inchar, len+1);//change string to array
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr+i, inchar[i]);// Write the values in eeprom
  }
  for (int i = len; i < 50; i++) {
    EEPROM.write(addr+i, 255); // clean the other bytes
  }
  EEPROM.commit(); // send the values

}
//-------------------------------------------------------------
// Function to read the eeprom
String read_eeprom(int addr) { // it want the address
   byte reading; // byte variable to save the value
   String strreading; // final string
   for (int i = addr; i < addr+50; i++) {
      reading = EEPROM.read(i); // read every byte of the eeprom
      if (reading != 255) { // read until theres no values
        strreading += (char)reading;
      }
   }
   return strreading; // return the final string
}
//-------------------------------------------------------------
// SETUP
void setup() {
  Serial.begin(115200); // Serial communication to 115200 bps
  EEPROM.begin(512); //Start the eeprom with 512 bytes
  delay(5000); // delay of 5 seconds
  Serial.println("Sending values to eeprom"); // message 
  send_eeprom(0,ssid); // send the ssid to the eeprom 
  send_eeprom(50,pass); // send the pass to the eeprom
  Serial.println("Sended"); // message
Serial.println("Starting lecture"); // message
  getssid = read_eeprom(0);// reead the ssid of the eeprom
  getpass = read_eeprom(50); // read the pass of the eeprom
Serial.println("Lecture finished");//message
// print the values of the eeprom
Serial.print("SSID = ");
Serial.println(getssid);
Serial.print("PASS = ");
Serial.println(getpass);

}
//-------------------------------------------------------------
void loop()
{
  //Nothing here
}
//-------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022