/// file: CarCtrl.h
/// Main Class for Car control 

#ifndef _CarCtrl_h_
#define _CarCtrl_h_

//// Micro-servo module ///////////////////////////
#include <Servo.h>  //servo library

class CLineTracking
{
  public:
    int RML;
    int R;
    int M;
    int L;
    bool TriggerLine;
  void init();
  bool readR();
  bool readM();
  bool readL();
 };

/// Support for timing
#define MY_TIMERS_NUM   4 // number of controlled objects
enum EMyTimers
{
  MY_TIMER_IR    = 0, // IR remote control
  MY_TIMER_MOTORS_CMD_TIMEOUT= 1,// motors drive
  MY_ULTRA_SONIC = 2// ultra-sonic distance measurement
};
class CMyTimers
{
  private:
    unsigned long  GlobalTime; /// msec, counts until 1000 hours
    unsigned long  NextTime[MY_TIMERS_NUM];// next time [msec]
  public:
    void init();
    void SetNextTime(int n, unsigned long msTimeout);// define new timeout target
    bool IsTimeout(int n);// check if it is timeout now
    void UpdateGlobalTime();// update GlobalTime timer each cylce 
};

class CBTremote
{
  private:
   char NewCmd;
  public:
  void ClearCommand() {  NewCmd=0; };
  char cmd() { return NewCmd;}
  int GetBTCommand()
  {
    char c = Serial.read();
    if (c>0)
    {
      NewCmd = c;
      return 1;
    }
    return 0;
  }
  CBTremote()
  {
    NewCmd=0;
  }
};


class CCarCtrl
{
  private:
    class Servo myservo;      // create servo object to control servo
    
  public:
    class CLineTracking LineTracking;
    void init();
    void MotionManager();
    void StartMicroServo();
    void CarCtrlMan();
  public:
      int rightDistance;
      int leftDistance;
      int middleDistance;
      unsigned long usecLastIrCmd;// time of last valid command
      bool bDebugIr;
      bool bDebugTrace;
      bool bDebugUs;
      bool bDebugCar;
      bool bNoTrace;
      bool bHasNewIrCmd;

  public:
      class CMyTimers MyTimers;
      class CBTremote BTremote;
};

extern class CCarCtrl CarCtrl;

#endif
