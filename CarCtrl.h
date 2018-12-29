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
#define MY_TIMERS_NUM   10 // number of controlled objects
enum EMyTimers
{
  MY_TIMER_IR    = 0, // IR remote control
  MY_TIMER_MOTORS_CMD_TIMEOUT= 1,// motors drive
  MY_ULTRA_SONIC = 2,// ultra-sonic distance measurement
  MY_MICR_SERVO  =3, // micro-servo for US head
  MY_TIMER_BEEPER = 4, // for beeper implementation
  MY_TIMER_BEEP_INTERVAL = 5,
  MY_TIMER_LED1 = 6, // for beeper implementation
  MY_TIMER_LED1_INTERVAL = 7,
  MY_TIMER_LED2 = 8, // for beeper implementation
  MY_TIMER_LED2_INTERVAL = 9
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

class CMyBeeper
{
  private:
    short TimerId_per;
    short TimerId_int;
    short beepVal;
    short msPeriod;
    short bEna;
    short msInterval;
    bool bOn;
    char pin;
    class CMyTimers *pMyTimers;
  public:
  bool IsOn()
  {
    return bEna ? true:false;
  }
  void TurnOn(short _bEna,short _msInterval,float _Hz)
  {
    bEna=_bEna;
    //beepVal = HIGH;
    msInterval=_msInterval;
    setFreq(  _Hz);
    //bOn=false;//true;
    pMyTimers->SetNextTime(TimerId_int,0);// start immediately
  }
  void setFreq(float Hz)
  {
    if (Hz==0)
      msPeriod=0;
      else
      {
     msPeriod =(short)( 1000.0f/Hz);
     if (msPeriod < 1)
         msPeriod = 1;
      }
  }
  void init (char _pin,short _TimerId_per,short _TimerId_int,class CMyTimers *_pMyTimers)
  {
    TimerId_per=_TimerId_per;
    TimerId_int= _TimerId_int;

    pin=_pin;
     pinMode(pin,OUTPUT);//pin 14
     digitalWrite(pin, LOW);
     setFreq(50);
     bEna=0;
     msInterval = 0;
     pMyTimers = _pMyTimers;
  }
  void process()
  {
    if (msInterval)
    {
      if (pMyTimers->IsTimeout(TimerId_int))
      {
        pMyTimers->SetNextTime(TimerId_int,msInterval);
        if (bEna==1)
        {
          bOn =( bOn==true) ? false:true;
          pMyTimers->SetNextTime(TimerId_per,msPeriod);
          beepVal = ( bOn==true) ? HIGH : LOW ;
        }
        else
        {
          bOn =false;
          
          if (bEna==0)
          {
            beepVal=LOW;
          }
          else
          if (bEna==2)
          {
             beepVal = (beepVal==HIGH) ? LOW:HIGH;
          }
        }
        digitalWrite(pin, beepVal);
      }
    }

    if (bOn)
    {
      if (pMyTimers->IsTimeout(TimerId_per))
      {
        beepVal = (beepVal==HIGH) ? LOW:HIGH;// 50% duty
        digitalWrite(pin, beepVal);
        pMyTimers->SetNextTime(TimerId_per,msPeriod);
      }
    }
        
  }

  void processLED()
  {
    if (msInterval)
    {
      if (pMyTimers->IsTimeout(TimerId_int))
      {
        pMyTimers->SetNextTime(TimerId_int,msInterval);
        if (bEna)
        {
          bOn =( bOn==true) ? false:true;
          beepVal = ( bOn==true) ? HIGH : LOW ;
        }
        else
        {
          bOn =false;
          beepVal = LOW;
        }
        digitalWrite(pin, beepVal);
      }
    }
  } // processLED
};//class

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

#define HEAD_STEP 10
/// Micro servo for US moving
class CHeadServo
{
  private:
    class Servo myservo;      // create servo object to control servo
  private:
    short degMyServoTarget;
  public:
    void IncTarget(short dx)
    {  degMyServoTarget += dx;// x>0:CCW
       degMyServoTarget = degMyServoTarget % 360;
    }
    void InitTaget(short x)
    {  degMyServoTarget =  x;}
    void Go()
    {
          myservo.write(degMyServoTarget);  //setservo position according to scaled value

    }
    void init()
    {
      InitTaget(90);
      myservo.attach(3);  // attach servo on pin 3 to servo object
      degMyServoTarget = 90;

    }
};

class CCarCtrl
{
  public:
    class CHeadServo HeadServo;
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
      class CMyBeeper MyBeeper;
      class CMyBeeper MyLED1;
      class CMyBeeper MyLED2;
};

extern class CCarCtrl CarCtrl;

#endif
