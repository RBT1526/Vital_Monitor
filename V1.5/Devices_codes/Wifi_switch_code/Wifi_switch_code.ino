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

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

const byte out_rel = 0;
const byte light = 1;
const byte button = 3;



#define API_KEY "AIzaSyCKOZaxb6cXjfSdCu8hDgRz59NDJ8hx14Q"
#define USER_EMAIL "devices@gmail.com"
#define USER_PASSWORD "gerardo02"
#define DATABASE_URL "https://medicproyect-d2db9-default-rtdb.firebaseio.com" 

long actual_time;
bool Button_mark = false;


String ssid = "";
String pass = ""; 

String e_ssid = "";
String e_pass = ""; 
String token_dir = "";

String token = "";
String data_value = "";

volatile bool dataChanged = false;



FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
QueryFilter query;
FirebaseData stream;




// ###############################################################################

// ###############################################################################

// ###############################################################################
// ###############################################################################
void Configure_wifi(){
 WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  digitalWrite(light,HIGH);
  while (!WiFi.smartConfigDone()) {
    delay(500);
  }
  digitalWrite(light,LOW);
  actual_time = millis();
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(light, HIGH);
    delay(950);
    digitalWrite(light, LOW);
    delay(950);
    bool check = check_for_button();
    if(check)
    erase_eeprom(); 
  }

  ssid = WiFi.SSID();
    pass = WiFi.psk();
writeStringToFlash(ssid, 0); // storing ssid at address 0
writeStringToFlash(pass, 50); // storing pss at address 40
  
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


e_ssid = readStringFromFlash(0);
e_pass = readStringFromFlash(50);
if(e_ssid ==  "" || e_pass == ""){
    Configure_wifi();
}
else{
    WiFi.begin(e_ssid, e_pass);
    actual_time = millis();
    while (WiFi.status() != WL_CONNECTED) {
    bool check = check_for_button();
    digitalWrite(light, HIGH);
    delay(950);
    check = check_for_button();
    digitalWrite(light, LOW);
    delay(950);
    check = check_for_button();
    if(check)
    erase_eeprom();
  }

  


}


}
bool check_for_button(){
    if((millis()-actual_time) >= 5000 && Button_mark){
        actual_time = millis();
       Button_mark= false;
        return true;
    }
    else{
    if(digitalRead(button) && !Button_mark){
        actual_time = millis();
        Button_mark = true;
        
    }
    else if(!digitalRead(button)){
        actual_time = millis();
        Button_mark = false;
        
    }
    return false;
    }
}

void erase_eeprom(){

 writeStringToFlash("", 0); // storing ssid at address 0
  writeStringToFlash("", 50);
  ESP.restart();
}

// ###############################################################################
// ###############################################################################


// ###############################################################################
// ###############################################################################


// ###############################################################################
void Config_database(){
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    config.max_token_generation_retry = 5;
    Firebase.begin(&config, &auth);
}
void search_token(String identifier){
    query.orderBy("Wifidir");
    query.equalTo(identifier);
    if (Firebase.RTDB.getJSON(&fbdo, "/VitalMonitor/Tokens", &query))
{
  //Success, then try to read the JSON payload value
 token = fbdo.jsonString();
if(token == "{}"){
    ESP.restart();
}
else{
FirebaseJson datos(token);
datos.iteratorBegin();
FirebaseJson::IteratorValue value = datos.valueAt(0);
token = String(value.key.c_str());
datos.iteratorEnd();
}

}
else
{
  ESP.restart();
}

//Clear all query parameters
query.clear();
}

void start_streaming (String uidToken){
      if(!Firebase.RTDB.beginStream(&stream, "/VitalMonitor/"+uidToken+"/Iot/Switch")){
          ESP.restart();
          }

    Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
}











void streamCallback(FirebaseStream data)
{
data_value = String(data.stringData().c_str());
  dataChanged = true;
}

void streamTimeoutCallback(bool timeout)
{

  if (!stream.httpConnected()){
    ESP.restart();    
  }
}
// ###############################################################################


void erase_data_stream(){
    bool checked_msj = false;
    while(checked_msj == false){
    if (Firebase.RTDB.getString(&fbdo, "/VitalMonitor/"+token+"/Iot/Switch")) {
      if(fbdo.to<String>() != "False"){
        Firebase.RTDB.setString(&fbdo, "/VitalMonitor/"+token+"/Iot/Switch", "False");
        
      }else{
        checked_msj = true;
      }
    

  } else {
    ESP.restart();
  }
    }
}
void setup()
{
    EEPROM.begin(512);   //start comunication
 pinMode(button,INPUT);
 pinMode(out_rel,OUTPUT);
 pinMode(light,OUTPUT);
 digitalWrite(light, HIGH);
    delay(2000);
    digitalWrite(light, LOW);
if(digitalRead(button)){
  erase_eeprom();
}
check_for_connection();
token_dir = WiFi.macAddress();
  token_dir.replace(":","");
  token_dir.toLowerCase();
  token_dir = WiFi.localIP().toString() + token_dir;
Config_database();
search_token(token_dir);
erase_data_stream();
start_streaming(token);
digitalWrite(light, HIGH);
    delay(500);
    digitalWrite(light, LOW);
    delay(500);
    digitalWrite(light, HIGH);
    delay(500);
    digitalWrite(light, LOW);
 actual_time = millis();
}
//-----------------------------------------------------------------------------------
//Main Loop
void loop()
{
  bool check = check_for_button();
  if(check){
  erase_eeprom();
  }
  if (dataChanged)
  {
    dataChanged = false;
    if(data_value == "True"){
      digitalWrite(out_rel, HIGH); 
    actual_time = millis();
    }
    else{
        digitalWrite(out_rel, LOW); 
    actual_time = millis();
    }
    
  }


  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
}
//-----------------------------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
