#include<Servo.h>

Servo servo;

const int switchPin = 14;
const int outputModulePin = 16;
const int analogInputPin = 19;
int lid_status = 0 ;
int analog ;


void setup()
{
  Serial.begin(9600);
  pinMode(switchPin, OUTPUT);
  pinMode(outputModulePin, OUTPUT);
  digitalWrite(outputModulePin, LOW);
  servo.attach(9);
  servo.write(0);


}

void loop()
{
  analog = analogRead(analogInputPin);
  Serial.println("The analog value in uno is" + String(analog));





  int val = digitalRead(switchPin);


  if (val == 1 )
  {
    if (lid_status == 0)
    {
      open_lid();
    }
    else if (lid_status == 1)
    {
      close_lid();
    }

  }
  else if(analog > 100)
  {
    if (lid_status == 0)
    {
      open_lid();
    }
    else if (lid_status == 1)
    {
      close_lid();
    }
    
  }


}


void open_lid()
{
  Serial.println("opening lid");
  for (int i = 0 ; i < 65 ; i++)
  {
    servo.write(i);
    delay(20);
  }

  lid_status = 1;
  analog = 0;
  digitalWrite(outputModulePin, HIGH);
}

void close_lid()
{
  Serial.println("closing Lid");
  for (int j = 65 ; j > 0 ; j--)
  {
    servo.write(j);
    analog = 0 ;
    delay(20);
  }

  lid_status = 0;
  digitalWrite(outputModulePin, LOW);
}
