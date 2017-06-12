///////////////////////////////////////////////////////////
//  RealLink.h
//  Implementation of the Class CRealLink
//  Created on:      10-7-2007
//  Revised author;  kouichi
//
//  ** Original **
//  Implementation of the Class CVirtualLink
//  Created on:      29-5-2007 16:09:59
//  Original author: Jo
///////////////////////////////////////////////////////////

#ifndef ___RealLink
#define ___RealLink

#include "Motor.h"
#include <fstream>

using namespace std;

/**
 * DHパラメータ構造体
 */
struct DHParam
{

public:
	double A;
	double Alpha;
	double D;
	double Theta;

};

enum DynamicParamName
{
	PARAM_A,
	PARAM_ALPHA,
	PARAM_D,
	PARAM_THETA
};
/**
 * PIDパラメータ構造体
 */
struct PIDParam
{

public:
	double P;
	double I;
	double D;
};

/**
 * 仮想的なリンクのクラス．
 * a,alpha,d,thetaのDHパラメータとPIDパラメータを保存する．
 * 目標角度を指定したら,現在の角度から，PID制御によるトルク出力もしゅつりょくする
 * 
 */
class CRealLink
{

public:
	CRealLink();
	virtual ~CRealLink();
	void Initialize(double data[14]);
	void InitDHParam(DynamicParamName dynamicParam, double a, double alpha, double d, double theta);
	void InitRegurator(double _minForce,double _maxForce,double _minAngle,double _maxAngle);
	void InitPIDParam(double p, double i, double d);

	void SetNowAngle(DWORD enc_count);
	void SetAngleOffset(double _angleoffset);
	void SetTargetAngle(double angle);

	double GetNowAngle(void);
	double GetTargetAngle(void);
	double GetTorque();

	int ShowDIOChannel(void){return DIOChannel;}
	void ClearParameter(void);

	MotorClass Motor;

private:
	int type;

	DHParam dhParam;
	DynamicParamName dynamicParam;
	PIDParam pidParam;
	double targetAngle;
	double maxForce;
	double minForce;
	double minAngle;
	double maxAngle;
	double targetVelocity;
	double nowAngle;
	double nowVelocity;

	double GearRatio;
	double AngleOffset;
	int DIOChannel;

	// PID制御パラメータ
	double e[3];	//偏差　0現在　1過去　2さらに過去
	double v;
	double errsum;


	GeneralFilter lpfilter;

};


#endif