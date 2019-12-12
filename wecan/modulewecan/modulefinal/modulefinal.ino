#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include<ESP8266WebServer.h>
#include<ESP8266HTTPClient.h>


// define credentials

#define STASSID "linux"
#define STAPSK "frankensteindcoster"
#define Link "http://76111a42.ngrok.io"
#define ServerDelay 5000
#define id "wecan1"


// define pins
const char* ssid = STASSID;
const char* password = STAPSK;
const int ledRed = 15;
const int ledGreen = 13 ;
const int trigPin = 14;
const int echoPin = 12;
const int checkPin = 3;
const int totalLevel = 17;
const int errorLevel = 2 ;
const int dataRetries = 10 ;



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
}


int lidStatus()
{
  double distance, duration;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // calculating the distance with respect to duration of sound released by trigger
  distance = ((duration / 2) / 29.1);
  Serial.println("The Distance is " + String(distance));
  if (distance > 3 && distance < totalLevel)
  {
    Serial.println("This Lid Is Closed");
    return 0 ;
  }
  else if (distance > totalLevel && distance < 50)
  {
    Serial.println("The Lid Is Open");
    return 1;
  }
  else if ( distance > 50 || distance <= 3)
  {
    Serial.println("The Lid Is Dirty");
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    return 2;
  }

}

void checkGrabage()
{
  double dur, dis;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  dur = pulseIn(echoPin, HIGH); // calculating the distance with respect to duration of sound released by trigger
  dis = ((dur / 2) / 29.1);
  currentLevel =(dis / 17) ;
  Serial.println("The Current Level of gatbage is " );
  Serial.println(currentLevel);
  if(currentLevel < 0.8)
  {
    digitalWrite(ledGreen, HIGH) ;
    digitalWrite(ledRed, LOW);
  }
  else if(currentLevel > 0.8)
  {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }
}

boolean sendDataServer()
{

  HTTPClient http;
  String data = String(Link) + "/?status=" + String(sensorStatus) + "&currentLevel=" + String(currentLevel) + "&id=" + String(id);
  http.begin(data);
  int httpCode = http.GET();
  String payload = http.getString();
  if (httpCode > 0 )
  {
    Serial.println(payload);
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

  wifiConnect();
}

void loop() {

  sensorStatus = lidStatus();
  if (sensorStatus == 0)
  {
    checkGrabage();
    sendDataServer();
  }
  else if(sensorStatus == 2)
  {
       sendDataServer();
 
  }
  delay(3000);
}
