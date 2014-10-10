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
	// �����p�����[�^�A��̓����悤�ɐݒ肳��Ă���
	// �y���Ӂz������ς��Ă��l�͕ς��Ȃ��AParameters.txt��ς��鎖�I
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
	// �e�֐߃p�����[�^�t�@�C���̏���
	Param = new ifstream();
	Param->open(fname);
	if(!*Param){
		printf("Flie Open Error %s \n",fname);
		return -1;
	}

	while(Param->peek() != EOF){
		if(Param->peek() == '/'){		Param->get(temp, 200); Param->get(ctemp);}	// �R�����g���Ɖ��s
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

//----- �ȍ~�A�e�A�[���T�u�Z�b�g�̏�����
	printf("Link & Joint initialize...");
	for(i=0; i<Torso_DOF; i++){
		realLink[i].Initialize(data[0][i]);
		realLink[i].Motor.Initialize(data[1][i]);
	}
	printf("Finish\n");


//----- interfaceboard -----
//#if USE_INTERFACE
	int DAChArray[6], DAChNum;

// �{�[�h�������̂��߂̃`�����l�����擾 
// �W���C���g01���珇�Ƀ`�����l���ԍ���ChArray�Ɋi�[
	for(i=0; i<Torso_DOF; i++)	DAChArray[i] = realLink[i].Motor.ShowDAChannel();
	DAChNum = Torso_DOF;					// �`�����l�����̊i�[

// interfaceboard�N���X�̏�����
	printf("Interface DA Board Initialize...\n");
	PCIDA.initialize(DAChArray,DAChNum,DA_10V);
	printf("Interface DA Board Initialize Finished\n");

//---------- �G���R�[�_�I�t�Z�b�g�ݒ� ---------------
	SetCountOffset();

//#endif


// [Change] ����̓N���X�̊O�ɏo��
//----- OpenGL�o�͗p���L�t�@�C���̃I�[�v��         -----
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
 * �ڕW�s�����
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
	///�����̊֐����A���Ԃ�E��n�ł̌v�Z������A�}�C�i�X����
	///��
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
	// �Y�������`�����l�����������̂��W���C���g���ɐ���
	for (i=0; i<Torso_DOF; i++){
		tmpCount[i] = read[realLink[i].Motor.ShowCNTChannel()-1];
		realLink[i].SetNowAngle(tmpCount[i]);  // �e�����N�̃J�E���^�l���p�x�ɕϊ����đ��
	}

}

void CRealTorso::SetCountOffset(void)
{
	DWORD MeanCount;

	MeanCount = realLink[0].Motor.ShowMeanCount();

	PCICNT.SetCount(MeanCount);
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
	// �d�͕⏞
	tmp_torque[0] = torque[0] - M*9.8*(L1+realLink[2].GetNowAngle())*sin(realLink[0].GetNowAngle());
	tmp_torque[1] = torque[1] - M*9.8*(L1+realLink[2].GetNowAngle())*sin(realLink[1].GetNowAngle());
	if(torque[0] == 0) tmp_torque[0] = 0.0;
	if(torque[1] == 0) tmp_torque[1] = 0.0;
	torque[0] = tmp_torque[0];
	torque[1] = tmp_torque[1];

	for(int i=0; i<Torso_DOF; i++) write[i] = realLink[i].Motor.CalcTorqueToDAData(torque[i]);

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

	const double offset_2ch = 7.75/180*PI;		// 090630,�C������A���Ă�����Z���Y���Ă�

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
			realLink[channel-1].SetNowAngle(buff_count[i]);  // �e�����N�̃J�E���^�l���p�x�ɕϊ�
			center[channel-1] += realLink[channel-1].GetNowAngle();
			printf("%f,",realLink[channel-1].GetNowAngle()*180/PI);
		}
		printf("\n");

		center[channel-1] /= snum;

		if(channel == 2) center[channel-1] += offset_2ch;	// ��2����Z��������Ă��邽��

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
	// ������
	//    ch : �`�F�b�N�`�����l��
	//    detect_pattern : [-1]�@�w��p�x�܂ňړ��@[0] 1���o�@[1] 1->0�G�b�W���o�@[2] 1->0 or 0->1�G�b�W���o
	//    angle_range : �ő匟�o��
	//    max_torque : �o�̓g���N [>0] �������X�C�[�v�@[<0] �������X�C�[�v
	//    time_span : �X�C�[�v����ɂ����鎞��
	//    gain : P����Q�C��
	//    result_angle : ���o���p�x(�����A�߂�l)
	// �߂�l�͌��o�� : [0] �����I���@[1] ���o�@[-1] �񌟏o

	int flag, exit_flag;
	int dio[2]={0,0}, c;
	double start_angle, tmp_angle, TimeSpan, tmp_refD, torque, K, e_range;
	long long int start_time;

	flag = 0;					// �X�C�[�v�I���t���O
	exit_flag = -1;				// DIO���o�t���O�C������Ԃł͌��o���s�ɐݒ�
	TimeSpan = time_span;		// �^�C���X�p����Q�C���ݒ�
	K = gain;

	if(detect_pattern == 1) dio[0] = 1;				// 1->0���o�̏ꍇ�͏�����r�p�ێ�DIO�z���1�ɂ���
	if(detect_pattern == 2){						// 1->0 0->1���o�̏ꍇ�́C�����DIO���Z�b�g
		dio[0] = GetDIOValue(realLink[ch].ShowDIOChannel());	// DIO�擾
	}

	realLink[ch].SetNowAngle(PCICNT.Get(realLink[ch].Motor.ShowCNTChannel()-1));	// �G���R�[�_�擾
	start_angle = realLink[ch].GetNowAngle();										// �J�n�p�x�i�[
	start_time = MainTimer.Elapsed();												// �J�n���Ԋi�[

	while(!flag){
		if(kbhit()) {c = getch(); if(c == 'q' || c == '9' ){				// �����I������
			SetTorqueDirect(ch, 0.0);
			return 0;
		}}
		dio[1] = GetDIOValue(realLink[ch].ShowDIOChannel());	// DIO�擾
		realLink[ch].SetNowAngle(PCICNT.Get(realLink[ch].Motor.ShowCNTChannel()-1));	// �G���R�[�_�擾
		tmp_angle = realLink[ch].GetNowAngle();
		// P����A�ڕW�p�x�܂œ�����
		if((double)(MainTimer.Elapsed() - start_time)/1000000 < TimeSpan){
			tmp_refD = (angle_range - start_angle)	// �e���|�����ȎQ�Ɗp���v�Z
						* (double)(MainTimer.Elapsed() - start_time)/1000000
						/ TimeSpan + start_angle;
		}
		else{
			tmp_refD = angle_range;					// �X�C�[�v���Ԃ��߂�����C�ŏI�p�x�ɍ��킹������
		}

		// �I������̊p�x�덷���e�l�̐ݒ�C�����͋��e�I�[�_�[���Ⴄ���߁�����]��P����̌��_���z�����邽��
		if(ch == 2)	e_range = 0.001;
		else if(ch == 1 || ch == 0) e_range = 0.05;
		else e_range = 0.01;

		if(abs(tmp_angle - angle_range) < e_range) flag = 1;	// �p�x�덷���e�l�ɓ�������X�C�[�v�I��

		torque = -K * (tmp_angle - tmp_refD);					// P����쓮
		if(abs(torque) > abs(max_torque)) torque = max_torque;	// �ݒ�ő�g���N�͒����Ȃ��悤��
		SetTorqueDirect(ch, torque);							// DA�o��
		
		switch(detect_pattern){
			case -1 : // �w��p�x�܂ňړ����邾��
				if(flag == 1){
					*result_angle = tmp_angle;	// ���݂̊p�x�ۑ�(�ꉞ)
					exit_flag = 0;				// ���o�����t���O�I��
				}
				break;
			case 0 : // 1���o
				if(dio[1] == 1){
					*result_angle = tmp_angle;	// ���݂̊p�x�ۑ�
					exit_flag = 1;				// ���o�����t���O�I��
					flag = 1;					// �����l���o�̏ꍇ�́A���o�����炷���Ƃ߂�
				}
				break;
			case 1 : // 1->0�G�b�W���o
				if((dio[0] == 1) && (dio[1] == 0)){
					*result_angle = tmp_angle;	// ���݂̊p�x�ۑ�
					exit_flag = 1;				// ���o�����t���O�I��
				}
				dio[0] = dio[1];				// ��O��DIO�l��ۑ�
				break;
			case 2 : // 1->0 or 0->1�G�b�W���o
				if(dio[0] != dio[1]){
					*result_angle = tmp_angle;	// ���݂̊p�x�ۑ�
					exit_flag = 1;				// ���o�����t���O�I��
				}
				dio[0] = dio[1];				// ��O��DIO�l��ۑ�
				break;
		}
	}
	SetTorqueDirect(ch, 0.0);						// DA�o�͒�~

	if(exit_flag == 1)			printf("Detection Success�I\n");
	else if(exit_flag == 2)		printf("Move successful to the specified position�I\n");
	else if(exit_flag == 0)		printf("Forced Termination�I\n");
	else if(exit_flag == -1)	printf("Detection Faliure�I\n");

	return exit_flag;
}

int CRealTorso::AutoZeroDetect(void)
{
	int ret, ch, i;
	int psig;
	double tmp_angle, init_angle, center_angle, edge_angle[2];
	double torque[Torso_DOF], center[Torso_DOF];

	const double offset_2ch = 7.75/180*PI;		// 090630,�C������A���Ă�����Z���Y���Ă�

	printf("0 Checkout Error\n");
	printf("Press [q] for detection finish\n");

	torque[0] = 0.3*6.0;
	torque[1] = 0.35*6.0;
	torque[2] = 0.35*3.5;
	torque[3] = 0.04*8.0;
	torque[4] = 0.075*4.0;
	torque[5] = 0.03*2.0;

	for(ch=5; ch>=0; ch--){

	// �e������
		printf("%d axis\n",ch+1);
		psig = GetDIOValue(realLink[ch].ShowDIOChannel());								// ����DIO�ǂݎ��
		realLink[ch].SetNowAngle(PCICNT.Get(realLink[ch].Motor.ShowCNTChannel()-1));
		init_angle = realLink[ch].GetNowAngle();										// �����p�x�ǂݎ��

	// 6,4��
		if(ch == 5 || ch == 3){
			if(psig == 0){
				// 1��T��
				printf("DIO is looking for Position 1...");
				ret = DIOChecker(ch, 0, 15.0/180*PI+init_angle, torque[ch], 1.0, 5.0, &tmp_angle);	// ������15���X�C�[�v
				if(ret == 1) center_angle = tmp_angle;	// ������Β����p�ɂ���
				else if(ret == -1) {	// ������Ȃ���΋t������
					ret = DIOChecker(ch, 0, -15.0/180*PI+init_angle, -torque[ch], 2.0, 5.0, &tmp_angle);	// ������15���X�C�[�v
					if(ret == 1) center_angle = tmp_angle;		// ������Β����p�ɂ���
					else return -1;	// ������Ȃ���ΏI������
				}
				else if(ret == 0) return -1;	// �����I����
			}
			else center_angle = init_angle;				// ���݂̊p�x�ۑ�

			// �Б�1->0�G�b�W�����o
			printf("DIO is looking for a possitive edge...");
			ret = DIOChecker(ch, 1, 8.0/180*PI+center_angle, torque[ch], 1.0, 5.0, &tmp_angle);	// ������15���X�C�[�v
			if(ret == 1) edge_angle[0] = tmp_angle;	// ������΃G�b�W�p����1�ɂ���
			else return -1;// �I������
			printf("DIO is looking for a negative edge...");
			ret = DIOChecker(ch, 1, -8.0/180*PI+center_angle, -torque[ch], 2.0, 5.0, &tmp_angle);	// ������15���X�C�[�v
			if(ret == 1) edge_angle[1] = tmp_angle;	// ������΃G�b�W�p����2�ɂ���
			else return -1;// �I������

			center[ch] = (edge_angle[0]+edge_angle[1])/2;	// �[���_�̊i�[
		}

	// 5,3��
		else if(ch == 4 || ch == 2){
			if(psig == 0){
				// 1��T��
				printf("DIO is looking for Position 1...");
				if(ch == 2)			ret = DIOChecker(ch, 0, -0.02+init_angle, -torque[ch], 2.0, 180.0, &tmp_angle);
				else if (ch == 4)	ret = DIOChecker(ch, 0, -55.0/180*PI+init_angle, -torque[ch], 1.0, 7.0, &tmp_angle);
				if(ret == 1) center_angle = tmp_angle;
				else return -1;// �I������
			}
			else center_angle = init_angle;				// ���݂̊p�x�ۑ�

			// �Б�1->0�G�b�W�����o
			printf("DIO is looking for a negative edge...");
			if(ch == 2)	ret = DIOChecker(ch, 1, 0.005+center_angle, torque[ch], 0.75, 240.0, &tmp_angle);
			else if(ch == 4) ret = DIOChecker(ch, 1, 0.0/180*PI, -torque[ch]*0.25, 1.0, 7.0, &tmp_angle);

			if(ret == 1) center[ch] = tmp_angle;	// ������΃[���_�ɂ���
			else return -1;// �I������

			if(ch == 2){		// �������͏����p���ɖ߂�悤�ɂ���(�Ȃ����߂肫��Ȃ��H�H)
				ret = DIOChecker(ch, -1, 0.0+center_angle, -torque[ch]*1.8, 1.0, 250.0, &tmp_angle);
			}
		}

	// 2��
		else if(ch == 1){
			printf("DIO is looking for a negative or positive edge...");
			ret = DIOChecker(ch, 2, -45.0/180*PI+init_angle, -torque[ch], 3.5, 8.0, &tmp_angle);
			if(ret == 1) center[ch] = tmp_angle + offset_2ch;	// ������΃[���_�ɂ���
			else return -1;// �I������

			// �����p�x�܂Ŗ߂�
			ret = DIOChecker(ch, -1, init_angle-5.0*PI/180, torque[ch], 2.0, 8.0, &tmp_angle);
		}

	// 1��
		else if(ch == 0){
			printf("DIO is looking for a negative or positive edge...");
			ret = DIOChecker(ch, 2, -30.0/180*PI+init_angle, -torque[ch], 2.0, 10.0, &tmp_angle);
			if(ret == 1) center[ch] = tmp_angle;	// ������΃[���_�ɂ���
			else if(ret == -1) {	// ������Ȃ���΋t������
				ret = DIOChecker(ch, 2, 15.0/180*PI+init_angle, torque[ch], 4.0, 10.0, &tmp_angle);
				if(ret == 1) center[ch] = tmp_angle;	// ������΃[���_�ɂ���
				else return -1;// �I������
			}
			else if(ret == 0) return -1;
		}
		Sleep(300);	// ���Ԃ͔��x��
	}
	// ���o�I��
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