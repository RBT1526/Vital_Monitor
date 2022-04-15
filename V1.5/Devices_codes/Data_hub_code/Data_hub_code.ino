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
#include <EEPROM.h>
#include <RH_NRF24.h>
#include <Wire.h>
#include <HTU2xD_SHT2x_Si70xx.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include "time.h"
#define API_KEY "AIzaSyCKOZaxb6cXjfSdCu8hDgRz59NDJ8hx14Q"
#define USER_EMAIL "devices@gmail.com"
#define USER_PASSWORD "gerardo02"
#define DATABASE_URL "https://medicproyect-d2db9-default-rtdb.firebaseio.com" 


const char* ntpServer = "pool.ntp.org";
const int zone_gmt = -7;
const long  gmtOffset_sec = zone_gmt * 3600;
const int   daylightOffset_sec = 3600;


long actual_time;
bool Button_mark = false;


float Temperature = 0;
float Humidity = 0;

String ssid = "";
String pass = ""; 

String e_ssid = "";
String e_pass = ""; 
String token_dir = "";

String token = "";
String data_value = "";

unsigned long sendDataPrevMillis = 0;
volatile bool dataChanged = false;



RH_NRF24 nrf24(2, 0);
HTU2xD_SHT2x_SI70xx ht2x(HTU2xD_SENSOR, HUMD_12BIT_TEMP_14BIT);
const byte button = 10;


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
QueryFilter query;
FirebaseData stream;




// ###############################################################################

bool getLocalTime(struct tm * info, uint32_t ms)
{
    uint32_t start = millis();
    time_t now;
    while((millis()-start) <= ms) {
        time(&now);
        localtime_r(&now, info);
        if(info->tm_year > (2016 - 1900)){
            return true;
        }
        delay(10);
    }
    return false;
}
String correct_date (String o_data){
    int data= o_data.toInt();
    if(data < 10){
        return "0"+o_data;
    }else{
        return o_data;
    }
}
String Get_time(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo,5000)){
    Serial.println("Failed to obtain time");
    return "R";
  }
  String dateinf = String(String(timeinfo.tm_year).toInt()  + 1900)+ "-"+ correct_date(String(String(timeinfo.tm_mon).toInt() + 1))+ "-"+ correct_date(String(timeinfo.tm_mday))+ " "+correct_date(String(timeinfo.tm_hour))+ ":"+correct_date(String(timeinfo.tm_min))+ ":"+correct_date(String(timeinfo.tm_sec));    
  return dateinf;
}

// ###############################################################################

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

// ###############################################################################
// ###############################################################################
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

// ###############################################################################
// ###############################################################################


// ###############################################################################
// ###############################################################################
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



// ###############################################################################
void Config_database(){
    Serial.println("--------------FIREBASE CONFIGURATION --------------");
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    config.max_token_generation_retry = 5;
    Firebase.begin(&config, &auth);
    Serial.println("--------------FIREBASE CONFIGURATION FINISHED --------------");
}
void search_token(String identifier){
    Serial.println("---------Getting token for data path-------------");
    query.orderBy("Dir");
    query.equalTo(identifier);

    if (Firebase.RTDB.getJSON(&fbdo, "/VitalMonitor/Tokens", &query))
{
  //Success, then try to read the JSON payload value
Serial.println("Data callback completed");
 token = fbdo.jsonString();
if(token == "{}"){
    Serial.println("There is no accounts linked");
    ESP.restart();
}
else{
    Serial.println("Account linked found!");
FirebaseJson datos(token);
datos.iteratorBegin();
FirebaseJson::IteratorValue value = datos.valueAt(0);
token = String(value.key.c_str());
datos.iteratorEnd();
Serial.print("Token= ");
Serial.println(token);
}
/*
datos.get(result , "oWyKR3UuoVV6Z1yY4V1Sav8c4VJ3");
 if (result.success)
    {
       
        Serial.println(result.to<String>().c_str());
    }
else{
    Serial.println("Fail_json dump");
}
*/

}
else
{
  //Failed to get JSON data at defined node, print out the error reason
  Serial.println("Account searching error: ");
  Serial.println(fbdo.errorReason());
  ESP.restart();
}

//Clear all query parameters
query.clear();
Serial.println("---------Getting token for data path finished-------------");
}

void start_streaming (String uidToken){
    Serial.println("---------------Stream Configuration---------------");
    Serial.print("Path = ");
    Serial.println("/VitalMonitor/"+uidToken+"/Iot/Press");
    Serial.print("Starting the stream........");
      if(!Firebase.RTDB.beginStream(&stream, "/VitalMonitor/"+uidToken+"/Iot/Press")){
          Serial.printf("Error :, %s\n\n", stream.errorReason().c_str());
          ESP.restart();
          }

    Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
Serial.println("OK");
Serial.println("---------------Stream Configuration finished---------------");
}











void streamCallback(FirebaseStream data)
{
data_value = String(data.stringData().c_str());
    Serial.print("New value! = ");
  Serial.println(data_value);
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timed out, resuming...\n");

  if (!stream.httpConnected()){
    
    Serial.printf("Error with stream: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
    ESP.restart();    
  }
}
// ###############################################################################




void json_form(String raw_data) {
  
  int Pos =  raw_data.indexOf("b");
  int Pos_1 = raw_data.indexOf("s");
  json.set("Bpm",raw_data.substring(0, Pos));
  json.set("Spo2",raw_data.substring((Pos + 1), Pos_1));
  Pos = raw_data.indexOf("f");
  json.set("Temp",raw_data.substring((Pos_1 + 1), Pos));
  json.set("Ahumi",String(Humidity));
  json.set("Atemp",String(Temperature));
  json.set("Date",Get_time());
}



void setup()
{
  Serial.begin(115200);
    EEPROM.begin(512);   //start comunication
  init_htu();
 init_radio();
 pinMode(button,INPUT);
Serial.println("Reset button OK");
//Configure_wifi();
check_for_connection();
configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println(Get_time());
token_dir = WiFi.macAddress();
  token_dir.replace(":","");
  token_dir.toLowerCase();
  token_dir = WiFi.localIP().toString() + token_dir;
  Serial.print("TOKEN FOR DIR = ");
  Serial.println(token_dir);
Config_database();
search_token(token_dir);
start_streaming(token);

    json.add("Bpm", "");
    json.add("Spo2","");
    json.add("Temp","");
    json.add("Date","");
    json.add("Sys","");
    json.add("Dia","");
    json.add("Ahumi","");
    json.add("Atemp","");
    Serial.printf("Send String... %s\n", Firebase.RTDB.setString(&fbdo, "/VitalMonitor/"+token+"/Iot/Press", "False") ? "ok" : fbdo.errorReason().c_str());
Serial.println("-----------SETUP FINISHED----------");
 actual_time = millis();
}
//-----------------------------------------------------------------------------------
//Main Loop
void loop()
{
  bool check = check_for_button();
  get_temp_hum();
  String msj = get_radio_msj();
  if(Firebase.ready() && msj != "" && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
  json_form(msj);
  sendDataPrevMillis = millis();
  Serial.printf("Send json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/VitalMonitor/"+token+"/Data", &json) ? "ok" : fbdo.errorReason().c_str());
  Serial.println(json.raw());
  }
  

  /*
  Serial.print("Temperature = ");
  Serial.print(Temperature);
  Serial.print("C Humidity = ");
  Serial.print(Humidity);
  Serial.print("% Message = '");
  Serial.print(msj);
  Serial.print("' Button = ");
  */
  if(check){
  Serial.println("PRESSED");
  erase_eeprom();
  }
  /*else
  Serial.println("");
  */

  if (dataChanged)
  {
    // AQUI SE HACE LO DE SACAR EL DIA Y SYS
    dataChanged = false;
    if(data_value == "True"){
      Serial.printf("Send String... %s\n", Firebase.RTDB.setString(&fbdo, "/VitalMonitor/"+token+"/Iot/Press", "False") ? "ok" : fbdo.errorReason().c_str());
    }
    
  }


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//-----------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
