#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include<ESP8266WebServer.h>
#include<ESP8266HTTPClient.h>


// define credentials

#define STASSID "linux"
#define STAPSK "frankensteindcoster"
#define Link "http://0389affb.ngrok.io"
#define ServerDelay 5000


// define pins
const char* ssid = STASSID;
const char* password = STAPSK;
const int ledRed = 15;
const int ledGreen = 13 ;
const int trigPin = 14;
const int echoPin = 12;
const int checkPin = 3;
const int totalLevel = 16.5;
const int errorLevel = 1 ;
const int dataRetries = 10 ;
const int lidCheckPin = 1 ;


double currentLevel;
int counter = 0 ;
double tempCurrentLevel = 0;
int sensorStatus = 0 ;



void calculateGarbage()
{
  Serial.println("counter" + counter);
  counter ++ ;
  boolean check = checkForSend();
  if (check) {
    tempCurrentLevel = tempCurrentLevel /  10;
    Serial.print("sending data");
    Serial.print(tempCurrentLevel);
    boolean checkServer =  sendDataServer();
    tempCurrentLevel = 0;
    statusCheck(checkServer);

  }
  else
  {
   
    double distance, duration;
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); // calculating the distance with respect to duration of sound released by trigger
    distance = (duration / 2) / 29.1;


    
    if (distance > totalLevel && distance > 0)
    {

      Serial.println("open");
      sensorStatus = 1 ;
    }
    else if (distance <= 4)
    {
      Serial.println("dirty");
      sendCleanMe();

    }
    else if ( distance <= totalLevel && distance > 0)
    {

      Serial.println("closed");
      sensorStatus = 0 ;
      Serial.println("Distance is  " + (String(distance)));
      currentLevel = (((distance - errorLevel) / (totalLevel)) * 100) ;
      currentLevel = 100 - currentLevel;
      tempCurrentLevel =  currentLevel + tempCurrentLevel;
      Serial.print("current level is" + String(tempCurrentLevel));
    }


  }



}


void led()
{
  double level = tempCurrentLevel /  counter;
  if ( level <= 60 && level >= 0)
  {
    digitalWrite(ledGreen, HIGH) ;
    digitalWrite(ledRed, LOW);
  }
  else if (level > 60 || level < 0)
  {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }
}


void statusCheck(boolean checkServer)
{
  if (checkServer)
  {
    Serial.println("Request Successfully Sent To Server");
  }
  else
  {
    Serial.println("Failed To Connect To The Server");
  }
}

boolean checkForSend()
{
  if (counter == dataRetries)
  {
    return true;
  }
  else
  {
    return false;
  }
}


void setup()
{

  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  //  pinMode(lidCheckPin , OUTPUT);
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



boolean sendDataServer()
{

  HTTPClient http;
  String data = "?status=" + String(sensorStatus) + "&currentLevel=" + String(currentLevel);
  http.begin(Link + data);
  int httpCode = http.GET();
  String payload = http.getString();
  if (httpCode > 0 )
  {
    Serial.println(payload);
    http.end();
    counter = 0 ;
    return true;
  }
  else
  {
    Serial.println("Failed To establish connection to the server");
    http.end();
    counter = 0 ;
    return false;
  }

}


void sendCleanMe()
{
  HTTPClient http;
  String data = "?status=" + String(sensorStatus) + "&currentLevel=" + String(currentLevel);
  http.begin(Link + data);
  http.begin(Link);
  int httpCode = http.GET();
  String payload = http.getString();
  if (httpCode > 0 )
  {
    Serial.println(payload);
    http.end();
    counter = 0 ;
    Serial.print("sent dirty signal");
  }
  else
  {
    Serial.println("Failed To establish connection to the server");
    counter = 0 ;
    http.end();
  }
}





void loop()
{



  calculateGarbage();
  led();



}
