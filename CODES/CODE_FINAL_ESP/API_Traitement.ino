#include <WiFi.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

#define RXD2 16
#define TXD2 17



const char* ssid     = "OnePlus 6";
const char* password = "12345678";

const char* host = "api.openweathermap.org";
const char* appid = "571dcf8befb1d8e4d22f10795d245a9f";
String codpost="06000";

String line;

void setup()
{
    int i=0;
    Serial.begin(115200);
  
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        i++;
        if(i>20){
          ESP.restart();
        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}



void loop()
{
  if(WiFi.status() != WL_CONNECTED){
    ESP.restart();
  }
  getWeather();
  delay(1000);
  parsWeather(line);

  char c;
  boolean first_char=true;
  String s="";
  boolean Read=false;
  while(Serial2.available()){
    Serial.print("reception en cours..");
    c=char(Serial2.read());
    if(Read){
      if(c=='#'){
        break;
      }
      else{
        s=s+String(c);
      }
      
    }
    if(first_char && c=='*'){
      Read=true;
      first_char=false;
    }
  }
  if(Read){
    codpost=s;
  }


}







void getWeather(){

    Serial.print("connection a ");
    Serial.println(host);

    
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/data/2.5/weather?zip=";
    url += codpost;
    url += ",fr&units=metric&appid=";
    url += appid;
  

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.println("GET "+url);
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }


    while(client.available()) {
        line = client.readStringUntil('\r');
        Serial.println(line);
    }
}


void parsWeather(String result){
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
  float wind = root["wind"]["speed"];
  float windDir = root["wind"]["deg"];
  String Direction;    
    if(windDir>=337 || windDir<22){
    Direction=",N";
    }
    else if(windDir>=22 && windDir<67){
    Direction=",NE";
    }
    else if(windDir>=67 && windDir<112){
    Direction=",E";
    }
    else if(windDir>=112 && windDir<157){
    Direction=",SE";
    }
    else if(windDir>=157 && windDir<202){
    Direction=",S";
    }
    else if(windDir>=202 && windDir<247){
    Direction=",SO";
    }
    else if(windDir>=247 && windDir<292){
    Direction=",O";
    }
    else if(windDir>=292 && windDir<337){
    Direction=",NE";
    }
  Serial.println("Meteo de la ville de "+location+","+country);
  Serial.println("La temperature est de "+String(temperature)+("°C"));
  Serial.println("La pression est de "+String(pressure)+" Pa");
  Serial.println("La meteo est "+weather+","+description);
  Serial.println("Le vent est de "+String(wind)+"km/h,"+String(windDir)+"degré");
  Serial.println("");
  Serial2.println('!'+location+'#');
  Serial2.println('$'+String(temperature)+'#');
  Serial2.println('*'+String(humidity)+'#');
  Serial2.println('='+String(wind)+'#');
  Serial2.println('&'+String(windDir)+String(Direction)+'#');
  
}
