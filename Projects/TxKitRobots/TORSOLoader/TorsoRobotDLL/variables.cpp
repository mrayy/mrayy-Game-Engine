#include "stdafx.h"
#include "variables.h"

InterfaceDABoard	PCIDA("FBIDA1");		// �C���^�t�F�[�XDA�{�[�h�N���X(����`)
InterfaceCNTBoard	PCICNT("FBIPENC17", 8);	// �C���^�t�F�[�X�J�E���^�{�[�h�N���X(����`)
InterfaceDIOBoard	PCIDIO("FBIDIO1");		// �C���^�t�F�[�XDIO�{�[�h�N���X(����`)

CPUTimer			MainTimer;			// CPU�^�C�}�N���X(����`)

FILE				*fp_torso;			// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�g���\�N���X�p)
FILE				*fp_link;			// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�����N�N���X�p)
FILE				*fp_routine;		// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�T�u���[�`���p)
FILE				*fp_board;			// �f�[�^�`�F�b�N�p�t�@�C���n���h��(�C���^�t�F�[�X�{�[�h�p)

CRealTorso			realTorso;			// �g���\�I�u�W�F�N�g
