/// file: CarCtrl.cpp
/// Main Class for Car control 
#include <Arduino.h>

#include "InfraRed.h"
#include "Motors.h"
#include "UltraSonicSen.h"
#include "CarCtrl.h"

#define MIN_US_DISTANCE_CM 20 /// minimal US-detected distance for stop forward
////////////////// LINE TRACKING //////////////
#define pinLT_R 10
#define pinLT_M 4
#define pinLT_L 2

void CLineTracking::init()
{
  pinMode(pinLT_R,INPUT);
  pinMode(pinLT_M,INPUT);
  pinMode(pinLT_L,INPUT);
}
bool CLineTracking::readR()
{
  return !digitalRead(pinLT_R);
}
bool CLineTracking::readM()
{
  return !digitalRead(pinLT_M);
}
bool CLineTracking::readL()
{
  return !digitalRead(pinLT_L);
}


////////////////////////////////////// CMyTimers /////////////
void CMyTimers::init()
{
  int n;
      GlobalTime = millis();// time now
      for (n=0;n<MY_TIMERS_NUM;n++)
        NextTime[n]=GlobalTime;// timeout
}

// define new timeout target
void CMyTimers::SetNextTime(int n, unsigned long msTimeout)
{
  UpdateGlobalTime();
   NextTime[n]=GlobalTime+msTimeout; 
}
// check if it is timeout now
bool CMyTimers::IsTimeout(int n)
{
  UpdateGlobalTime();
  return (GlobalTime > NextTime[n]  )  ? 1:0;
}
// update GlobalTime timer each cylce
void CMyTimers::UpdateGlobalTime()
{
       GlobalTime = millis();// update time now
}
//////////////////////////////////////

/// Initial muxing
void CCarCtrl::init()
{
    /// Init micro-servo
     bDebugIr=1;
     bDebugTrace=0;
     bDebugUs=0;
     bDebugCar=0;//1;
     bNoTrace = 1;

    LineTracking.RML = 15;
    HeadServo.init();
    InfraRed.init();
    Motor.init();
    UltraSonicSen.init();
    CarCtrl.MyTimers.init();
    CarCtrl.MyTimers.SetNextTime(MY_TIMER_IR,100);
}
/// Called when unlocked
void CCarCtrl::StartMicroServo()
{   HeadServo.InitTaget(90);
    HeadServo.Go();
    delay(500); 
}

void CCarCtrl::MotionManager()
{
  if (InfraRed.LockState.loc_state==LOCK_STATE_OFF)
  {
    Motor.KillMotor();
    return; // do nothing, motor off
  }
  // process the motion algorithm 
  if(InfraRed.Ir_value==IREM_KEY_star)
    CarCtrl.LineTracking.TriggerLine = 0;// clear triggered line processed
  
    if (CarCtrl.middleDistance <= MIN_US_DISTANCE_CM && InfraRed.Ir_value == IREM_KEY_up )
    {
      InfraRed.Ir_value =IREM_KEY_ok;//stop
       if (Motor.in_motion)
       if (bDebugCar) Serial.println("middleDistance-ST");
       Motor.stop();
    }
    // timeout
    if ( CarCtrl.MyTimers.IsTimeout(MY_TIMER_IR) )
    //if(millis() - CarCtrl.usecLastIrCmd > 100  )
    {
      if (Motor.in_motion)
          if (bDebugCar) Serial.println("Timeout-ST");
      Motor.stop();
      InfraRed.Ir_value =IREM_KEY_ok;//stop
      CarCtrl.usecLastIrCmd = millis();
    }
    
    /// Finish line
    if (  CarCtrl.LineTracking.TriggerLine )
    {
       if (Motor.in_motion)
          if (bDebugCar) Serial.println("line-ST");
      //CarCtrl.LineTracking.TriggerLine = 0;// triggered line processed
            Motor.stop();
            InfraRed.Ir_value =IREM_KEY_ok;//stop
            CarCtrl.usecLastIrCmd = millis();
    }
    
     switch(InfraRed.Ir_value)
     {
      case IREM_KEY_up:
      if (middleDistance > MIN_US_DISTANCE_CM)
      {
        Motor.forward();
      }
      break;
      case IREM_KEY_dn: Motor.back(); break;
      case IREM_KEY_left: Motor.left_freewheel(); break;
      case IREM_KEY_right: Motor.right_freewheel();break;
      case IREM_KEY_ok:
      Motor.stop(); 
          break;
      default: break;
    }

    if ( CarCtrl.MyTimers.IsTimeout(MY_MICR_SERVO) )
    {
        HeadServo.Go();
        CarCtrl.MyTimers.SetNextTime(MY_MICR_SERVO,100);
    }

}

void CCarCtrl::CarCtrlMan()
{
  int stat;
  int Timeout;
  int RML;
  int middleDistance;
  char sCmd;
  bool bNewCmdBT;
   CarCtrl.MyTimers.UpdateGlobalTime();


    middleDistance = UltraSonicSen.GetDistancebyUS();
   if (bDebugUs && middleDistance != CarCtrl.middleDistance)
   {
    Serial.print("US =");
    Serial.println(CarCtrl.middleDistance  ,DEC);
   }
   CarCtrl.middleDistance =middleDistance;
   
   //CarCtrl.LineTracking.RML=CarCtrl.LineTracking.readRML();
   //Serial.println(CarCtrl.LineTracking.RML  ,HEX);

  CarCtrl.LineTracking.L=CarCtrl.LineTracking.readL();
   //if (bEnaTx & 0x1) Serial.print(CarCtrl.LineTracking.L  ,HEX);
   CarCtrl.LineTracking.M=CarCtrl.LineTracking.readM();
   //if (bEnaTx & 0x1) Serial.print(CarCtrl.LineTracking.M  ,HEX);
   CarCtrl.LineTracking.R=CarCtrl.LineTracking.readR();
   //if (bEnaTx & 0x1) Serial.println(CarCtrl.LineTracking.R  ,HEX);


   RML= CarCtrl.LineTracking.L*4+CarCtrl.LineTracking.M*2+CarCtrl.LineTracking.R;
   if (CarCtrl.LineTracking.RML !=RML )
    if (bDebugTrace)
    {
      Serial.print("RML=");
      Serial.println(CarCtrl.LineTracking.RML  ,BIN);
    }
   CarCtrl.LineTracking.RML= RML;

   if (CarCtrl.LineTracking.L||CarCtrl.LineTracking.M||CarCtrl.LineTracking.R )
   {
    if (bDebugCar) Serial.println("LINE");
    if (!CarCtrl.bNoTrace)
    {
      CarCtrl.LineTracking.TriggerLine = 1;// triggered line
    }
   }
  //Serial.println(LockState.middleDistance  ,DEC);

  if ( CarCtrl.MyTimers.IsTimeout(MY_TIMER_IR) )
  {
    stat = InfraRed.GetIrCommand();// check if was a new command
    if (stat)
    {
       InfraRed.Resume();
       CarCtrl.MyTimers.SetNextTime(MY_TIMER_IR,140);
       if (bDebugIr)
       {
        Serial.print("IR =");
        Serial.println(InfraRed.IrResult()   ,HEX);
       }
       if (InfraRed.IrResult() !=0xFFFFFFFF)
        {
          InfraRed.Ir_value = InfraRed.IrResult();
          InfraRed.LockManager();
          CarCtrl.bHasNewIrCmd=1;
        }
        CarCtrl.MyTimers.SetNextTime(MY_TIMER_IR,100);
        if ( InfraRed.Ir_value == IREM_KEY_left ||  InfraRed.Ir_value == IREM_KEY_right)
            Timeout=30;
        else
            Timeout=150;
            
        CarCtrl.MyTimers.SetNextTime(MY_TIMER_MOTORS_CMD_TIMEOUT,Timeout);
        CarCtrl.usecLastIrCmd = millis();// time now
     }
  }
  stat = BTremote.GetBTCommand();// check if was a new command by bluetooth
  if (stat)
  {
    
    sCmd = BTremote.cmd();
    BTremote.ClearCommand();
              Serial.println(sCmd   ,DEC);
bNewCmdBT=0;
    switch (sCmd)
    {
      case '$':
      ///unlock
         break;
      case 'f':bNewCmdBT=1;
       InfraRed.Ir_value=IREM_KEY_up;break;
      case 'b':bNewCmdBT=1;
       InfraRed.Ir_value=IREM_KEY_dn;break;
      case 'l':bNewCmdBT=1;
       InfraRed.Ir_value=IREM_KEY_left;break;
      case 'r':bNewCmdBT=1;
       InfraRed.Ir_value=IREM_KEY_right;break;
      case '<':
       InfraRed.Ir_value=IREM_KEY_7;
       CarCtrl.HeadServo.IncTarget(HEAD_STEP);break;
      case '>':
       InfraRed.Ir_value=IREM_KEY_9;
       CarCtrl.HeadServo.IncTarget(-HEAD_STEP);break;
       
    }
    if (bNewCmdBT)
    {
          InfraRed.LockManager();
          CarCtrl.bHasNewIrCmd=1;
          CarCtrl.MyTimers.SetNextTime(MY_TIMER_IR,100);
        if ( InfraRed.Ir_value == IREM_KEY_left ||  InfraRed.Ir_value == IREM_KEY_right)
            Timeout=30;
        else
            Timeout=150;

          CarCtrl.MyTimers.SetNextTime(MY_TIMER_MOTORS_CMD_TIMEOUT,Timeout);
          CarCtrl.usecLastIrCmd = millis();// time now
    }
          
  }
  
  CarCtrl.MotionManager();
  
}

