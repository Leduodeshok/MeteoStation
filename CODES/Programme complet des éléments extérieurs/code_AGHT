#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SoftwareSerial.h>;
#define RX 3
#define TX 4
SoftwareSerial ac(RX,TX);

#define DHTPIN            7
#define DHTTYPE           DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// constant pour anemometre/ girouette 
const int m_time = 1;  
int wind_ct = 0;
float wind = 0.0;
unsigned long time = 0;

void setup() {
  ac.begin(9600);
  Serial.begin(9600);
  time = millis();

  // humidity
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  delayMS = sensor.min_delay / 1000;
}

void loop() {
  meassure();
  girouette();
  humidity();
  delay(1000);
  Serial.println("");
}

void countWind() {
  wind_ct ++;
}

void meassure() {
  wind_ct = 0;
  time = millis();
  attachInterrupt(1, countWind, RISING);
  delay(1000 * m_time);
  detachInterrupt(1);
  wind = (float)wind_ct / (float)m_time * 2.4/3;
  Serial.print("VITESSE DU VENT ");
  Serial.print(wind);       //Speed in Km/h
  Serial.print(" km/h - ");
  Serial.print(wind / 3.6); //Speed in m/s
  Serial.println(" m/s");
  ac.print("C");
  ac.print(wind);
}

void girouette(){
  int vent=0;
  int sensorValue = analogRead(A1);
  Serial.print("La direction du vent : ");
  if(sensorValue>200 && sensorValue<300){
    Serial.println("SUD " );
    vent=1;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>100 && sensorValue<200){
    Serial.println("SUD-EST ");
    vent=2;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>10 && sensorValue<100){
    Serial.println("EST ");
    vent=3;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>400 && sensorValue<500){
    Serial.println("NORD EST ");
    vent=4;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>750 && sensorValue<800){
    Serial.println("NORD ");
    vent=5;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>800 && sensorValue<900){
    Serial.println("NORD-OUEST");
    vent=6;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>900 && sensorValue<1023){
    Serial.println("OUEST");
    vent=7;
    //Serial.println(sensorValue);
  }
  else if(sensorValue>600 && sensorValue<700){
    Serial.println("SUD-OUEST ");
    vent=8;
    //Serial.println(sensorValue);
  }
  switch(vent){
    case 1 :
      ac.print("V");
      ac.print("sud");
    break;
    case 2 :
      ac.print("V");
      ac.print("sud-est");
    break;
    case 3 :
      ac.print("V");
      ac.print("est");
    break;
    case 4 :
      ac.print("V");
      ac.print("nord-est");
    break;
    case 5 :
      ac.print("V");
      ac.print("nord");
    break;
    case 6 :
      ac.print("V");
      ac.print("nord-ouest");
    break;
    case 7 :
      ac.print("V");
      ac.print("ouest");
    break;
    case 8 :
      ac.print("V");
      ac.print("sud-ouest");
    break;
  }
}
void humidity() {
  //delay(delayMS);
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    ac.print("T");
    ac.print(event.temperature);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    ac.print("H");
    ac.print(event.relative_humidity);
  }
}
