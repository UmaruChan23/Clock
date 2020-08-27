#include <ArduinoJson.h> 
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <EEPROM.h> 
#include <SoftwareSerial.h>
SoftwareSerial s(3,1);
const char* ssid     = "your WIFI name";
const char* password = "your password";
int weather[4];

const char* host = "api.openweathermap.org";
String line; 

void setup() {
  //Wire.onRequest(requestEvent);
  EEPROM.begin(256);
  s.begin(57600);
  pinMode(D0,OUTPUT);
  
  delay(10);
  
  
  // We start by connecting to a WiFi network

  s.println();
  s.println();
  s.print("Connecting to ");
  s.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    s.print(".");
  }

  s.println("");
  s.println("WiFi connected");  
  s.println("IP address: ");
  s.println(WiFi.localIP());

    jsonGet();
}
 
void loop() {
   
   StaticJsonBuffer<2000> jsonBuffer;                  
   JsonObject& root = jsonBuffer.parseObject(line);    
   if (!root.success()) {
    s.println("parseObject() failed");             
     jsonGet();                                         
    return;                                             
  }else{digitalWrite(D0,HIGH);}
  
  
                              //Send to Serial
  
  float tempK = root["main"]["temp"];                   
  
  int pressurehPa = root["main"]["pressure"]; 
  
  pressurehPa=(pressurehPa/1.333);
  
  int wind=(root["wind"]["speed"]);
  
  
  EEPROM.write(1,(int)round(tempK - 273.15));
  byte hi = highByte(pressurehPa); // Выделяем старший байт 
  byte low = lowByte(pressurehPa); // Выделяем младший байт
  EEPROM.write(2,hi);
  EEPROM.write(3,low);
  EEPROM.write(4,wind);
  s.write(EEPROM.read(1));
  s.write(EEPROM.read(2));
  s.write(EEPROM.read(3));
  s.write(EEPROM.read(4));
  delay(50000);
}

void jsonGet() {
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    //s.println("connection failed");
    return;
  }
  
    client.println("GET /data/2.5/weather?id=524901&appid=b65f9a88518f56537fd69b84078beb2e HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
 
  delay(1500);
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    line = client.readStringUntil('\r'); 
  }
  Serial.print(line);
  Serial.println();
  Serial.println("closing connection");
}
