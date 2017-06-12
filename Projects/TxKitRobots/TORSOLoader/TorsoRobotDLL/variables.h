#ifndef __VARIABLES
#define __VARIABLES

#include <stdio.h>
#include <iostream>
using namespace std;

#include "Interfaceboard.h"
extern InterfaceDABoard  PCIDA;			// �C���^�t�F�[�XDA�{�[�h�N���X(����`)
extern InterfaceCNTBoard PCICNT;		// �C���^�t�F�[�X�J�E���^�{�[�h�N���X(����`)
extern InterfaceDIOBoard PCIDIO;		// �C���^�t�F�[�XDIO�{�[�h�N���X(����`)

#include "CPUTimer.h"
extern CPUTimer MainTimer;				// CPU�^�C�}�N���X(����`)

extern FILE *fp_torso;					// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�g���\�N���X�p)
extern FILE *fp_link;					// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�����N�N���X�p)
extern FILE *fp_routine;				// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�T�u���[�`���p)
extern FILE *fp_board;					// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�C���^�t�F�[�X�{�[�h�p)

#include "RealTorso.h"
extern CRealTorso	realTorso;			// �g���\�I�u�W�F�N�g


extern int LoadData(ifstream& data_file, char partition,...);
extern int FirstMoving(void);
extern int FinishMoving(void);
extern int MainControl_task(int CalibSelect);
extern int Debug_task(void);

#endif