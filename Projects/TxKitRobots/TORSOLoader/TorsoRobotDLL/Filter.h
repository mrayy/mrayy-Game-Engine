#ifndef __GENERAL_FILTER
#define __GENERAL_FILTER

class GeneralFilter{
	//----------------------------------------------//
	// �Œ�f�[�^
	//----------------------------------------------//
	float lpcutoff_freq;													// ���[�p�X�t�B���^�p�J�b�g�I�t���g��

	double n_old_x1;														// �ߋ��̃f�[�^�i�[�ilpfilter_normal�p�j
	double n_old_x1_dot;													// �ߋ��̃f�[�^�i�[�ilpfilter_normal�p�j
	__int64 n_old_time;														// �ߋ��̎��Ԋi�[
	double A_old_x1;														// �ߋ��̃f�[�^�i�[�ilpfilter_genA�p�j
	double A_old_x1_dot;													// �ߋ��̃f�[�^�i�[�ilpfilter_genA�p�j
	__int64 A_old_time;														// �ߋ��̎��Ԋi�[
	double V_old_x1;														// �ߋ��̃f�[�^�i�[�ilpfilter_genV�p�j
	__int64 V_old_time;														// �ߋ��̎��Ԋi�[

public:
	//----------------------------------------------//
	// �֐�
	//----------------------------------------------//
	void set_lpfreq(float _lpcutoff_freq);									// ���[�p�X�t�B���^�J�b�g�I�t���g���Z�b�g�֐�
	void clearparameter(void);
	double lpfilter_normal(double u, __int64 time);							// 2�����[�p�X�t�B���^�A1�����f�[�^�p�iex.�I�Z���T�j
	double lpfilter_genA(double u, __int64 time);							// 2�����[�p�X�t�B���^�A�����x�v�Z�p
	double lpfilter_genV(double u, __int64 time);							// 1�����[�p�X�t�B���^�A���x�v�Z�p
};

#endif