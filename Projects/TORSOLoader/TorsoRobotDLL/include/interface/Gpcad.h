// -----------------------------------------------------------------------
//		
//		�w�b�_�t�@�C��
//
//		File Name	:	GpcAd.h
//
//		Ver 1.02
//
//									Copyright (C) 1999-2001 Interface Corp.
// -----------------------------------------------------------------------

#if !defined( _FbiAd_H_ )
#define _FbiAd_H_

#ifdef __cplusplus
extern	"C" {
#endif

//-----------------------------------------------------------------------------------------------
//
//		�����������ʎq
//
//-----------------------------------------------------------------------------------------------
#define FLAG_SYNC	1	// �����ŃT���v�����O����
#define FLAG_ASYNC	2	// �񓯊��ŃT���v�����O����

//-----------------------------------------------------------------------------------------------
//
//		�t�@�C���`�����ʎq
//
//-----------------------------------------------------------------------------------------------
#define FLAG_BIN	1	// �o�C�i���`���t�@�C��
#define FLAG_CSV	2	// �b�r�u�`���t�@�C��

//-----------------------------------------------------------------------------------------------
//
//		�T���v�����O��Ԏ��ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_STATUS_STOP_SAMPLING		1	// �T���v�����O�͒�~���Ă��܂�
#define AD_STATUS_WAIT_TRIGGER		2	// �T���v�����O�̓g���K�҂���Ԃł�
#define AD_STATUS_NOW_SAMPLING		3	// �T���v�����O���쒆�ł�

//-----------------------------------------------------------------------------------------------
//
//		�C�x���g�v�����ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_EVENT_SMPLNUM			1	// �w�茏���C�x���g
#define AD_EVENT_STOP_TRIGGER		2	// �g���K�ɂ��T���v�����O��~
#define AD_EVENT_STOP_FUNCTION		3	// �֐��ɂ��T���v�����O��~
#define AD_EVENT_STOP_TIMEOUT		4	// TIMEOUT�ɂ��T���v�����O��~
#define AD_EVENT_STOP_SAMPLING		5	// �T���v�����O�I��
#define	AD_EVENT_STOP_SCER			6	// �T���v�����O�N���b�N�G���[�ɂ��T���v�����O��~
#define	AD_EVENT_STOP_ORER			7	// �I�[�o�����G���[�ɂ��T���v�����O��~
#define	AD_EVENT_SCER				8	// �T���v�����O�N���b�N�G���[
#define	AD_EVENT_ORER				9	// �I�[�o�����G���[
#define AD_EVENT_STOP_LV_1			10	// PCI-3179�`�����l���P�T���v�����O�I��
#define AD_EVENT_STOP_LV_2			11	// PCI-3179�`�����l���Q�T���v�����O�I��
#define AD_EVENT_STOP_LV_3			12	// PCI-3179�`�����l���R�T���v�����O�I��
#define AD_EVENT_STOP_LV_4			13	// PCI-3179�`�����l���S�T���v�����O�I��

//-----------------------------------------------------------------------------------------------
//
//		���͎d�l���ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_INPUT_SINGLE				1	// �V���O���G���h����
#define AD_INPUT_DIFF				2	// ��������

//-----------------------------------------------------------------------------------------------
//
//		�����u�q���ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_ADJUST_BIOFFSET			1	// �o�C�|�[���I�t�Z�b�g����
#define AD_ADJUST_UNIOFFSET			2	// ���j�|�[���I�t�Z�b�g����
#define AD_ADJUST_BIGAIN			3	// �o�C�|�[���Q�C������
#define AD_ADJUST_UNIGAIN			4	// ���j�|�[���Q�C������

//-----------------------------------------------------------------------------------------------
//
//		�������쎯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_ADJUST_UP				1	// �A�b�v
#define AD_ADJUST_DOWN				2	// �_�E��
#define AD_ADJUST_STORE				3	// �X�g�A����
#define AD_ADJUST_STANDBY			4	// �X�^���o�C
#define AD_ADJUST_NOT_STORE			5	// �X�g�A���Ȃ�

//-----------------------------------------------------------------------------------------------
//
//		�f�[�^���ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_DATA_PHYSICAL			1	// ������(�d��[V]/�d��[mA])
#define AD_DATA_BIN8				2	// 8bit�o�C�i��
#define AD_DATA_BIN12				3	// 12bit�o�C�i��
#define AD_DATA_BIN16				4	// 16bit�o�C�i��
#define AD_DATA_BIN24				5	// 24bit�o�C�i��

//-----------------------------------------------------------------------------------------------
//
//		�f�[�^�ϊ����ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_CONV_SMOOTH				1	// �f�[�^�ɑ΂��A�X���[�W���O�ɂ��ϊ����s���܂��B
#define AD_CONV_AVERAGE1		0x100	// �f�[�^�ɑ΂��A�P�����ςɂ��ϊ����s���܂��B
#define AD_CONV_AVERAGE2		0x200	// �f�[�^�ɑ΂��A�ړ����ςɂ��ϊ����s���܂��B

//-----------------------------------------------------------------------------------------------
//
//		�T���v�����O�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_IO_SAMPLING				1	// I/O����
#define AD_FIFO_SAMPLING			2	// FIFO����
#define AD_MEM_SAMPLING				4	// ����������
#define	AD_BM_SAMPLING				8	// �o�X�}�X�^����

//-----------------------------------------------------------------------------------------------
//
//		�g���K�|�C���g�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_TRIG_START				1	// �X�^�[�g�g���K(�f�t�H���g)
#define AD_TRIG_STOP				2	// �X�g�b�v�g���K
#define AD_TRIG_START_STOP			3	// �X�^�[�g�X�g�b�v�g���K

//-----------------------------------------------------------------------------------------------
//
//		�g���K�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_FREERUN					1	// �g���K�Ȃ�(�f�t�H���g)
#define AD_EXTTRG					2	// �O���g���K
#define AD_EXTTRG_DI				3	// �O��+DI�g���K
#define AD_LEVEL_P					4	// ���x���g���K�v���X
#define AD_LEVEL_M					5	// ���x���g���K�}�C�i�X
#define AD_LEVEL_D					6	// ���x���g���K�f���A��
#define AD_INRANGE					7	// ���x���g���K�C�������W
#define AD_OUTRANGE					8	// ���x���g���K�A�E�g�����W
#define AD_ETERNITY					9	// �����T���v�����O
#define AD_SMPLNUM					10	// �w�茏��
#define AD_START_P1			0x00000010	// �X�^�[�g�g���K ���x���P�����オ��
#define AD_START_M1			0x00000020	// �X�^�[�g�g���K ���x���P����������
#define AD_START_D1			0x00000040	// �X�^�[�g�g���K ���x���P�����オ��܂��͗���������
#define AD_START_P2			0x00000080	// �X�^�[�g�g���K ���x���Q�����オ��
#define AD_START_M2			0x00000100	// �X�^�[�g�g���K ���x���Q����������
#define AD_START_D2			0x00000200	// �X�^�[�g�g���K ���x���Q�����オ��܂��͗���������
#define AD_STOP_P1			0x00000400	// �X�g�b�v�g���K ���x���P�����オ��
#define AD_STOP_M1			0x00000800	// �X�g�b�v�g���K ���x���P����������
#define AD_STOP_D1			0x00001000	// �X�g�b�v�g���K ���x���P�����オ��܂��͗���������
#define AD_STOP_P2			0x00002000	// �X�g�b�v�g���K ���x���Q�����オ��
#define AD_STOP_M2			0x00004000	// �X�g�b�v�g���K ���x���Q����������
#define AD_STOP_D2			0x00008000	// �X�g�b�v�g���K ���x���Q�����オ��܂��͗���������
#define AD_ANALOG_FILTER	0x00010000	// �A�i���O�g���K�t�B���^���g�p����

//-----------------------------------------------------------------------------------------------
//
//		�ɐ��w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_DOWN_EDGE				1	// ����������G�b�W(�f�t�H���g)
#define AD_UP_EDGE					2	// �����オ��G�b�W
#define AD_EXTRG_IN					3	// �O���g���K����
#define AD_EXCLK_IN					4	// �O���N���b�N����

#define AD_EDGE_P1				0x0010	// ���x���P����������G�b�W
#define AD_EDGE_M1				0x0020	// ���x���P�����オ��G�b�W
#define AD_EDGE_D1				0x0040	// ���x���P���x���P�����オ��܂��͗���������
#define AD_EDGE_P2				0x0080	// ���x���Q����������G�b�W
#define AD_EDGE_M2				0x0100	// ���x���Q�����オ��G�b�W
#define AD_EDGE_D2				0x0200	// ���x���Q���x���P�����オ��܂��͗���������
#define	AD_DISABLE			0x80000000	// �����i�g���K�p���X���������j

//-----------------------------------------------------------------------------------------------
//
//		�p���X�ɐ��w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_LOW_PULSE				1	// LOW�p���X(�f�t�H���g)
#define AD_HIGH_PULSE				2	// HIGH�p���X

//-----------------------------------------------------------------------------------------------
//
//		�{�����[�h�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_NORMAL_MODE				1	// �ʏ탂�[�h�Ŏg�p����i���Ή��{�[�h�j
#define AD_FAST_MODE				2	// �{�����[�h�Ŏg�p����

//-----------------------------------------------------------------------------------------------
//
//		�X�e�[�^�X�t���w�莯�ʎq�i�o�X�}�X�^�����j
//
//-----------------------------------------------------------------------------------------------
#define AD_NO_STATUS				1	// �X�e�[�^�X�Ȃ��i�f�t�H���g�j
#define AD_ADD_STATUS				2	// �X�e�[�^�X����

//-----------------------------------------------------------------------------------------------
//
//		�G���[����w�莯�ʎq�i�o�X�}�X�^�����j
//
//-----------------------------------------------------------------------------------------------
#define AD_STOP_SCER				2	// �X�L�����N���b�N�G���[�Œ�~
#define AD_STOP_ORER				4	// �I�[�o�����G���[�Œ�~

//-----------------------------------------------------------------------------------------------
//
//		�T���v�����O�f�[�^�ۑ��w�莯�ʎq�i�o�X�}�X�^�����j
//
//-----------------------------------------------------------------------------------------------
#define AD_APPEND					1	// �ǉ���������
#define AD_OVERWRITE				2	// �㏑����������

//-----------------------------------------------------------------------------------------------
//
//		�����W�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_0_1V				0x00000001	// �d���@���j�|�[��0�`1V
#define AD_0_2P5V			0x00000002	// �d���@���j�|�[��0�`2.5V
#define AD_0_5V				0x00000004	// �d���@���j�|�[��0�`5V
#define AD_0_10V			0x00000008	// �d���@���j�|�[��0�`10V
#define AD_1_5V				0x00000010	// �d���@���j�|�[��1�`5V
#define AD_0_2V				0x00000020	// �d���@���j�|�[��0�`2V
#define AD_0_0P125V			0x00000040	// �d���@���j�|�[��0�`0.125V
#define AD_0_1P25V			0x00000080	// �d���@���j�|�[��0�`1.25v
#define AD_0_0P625V			0x00000100	// �d���@���j�|�[��0�`0.625V
#define AD_0_20mA			0x00001000	// �d���@���j�|�[��0�`20mA
#define AD_4_20mA			0x00002000	// �d���@���j�|�[��4�`20mA
#define AD_1V				0x00010000	// �d���@�o�C�|�[�� �}1V
#define AD_2P5V				0x00020000	// �d���@�o�C�|�[�� �}2.5V
#define AD_5V				0x00040000	// �d���@�o�C�|�[�� �}5V
#define AD_10V				0x00080000	// �d���@�o�C�|�[�� �}10V
#define AD_0P125V			0x00400000	// �d���@�o�C�|�[�� �}0.125V
#define AD_1P25V			0x00800000	// �d���@�o�C�|�[�� �}1.25V
#define AD_0P625V			0x01000000	// �d���@�o�C�|�[�� �}0.625V

//-----------------------------------------------------------------------------------------------
//
//		�≏�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_ISOLATION				1	// �≏�{�[�h
#define AD_NOT_ISOLATION			2	// ��≏�{�[�h

//-----------------------------------------------------------------------------------------------
//
//		�����������T���v�����O���[�h�w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_MASTER_MODE				1	// �}�X�^���[�h
#define AD_SLAVE_MODE				2	// �X���[�u���[�h

//-----------------------------------------------------------------------------------------------
//
//		�r���w�莯�ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_SELF_CALIBRATION			1	// ���Ȋr��
#define AD_ZEROSCALE_CALIBRATION	2	// �[���X�P�[���r��
#define AD_FULLSCALE_CALIBRATION	3	// �t���X�P�[���r��

//-----------------------------------------------------------------------------------------------
//
//		�G���[���ʎq
//
//-----------------------------------------------------------------------------------------------
#define AD_ERROR_SUCCESS						0x00000000
#define AD_ERROR_NOT_DEVICE						0xC0000001
#define AD_ERROR_NOT_OPEN						0xC0000002
#define AD_ERROR_INVALID_HANDLE					0xC0000003
#define AD_ERROR_ALREADY_OPEN					0xC0000004
#define AD_ERROR_NOT_SUPPORTED					0xC0000009
#define AD_ERROR_NOW_SAMPLING					0xC0001001
#define AD_ERROR_STOP_SAMPLING					0xC0001002
#define AD_ERROR_START_SAMPLING					0xC0001003
#define AD_ERROR_SAMPLING_TIMEOUT				0xC0001004
#define AD_ERROR_INVALID_PARAMETER				0xC0001021
#define AD_ERROR_ILLEGAL_PARAMETER				0xC0001022
#define AD_ERROR_NULL_POINTER					0xC0001023
#define AD_ERROR_GET_DATA						0xC0001024
#define AD_ERROR_FILE_OPEN						0xC0001041
#define AD_ERROR_FILE_CLOSE						0xC0001042
#define AD_ERROR_FILE_READ						0xC0001043
#define AD_ERROR_FILE_WRITE						0xC0001044
#define AD_ERROR_INVALID_DATA_FORMAT			0xC0001061
#define AD_ERROR_INVALID_AVERAGE_OR_SMOOTHING	0xC0001062
#define AD_ERROR_INVALID_SOURCE_DATA			0xC0001063
#define AD_ERROR_NOT_ALLOCATE_MEMORY			0xC0001081
#define AD_ERROR_NOT_LOAD_DLL					0xC0001082
#define AD_ERROR_CALL_DLL						0xC0001083

// -----------------------------------------------------------------------
//
//		���[�U�֐��@�^�w��
//
// -----------------------------------------------------------------------
typedef void (CALLBACK CONVPROC)(
	WORD wCh,		// �`�����l���ԍ�
	DWORD dwCount,	// �f�[�^����
	LPVOID lpData	// �f�[�^�ւ̃|�C���^
);
typedef CONVPROC FAR *LPCONVPROC;

typedef void (CALLBACK ADCALLBACK)(DWORD dwUser);
typedef ADCALLBACK FAR *LPADCALLBACK;

// -----------------------------------------------------------------------
//	�e�`�����l�����̃T���v�����O�����\����
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChNo; 
	ULONG			ulRange; 
} ADSMPLCHREQ, *PADSMPLCHREQ;

// -----------------------------------------------------------------------
//	�g���K�����\����
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChNo; 
	float			fTrigLevel;
	float			fHysteresis;
} ADTRIGCHREQ, *PADTRIGCHREQ;

// -----------------------------------------------------------------------
//	�T���v�����O�����\����
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChCount;
	ADSMPLCHREQ		SmplChReq[256]; 
	ULONG			ulSamplingMode; 
	ULONG			ulSingleDiff; 
	ULONG			ulSmplNum; 
	ULONG			ulSmplEventNum;
	float			fSmplFreq; 
	ULONG			ulTrigPoint;
	ULONG			ulTrigMode; 
	LONG			lTrigDelay; 
	ULONG			ulTrigCh; 
	float			fTrigLevel1; 
	float			fTrigLevel2; 
	ULONG			ulEClkEdge; 
	ULONG			ulATrgPulse; 
	ULONG			ulTrigEdge; 
	ULONG			ulTrigDI; 
	ULONG			ulFastMode; 
} ADSMPLREQ, *PADSMPLREQ;

// -----------------------------------------------------------------------
//	�o�X�}�X�^�����T���v�����O�����\����
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulChCount;
	ADSMPLCHREQ		SmplChReq[256];
	ULONG			ulSingleDiff;
	ULONG			ulSmplNum;
	ULONG			ulSmplEventNum;
	ULONG			ulSmplRepeat;
	ULONG			ulBufferMode;
	float			fSmplFreq;
	float			fScanFreq;
	ULONG			ulStartMode;
	ULONG			ulStopMode;
	ULONG			ulPreTrigDelay;
	ULONG			ulPostTrigDelay;
	ADTRIGCHREQ		TrigChReq[2];
	ULONG			ulATrgMode;
	ULONG			ulATrgPulse;
	ULONG			ulStartTrigEdge;
	ULONG			ulStopTrigEdge;
	ULONG			ulTrigDI;
	ULONG			ulEClkEdge;
	ULONG			ulFastMode; 
	ULONG			ulStatusMode;
	ULONG			ulErrCtrl;
} ADBMSMPLREQ, *PADBMSMPLREQ;

// -----------------------------------------------------------------------
//	�{�[�h�d�l�\����
// -----------------------------------------------------------------------
typedef struct {
	ULONG			ulBoardType; 
	ULONG			ulBoardID; 
	DWORD			dwSamplingMode; 
	ULONG			ulChCountS; 
	ULONG			ulChCountD; 
	ULONG			ulResolution; 
	DWORD			dwRange;	
	ULONG			ulIsolation; 
	ULONG			ulDi; 
	ULONG			ulDo; 
} ADBOARDSPEC, *PADBOARDSPEC;

#ifdef __cplusplus
}
#endif

#endif
