///////////////////////////////////////////////////////////
//  RealTorso.cpp
//  Implementation of the Class CRealTorso
//  Created on:      10-7-2007
//  Revised author : kouichi
//
//  ** Original **
//  VirtualTorso.cpp
//  Implementation of the Class CVirtualTorso
//  Created on:      29-5-2007 16:10:00
//  Original author : Jo
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealTorso.h"
#include <Math.h>
#define M_PI 3.1415926535

#include "vectors.h"
#include "InterfaceBoard.h"
#include "variables.h"
#include "IRobotController.h"
#include <iostream>
#include <conio.h>

using namespace std;

#pragma warning (disable : 4996)

CRealTorso::CRealTorso()
:L1(0.5)
{
	m_provider = 0;
	// 初期パラメータ、大体動くように設定されている
	// 【注意】ここを変えても値は変わらない、Parameters.txtを変える事！
	realLink[0].InitPIDParam(5.2,0.001,0.032);
	realLink[0].InitRegurator(-1.2,1.2,-30*M_PI/180,30*M_PI/180);

	realLink[1].InitPIDParam(5.2,0.001,0.032);
	realLink[1].InitRegurator(-1.2,1.2,-30*M_PI/180,30*M_PI/180);

	realLink[2].InitPIDParam(32.0,0.001,0.025);
	realLink[2].InitRegurator(-1.8,1.8,0,0.2);

	realLink[3].InitPIDParam(4.0,0.0001,0.0042);
	realLink[3].InitRegurator(-0.36,0.36,-30*M_PI/180,30*M_PI/180);

	realLink[4].InitPIDParam(3.8,0.0001,0.0042);
	realLink[4].InitRegurator(-0.36,0.36,-45*M_PI/180,45*M_PI/180);

	realLink[5].InitPIDParam(1.8,0.00001,0.0025);
	realLink[5].InitRegurator(-0.36,0.36,-M_PI,M_PI);

}


CRealTorso::~CRealTorso()
{

}

void CRealTorso::ChangeLimitRange(int sel)
{
	if(sel == 0){
		realLink[0].InitRegurator(-1.2,1.2,-30*M_PI/180,30*M_PI/180);
		realLink[1].InitRegurator(-1.2,1.2,-30*M_PI/180,30*M_PI/180);
	}
	if(sel == 1){
		realLink[0].InitRegurator(-1.2,1.2,-35*M_PI/180,35*M_PI/180);
		realLink[1].InitRegurator(-1.2,1.2,-35*M_PI/180,35*M_PI/180);
	}
}


int CRealTorso::InitTorso(const char *fname)
{
	char	item[50]={0}, temp[200],ctemp;
	double  data[2][Torso_DOF][14]={0};
	double  tmp_data[Torso_DOF]={0};

	int	i, Part, Element;

	ifstream *Param;
	// 各関節パラメータファイルの処理
	Param = new ifstream();
	Param->open(fname);
	if(!*Param){
		printf("Flie Open Error %s \n",fname);
		return -1;
	}

	while(Param->peek() != EOF){
		if(Param->peek() == '/'){		Param->get(temp, 200); Param->get(ctemp);}	// コメント文と改行
		else if(Param->peek() == '\n'){	Param->get(ctemp);}
		else{
			Param->getline(item, 50, ':');
			for(i=0; i<Torso_DOF; i++)	*Param >> tmp_data[i];//Param->getline(tmp_data[i], 20, ',');
			if(Param->peek() != '\n'){
				Param->get(temp, 200); Param->get(ctemp);
			}
			else Param->get(ctemp);

			if     (!strcmp(item, "Link_Type")){				Part = 0; Element = 0;}
			else if(!strcmp(item, "Link_DH_alpha")){			Part = 0; Element = 1;}
			else if(!strcmp(item, "Link_DH_a")){				Part = 0; Element = 2;}
			else if(!strcmp(item, "Link_DH_d")){				Part = 0; Element = 3;}
			else if(!strcmp(item, "Link_DH_theta")){			Part = 0; Element = 4;}
			else if(!strcmp(item, "Link_LimitAngle_p")){		Part = 0; Element = 5;}
			else if(!strcmp(item, "Link_LimitAngle_n")){		Part = 0; Element = 6;}
			else if(!strcmp(item, "Link_LimitTorque")){			Part = 0; Element = 7;}
			else if(!strcmp(item, "Link_GearRatio")){			Part = 0; Element = 8;}
			else if(!strcmp(item, "Link_PGain")){				Part = 0; Element = 9;}
			else if(!strcmp(item, "Link_IGain")){				Part = 0; Element = 10;}
			else if(!strcmp(item, "Link_DGain")){				Part = 0; Element = 11;}
			else if(!strcmp(item, "Link_DIOChannel")){			Part = 0; Element = 12;}
			else if(!strcmp(item, "Link_FilterFreq")){			Part = 0; Element = 13;}
			else if(!strcmp(item, "Motor_TorqueConst")){		Part = 1; Element = 0;}
			else if(!strcmp(item, "Motor_AmpFactor")){			Part = 1; Element = 1;}
			else if(!strcmp(item, "Motor_ResolutionMulti")){	Part = 1; Element = 2;}
			else if(!strcmp(item, "Motor_Resolution")){			Part = 1; Element = 3;}
			else if(!strcmp(item, "Motor_DirectionMotor")){		Part = 1; Element = 4;}
			else if(!strcmp(item, "Motor_DirectionEncoder")){	Part = 1; Element = 5;}
			else if(!strcmp(item, "Motor_DAChannel")){			Part = 1; Element = 6;}
			else if(!strcmp(item, "Motor_CNTChannel")){			Part = 1; Element = 7;}
			else continue;

			for(i=0;i<Torso_DOF;i++) data[Part][i][Element] = tmp_data[i];
		}
	}
	Param->close();
	delete Param;

//----- 以降、各アームサブセットの初期化
	printf("Link & Joint initialize...");
	for(i=0; i<Torso_DOF; i++){
		realLink[i].Initialize(data[0][i]);
		realLink[i].Motor.Initialize(data[1][i]);
	}
	printf("Finish\n");


//----- interfaceboard -----
//#if USE_INTERFACE
	int DAChArray[6], DAChNum;

// ボード初期化のためのチャンネル情報取得 
// ジョイント01から順にチャンネル番号をChArrayに格納
	for(i=0; i<Torso_DOF; i++)	DAChArray[i] = realLink[i].Motor.ShowDAChannel();
	DAChNum = Torso_DOF;					// チャンネル数の格納

// interfaceboardクラスの初期化
	printf("Interface DA Board Initialize...\n");
	PCIDA.initialize(DAChArray,DAChNum,DA_10V);
	printf("Interface DA Board Initialize Finished\n");

//---------- エンコーダオフセット設定 ---------------
	SetCountOffset();

//#endif


// [Change] これはクラスの外に出す
//----- OpenGL出力用共有ファイルのオープン         -----
#ifdef GLOUTPUT
	printf("Shared Files for GL Output Open ...\n");
	if (!GLOutFRec.Open(GL_OUTFILE)) printf("Shared Files Open Successed\n");
	else{
		printf("Can't Open Shared Files\n");
		getchar();
	}
#endif

	return 0;

}


void CRealTorso::GetTorques(double *torques)
{
	for(int i=0;i<Torso_DOF;i++){
		torques[i]=realLink[i].GetTorque();
	}
}


void CRealTorso::SetNowAngles(DWORD *count)
{
	for(int i=0;i<Torso_DOF;i++)
		realLink[i].SetNowAngle(count[i]);
}

void CRealTorso::SetTargetAngles(double *angles)
{
	for(int i=0;i<Torso_DOF;i++)
		realLink[i].SetTargetAngle(angles[i]);
}


/**
 * 目標行列入力
 */
void CRealTorso::SetTargetMatrix(double *matrix)
{
	for(int i=0;i<16;i++)
		targetMatrix[i]=matrix[i];
}

void CRealTorso::CalcTargetAngles(double *param)
{
	double len=vecLength3(targetMatrix[12],targetMatrix[13],targetMatrix[14]-0.17);
	param[2]=len-L1;

	if(len!=0){
		double tmp=targetMatrix[12]/len;
		tmp=tmp>+1?+1:tmp;
		tmp=tmp<-1?-1:tmp;
		param[1]=asin(tmp);


		tmp=-targetMatrix[13]/(len*cos(param[1]));
		tmp=tmp>+1?+1:tmp;
		tmp=tmp<-1?-1:tmp;
		param[0]=asin(tmp);
	}else{
		param[0]=0;
		param[1]=0;
	}
	

	double mat[16],tmp[16],tmp2[16];
	MakeMatrix(axisX,0,0,0,-0.17,mat);
	MultiplyMatrix(mat,targetMatrix,tmp2);
	MakeMatrix(axisX,-param[0],0,0,0,mat);
	MultiplyMatrix(mat,tmp2,tmp);
	MakeMatrix(axisY,-param[1],0,0,-len,mat);
	MultiplyMatrix(mat,tmp,tmp2);
	MatrixtoXYZ(tmp2,&param[3],&param[4],&param[5]);
	///↑この関数が、たぶん右手系での計算だから、マイナスつける
	///↓
	param[3]=-param[3];
	param[4]=-param[4];
	param[5]=-param[5];

	//printf("%0.3f,%0.3f,%0.3f\n", param[3], param[4], param[5]);
}

void CRealTorso::GetTargetAngles(double *angles)
{
	for(int i=0;i<Torso_DOF;i++)
		angles[i]=realLink[i].GetTargetAngle();
}

void CRealTorso::GetNowAngles(double *angles)
{
	for(int i=0;i<Torso_DOF;i++)
		angles[i]=realLink[i].GetNowAngle();
}

void CRealTorso::GetCounterValue(void)
{
	DWORD tmpCount[8];
	DWORD read[8];
	int i;

	PCICNT.Get(read);
	// 添え字がチャンネルだったものをジョイント順に整列
	for (i=0; i<Torso_DOF; i++){
		tmpCount[i] = read[realLink[i].Motor.ShowCNTChannel()-1];
		realLink[i].SetNowAngle(tmpCount[i]);  // 各リンクのカウンタ値を角度に変換して代入
	}

}

void CRealTorso::SetCountOffset(void)
{
	printf("Setting Count Offset\n");
	DWORD MeanCount;

	MeanCount = realLink[0].Motor.ShowMeanCount();

	PCICNT.SetCount(MeanCount);
	printf("Setting Count Offset - done\n");
}

int CRealTorso::GetDIOValue(int channel)
{
	int dio_data;

	PCIDIO.DInput(&dio_data, channel, 1);

	return dio_data;
}


void CRealTorso::SetDAValue(double *torque)
{
	double write[Torso_DOF];
	double tmp_torque[2];
	float M;

	M = (float)0.20;
	// 重力補償
	tmp_torque[0] = torque[0] - M*9.8*(L1+realLink[2].GetNowAngle())*sin(realLink[0].GetNowAngle());
	tmp_torque[1] = torque[1] - M*9.8*(L1+realLink[2].GetNowAngle())*sin(realLink[1].GetNowAngle());
	if(torque[0] == 0) tmp_torque[0] = 0.0;
	if(torque[1] == 0) tmp_torque[1] = 0.0;
	torque[0] = tmp_torque[0];
	torque[1] = tmp_torque[1];

	for(int i=0; i<Torso_DOF; i++) write[i] = realLink[i].Motor.CalcTorqueToDAData(torque[i]);

	printf("Writing PCIDA DAOut\n");
	PCIDA.DAOut(write);
}

void CRealTorso::SetTorqueDirect(int ch, double torque)
{
	double write[Torso_DOF];

	for(int i=0; i<Torso_DOF; i++){
		if(i == ch) write[i] = realLink[i].Motor.CalcTorqueToDAData(torque);
		else write[i] = 0.0;
	}
//	printf("%f,%f,%f,%f,%f,%f\n",write[0],write[1],write[2],write[3],write[4],write[5]);
	PCIDA.DAOut(write);
}

int CRealTorso::ZeroDetect(void)
{
	int channel, flag, i;
	int psig[2], c;
	const int snum = 1;
	DWORD buff_count[snum];
	double center[Torso_DOF];

	const double offset_2ch = 7.75/180*PI;		// 090630,修理から帰ってきたらZ相ズレてた

	printf("0 Checkout Error\n");
	printf("Manually move near the origin\n");
	printf("Press [q] for detection finish\n");
	for(channel=1; channel<=Torso_DOF; channel++){
		printf("%d Axis\n",channel);
		psig[0] = GetDIOValue(realLink[channel-1].ShowDIOChannel());
		psig[1] = psig[0];
		flag = 0;
		while(flag < snum){
			psig[0] = GetDIOValue(realLink[channel-1].ShowDIOChannel());
			if(psig[0] != psig[1]){
				buff_count[flag] = PCICNT.Get(realLink[channel-1].Motor.ShowCNTChannel()-1);
				flag++;
			}
			psig[1] = psig[0];
			if(kbhit()){ c = getch(); if(c == 'q' || c  == '9') return -1;}
		}
		center[channel-1] = 0.0;
		for(i=0; i<snum; i++){
			realLink[channel-1].SetNowAngle(buff_count[i]);  // 各リンクのカウンタ値を角度に変換
			center[channel-1] += realLink[channel-1].GetNowAngle();
			printf("%f,",realLink[channel-1].GetNowAngle()*180/PI);
		}
		printf("\n");

		center[channel-1] /= snum;

		if(channel == 2) center[channel-1] += offset_2ch;	// 第2軸のZ相がずれているため

		printf("%d Axis End\n",channel);
	}
	printf("Exit 0 Checkout\n");
	for(i=0; i<Torso_DOF; i++){
		printf("%d:%f\n",i,center[i]*180/PI);
		realLink[i].SetAngleOffset(center[i]);
	}
	printf("Angle offset set succcessful\n");
	getchar();

	return 0;
}

int CRealTorso::DIOChecker(int ch, int detect_pattern, double angle_range, double max_torque, double time_span, double gain, double *result_angle)
{
	// 引数は
	//    ch : チェックチャンネル
	//    detect_pattern : [-1]　指定角度まで移動　[0] 1検出　[1] 1->0エッジ検出　[2] 1->0 or 0->1エッジ検出
	//    angle_range : 最大検出域
	//    max_torque : 出力トルク [>0] 正方向スイープ　[<0] 負方向スイープ
	//    time_span : スイープ動作にかける時間
	//    gain : P制御ゲイン
	//    result_angle : 検出時角度(実質、戻り値)
	// 戻り値は検出可否 : [0] 強制終了　[1] 検出　[-1] 非検出

	int flag, exit_flag;
	int dio[2]={0,0}, c;
	double start_angle, tmp_angle, TimeSpan, tmp_refD, torque, K, e_range;
	long long int start_time;

	flag = 0;					// スイープ終了フラグ
	exit_flag = -1;				// DIO検出フラグ，初期状態では検出失敗に設定
	TimeSpan = time_span;		// タイムスパンやゲイン設定
	K = gain;

	if(detect_pattern == 1) dio[0] = 1;				// 1->0検出の場合は初期比較用保持DIO配列を1にする
	if(detect_pattern == 2){						// 1->0 0->1検出の場合は，現状のDIOをセット
		dio[0] = GetDIOValue(realLink[ch].ShowDIOChannel());	// DIO取得
	}

	realLink[ch].SetNowAngle(PCICNT.Get(realLink[ch].Motor.ShowCNTChannel()-1));	// エンコーダ取得
	start_angle = realLink[ch].GetNowAngle();										// 開始角度格納
	start_time = MainTimer.Elapsed();												// 開始時間格納

	while(!flag){
		if(kbhit()) {c = getch(); if(c == 'q' || c == '9' ){				// 強制終了処理
			SetTorqueDirect(ch, 0.0);
			return 0;
		}}
		dio[1] = GetDIOValue(realLink[ch].ShowDIOChannel());	// DIO取得
		realLink[ch].SetNowAngle(PCICNT.Get(realLink[ch].Motor.ShowCNTChannel()-1));	// エンコーダ取得
		tmp_angle = realLink[ch].GetNowAngle();
		// P制御、目標角度まで動かす
		if((double)(MainTimer.Elapsed() - start_time)/1000000 < TimeSpan){
			tmp_refD = (angle_range - start_angle)	// テンポラリな参照角を計算
						* (double)(MainTimer.Elapsed() - start_time)/1000000
						/ TimeSpan + start_angle;
		}
		else{
			tmp_refD = angle_range;					// スイープ時間が過ぎたら，最終角度に合わせ続ける
		}

		// 終了判定の角度誤差許容値の設定，直動は許容オーダーが違うため＆腰回転はP制御の欠点を吸収するため
		if(ch == 2)	e_range = 0.001;
		else if(ch == 1 || ch == 0) e_range = 0.05;
		else e_range = 0.01;

		if(abs(tmp_angle - angle_range) < e_range) flag = 1;	// 角度誤差許容値に入ったらスイープ終了

		torque = -K * (tmp_angle - tmp_refD);					// P制御駆動
		if(abs(torque) > abs(max_torque)) torque = max_torque;	// 設定最大トルクは超えないように
		SetTorqueDirect(ch, torque);							// DA出力
		
		switch(detect_pattern){
			case -1 : // 指定角度まで移動するだけ
				if(flag == 1){
					*result_angle = tmp_angle;	// 現在の角度保存(一応)
					exit_flag = 0;				// 検出完了フラグオン
				}
				break;
			case 0 : // 1検出
				if(dio[1] == 1){
					*result_angle = tmp_angle;	// 現在の角度保存
					exit_flag = 1;				// 検出完了フラグオン
					flag = 1;					// 中央値検出の場合は、検出したらすぐとめる
				}
				break;
			case 1 : // 1->0エッジ検出
				if((dio[0] == 1) && (dio[1] == 0)){
					*result_angle = tmp_angle;	// 現在の角度保存
					exit_flag = 1;				// 検出完了フラグオン
				}
				dio[0] = dio[1];				// 一つ前のDIO値を保存
				break;
			case 2 : // 1->0 or 0->1エッジ検出
				if(dio[0] != dio[1]){
					*result_angle = tmp_angle;	// 現在の角度保存
					exit_flag = 1;				// 検出完了フラグオン
				}
				dio[0] = dio[1];				// 一つ前のDIO値を保存
				break;
		}
	}
	SetTorqueDirect(ch, 0.0);						// DA出力停止

	if(exit_flag == 1)			printf("Detection Success！\n");
	else if(exit_flag == 2)		printf("Move successful to the specified position！\n");
	else if(exit_flag == 0)		printf("Forced Termination！\n");
	else if(exit_flag == -1)	printf("Detection Faliure！\n");

	return exit_flag;
}

int CRealTorso::AutoZeroDetect(void)
{
	int ret, ch, i;
	int psig;
	double tmp_angle, init_angle, center_angle, edge_angle[2];
	double torque[Torso_DOF], center[Torso_DOF];

	const double offset_2ch = 7.75/180*PI;		// 090630,修理から帰ってきたらZ相ズレてた

	printf("0 Checkout Error\n");
	printf("Press [q] for detection finish\n");

	torque[0] = 0.3*6.0;
	torque[1] = 0.35*6.0;
	torque[2] = 0.35*3.5;
	torque[3] = 0.04*8.0;
	torque[4] = 0.075*4.0;
	torque[5] = 0.03*2.0;

	for(ch=5; ch>=0; ch--){

	// 各軸共通
		printf("%d axis\n",ch+1);
		psig = GetDIOValue(realLink[ch].ShowDIOChannel());								// 初期DIO読み取り
		realLink[ch].SetNowAngle(PCICNT.Get(realLink[ch].Motor.ShowCNTChannel()-1));
		init_angle = realLink[ch].GetNowAngle();										// 初期角度読み取り

	// 6,4軸
		if(ch == 5 || ch == 3){
			if(psig == 0){
				// 1を探す
				printf("DIO is looking for Position 1...");
				ret = DIOChecker(ch, 0, 15.0/180*PI+init_angle, torque[ch], 1.0, 5.0, &tmp_angle);	// 正方向15°スイープ
				if(ret == 1) center_angle = tmp_angle;	// 見つかれば中央角にする
				else if(ret == -1) {	// 見つからなければ逆方向へ
					ret = DIOChecker(ch, 0, -15.0/180*PI+init_angle, -torque[ch], 2.0, 5.0, &tmp_angle);	// 負方向15°スイープ
					if(ret == 1) center_angle = tmp_angle;		// 見つかれば中央角にする
					else return -1;	// 見つからなければ終了処理
				}
				else if(ret == 0) return -1;	// 強制終了時
			}
			else center_angle = init_angle;				// 現在の角度保存

			// 片側1->0エッジを検出
			printf("DIO is looking for a possitive edge...");
			ret = DIOChecker(ch, 1, 8.0/180*PI+center_angle, torque[ch], 1.0, 5.0, &tmp_angle);	// 正方向15°スイープ
			if(ret == 1) edge_angle[0] = tmp_angle;	// 見つかればエッジ角その1にする
			else return -1;// 終了処理
			printf("DIO is looking for a negative edge...");
			ret = DIOChecker(ch, 1, -8.0/180*PI+center_angle, -torque[ch], 2.0, 5.0, &tmp_angle);	// 負方向15°スイープ
			if(ret == 1) edge_angle[1] = tmp_angle;	// 見つかればエッジ角その2にする
			else return -1;// 終了処理

			center[ch] = (edge_angle[0]+edge_angle[1])/2;	// ゼロ点の格納
		}

	// 5,3軸
		else if(ch == 4 || ch == 2){
			if(psig == 0){
				// 1を探す
				printf("DIO is looking for Position 1...");
				if(ch == 2)			ret = DIOChecker(ch, 0, -0.02+init_angle, -torque[ch], 2.0, 180.0, &tmp_angle);
				else if (ch == 4)	ret = DIOChecker(ch, 0, -55.0/180*PI+init_angle, -torque[ch], 1.0, 7.0, &tmp_angle);
				if(ret == 1) center_angle = tmp_angle;
				else return -1;// 終了処理
			}
			else center_angle = init_angle;				// 現在の角度保存

			// 片側1->0エッジを検出
			printf("DIO is looking for a negative edge...");
			if(ch == 2)	ret = DIOChecker(ch, 1, 0.005+center_angle, torque[ch], 0.75, 240.0, &tmp_angle);
			else if(ch == 4) ret = DIOChecker(ch, 1, 0.0/180*PI, -torque[ch]*0.25, 1.0, 7.0, &tmp_angle);

			if(ret == 1) center[ch] = tmp_angle;	// 見つかればゼロ点にする
			else return -1;// 終了処理

			if(ch == 2){		// 直動軸は初期姿勢に戻るようにする(なぜか戻りきらない？？)
				ret = DIOChecker(ch, -1, 0.0+center_angle, -torque[ch]*1.8, 1.0, 250.0, &tmp_angle);
			}
		}

	// 2軸
		else if(ch == 1){
			printf("DIO is looking for a negative or positive edge...");
			ret = DIOChecker(ch, 2, -45.0/180*PI+init_angle, -torque[ch], 3.5, 8.0, &tmp_angle);
			if(ret == 1) center[ch] = tmp_angle + offset_2ch;	// 見つかればゼロ点にする
			else return -1;// 終了処理

			// 初期角度まで戻る
			ret = DIOChecker(ch, -1, init_angle-5.0*PI/180, torque[ch], 2.0, 8.0, &tmp_angle);
		}

	// 1軸
		else if(ch == 0){
			printf("DIO is looking for a negative or positive edge...");
			ret = DIOChecker(ch, 2, -30.0/180*PI+init_angle, -torque[ch], 2.0, 10.0, &tmp_angle);
			if(ret == 1) center[ch] = tmp_angle;	// 見つかればゼロ点にする
			else if(ret == -1) {	// 見つからなければ逆方向へ
				ret = DIOChecker(ch, 2, 15.0/180*PI+init_angle, torque[ch], 4.0, 10.0, &tmp_angle);
				if(ret == 1) center[ch] = tmp_angle;	// 見つかればゼロ点にする
				else return -1;// 終了処理
			}
			else if(ret == 0) return -1;
		}
		Sleep(300);	// 軸間は微休息
	}
	// 検出終了
	printf("Zero position inspection exit\n");
	for(i=0; i<Torso_DOF; i++){
		printf("%d:%f\n",i,center[i]*180/PI);
		realLink[i].SetAngleOffset(center[i]);
	}
	printf("Angle offset set completed\n");

	return 0;
}

void CRealTorso::SetInitialAngles(double *angles)
{
	for(int i=0; i<Torso_DOF; i++)
		Initial_Angles[i] = angles[i];
}

void CRealTorso::GetInitialAngles(double *angles)
{
	for(int i=0; i<Torso_DOF; i++)
		angles[i] = Initial_Angles[i];
}

void CRealTorso::SendToGLprogram(void)
{
	int i;

#ifdef GLOUTPUT
	for(i=0; i<Torso_DOF; i++){
		GLOutFRec.sender.master_angle[i] = realLink[i].GetTargetAngle();
		GLOutFRec.sender.slave_angle[i] = realLink[i].GetNowAngle();
	}
	GLOutFRec.Send();
#endif

}

void CRealTorso::ClearParameter(void)
{
	for(int i=0;i<Torso_DOF;i++)
		realLink[i].ClearParameter();
}