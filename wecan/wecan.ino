#include<Servo.h>

Servo servo;

  int pin = 14;
  int lid_status = 0 ;


void setup()
{
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  servo.attach(9);
  servo.write(0);
  
  
}

void loop()
{
  
  int val = digitalRead(pin);
  
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
}

