#include <ESP8266WiFi.h>
#include <EEPROM.h>

long actual_time;
bool Button_mark = false;
const byte button = 10;

String ssid = "";
String pass = ""; 

String e_ssid = "";
String e_pass = ""; 

void Configure_wifi(){
Serial.println("--------------------- Configuration mode --------------------");
 WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("SmartConfig done.");
  Serial.println("Waiting for WiFi");
  actual_time = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    bool check = check_for_button();
    if(check)
    erase_eeprom(); 
  }
  Serial.println("WiFi Connected.");
  ssid = WiFi.SSID();
    pass = WiFi.psk();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Sending values to eeprom....");
writeStringToFlash(ssid, 0); // storing ssid at address 0
writeStringToFlash(pass, 50); // storing pss at address 40
Serial.println("Values sended");
Serial.println("--------------------- Configuration Finished --------------------");
  
}







void writeStringToFlash(String a,int addr ) { // it want an address and the string to write
  int len = a.length(); // get the string lenght
  char inchar[50]; //create a char to save the string
  a.toCharArray(inchar, len + 1); //change string to array
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr + i, inchar[i]); // Write the values in eeprom
  }
  for (int i = len; i < 50; i++) {
    EEPROM.write(addr + i, 255); // clean the other bytes
  }
  EEPROM.commit(); // send the values

}
//-------------------------------------------------------------
// Function to read the eeprom
String readStringFromFlash(int addr) { // it want the address
  byte reading; // byte variable to save the value
  String strreading; // final string
  for (int i = addr; i < addr + 50; i++) {
    reading = EEPROM.read(i); // read every byte of the eeprom
    if (reading != 255) { // read until theres no values
      strreading += (char)reading;
    }
  }
  return strreading; // return the final string
}



void check_for_connection () {

Serial.println("-------------Starting connection try---------");
e_ssid = readStringFromFlash(0);
e_pass = readStringFromFlash(50);
Serial.print("SSID = ");
Serial.print(e_ssid);
Serial.print(" PASS = ");
Serial.println(e_pass);
if(e_ssid ==  "" || e_pass == ""){
    Serial.println("NO SSID OR PASS REGISTERED");
    Configure_wifi();
}
else{
    WiFi.begin(e_ssid, e_pass);
    actual_time = millis();
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    bool check = check_for_button();
    if(check)
    erase_eeprom();
  }
 Serial.println("-------------Connection finished succesfully---------"); 

  


}

/*
// 1V CHECK EEPROM CODE 
Serial.println("Reading eeprom values....");
e_ssid = readStringFromFlash(0);
e_pass = readStringFromFlash(50);
Serial.println("Readings finished");*/

}




bool check_for_button(){
    if((millis()-actual_time) >= 5000){
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

void erase_eeprom(){
    Serial.println("ERASING EEPROM......");
 writeStringToFlash("", 0); // storing ssid at address 0
  writeStringToFlash("", 50);
  Serial.println("EEPROM ERASED");
  ESP.restart();
}
void setup()
{
  Serial.begin(115200);
    EEPROM.begin(512);   //start comunication
 pinMode(button,INPUT);
Serial.println("Reset button OK");
//Configure_wifi();
check_for_connection();


Serial.println("-----------SETUP FINISHED----------");
 actual_time = millis();
}
void loop()
{
  bool check = check_for_button();
  if(check){
  Serial.println("PRESSED");
  erase_eeprom();
  }}