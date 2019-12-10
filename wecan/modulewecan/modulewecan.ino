#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define STASSID "linux"
#define STAPSK "frankensteindcoster"


// define pins
const char* ssid = STASSID;
const char* password = STAPSK;
const int ledRed = 15;
const int ledGreen = 13 ;
const int trigPin = 14;
const int echoPin = 12;
const int checkPin = 3;
const int total_level = 16.5;
const int errorLevel = 1 ;

double current_level;



void calculateGarbage()
{
  Serial.println("Distance");
  double distance, duration;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // calculating the distance with respect to duration of sound released by trigger
  distance = (duration / 2) / 29.1;
  Serial.println(distance);
  current_level = ((distance - errorLevel) / (total_level)) * 100 ;
  current_level = 100 - current_level;
  Serial.print("current level");
  Serial.print(current_level);
  
}



void setup()
{

  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  wifiConnect();






}

void wifiConnect()
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);



  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi connected");
  Serial.println("IP address");
  Serial.print(WiFi.localIP());
}



void getExternalIp()
{
  WiFiClient client;
  if (!client.connect("api.ipify.org", 80)) {
    Serial.println("Failed to connect with 'api.ipify.org' !");
  }
  else
  {
    int timeout = millis() + 5000;
    client.print("GET /?format=json HTTP/1.1\r\nHost: api.ipify.org\r\n\r\n");
    while (client.available() == 0) {
      if (timeout - millis() < 0) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    int size;
    while ((size = client.available()) > 0) {
      uint8_t* msg = (uint8_t*)malloc(size);
      size = client.read(msg, size);
      Serial.write(msg, size);
      free(msg);
    }
  }

}





void loop()
{
  calculateGarbage();
  if(current_level <= 60 && current_level > 0)
  {
    digitalWrite(ledGreen,HIGH) ;
    digitalWrite(ledRed,LOW);
  }
  else if(current_level > 60 || current_level < 0)
  {
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledGreen,LOW);
  }
  delay(500);
}
