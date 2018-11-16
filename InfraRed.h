/// file: InfraRed.h
/// Class for Ir remote control of the car with lock manager

#ifndef _InfraRed_h_
#define _InfraRed_h_

#include <IRremote.h>

#define IR_CONTROL_DEVICE_ARD 1
#define IR_CONTROL_DEVICE_LG_MP3 2

#define USED_IR_CONTROL_DEVICE IR_CONTROL_DEVICE_LG_MP3

/// Ir control keyboard codes:
#if USED_IR_CONTROL_DEVICE==IR_CONTROL_DEVICE_ARD
  #include "ARD_IrCodes.h"
  #elif USED_IR_CONTROL_DEVICE==IR_CONTROL_DEVICE_LG_MP3
  #include "LG_IrCodes.h"
#endif
  
///// Ir module /////////////////////////////
/// Hardware mapping for Ir module
#define RECV_PIN 12 // Ir recive PIN
#define LED_L_PIN 13 // LED L pin



#define keyseq_LEN  3 //7
#define key_mode_IND (keyseq_LEN-2) // last before final 'Ok'
#define lock_seq_LEN 3
struct CLockState
{
  bool LED_state ;
  int  loc_state; // disabled on enabled
  int  AlgMode;// type of control
  unsigned long  keyseq[keyseq_LEN];//!< key sequence
  unsigned long  lockseq[lock_seq_LEN];//!< lock sequence: *,0,*
  int  lock_ind;//current lock ke index

};

/// Lock-unlock-mode state machine.
enum ELockStates
{
  LOCK_STATE_SHUT_DOWN=-1, // low power state
  LOCK_STATE_OFF=0,
  LOCK_STATE_ON=1 // enabled
};
enum EAlgModes
{
  ALG_MODE_NOTHING = 0,
  ALG_MODE_DIRECT = 1,// direct control by Ir buttons
  ALG_MODE_US = 2,// use ultra-sound automatic control
  ALG_MODE_TRACE = 4// use trace module automatic control
};

class CInfraRed
{
  private:
  class IRrecv  *irrecv;//(RECV_PIN);
  struct decode_results results;
public:
 struct CLockState LockState;
public: 
 unsigned long   Ir_value;// new value accepted from Ir

 public:
 void init();
 void InitLockState();
 void LedOn();
 void LedOff();
 void LockManager();
 bool GetIrCommand();
 unsigned long  IrResult();
 void Resume();
  CInfraRed()
 {
  irrecv = new class IRrecv(RECV_PIN);
 }
};
extern class CInfraRed InfraRed;
#endif

