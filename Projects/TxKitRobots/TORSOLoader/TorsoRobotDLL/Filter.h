#ifndef __GENERAL_FILTER
#define __GENERAL_FILTER

class GeneralFilter{
	//----------------------------------------------//
	// 固定データ
	//----------------------------------------------//
	float lpcutoff_freq;													// ローパスフィルタ用カットオフ周波数

	double n_old_x1;														// 過去のデータ格納（lpfilter_normal用）
	double n_old_x1_dot;													// 過去のデータ格納（lpfilter_normal用）
	__int64 n_old_time;														// 過去の時間格納
	double A_old_x1;														// 過去のデータ格納（lpfilter_genA用）
	double A_old_x1_dot;													// 過去のデータ格納（lpfilter_genA用）
	__int64 A_old_time;														// 過去の時間格納
	double V_old_x1;														// 過去のデータ格納（lpfilter_genV用）
	__int64 V_old_time;														// 過去の時間格納

public:
	//----------------------------------------------//
	// 関数
	//----------------------------------------------//
	void set_lpfreq(float _lpcutoff_freq);									// ローパスフィルタカットオフ周波数セット関数
	void clearparameter(void);
	double lpfilter_normal(double u, __int64 time);							// 2次ローパスフィルタ、1次元データ用（ex.肘センサ）
	double lpfilter_genA(double u, __int64 time);							// 2次ローパスフィルタ、加速度計算用
	double lpfilter_genV(double u, __int64 time);							// 1次ローパスフィルタ、速度計算用
};

#endif