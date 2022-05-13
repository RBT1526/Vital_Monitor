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

String diastolic = "";
String systolic = "";
String data_sys_dia_msj = "";

bool sdflag = false;

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
    ESP.restart();
    return "R";
  }
  String dateinf = String(String(timeinfo.tm_year).toInt()  + 1900)+ "-"+ correct_date(String(String(timeinfo.tm_mon).toInt() + 1))+ "-"+ correct_date(String(timeinfo.tm_mday))+ " "+correct_date(String(timeinfo.tm_hour))+ ":"+correct_date(String(timeinfo.tm_min))+ ":"+correct_date(String(timeinfo.tm_sec));    
  return dateinf;
}

// ###############################################################################

void init_radio () {
 if (!nrf24.init()){
    Serial.println("init failed");
    delay(5000);
 }
    
if (!nrf24.setChannel(1))
  {
      Serial.println("setChannel failed");
      delay(5000);
  }
    
if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)){
      Serial.println("setRF failed");
      delay(5000); 
  }
    

Serial.println("RADIO OK");
}


void init_htu () {
    if (ht2x.begin() != true) //reset sensor, set heater off, set resolution, check power (sensor doesn't operate correctly if VDD < +2.25v)
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
    if((millis()-actual_time) >= 5000 && Button_mark){
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
  
  json.set("Date",Get_time());
  if(diastolic != "" && systolic != ""){
    if(diastolic == "O" || systolic == "O"){
json.set("Dia","");
  json.set("Sys","");
    }else{
    json.set("Dia",diastolic);
  json.set("Sys",systolic);
  sdflag = true;
    }
  diastolic = "";
  systolic = "";

  }
  else{
    
    json.set("Dia","");
  json.set("Sys","");
    
  }
  if(String(Humidity) == "255.00" || String(Temperature) == "255.00"){
json.set("Ahumi","R");
  json.set("Atemp","R");
  }
  else{
  json.set("Ahumi",String(Humidity));
  json.set("Atemp",String(Temperature));
  }
}


void send_data(String data_to_send){
  const char *send_form = data_to_send.c_str();
  if(!nrf24.send((uint8_t *)send_form, strlen(send_form))){
  Serial.println("Error sending the message");
  ESP.restart();
  }
  Serial.println("Message sended!");
  nrf24.waitPacketSent();
}
int check_for_dia(){
  Serial.println("checking_for_residual message");
String msj = get_radio_msj();

long temp = millis();
byte msj_count = 0;

while (msj == ""){
  
  msj = get_radio_msj();
  if((millis()-temp)>= 1000){
    Serial.println("waiting for message....");
    temp = millis();
    msj_count++;
  }
  if(msj_count >= 7){
  Serial.println("message not found returning value");
  return 3;}
}
Serial.println("message received");
if(msj.indexOf("Z") != -1){
data_sys_dia_msj = msj;
return 1;
}
else
return 2;
}
void format_pressure_data(){
  int pos=  data_sys_dia_msj.indexOf("y");
    int pos1 = data_sys_dia_msj.indexOf("d");
    systolic = data_sys_dia_msj.substring((pos+1), pos1);
    pos = data_sys_dia_msj.indexOf("h");
    diastolic = data_sys_dia_msj.substring((pos1 + 1), pos);
    
    data_sys_dia_msj = "";
}
bool get_dia_sys(){
  Serial.println("---------------------getting_pressure_Data----------");
Serial.println("---------------First part-----------------");
send_data("M");

int  checked = 0;
while(checked != 2){
checked = check_for_dia();
switch (checked)
{
case 1:
  Serial.println("The message have dia and sys rechecking");
  send_data("M");
  break;

case 2:
Serial.println("The message was received");
checked = 2;
  break;
  case 3:
  Serial.println("Theres no message");
  return false;
  break;
}
}
Serial.println("---------------second part-----------------");


send_data("A");



Serial.println("waiting 5 seconds");
delay(5000);



checked = check_for_dia();


while(checked != 3){


checked = check_for_dia();
switch (checked)
{

case 1:
  Serial.println("The message have dia and sys rechecking");
  send_data("M");
  break;

case 2:
Serial.println("The message was received");
send_data("A");

Serial.println("waiting 3 seconds");
delay(3000);

  break;

  case 3:
  Serial.println("Theres no message");
  Serial.println("MESSAGE RECEIVED SUCCESFULLY");
  break;

}
}
Serial.println("---------------------third part--------------");

long time_temp = millis();
checked = 0;
Serial.println("Starting wait_time");
int countermsj = 0;
while((millis()-time_temp)< 90000){
  Serial.print("MILLIS_TEMP = ");
  Serial.println(millis()-time_temp);
  Firebase.RTDB.readStream(&stream);
checked = check_for_dia();

switch (checked)
{

case 1:
    Serial.println("The message have dia and sys extracting values");
    format_pressure_data();  
    return true;
  break;

case 2:
  Serial.println("The message don't have the pressure data");
  countermsj++;
  if(countermsj>4){
    countermsj = 0;
  return false;
  }
  break;

}

}
Serial.println("exeded time");

Serial.println("---------------------data_function finished----------");
return false;
}

void erase_data_stream(){
  Serial.println("TRYING GET DATA OF STREAM");
    bool checked_msj = false;
    while(checked_msj == false){
    if (Firebase.RTDB.getString(&fbdo, "/VitalMonitor/"+token+"/Iot/Press")) {
      if(fbdo.to<String>() != "False"){
        Serial.printf("Send String... %s\n", Firebase.RTDB.setString(&fbdo, "/VitalMonitor/"+token+"/Iot/Press", "False") ? "ok" : fbdo.errorReason().c_str());
      }else{
        checked_msj = true;
      }
    

  } else {
    Serial.println(fbdo.errorReason());
    ESP.restart();
  }
    }
    Serial.println("DATA_checked");
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
if(digitalRead(button)){
  erase_eeprom();
}
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
erase_data_stream();
start_streaming(token);

    json.add("Bpm", "");
    json.add("Spo2","");
    json.add("Temp","");
    json.add("Date","");
    json.add("Sys","");
    json.add("Dia","");
    json.add("Ahumi","");
    json.add("Atemp","");
    
    
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
  Serial.print("Send json...");
  if(Firebase.RTDB.pushJSON(&fbdo, "/VitalMonitor/"+token+"/Data", &json)){
    Serial.println("ok");
    if(sdflag){
    String pathofdata = String(fbdo.pushName());
    Serial.println(pathofdata);
    Serial.printf("Send json... %s\n", Firebase.RTDB.setStringAsync(&fbdo, "/VitalMonitor/"+token+"/Lval", pathofdata) ? "ok" : fbdo.errorReason().c_str());
    sdflag = false;
  }
  }
  else{
    Serial.println(fbdo.errorReason().c_str());
  }
  
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
    dataChanged = false;
    if(data_value == "True"){
      bool data_get = get_dia_sys();
      if(data_get){
     
          
      Serial.println("finished_data_sys");
      }
      else{
        diastolic = "";
        systolic = "";
        data_sys_dia_msj = "";
        Serial.println("finished_data_sys with error");
      }
      erase_data_stream();  
    actual_time = millis();
    }
    
  }


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//-----------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
