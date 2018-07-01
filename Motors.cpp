//////////// MOTION FUNCTIONS /////////////////
#include <Arduino.h>
#include "Motors.h"
/*define channel enable output pins*/
#define LEFT_ENA 5    // Left  wheel speed
#define RIGHT_ENB 6   // Right wheel speed
/*define logic control output pins*/
#define LEFT_UP_IN1 7   // Left  wheel forward
#define LEFT_DN_IN2 8   // Left  wheel reverse


#define RIGHT_UP_IN4 11  // Right wheel forward
#define RIGHT_DN_IN3 9   // Right wheel reverse

#define carSpeed_turn 250  // initial speed of car >=0 to <=255
#define carSpeed_free 250

void CMotor::forward()
{ 
  digitalWrite(LEFT_ENA,HIGH);// 100% PWM
  digitalWrite(RIGHT_ENB,HIGH);// 100% PWM
  digitalWrite(LEFT_UP_IN1,HIGH);
  digitalWrite(LEFT_DN_IN2,LOW);
  digitalWrite(RIGHT_DN_IN3,LOW);
  digitalWrite(RIGHT_UP_IN4,HIGH);
  in_motion=1;
  //Serial.println("go forward!");
}
void CMotor::back(){
  digitalWrite(LEFT_ENA,HIGH);
  digitalWrite(RIGHT_ENB,HIGH);
  digitalWrite(LEFT_UP_IN1,LOW);
  digitalWrite(LEFT_DN_IN2,HIGH);
  digitalWrite(RIGHT_DN_IN3,HIGH);
  digitalWrite(RIGHT_UP_IN4,LOW);
  in_motion=1;
  //Serial.println("go back!");
}
void CMotor::left(){
  analogWrite(LEFT_ENA,carSpeed_turn);/// PWM duty=carSpeed_turn/255 * 100%
  analogWrite(RIGHT_ENB,carSpeed_turn);
  digitalWrite(LEFT_UP_IN1,LOW);
  digitalWrite(LEFT_DN_IN2,HIGH);
  digitalWrite(RIGHT_DN_IN3,LOW);
  digitalWrite(RIGHT_UP_IN4,HIGH); 
  in_motion=1;
  //Serial.println("go left!");
}
void CMotor::right(){
  analogWrite(LEFT_ENA,carSpeed_turn);
  analogWrite(RIGHT_ENB,carSpeed_turn);
  digitalWrite(LEFT_UP_IN1,HIGH);
  digitalWrite(LEFT_DN_IN2,LOW);
  digitalWrite(RIGHT_DN_IN3,HIGH);
  digitalWrite(RIGHT_UP_IN4,LOW);
  in_motion=1;
  //Serial.println("go right!");
}
void CMotor::stop(){
  digitalWrite(LEFT_ENA, LOW);
  digitalWrite(RIGHT_ENB, LOW);
  //Serial.println("STOP!");  
  in_motion=0;
}
void CMotor::left_freewheel()
{
  analogWrite(LEFT_ENA,carSpeed_free);
  analogWrite(RIGHT_ENB,carSpeed_turn);
  digitalWrite(LEFT_UP_IN1,LOW);
  digitalWrite(LEFT_DN_IN2,HIGH);
  digitalWrite(RIGHT_DN_IN3,LOW);
  digitalWrite(RIGHT_UP_IN4,HIGH); 
  in_motion=1;
  //Serial.println("go left!");
}
void CMotor::right_freewheel()
{
  analogWrite(LEFT_ENA,carSpeed_turn);
  analogWrite(RIGHT_ENB,carSpeed_free);
  digitalWrite(LEFT_UP_IN1,HIGH);
  digitalWrite(LEFT_DN_IN2,LOW);
  digitalWrite(RIGHT_DN_IN3,HIGH);
  digitalWrite(RIGHT_UP_IN4,LOW);
  in_motion=1;
  //Serial.println("go right!");
}

void CMotor::KillMotor()
{
  stop();
}
void CMotor::init()
{
  pinMode(LEFT_UP_IN1,OUTPUT);
  pinMode(LEFT_DN_IN2,OUTPUT);
  pinMode(RIGHT_DN_IN3,OUTPUT);
  pinMode(RIGHT_UP_IN4,OUTPUT);
  pinMode(LEFT_ENA,OUTPUT);// used as PWM 980 Hz output
  pinMode(RIGHT_ENB,OUTPUT);// used as PWM 980 Hz output
  stop();
}

