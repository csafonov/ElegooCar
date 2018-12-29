/// file: InfraRed.cpp
/// IR remote controller with lock manager
#include <Arduino.h>

#include "InfraRed.h"
#include "CarCtrl.h"


//////////////////////////////////////////////



void CInfraRed::init()
{
  irrecv->enableIRIn();
  InitLockState();
}

bool CInfraRed::GetIrCommand()
{
    return irrecv->decode(&results);
}
unsigned long CInfraRed::IrResult()
{
  return results.value;
}
void CInfraRed::Resume()
{
  irrecv->resume();
}
void CInfraRed::LedOn()
{ 
  LockState.LED_state = true;
  digitalWrite(LED_L_PIN,LockState.LED_state);
}
void CInfraRed::LedOff()
{ 
  LockState.LED_state = false;
  digitalWrite(LED_L_PIN,LockState.LED_state);
}
void CInfraRed::InitLockState()
{
  LockState.loc_state = LOCK_STATE_OFF;// initial state
  LockState.AlgMode = ALG_MODE_NOTHING;
  /** len=7
  LockState.keyseq[0] = IREM_KEY_number;// init the key sequence for unlock
  LockState.keyseq[1] = IREM_KEY_0;//IREM_KEY_2;
  LockState.keyseq[2] = IREM_KEY_0;// init the key sequence for unlock
  LockState.keyseq[3] = IREM_KEY_0;//IREM_KEY_1;
  LockState.keyseq[4] = IREM_KEY_0;//IREM_KEY_1;
  LockState.keyseq[5] = 0;
  LockState.keyseq[6] = IREM_KEY_ok;
  */
#if USED_IR_CONTROL_DEVICE==IR_CONTROL_DEVICE_ARD
  // unlock password: #,0,Ok
  LockState.keyseq[0] = IREM_KEY_number;// init the key sequence for unlock
  LockState.keyseq[1] = 0;// here wait for '0'
  LockState.keyseq[2] = IREM_KEY_ok;// init the key sequence for unlock
  // lock password: *,0,*
  LockState.lock_ind = 0;
  LockState.lockseq[0]=IREM_KEY_star;
  LockState.lockseq[1]=IREM_KEY_0;
  LockState.lockseq[2]=IREM_KEY_star;
  #elif USED_IR_CONTROL_DEVICE==IR_CONTROL_DEVICE_LG_MP3
  // unlock password: POWER,0,key_Stop_small
  LockState.keyseq[0] = key_Power;// init the key sequence for unlock
  LockState.keyseq[1] = 0;// here wait for '0'
  LockState.keyseq[2] = key_Stop_small;// init the key sequence for unlock
  // lock password: Mute,0,Mute
  LockState.lock_ind = 0;
  LockState.lockseq[0]=key_Mute;
  LockState.lockseq[1]=IREM_KEY_0;
  LockState.lockseq[2]=key_Mute;
  #endif
  LedOff();
}

/// Lock manager
/// User should press the following sequence in order to start the machine:
/// #,key1,key2,key3,key4,<mode number=1|2|4>,Ok. If successed - LED is 
/// and the appropriate mode is inabled. If was error - lock is reset and should repeat.
/// If unlocked, the OFF mode is set by:
/// *,0,* and the LED is OFF.
void CInfraRed::LockManager()
{

 // BEEPERS
 //  if (Ir_value == key_Volume_UP)
 //      CarCtrl.MyBeeper.TurnOn(1,900,1000);
 //  if (Ir_value == key_Volume_DN)
 //      CarCtrl.MyBeeper.TurnOn(0,900,1000);
 
// LEDS
   if (Ir_value == key_Volume_UP)
       CarCtrl.MyLED1.TurnOn(1,200,0);
   if (Ir_value == key_Volume_DN)
       CarCtrl.MyLED1.TurnOn(0,200,0);
    if (Ir_value == key_Preset_UP)
       CarCtrl.MyLED2.TurnOn(1,200,0);
   if (Ir_value == key_Preset_DN)
       CarCtrl.MyLED2.TurnOn(0,200,0);
       
  switch (LockState.loc_state)
  {
    case LOCK_STATE_OFF://0
      
      if (  LockState.keyseq[LockState.lock_ind] == Ir_value || LockState.lock_ind==key_mode_IND)
      {
        if (CarCtrl.bDebugIr)  Serial.println("Guess Ok");
        if (LockState.lock_ind >= keyseq_LEN-1)
        {
          // success unlock
          LockState.lock_ind=0;
          LockState.loc_state=LOCK_STATE_ON;
          LedOn();
          /// Start micro-servo
          CarCtrl.StartMicroServo();
 
          if (CarCtrl.bDebugIr) Serial.println("UNLOCKED");
        }
        else
        {
          if ( LockState.lock_ind==key_mode_IND )
          {
            switch (Ir_value)
            {
              case IREM_KEY_0://IREM_KEY_1:
                LockState.AlgMode = ALG_MODE_DIRECT;
                LockState.lock_ind++;
                break;
                default:// invalid alg
                    LockState.lock_ind=0;//restart
                    if (CarCtrl.bDebugIr)  Serial.println("AlgMode BAD, reset");
                    break;
            }
          }
          else // not key_mode_IND 
          {
            LockState.lock_ind++;
          }
        }
      }
      else // invalid key
      {
        if (CarCtrl.bDebugIr)  Serial.println("Guess BAD, reset");
        LockState.lock_ind=0;//restart
      }
     break;

    case LOCK_STATE_ON://1

// Special commands 
// <1> = enable trace, <2>=enable Ir debug, <3>=enable/disable US
// <4> = disable trace,<5>=disable Ir debug,<6>=no trace,
//                     <0>=stop by trace
    if (Ir_value == IREM_KEY_1)
       CarCtrl.bDebugTrace  = 1;
    if (Ir_value == IREM_KEY_4)
       CarCtrl.bDebugTrace  = 0;
    
       
    if (Ir_value == IREM_KEY_2)
       CarCtrl.bDebugIr  = 1;
    if (Ir_value == IREM_KEY_5)
       CarCtrl.bDebugIr  = 1;

    if (Ir_value == IREM_KEY_3)
       CarCtrl.bDebugUs  = !CarCtrl.bDebugUs;
       
    if (Ir_value == IREM_KEY_6)
    {
       CarCtrl.bNoTrace  = 1;
       CarCtrl.LineTracking.TriggerLine = 0;// clear triggered line processed
    }
       
    if (Ir_value == IREM_KEY_0)
       CarCtrl.bNoTrace  = 0;

    if (Ir_value == IREM_KEY_9)
      CarCtrl.HeadServo.IncTarget(-HEAD_STEP);
    if (Ir_value == IREM_KEY_7)
      CarCtrl.HeadServo.IncTarget(HEAD_STEP);   




      if ( LockState.lock_ind==0 && Ir_value != IREM_KEY_star)
        break; // do nothing with lock/unlock, perform control
      if (  LockState.lockseq[LockState.lock_ind] == Ir_value)
      {
        if (CarCtrl.bDebugIr)  Serial.println("Locking.");
        if (LockState.lock_ind >= lock_seq_LEN-1)
        {
          // lock now
          LockState.lock_ind=0;
          LockState.loc_state=LOCK_STATE_OFF;
          LedOff();
          if (CarCtrl.bDebugIr)  Serial.println("RELOCKED");
        }
        else
        {
          LockState.lock_ind++;//next lock key wait
        }
      }
      else
      {
        LockState.lock_ind=0;//reset locking
        if (CarCtrl.bDebugIr)  Serial.println("reset locking");
      }
    break;
  }
}

