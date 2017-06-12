
#include "stdafx.h"
#include "DummyClass ADBoard.H"

double Class_ADBoard::ADIn(int ADChannel) {return(1.0);}

int Class_DABoard::DAOut(int DAChannel, double Vout) {return(1);}

int Class_CounterBoard::SetCount(int CNTChannel, int CountMedian) {return(1);}
int Class_CounterBoard::Get(int CNTChannel) {return(1);}



