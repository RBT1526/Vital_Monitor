//Register device code Gerardo Fregoso Jiménez
//
//
//Gerardo Fregoso Jiménez
//
//
//-------------------------------------------------------------
#include<EEPROM.h>//EEPROM library added
#include <SoftwareSerial.h>// include serial library
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
#define DATABASE_URL "https://medicproyect-d2db9-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "h2lngwx8bLp73YE0C7YfJ6Cttbu6wpksdd4zPs21"

//---------––---------------------------------------------------------
/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;
SoftwareSerial miBt(5, 16); // Declare the (tx and rx) pines

//-------------------------------------------------------------
String getssid = ""; //example getvariable to read eeprom
String getpass = ""; // example getvariable to read eeprom
String getuser = ""; // example getvariable to read eeprom
char Dato;
String usuar = "";
//-------------------------------------------------------------
//Write data function
void send_eeprom(int addr, String a) { // it want an address and the string to write
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
String read_eeprom(int addr) { // it want the address
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
//-------------------------------------------------------------
// SETUP
void setup() {
  StaticJsonDocument<256> doc;
  Serial.begin(115200); // Serial communication to 115200 bps
  pinMode(2, OUTPUT); //Declare the led pin as output
  miBt.begin(38400);// start the Bluethooth communication
  EEPROM.begin(512); //Start the eeprom with 512 bytes
  
  send_eeprom(0, ""); // send the ssid to the eeprom
  send_eeprom(50, ""); // send the pass to the eeprom
  send_eeprom(100, ""); // send the pass to the eeprom
  
  getssid = read_eeprom(0);// reead the ssid of the eeprom
  getpass = read_eeprom(50); // read the pass of the eeprom
  Serial.println("Lecture finished");//message
  // print the values of the eeprom
  Serial.print("SSID = ");
  Serial.print(getssid);
  Serial.println(getssid.length());
  Serial.print("PASS = ");
  Serial.print(getpass);
  Serial.println(getpass.length());
  Serial.print("User = ");
  Serial.print(getuser);
  Serial.println(getuser.length());

  if (getssid.length() != 0) {
    WiFi.begin(getssid, getpass);  // try to connect to wifi
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
  }
  else {

    Serial.println("NOT CONNECTED");
    int bandera = 0;
    while (bandera == 0) {

      if (miBt.available()) { // if we have a message on bluethoot

        Dato = miBt.read(); // read the message and store on a char variable
        if (Dato == 'C') { // if the char is 1 turn off the led

          miBt.print("OK");
          String datas = "";
          char datos_s;
          while (true) {

            if (miBt.available()) { // if we have a message on bluethoot ssid
              datos_s = miBt.read(); // read the message and store on a char variable
              if (datos_s  == ';') {
                break;
              }
              datas += datos_s;
              Serial.println("Sacando datos");

              //Serial.println(datos_s);

            }


          }
          Serial.print("Datas = ");
          Serial.println(datas);
          DeserializationError error = deserializeJson(doc, datas);
          if (error) {

            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            miBt.print("ERRORES");
          }
          else {

            String ssid = doc["ssid"];
            Serial.println(ssid);
            String pass = doc["pass"];
            Serial.println(pass);
            String user = doc["user"];
            Serial.println(user);
            WiFi.begin(ssid, pass);  // try to connect to wifi
            Serial.print("Connecting to Wi-Fi");
            byte connected = 1;
            byte counts = 0;
            while (connected == 1) {

              Serial.print(WiFi.status());
              delay(300);
              if (WiFi.status() != 7 && WiFi.status() != 3 || counts > 100) {
                connected = 0;
              }
              if (WiFi.status() == 3) {
                connected = 2;
              }
              counts ++;
            }
            if (connected == 2) {
              Serial.println();
              Serial.print("Connected with IP: ");
              Serial.println(WiFi.localIP()); // print ip of the device
              Serial.println();
              config.database_url = DATABASE_URL;
              config.signer.tokens.legacy_token = DATABASE_SECRET;

              Firebase.reconnectWiFi(true);

              /* Initialize the library with the Firebase authen and config */
              Firebase.begin(&config, &auth);
              String test = ""; //save string
              if (Firebase.RTDB.getString(&fbdo, "/Medical_project/" + user + "/Device", &test)) {
                Serial.println(test);
                Serial.println("Sending values to eeprom"); // message

                send_eeprom(0, ssid); // send the ssid to the eeprom
                send_eeprom(50, pass); // send the pass to the eeprom
                send_eeprom(100, user); // send the ssid to the eepro
                bandera = 1;
                miBt.print("OKA");
              } // get the string of Device
              else {
                miBt.print("ERROR");
              }

            }
            else {
              miBt.print("ERROR");
            }
          }
        }

      }
    }

  }

}
//-------------------------------------------------------------
void loop()
{
Serial.println("successfully registered");
}
//-------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
