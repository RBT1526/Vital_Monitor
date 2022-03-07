//Gerardo Fre
//
//
//
//
//-----------------------------------------
#include "esp_camera.h"
#include <WiFi.h>
#include "BluetoothSerial.h"
#include <ArduinoJson.h>
#include<EEPROM.h>//EEPROM library added
#include <ArduinoWebsockets.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial miBt;

  
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
int Led = 4;
String getssid = ""; //example getvariable to read eeprom
String getpasss = ""; // example getvariable to read eeprom
char Dato;
const char* websocket_server_host = "20.94.218.189";
const uint16_t websocket_server_port = 65080;

using namespace websockets;
WebsocketsClient client;

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

void setup() {
  StaticJsonDocument<256> doc;
  Serial.begin(115200);
 
    Serial.setDebugOutput(true);

  pinMode(Led, OUTPUT);
  digitalWrite(Led, LOW);
  EEPROM.begin(512); //Start the eeprom with 512 bytes
  /*
  send_eeprom(0, ""); // send the ssid to the eeprom
  send_eeprom(50, ""); // send the pass to the eeprom
  */
  connects:
  getssid = read_eeprom(0);// reead the ssid of the eeprom
  getpasss = read_eeprom(50); // read the pass of the eeprom
  if (getssid.length() != 0) {
    const char* pruebas = getssid.c_str();
    const char* pruebapass = getpasss.c_str();
    WiFi.begin(pruebas, pruebapass);  // try to connect to wifi
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(300);
    }



  }
  else {
     miBt.begin("Camara_VitalMonitor");



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

              //Serial.println(datos_s);

            }


          }
          DeserializationError error = deserializeJson(doc, datas);
          if (error) {

            miBt.print("ERRORES");
          }
          else {

            String ssid = doc["ssid"];
            String pass = doc["pass"];

           
            const char* pruebas = ssid.c_str();
            const char* pruebapass = pass.c_str();
            WiFi.begin(pruebas, pruebapass);  // try to connect to wifi
            byte connected = 1;
            byte counts = 0;
            while (connected == 1) {

              miBt.print(WiFi.status());
              delay(300);
              if (counts > 100) {
                connected = 0;
              }
              if (WiFi.status() == 3) {
                connected = 2;
              }
              counts ++;
            }
            if (connected == 2) {

              send_eeprom(0, ssid); // send the ssid to the eeprom
              send_eeprom(50, pass); // send the pass to the eeprom
              bandera = 1;
              miBt.print("OKA");
              // get the string of Device
              delay(1000);
            miBt.end();
            goto connects;

            }
            else {
              miBt.print("ERROR");
            }
          }
        }

      }
    }

  }

camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 40;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

while(!client.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
  }

}

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if(!fb){
    
    esp_camera_fb_return(fb);
    return;
  }

  if(fb->format != PIXFORMAT_JPEG){
    return;
  }

  client.sendBinary((const char*) fb->buf, fb->len);
  esp_camera_fb_return(fb);
}
