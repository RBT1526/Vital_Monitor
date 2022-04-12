//EEprom write and read example code for esp8266
//
//
//Gerardo Fregoso Jiménez code with EEPROM library
//
//
//-------------------------------------------------------------
#include<EEPROM.h>//EEPROM library added
#include <SoftwareSerial.h>// include serial library
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>
#include <RH_NRF24.h>
#include "time.h"

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
RH_NRF24 nrf24(2, 4);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -25200;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 0;  //Replace with your daylight offset (seconds)
//-------------------------------------------------------------
String getssid = ""; //example getvariable to read eeprom
String getpass = ""; // example getvariable to read eeprom
String getuser = ""; // example getvariable to read eeprom
char Dato;
String usuar = "";
String BPM = "";
String oxigeno = "";
String TEMP = "";
String test = "";
String user = "";
String dia = "";
String sys = "";
String hr = "";
String reg  ="";
String Month;
String day;
String year;
String clocka;
unsigned long tiempo = 0; //variable for time comparation
 
//-------------------------------------------------------------
//Write data function
void Get_Time()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  String pus =  (asctime(timeinfo));
  Month = pus.substring(4, 7);
  day = pus.substring(8, 10);
  year = pus.substring(20,24);
  clocka = pus.substring(11,19);
  Month.replace(" ", "");
  year.replace(" ", "");
  clocka.replace(" ", "");
  day.replace(" ", "");
}

void get_p_data(){
  Serial.println("sera?");
  bool msj = false;
  uint8_t datas[] = "A";
      nrf24.send(datas, sizeof(datas));
      nrf24.waitPacketSent();
      tiempo = millis();
    
  bool send_data = false;
  Serial.println("ok papu");
  while(send_data == false){
  
    if (nrf24.available())
  {
       //Serial.println("na ni de pedo");
        uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];//create the storage of 32 bytes for the message incoming
    uint8_t len = sizeof(buf);// lenght of the buffer.
    ///////////////////////////////////////////////////////////////////////////////7/
    // Should be a reply message for us now
    if (nrf24.recv(buf, &len))
    {
     Serial.println("aqui andamos");
      msj = true;
      uint8_t datos[] = "A";
      nrf24.send(datos, sizeof(datos));
      
      nrf24.waitPacketSent();
      Serial.println("pase");
      tiempo = millis();
    }
      //
      //Serial.println("ya no te creo");
      }
     delay(10); 
     //Serial.println("ya no te creo x2");
    if((millis() - tiempo)>5000 && msj == false){
      Serial.println("o si?");
      send_data = true;
    }
    //Serial.println("el mensaje es falso");
    msj = false;
  
  }
  Serial.println("Se mando el msj");
bool got_data = false;

while (got_data == false){
  
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
      Serial.println("llego hasta aca");
      data_received.trim();
      Serial.print("got data");
      Serial.println(data_received);
      
if(data_received.indexOf("Z") != -1){
        int posicion =  data_received.indexOf("y");
        int posicion1 = data_received.indexOf("d");
    sys = data_received.substring((posicion+1), posicion1);
  //Serial.println(BPM);
    posicion = data_received.indexOf("h");
    dia = data_received.substring((posicion1 + 1), posicion);
  //Serial.println(oxigeno);
    posicion1 = data_received.indexOf("Z");
    hr =  data_received.substring((posicion + 1), posicion1);
    got_data = true;

      Serial.println("all data received");
      }
    
    }
    else
    {
      Serial.println("recv failed");
    }
    ///////////////////////////////////////////////////////////////////////////////7/
    
  }
  delay(10);
}

}
void separar(String dats) {
  int posicion =  dats.indexOf("b");
  BPM = dats.substring(0, posicion);
  //Serial.println(BPM);
  int posicion1 = dats.indexOf("s");
  oxigeno = dats.substring((posicion + 1), posicion1);
  //Serial.println(oxigeno);
  posicion = dats.indexOf("f");
  TEMP =  dats.substring((posicion1 + 1), posicion);
  //Serial.println(TEMP);
}

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
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(1))//set channel
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

  pinMode(2, OUTPUT); //Declare the led pin as output
  miBt.begin(38400);// start the Bluethooth communication
  EEPROM.begin(512); //Start the eeprom with 512 bytes
  /*
  send_eeprom(0, ""); // send the ssid to the eeprom
  send_eeprom(50, ""); // send the pass to the eeprom
  send_eeprom(100, ""); // send the pass to the eeprom
  */
  getssid = read_eeprom(0);// reead the ssid of the eeprom
  getpass = read_eeprom(50); // read the pass of the eeprom
  user = read_eeprom(100); // read the pass of the eeprom
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
     config.database_url = DATABASE_URL;
              config.signer.tokens.legacy_token = DATABASE_SECRET;

              Firebase.reconnectWiFi(true);

              /* Initialize the library with the Firebase authen and config */
              Firebase.begin(&config, &auth);
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
            miBt.print("ERROR");
          }
          else {

            String ssid = doc["ssid"];
            Serial.println(ssid);
            String pass = doc["pass"];
            Serial.println(pass);
            String usar = doc["user"];
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

              Firebase.begin(&config, &auth);
              /* Initialize the library with the Firebase authen and config */
              Firebase.RTDB.set(&fbdo, "/Medical_project/"+usar+"/Device", "False"); // send count  ++ to Bpm
              if (Firebase.RTDB.getString(&fbdo, "/Medical_project/" + usar + "/Device", &test)) {
                Serial.println(test);
                Serial.println("Sending values to eeprom"); // message

                send_eeprom(0, ssid); // send the ssid to the eeprom
                send_eeprom(50, pass); // send the pass to the eeprom
                send_eeprom(100, usar); // send the ssid to the eepro
                user = usar;
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
 configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 Get_Time();
}
//-------------------------------------------------------------
void loop()
{
  
  //Serial.println("ENTRO");
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
      Serial.print("got request: ");
      Serial.println(data_received);
      if (data_received.indexOf("f") != -1){
       
      separar(data_received);
      BPM.trim();
      TEMP.trim();
      oxigeno.trim();
      if(BPM != "" && TEMP != "" && oxigeno != ""){
       
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Datos/Bpm", BPM); // send count  ++ to Bpm
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Datos/Temp", TEMP);// send count  count =+2 to Temp
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Datos/Spo2", oxigeno);// send count  count ++ to Spo2
        Firebase.RTDB.getString(&fbdo, "/Medical_project/"+user+"/Presion/Mandar", &test); // get the string of Device
        Serial.println("Mando los primeros datos");
        Get_Time();
       Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/anio:",year);
       Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Mes:",Month);
       Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Dia:",day);
      if(dia != ""){
         Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/Fecha/hora:"+clocka, day+"/"+Month+"/"+year); // send count  ++ to Bpm
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/SPO2/hora:"+clocka,oxigeno+"%" );
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/BPM/hora:"+clocka, BPM );
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/TEMP/hora:"+clocka, TEMP);
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/Dia/hora:"+clocka, dia );
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/Sis/hora:"+clocka, sys );
        dia = "";
        sys = "";
        }
        else{
          Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/Fecha/hora:"+clocka, day+"/"+Month+"/"+year); // send count  ++ to Bpm
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/SPO2/hora:"+clocka,oxigeno+"%" );
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/BPM/hora:"+clocka, BPM );
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/TEMP/hora:"+clocka, TEMP);
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/Dia/hora:"+clocka, " ");
      Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/regd/"+year+"/"+Month+"/"+day+"/Sis/hora:"+clocka, " ");
        }
        Serial.println("Mando los segundos datos");
       
        Serial.println(test);
        if(test.indexOf("True") != -1){
        get_p_data();
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Presion/Mandar", "False");// send count  count ++ to Spo2
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Presion/Dia",dia );// send count  count ++ to Spo2
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Presion/Sys", sys);// send count  count ++ to Spo2
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Presion/HR", hr);// send count  count ++ to Spo2
        Get_Time();
        Firebase.RTDB.set(&fbdo, "/Medical_project/"+user+"/Presion/Fecha",day+"/"+Month+"/"+year+" "+clocka );// send count  count ++ to Spo2
        
        Serial.print("sys: ");
        Serial.print(sys);
        Serial.print(" dia: ");
        Serial.print(dia);
        Serial.print(" hr: ");
        Serial.println(hr);
        
        }
      }
      }
    }
    else
    {
      Serial.println("recv failed");
    }
}
}
//-------------------------------------------------------------
//Gerardo Fregoso Jiménez 2022
