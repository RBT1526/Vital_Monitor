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

    if (Firebase.RTDB.getJSON(&fbdo, "/VitalMonitor", &query))
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
    
    search_token("192.168.1.9668c63ab8271z");
    
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
Serial.printf("Send json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/VitalMonitor/"+token+"/Data", &json) ? "ok" : fbdo.errorReason().c_str());
delay(1000);

//aqui la condicion de tomar presion


}
