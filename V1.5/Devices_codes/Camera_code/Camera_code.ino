#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <EEPROM.h>

const byte button = 4;
const byte light  = 12;

String ssid = "";
String pass = ""; 
String e_ssid = "";
String e_pass = ""; 

long actual_time;
bool Button_mark = false;

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

const char* websocket_server_host = "20.94.218.189";
const uint16_t websocket_server_port = 65080;
using namespace websockets;
WebsocketsClient client;



void Configure_wifi(){
Serial.println("--------------------- Configuration mode --------------------");
 WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  Serial.println("Waiting for SmartConfig.");
  digitalWrite(light,HIGH);
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }
    digitalWrite(light,LOW);
  Serial.println("");
  Serial.println("SmartConfig done.");
  Serial.println("Waiting for WiFi");
  actual_time = millis();
  while (WiFi.status() != WL_CONNECTED) {
     digitalWrite(light, HIGH);
    delay(950);
    digitalWrite(light, LOW);
    delay(950);
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
    WiFi.begin(e_ssid.c_str(), e_pass.c_str());
    actual_time = millis();
    while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(light, HIGH);
    bool check = check_for_button();
    delay(950);
    check = check_for_button();
    digitalWrite(light, LOW);
    check = check_for_button();
    delay(950);
    Serial.print(".");
    check = check_for_button();
    if(check)
    erase_eeprom();
  }
 Serial.println("-------------Connection finished succesfully---------"); 
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
    Serial.println("ERASING EEPROM......");
 writeStringToFlash("", 0); // storing ssid at address 0
  writeStringToFlash("", 50);
  Serial.println("EEPROM ERASED");
  ESP.restart();
}



void setup() {
  Serial.begin(115200);
  EEPROM.begin(512); 
  pinMode(light,OUTPUT);
pinMode(button,INPUT);



  Serial.setDebugOutput(true);
  Serial.println();
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
  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 40;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  digitalWrite(light,HIGH);
  delay(2000);
  digitalWrite(light,LOW);
delay(2000);

actual_time = millis();
  check_for_connection();
 


  while(!client.connect(websocket_server_host, websocket_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
 
 actual_time = millis();
}

void loop() {
bool check = check_for_button();
//bool check = false;
if(check){
  Serial.println("PRESSED");
  erase_eeprom();
  }


  camera_fb_t *fb = esp_camera_fb_get();
  if(!fb){
    Serial.println("Camera capture failed");    
    esp_camera_fb_return(fb);
    return;
  }

  if(fb->format != PIXFORMAT_JPEG){
    Serial.println("Non-JPEG data not implemented");
    return;
  }

  client.sendBinary((const char*) fb->buf, fb->len);
  esp_camera_fb_return(fb);
}
