#ifndef		_TEXARTNCORDLIB_H_
#define		_TEXARTNCORDLIB_H_
//*******************************************/
//*    include								*/
//*******************************************/
#include	<windows.h>
#include	<stdio.h>
#ifndef	LIBLARY
#include	"TexARTNCorddriverif.h"
#endif

//*******************************************/
//*    type									*/
//*******************************************/
//*******************************************/
//*    define								*/
//*******************************************/
//�֐��߂�l
#define	NCORD_NORMAL		(0)		//����I��
#define	NCORD_ERR_PARAM		(-100)	//�p�����[�^�G���[
#define	NCORD_ERR_COMMAND	(-101)	//�R�}���h����M�G���[
#define	NCORD_ERR_GETLOG	(-102)	//���O����M�G���[
#define	NCORD_ERR_REG_WRITE	(-103)	//���W�X�^���C�g�G���[
#define	NCORD_ERR_REG_READ	(-104)	//���W�X�^���[�h�G���[

#define	NCord_DEVERR		(-1)	// �f�o�C�X�n���h�����ُ�
#define	NCord_PARAERR1		(-10)	// �p�����[�^�t�@�C�����݂���Ȃ�
//#define	NCord_PARAERR2		(-11)	// �p�����[�^�t�@�C���̓��e���s��
#define	NCord_GAINERR		(-12)	// ���Q�C���̑��M�G���[
#define	NCord_STARTERR		(-13)	// �V�X�e���̋N�����s
#define	NCord_DRVERR		(-14)	// �h���C�o�R�}���h�̑��M�G���[
#define	NCord_VALUEERR		(-15)	// �R�}���h�ɑ΂��鉞����M�G���[
#define	NCord_RSTERR		(-16)	// ���Z�b�g�G���[
#define	NCord_CNTERR		(-17)	// �J�E���^�ݒ�G���[
#define	NCord_FRERR			(-19)	// �ҋ@��ԑJ�ڎ��s

// ���[�^�h���C�o��� '09.09.18
#define		NCORD_MOTER_TYTPE01	0x01			// Hibot�� 1AxisDCPower  Module �i10A Module�j 
#define		NCORD_MOTER_TYTPE02	0x02			// Hibot�� 3Axes DC Power Module �i5A�~3 Module�j
#define		NCORD_MOTER_TYTPE80	0x80			// Allegro�� A3995 Driver �iVolt Reference Control�j

#define		NCORD_RSLT_OK		0x0000			// ����I��
#define		NCORD_RSLT_NG		0x0001			// �ُ�I��

#define		NCORD_DRV_CNT		16				// �R�}���h���X�|���X �\���̂Ŏg�p
#define		NCORD_RSV1			3				// �R�}���h���X�|���X �\���̂Ŏg�p
#define		NCORD_AD_CNT		24				// �R�}���h���X�|���X �\���̂Ŏg�p
#define		NCORD_NUM_CNT		4				// �R�}���h���X�|���X �\���̂Ŏg�p
//*******************************************/
//*    struct								*/
//*******************************************/
typedef	struct
{
	unsigned short		cmnd;				// �R�}���h
	unsigned short		trm;				// �^�[�~�i���ԍ�
	unsigned short		cnt;				// ���Z�b�g����J�E���^�ԍ�(�J�E���^���Z�b�g�R�}���h���g�p)
//	unsigned long		val;				// �J�E���^�ɐݒ肷��l(�J�E���^���Z�b�g�R�}���h���g�p)
	unsigned short		valL;				// �J�E���^�ɐݒ肷��l(�J�E���^���Z�b�g�R�}���h���g�p)
	unsigned short		valH;				// �J�E���^�ɐݒ肷��l(�J�E���^���Z�b�g�R�}���h���g�p)
	unsigned short		rsv1[NCORD_RSV1];	// ��
	unsigned short		drv[NCORD_DRV_CNT];	// ���[�^�h���C�oX�ɑ΂���h���C�o�R�}���h���̓Q�C���l
	unsigned char		moter[NCORD_DRV_CNT];	// ���[�^�h���C�o0�`F�ɑ΂��郂�[�^�h���C�o���(1Byte)�B
}NCORD_CMND,*PNCORD_CMND;
typedef	struct
{
	unsigned short		resp;				// ���X�|���X
	unsigned short		trm;				// �^�[�~�i���ԍ�
	unsigned short		rslt;				// ����
	unsigned short		cnt_bit;			// �J�E���^�l���4�r�b�g �~4�J�E���^�� '09.09.24 M.S
	unsigned short		cnt[NCORD_NUM_CNT];	// ���[�^�h���C�oJA1�`JA4�݂̂Ŏg�p����J�E���^�l
	unsigned short		ad[NCORD_AD_CNT];	// ���[�^�h���C�oJA1�`JA7/JH1�`JH16�Ŏg�p����|�e���V�����[�^AD�l(16ch��)
											// ���[�^�h���C�oJH1�`JH16�Ŏg�p����̓Z���TAD�l(8ch��)
}NCORD_RESP,*PNCORD_RESP;
typedef union
{
	NCORD_CMND	cmnd;
	NCORD_RESP	resp;
}NCORD_CMND_RESP,*PNCORD_CMND_RESP;
//*******************************************/
//*    function								*/
//*******************************************/
#ifdef __cplusplus
extern "C" {
#endif
extern	HANDLE
NCord_OpenDevice(
	int						nDevNum
);
extern	void
NCord_CloseDevice(
	HANDLE					hDev
);
extern	int
NCord_Command(
	HANDLE					hDev,
	BYTE					*inBuffer,
	int						insize,
	BYTE					*outbuffer,
	int						outsize,
	unsigned long			*recv,
	int						flag
);
extern	int
NCord_GetLog(
	HANDLE					hDev,
	BYTE					*buffer,
	int						size
);
extern	int
NCord_WriteRegister(
	HANDLE					hDev,
	DWORD					offset,
	DWORD					value
);
extern	int
NCord_ReadRegister(
	HANDLE					hDev,
	DWORD					offset,
	DWORD*					pvalue
);

#ifdef __cplusplus
}
#endif

#endif
