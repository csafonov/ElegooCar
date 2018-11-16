/// File: ARD_IrCodes.h
// standard arduino control 
//NEC-protocol
//Each button sends 1 or 2 codes: first is operational,
//second=0xFFFFFFFF means REPEAT BUTTON
// KEY		OPERATIONAL CODES
enum ERemoteKeys  
{
  key_PressedRepeat=0xFFFFFFFF,
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
