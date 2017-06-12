///////////////////////////////////////////////////////////
//  RealLink.cpp
//  Implementation of the Class CRealLink
//  Created on:      10-7-2007
//  Revised author : kouichi
//
//  ** Original **
//  VirtualLink.cpp
//  Implementation of the Class CVirtualLink
//  Created on:      29-5-2007 16:10:00
//  Original author: Jo
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealLink.h"
#include "variables.h"


CRealLink::CRealLink(){}

CRealLink::~CRealLink(){}

void CRealLink::InitRegurator(double _minForce,double _maxForce,double _minAngle,double _maxAngle)
{
	minForce=_minForce;
	maxForce=_maxForce;
	minAngle=_minAngle;
	maxAngle=_maxAngle;
}

double CRealLink::GetTorque()
{
	errsum+=e[0];
	e[0]=targetAngle-nowAngle;
	v=targetVelocity-nowVelocity;

//	fprintf(fp_link,"%f,%f,",targetAngle, nowAngle);
	double torque= pidParam.P*(e[0]) + pidParam.I*(errsum) + pidParam.D*(v);

	e[2]=e[1];
	e[1]=e[0];
//	torque /= GearRatio;
	if(torque<minForce)torque=minForce;
	if(torque>maxForce)torque=maxForce;
	return torque;
}


void CRealLink::InitDHParam(DynamicParamName _dynamicParam, double a, double alpha, double d, double theta)
{
	dhParam.A=a;
	dhParam.Alpha=alpha;
	dhParam.D=d;
	dhParam.Theta=theta;
	dynamicParam=_dynamicParam;
}


void CRealLink::InitPIDParam(double p, double i, double d)
{
	pidParam.P=p;
	pidParam.I=i;
	pidParam.D=d;
}


void CRealLink::SetNowAngle(DWORD enc_count)
{

	Motor.CalcAngularParameter(enc_count);
	nowAngle = Motor.Angle / GearRatio - AngleOffset;
	nowVelocity = Motor.AngleVelocity; // / GearRatio;
}

void CRealLink::SetAngleOffset(double _angleoffset)
{
	AngleOffset = _angleoffset;
}

double CRealLink::GetTargetAngle(void)
{
	return targetAngle;
}

double CRealLink::GetNowAngle(void)
{
	return nowAngle;
}

void CRealLink::SetTargetAngle(double angle)
{
	targetAngle=angle;
	if(targetAngle<minAngle)targetAngle=minAngle;
	if(targetAngle>maxAngle)targetAngle=maxAngle;

	// フィルタによる角速度計算
	targetVelocity = lpfilter.lpfilter_genV(targetAngle, MainTimer.Elapsed());
//	fprintf(fp_link,"%f,%f,",targetAngle, targetVelocity);
}

void CRealLink::Initialize(double data[14])
{
	double tmpCFreq=2000.0;

	type			= (int)data[0];
	dhParam.Alpha	= data[1];
	dhParam.A		= data[2];
	dhParam.D		= data[3];
	dhParam.Theta	= data[4] / 180.0 * PI;
	maxAngle		= data[5];// / 180.0 * PI;
	minAngle		= data[6];// / 180.0 * PI;
	maxForce		= data[7];
	minForce		= -data[7];
	GearRatio		= data[8];
	pidParam.P		= data[9];
	pidParam.I		= data[10];
	pidParam.D		= data[11];

	DIOChannel		= (int)data[12];
	tmpCFreq		= data[13];

	if(type == 0){
		maxAngle = maxAngle / 180.0 * PI;
		minAngle = minAngle / 180.0 * PI;
	}

	//	Motor.Initialize(MotorFile);

	AngleOffset = 0.0;

	lpfilter.set_lpfreq((float)tmpCFreq);


//	fprintf(fp_link, "link, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %f\n",
//					type, dhParam.Alpha, dhParam.A, dhParam.D, dhParam.Theta, 
//					maxAngle, minAngle, maxForce, minForce, GearRatio, 
//					pidParam.P, pidParam.I, pidParam.D, DIOChannel, tmpCFreq);
}

void CRealLink::ClearParameter(void)
{
	targetAngle = 0.0;
	targetVelocity = 0.0;
	nowAngle = 0.0;
	nowVelocity = 0.0;

	// PID制御パラメータ
	e[0]=0; e[1]=0; e[2]=0;		//偏差　0現在　1過去　2さらに過去
	v=0;
	errsum=0;

	lpfilter.clearparameter();
	Motor.lpfilter.clearparameter();
}