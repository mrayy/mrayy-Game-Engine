//====================================================================
//  Motorクラス
//  オリジナルはKawabuchi作成のTexART_ServoMotor000である
//====================================================================

#ifndef __MotorClass
#define __MotorClass

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "CPUTimer.h"
#include "Filter.h"
#include "InterfaceBoard.h"

int LoadData(ifstream& data_file, char partition, ...);

#ifndef ___PI
#define ___PI
const double PI = 3.141592654;
const double pi = 3.141592654;
#endif

///-----[Motorクラス]-------------------------------------------------
class MotorClass {
public:
	double Angle;
	double AngleVelocity;
	double Torque;

protected:
	double TorqueConst;
	double AmpFactor;

	int    Resolution;
	int    ResolutionMulti;
	int    RealResolution;
	unsigned long MeanCount;

	double CommandVoltage;
	unsigned long EncoderCount;

	double LastTorque;

	int    Direction_encoder;
	int    Direction_motor;

	int    DAChannel;
	int    CNTChannel;

public:
	void Initialize(double data[8]);
	void CalcAngularParameter(DWORD read_theta);
	double CalcTorqueToDAData(double _torque);

	int ShowDAChannel(void){return DAChannel;};
	int ShowCNTChannel(void){return CNTChannel;};
	unsigned long ShowMeanCount(void){return MeanCount;}

	GeneralFilter lpfilter;
};

#endif
