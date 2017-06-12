//====================================================================
//  MotorClassクラス
//  オリジナルはKawabuchi作成のTexART_ServoMotor000である
//====================================================================

#include "stdafx.h"
#include "Motor.h"
#include "variables.h"


///-----[Motor::Initialize]-------------------------------------
void MotorClass::Initialize(double data[8])
{
	double  tmpCFreq=20.0;

	TorqueConst			= data[0];
	AmpFactor			= data[1];
	ResolutionMulti		= (int)data[2];
	Resolution			= (int)data[3];
	Direction_motor		= (int)data[4];
	Direction_encoder	= (int)data[5];
	DAChannel			= (int)data[6];
	CNTChannel			= (int)data[7];

//	fprintf(fp_link, "motor, %f, %f, %d, %d, %d, %d, %d, %d\n", 
//		TorqueConst, AmpFactor, Resolution, ResolutionMulti, 
//		Direction_motor, Direction_encoder, DAChannel, CNTChannel);

	RealResolution		= ResolutionMulti * Resolution;
	MeanCount			= (unsigned long)(0xFFFFFF / 2);

	// ローパスフィルタのカットオフ周波数セットと初期化
	// 本来はinitでやるべきだが、プロパティ読み込みが別関数化されているのでここで行う
	lpfilter.set_lpfreq((float)tmpCFreq);

	// Initialize the EncoderCount and the Counter board.
	EncoderCount = (unsigned long)(0xFFFFFF / 2);

}

//---------------------------------------------------------
// エンコーダカウンタ値から角度、角速度、角加速度を求める
//---------------------------------------------------------
void MotorClass::CalcAngularParameter(DWORD read_count)
{
  // Calculate the present values for the ring buffers.
	Angle = Direction_encoder * (double)(read_count - (double)MeanCount ) / RealResolution * 2.0 * PI;
	AngleVelocity = lpfilter.lpfilter_genV(Angle, MainTimer.Elapsed());

}

//---------------------------------------------------------
// トルクをDA出力用電圧に変換
//---------------------------------------------------------
double MotorClass::CalcTorqueToDAData(double _torque)
{
  // Keep last torque.
  LastTorque = Torque = _torque;

  // Calculate CommandVoltage.
  // Current control type amplifier.
  CommandVoltage = Direction_motor * Torque / AmpFactor / TorqueConst;
  if     (CommandVoltage >  5.0) CommandVoltage =  5.0;
  else if(CommandVoltage < -5.0) CommandVoltage = -5.0;

  return CommandVoltage;
}