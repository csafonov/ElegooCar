/// file: Motor.h
/// Class for Drive controll by PWM

#ifndef _Motors_h_
#define _Motors_h_

class CMotor
{
  public:
  bool in_motion;
public:
  void forward();
  void back()  ;
  void left();
  void right();
  void stop();
  void left_freewheel();
  void right_freewheel();
  void KillMotor();
  void init();
};
extern class CMotor Motor;
#endif

