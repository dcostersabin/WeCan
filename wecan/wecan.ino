
int trigPin = 2;
int echoPin = 3;


void setup()
{
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  
}

void loop()
{
  distance_check();
  delay(1000);
}

void distance_check()
{
  long duration,distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;
  Serial.println(distance);
 
}
