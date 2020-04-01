
#include <ESP8266HTTPClient.h>
#include<ESP8266WebServer.h>
#include<ESP8266HTTPClient.h>

#include <Arduino.h>
#ifdef ARDUINO_ARCH_SAMD
#include <WiFi101.h>
#elif defined ARDUINO_ARCH_ESP8266
#include <ESP8266WiFi.h>
#elif defined ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#error Wrong platform
#endif 

#include <WifiLocation.h>
// define credentials

#define STASSID ""
#define STAPSK ""
#define Link ""
#define ServerDelay 5000
#define id "wecan1"
const char* googleApiKey = "";




// define pins
const char* ssid = STASSID;
const char* password = STAPSK;
const int ledRed = 15;
const int ledGreen = 13 ;
const int trigPin = 14;
const int echoPin = 12;
const int outputAnalogPin = 3;
const double totalLevel = 15;
//const double errorLevel = 2 ;
//const int dataRetries = 10 ;
const int analogReadPin = A0;
long distance, duration;
int openSignalCheck = 0;

String lat,lon,acc,adj;

WifiLocation location(googleApiKey);


double currentLevel = 0 ;
int sensorStatus = 0 ;

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

  Serial.println(" ");
  Serial.println("Wifi connected");
  Serial.println("IP address");
  Serial.println(WiFi.localIP());
  location_t loc = location.getGeoFromWiFi();

    Serial.println("Location request data");
    Serial.println(location.getSurroundingWiFiJson());
//    adj = String(location.getSurroundingWiFiJson());
    Serial.println("Latitude: " + String(loc.lat, 7));
    lat = String(loc.lat, 7);
    Serial.println("Longitude: " + String(loc.lon, 7));
    lon = String(loc.lon, 7);
    Serial.println("Accuracy: " + String(loc.accuracy));
    acc = String(loc.accuracy);
}


void checkDistance()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // calculating the distance with respect to duration of sound released by trigger
  distance = ((duration / 2) / 29.1);


}

int lidStatus()
{
  int lidSignal = analogRead(analogReadPin);
  sendDataServer();
  checkDistance();


  if (lidSignal >= 500)
  {
//    digitalWrite(outputAnalogPin , HIGH);
      Serial.println("The Lead is open");
    
    if (distance > 50 || distance <= 3)
    {
      Serial.println("The Lid Is Dirty");
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
      return 2;
    }

    return 1;
  }
  else if (lidSignal < 500)
  {
//    digitalWrite(outputAnalogPin , LOW);

    Serial.println("The Lead is closed");
    if (distance > 50 || distance <= 3)
    {
      Serial.println("The Lid Is Dirty");
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
      return 2;
    }
    return 0;
  }
  else
  {
    Serial.println("Time For Maintainance");
    return 2;
  }


}

void checkGrabage()
{

  Serial.println("The Distance is " + String(distance));
  currentLevel = ((((totalLevel - (distance))) / totalLevel)) ;
  Serial.println("The Current Level of gatbage is " );
  Serial.println(currentLevel);
  if (currentLevel < 0.8)
  {
    digitalWrite(ledGreen, HIGH) ;
    digitalWrite(ledRed, LOW);
  }
  else if (currentLevel > 0.8)
  {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }
}

boolean sendDataServer()
{

  HTTPClient http;
  String data = String(Link) + "/?status=" + String(sensorStatus) + "&currentLevel=" + String(currentLevel) + 
  "&id=" + String(id) + "&openSignalCheck=" + String(openSignalCheck) +"&lon=" + (lon)+ "&lat=" +(lat)+"&acc="+ (acc);
  http.begin(data);
  int httpCode = http.GET();
  String payload = http.getString();
  if (httpCode > 0 )
  {
    Serial.println(payload);

    if(String("Open").equals(payload))
    {
      Serial.println("opening");
      digitalWrite(outputAnalogPin,HIGH);
      delay(200);
      digitalWrite(outputAnalogPin,LOW);
      openSignalCheck = 1 ;
    }
    else if(String("Close").equals(payload))
    {
      Serial.println("CLosing");
      digitalWrite(outputAnalogPin,HIGH);
      delay(200);
      digitalWrite(outputAnalogPin,LOW);
      openSignalCheck = 0;
    }
 
    http.end();
  }
  else
  {
    Serial.println("Failed To establish connection to the server");
    http.end();
  }

}





void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(outputAnalogPin, OUTPUT);
  wifiConnect();
}

void loop() {

  sensorStatus = lidStatus();
  if (sensorStatus == 0)
  {
    checkGrabage();
    sendDataServer();
  }
  else if (sensorStatus == 2)
  {
    sendDataServer();

  }
  int sensorvalue = analogRead(analogReadPin);
  Serial.println("the analog value is " + String(sensorvalue));
  delay(3000);
}
