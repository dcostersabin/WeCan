#include<Servo.h>

Servo servo;

  int switchPin = 14;
  int lid_status = 0 ;
  int outputModulePin = 16;


void setup()
{
  Serial.begin(9600);
  pinMode(switchPin, OUTPUT);
  pinMode(outputModulePin, OUTPUT);
  digitalWrite(outputModulePin,LOW);
  servo.attach(9);
  servo.write(0);
  
  
}

void loop()
{
  
  int val = digitalRead(switchPin);
  
  if(val == 1)
  {
    if(lid_status == 0)
    {
      open_lid();
    }
    else if(lid_status == 1)
    {
      close_lid();
    }
    
  }
  
  
}


void open_lid()
{
  Serial.println("opening lid");
  for(int i = 0 ; i < 60 ; i++)
      {
        servo.write(i);
        delay(20);
      }
      
      lid_status = 1; 
      digitalWrite(outputModulePin, HIGH);
}

void close_lid()
{
  Serial.println("closing Lid");
for(int j = 60 ; j > 0 ; j--)
      {
        servo.write(j);
        delay(20);
      }
      
      lid_status = 0;
      digitalWrite(outputModulePin,LOW);
}
