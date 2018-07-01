/// file: UltraSonicSen.cpp
/// Class for ultra sonic sensor control

#include <Arduino.h>
#include "UltraSonicSen.h"

//Ultrasonic distance measurement Sub function
void CUltraSonicSen::init()
{
  /// Init Ultrasonic
       Echo = A4;  
       Trig = A5;

  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
}
int CUltraSonicSen::GetDistancebyUS()
{
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
} 

