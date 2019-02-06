#include <WiFi.h>
#include <ArduinoJson.h>
#include<SoftwareSerial.h>
#define RX 16
#define TX 17
SoftwareSerial ard  (RX,TX);
String data;
String result;
char ssid[]="OnePlus 6";
const char* pass="12345678";
String APIKEY="571dcf8befb1d8e4d22f10795d245a9f";
//String CityID="06000";




WiFiClient client;
char servername[]="178.62.207.82";



void setup() {
  ard.begin(9600);
  delay(500);
  Serial.begin(115200);
  WiFi.begin(ssid,pass);
  while(WiFi.status() !=WL_CONNECTED){
    delay(500);
   
  }
  Serial.println("Connected");

}

void loop() {
    if (!client.connect(servername, 80)) {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
    }
    
 getWeatherData("06600");
 delay(1250);

  
}

    



void getWeatherData(String CityID){
  client.println("GET /data/2.5/weather?zip="+CityID+",fr&units=metric&appid="+APIKEY);  
  result=client.readStringUntil('\n');
  client.stop();
  result.replace('[', ' ');
  result.replace(']', ' ');
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  String location = root["name"];
  String country = root["sys"]["country"];
  float temperature = root["main"]["temp"];
  float humidity = root["main"]["humidity"];
  String weather = root["weather"]["main"];
  String description = root["weather"]["description"];
  float pressure = root["main"]["pressure"];
  /*String temp ="/T/"+ String(temperature);
  String humi = "/H/"+String(humidity);
  String met= "/W/"+weather;
  String vill="/V/"+location;*/
  Serial.println("Meteo de la ville de "+location+","+country);
  Serial.println("La temperature est de "+String(temperature)+("°C"));
  Serial.println("La pression est de "+String(pressure)+" Pa");
  Serial.println("La meteo est "+weather+","+description);
  Serial.println("");

}
 
