/// file: InfraRed.h
/// Class for Ir remote control of the car with lock manager

#ifndef _InfraRed_h_
#define _InfraRed_h_

#include <IRremote.h>

///// Ir module /////////////////////////////
/// Hardware mapping for Ir module
#define RECV_PIN 12 // Ir recive PIN
#define LED_L_PIN 13 // LED L pin


/// Ir control keyboard codes.
enum ERemoteKeys
{
  IREM_KEY_1 = 0xFF6897,
  IREM_KEY_2 = 0xFF9867,
  IREM_KEY_3 = 0xFFB04F,
  IREM_KEY_4 = 0xFF30CF,
  IREM_KEY_5 = 0xFF18E7,
  IREM_KEY_6 = 0xFF7A85,
  IREM_KEY_7 = 0xFF10EF,
  IREM_KEY_8 = 0xFF38C7,
  IREM_KEY_9 = 0xFF5AA5,
  IREM_KEY_0 = 0xFF4AB5,
      IREM_KEY_star =0xFF42BD,
      IREM_KEY_number =0xFF52AD,
      IREM_KEY_ok =0xFF02FD,
      IREM_KEY_left =0xFF22DD,
      IREM_KEY_right =0xFFC23D,
      IREM_KEY_up =0xFF629D,
      IREM_KEY_dn =0xFFA857
};

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

