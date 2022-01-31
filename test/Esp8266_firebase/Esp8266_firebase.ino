// authenticate using the legacy token or database secret firebase example esp8266
//
//
//
//Gerardo Fregoso Jiménez Code based on firebase library
//
//
//
//
//
//---------––---------------------------------------------------------
#include <ESP8266WiFi.h>


#include <Firebase_ESP_Client.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "NASA"
#define WIFI_PASSWORD "261121242611212404"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://medicproyect-d2db9-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "h2lngwx8bLp73YE0C7YfJ6Cttbu6wpksdd4zPs21"

//---------––---------------------------------------------------------
/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0; //timekeeper
int count = 0; // counter 
String test = ""; //save string

//---------––---------------------------------------------------------
void setup()
{

    Serial.begin(115200); // start serial with 115200 bps

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // try to connect to wifi
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP()); // print ip of the device
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); // print firebase version


    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
}

//---------––---------------------------------------------------------
void loop()
{   
    //every 1 second
    if (millis() - dataMillis > 1000)
    {
        dataMillis = millis();
        Firebase.RTDB.setInt(&fbdo, "/Medical_project/Gerardo /Datos/Bpm", count++); // send count  ++ to Bpm
        Firebase.RTDB.setInt(&fbdo, "/Medical_project/Gerardo /Datos/Temp", count+=2);// send count  count =+2 to Temp
        Firebase.RTDB.setInt(&fbdo, "/Medical_project/Gerardo /Datos/Spo2", count++);// send count  count ++ to Spo2
        Firebase.RTDB.getString(&fbdo, "/Medical_project/Gerardo /Device", &test); // get the string of Device
        Serial.println(test);
    }
    
}

//---------––---------------------------------------------------------
//Gerardo Fregoso Jiménez 2022