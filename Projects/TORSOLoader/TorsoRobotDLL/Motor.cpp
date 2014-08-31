//====================================================================
//  MotorClass�N���X
//  �I���W�i����Kawabuchi�쐬��TexART_ServoMotor000�ł���
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

	// ���[�p�X�t�B���^�̃J�b�g�I�t���g���Z�b�g�Ə�����
	// �{����init�ł��ׂ������A�v���p�e�B�ǂݍ��݂��ʊ֐�������Ă���̂ł����ōs��
	lpfilter.set_lpfreq((float)tmpCFreq);

	// Initialize the EncoderCount and the Counter board.
	EncoderCount = (unsigned long)(0xFFFFFF / 2);

}

//---------------------------------------------------------
// �G���R�[�_�J�E���^�l����p�x�A�p���x�A�p�����x�����߂�
//---------------------------------------------------------
void MotorClass::CalcAngularParameter(DWORD read_count)
{
  // Calculate the present values for the ring buffers.
	Angle = Direction_encoder * (double)(read_count - (double)MeanCount ) / RealResolution * 2.0 * PI;
	AngleVelocity = lpfilter.lpfilter_genV(Angle, MainTimer.Elapsed());

}

//---------------------------------------------------------
// �g���N��DA�o�͗p�d���ɕϊ�
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