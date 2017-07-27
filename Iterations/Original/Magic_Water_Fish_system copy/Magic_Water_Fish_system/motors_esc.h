#ifndef motors_h
#define motors_h

class motorsclass{
  public:
    motorsclass();
    void SETUP();
    void motorMOVE(int J,int K,int M,int N, int O);
    void motorMoveLATERAL(int J,int K,int M,int N,int O);
    void motorMoveUP(int J,int K, int M, int N, int O);
};

extern motorsclass motors;

#endif
