///////////////////////////////////////////////////////////
//  RealTorso.h
//  Implementation of the Class CRealTorso
//  Created on:      10-7-2007
//  Revised author : kouichi
//
//  ** Original **
//  Implementation of the Class CVirtualTorso
//  Created on:      29-5-2007 16:10:00
//  Original author : Jo
///////////////////////////////////////////////////////////

#ifndef ___REALTORSO
#define ___REALTORSO

#include "RealLink.h"
#include "defines.h"

/**
 * リアルトルソ本体．目標姿勢行列と現在リンク角度を入力すると，目標リンク角度，出力トルクを出力する．
 */

class IRobotStatusProvider;

class CRealTorso 
{

public:
	CRealTorso();
	virtual ~CRealTorso();
	int  InitTorso(const char *fname);
	void ChangeLimitRange(int sel);

	void SetNowAngles(DWORD *count);
	void SetTargetAngles(double *angles);
	void SetTargetMatrix(double *matrix);
	void SetAngleOffset(double *center);
	void SetInitialAngles(double *angles);

	void GetNowAngles(double *angles);
	void GetTargetAngles(double *angles);
	void GetInitialAngles(double *angles);
	void GetTorques(double *torques);

	void CalcTargetAngles(double *param);
	void ClearParameter(void);

	int  ZeroDetect(void);
	int  AutoZeroDetect(void);

	void SendToGLprogram(void);

	// interface アクセス関数
	void GetCounterValue(void);
	void SetCountOffset(void);
	int  GetDIOValue(int channel);
	void SetDAValue(double *torque);
	void SetTorqueDirect(int ch, double torque);

	int DIOChecker(int ch, int detect_pattern, double angle_range, double torque, double time_span, double gain, double *result_angle);

	const double L1;

	void SetProvider(IRobotStatusProvider* p){ m_provider = p; }

private:
	/**
	 * 目標姿勢行列
	 */
	double targetMatrix[16];
	CRealLink realLink[Torso_DOF];

	double Initial_Angles[Torso_DOF];

	IRobotStatusProvider* m_provider;

};
#endif