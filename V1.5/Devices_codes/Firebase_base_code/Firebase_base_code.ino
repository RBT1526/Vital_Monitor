  #include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "NASA"
#define WIFI_PASSWORD "261121242611212404"


#define API_KEY "AIzaSyCKOZaxb6cXjfSdCu8hDgRz59NDJ8hx14Q"
#define USER_EMAIL "devices@gmail.com"
#define USER_PASSWORD "gerardo02"
#define DATABASE_URL "https://medicproyect-d2db9-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;
FirebaseJson json_child;
FirebaseJsonData result;
QueryFilter query;



String token = "";
String data_value = "";

unsigned long sendDataPrevMillis = 0;
long timese;
volatile bool dataChanged = false;
FirebaseData stream;

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








void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }




    Config_database();
    
    search_token("prueba2.221");
    start_streaming(token);
    json.add("Bpm", "");
    json.add("SPO2","");
    json.add("Temp","");
    json.add("Date","");
    json.add("Sys","");
    json.add("Dia","");



    
}

void loop()
{
    
//aqui obtengo los datos 
//datos obtenidos xd 
    json.set("Bpm", 72);
    json.set("SPO2",98);
    json.set("Temp",32);
    json.set("Date","12/12/2010 24:42:12");
    json.set("Sys",122);
    json.set("Dia",89);



// aqui debo de mandar las cosas a firebase
 if (Firebase.ready() && (millis() - sendDataPrevMillis > 3000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Serial.printf("Send json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/VitalMonitor/"+token+"/Data", &json) ? "ok" : fbdo.errorReason().c_str());
  }

  if (dataChanged)
  {
    dataChanged = false;
    // When stream data is available, do anything here...
  }


}
