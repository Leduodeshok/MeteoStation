
////////////////////INCLUSION DES LIB POUR L ECRAN////////////////////////
#include <U8glib.h>
#include <OneWire.h>
U8GLIB_ST7920_128X64 u8g(2, 3, 4, U8G_PIN_NONE);
char temp_string[5]; 
char hum_string[5];

////////////////////INCLUSION DES LIB POUR LE SENSOR/////////////////////
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN            5
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN,DHTTYPE);
int delayMS=100;

////////////////////CREATION CARACTERE////////////////////////////////////

const uint8_t fleche[] PROGMEM = {
  0x00,     
  0x10,     
  0x18,       
  0x1c,        
  0x1e,    
  0x1c,         
  0x18,       
  0x10         
};


//////////////////////SETUP///////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  u8g.setFont(u8g_font_8x13);
  u8g.setColorIndex(1);
  

}

void loop() {
  u8g.firstPage();
  do {   
    draw();
  } while( u8g.nextPage() );

}



void draw(){
  
  u8g.drawFrame(0,0,128,31);            // upper frame
  u8g.drawFrame(0,33,128,31);           // lower frame
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  u8g.drawBitmapP(7,5,1,8,fleche);
  u8g.drawStr( 15, 13, "Temperature");       // do this for temperature
  dtostrf(event.temperature, 3, 1, temp_string);
  u8g.drawStr(33,27, temp_string);
  u8g.drawStr(70,27, "*C"); 



  dht.humidity().getEvent(&event);
  u8g.drawStr(26,45, "Humidite");             // do this for humidity
  dtostrf(event.relative_humidity, 3, 1, hum_string);
  u8g.drawStr(40,60, hum_string);
  u8g.drawStr(75,60, "%"); 
}
