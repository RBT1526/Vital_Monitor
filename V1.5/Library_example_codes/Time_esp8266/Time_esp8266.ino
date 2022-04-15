#include <ESP8266WiFi.h>
#include "time.h"

const char* ssid       = "NASA";
const char* password   = "261121242611212404";



const char* ntpServer = "pool.ntp.org";
const int zone_gmt = -7;
const long  gmtOffset_sec = zone_gmt * 3600;
const int   daylightOffset_sec = 3600;

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



void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println(Get_time());

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  Serial.println(Get_time());
}