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
 * DH�p�����[�^�\����
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
 * PID�p�����[�^�\����
 */
struct PIDParam
{

public:
	double P;
	double I;
	double D;
};

/**
 * ���z�I�ȃ����N�̃N���X�D
 * a,alpha,d,theta��DH�p�����[�^��PID�p�����[�^��ۑ�����D
 * �ڕW�p�x���w�肵����,���݂̊p�x����CPID����ɂ��g���N�o�͂������傭����
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

	// PID����p�����[�^
	double e[3];	//�΍��@0���݁@1�ߋ��@2����ɉߋ�
	double v;
	double errsum;


	GeneralFilter lpfilter;

};


#endif