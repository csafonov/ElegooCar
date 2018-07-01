/// MyTstIR.ino
/// Car control using IR and other sensors

/// Motor drive control
#include "Motors.h"
CMotor Motor;

/// Ir Remote controller 
#include "InfraRed.h"
CInfraRed InfraRed;

#include "UltraSonicSen.h"
CUltraSonicSen UltraSonicSen;

#include "CarCtrl.h"
CCarCtrl CarCtrl;
 
///////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  CarCtrl.init();
  //without bluetooth: Serial.begin(115200);//open serial and set the baudrate
  Serial.begin(9600);/// bluetoth
  //Serial.print("AT");
  //delay(1500);
  //Serial.print("AT+DEFAULT");
  // delay(1500); 
}

  // Test Bluetooth uart commnication
  int incomingByte = 0;  // for incoming serial data

void loop()
{


// reply only when you receive data:
//  if (Serial.available() > 0) {
    // read the incoming byte:
//    incomingByte = Serial.read();
//    Serial.write(incomingByte);
//    // say what you got:
 //   //Serial.print("I received: ");
//    //Serial.println(incomingByte, DEC);
//  }

  
  // put your main code here, to run repeatedly:
  CarCtrl.CarCtrlMan();
 }
