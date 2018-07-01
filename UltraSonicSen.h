/// file: UltraSonicSen.h
/// Class for ultra sonic sensor control

#ifndef _UltraSonicSen_h_
#define _UltraSonicSen_h_

class CUltraSonicSen
{
private:
    int Echo;  
    int Trig;
public:
  CUltraSonicSen()
  {
  };
  void init();
  int GetDistancebyUS();
};
extern class CUltraSonicSen UltraSonicSen;
#endif

