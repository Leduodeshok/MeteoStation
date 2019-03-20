#include<SoftwareSerial.h>
#define RX 8
#define TX 9
SoftwareSerial esp(RX, TX);
#define RX2 10
#define TX2 11
SoftwareSerial bt(RX2,TX2);

String Ville; 
String Temp;
String Hum;
String Vent;
String VentDir;
String s = "";

void setup() {
  Serial.begin(9600);
 delay(500);
 bt.begin(9600);
 esp.begin(9600);
 delay(500);

}

void loop() {
  char c;
  int Read=0;
  boolean first_char = true;
  String s = "";
  if(bt.available()){
    Serial.println(char(bt.read()));
  }
  while(esp.available()) {
    Serial.print(char(esp.read()));
    c = char(esp.read());
    
    if(Read!=0){
      
        if(c=='#'){
          break;
          
        }
        
        else{
          s=s+String(c);
        }
        
      }
      
      else{
        Read=0;
        first_char=true;
      }
    
    if (first_char){
      
      if(c=='!'){
        Read=1;
      }
      if(c=='$'){
        Read=2;
      }
      if(c=='*'){
        Read=3;
      }
      if(c=='='){
        Read=4;
      }
      if(c=='&'){
        Read=5;
      }
      first_char=false;
    }
    
  }

    switch(Read){
    case 1:Ville=s;   break;
    case 2:Temp=s;    break;
    case 3:Hum=s;     break;
    case 4:Vent=s;    break;
    case 5:VentDir=s; break;
  }//}
  Serial.print(Ville);
  Serial.println(Temp+"temp");
  Serial.println(Hum+"hum");
  Serial.println(Vent+"km/h");
  Serial.println(VentDir+"ventDier");
  delay(400);
  
}
