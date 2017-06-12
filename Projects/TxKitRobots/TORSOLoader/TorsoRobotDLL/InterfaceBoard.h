//InterfaceBoard.h
// �쐬��: ���{���V
// ���藚��
// ������F����ҁF������e
// 2002/2/27 : �쓈�@����: �C���^�[�t�F�[�X�̃J�E���^�{�[�h�p�֐��ɂ͂Q�n�����邽�߁A
//                      ���̗����ɑΉ��ł���悤�ɁA�f�o�C�X������K�؂Ȋ֐��n�𗘗p����悤�ɂ����B
//                      �܂��A���܂��܂ȏ����ݒ�ɑΉ��ł���悤�ɏ������֐���ǉ������B
// 2007/12/4 : Pana : �g���\�p�ɑ啝��������

#ifndef ___INTERFACE
#define ___INTERFACE

#include <windows.h>
#include <stdio.h>

// Header files for PC-Board producted by Interface Co.
#include "interface/Fbida.h"     // DA�{�[�h�p�w�b�_�t�@�C��
#include "interface/Fbiad.h"     // AD�{�[�h�p�w�b�_�t�@�C��
#include "interface/FbiPenc.h"   // PCI-6204�n��J�E���^�{�[�h�p�w�b�_�t�@�C��
#include "interface/FbiMl.h"     // ���������N�{�[�h�p�w�b�_�t�@�C��(�ǉ��FWatanabe)
#include "interface/FbiDio.h"    // 

// InitMode�֐������p�����[�^
// ��Ԗڂ̈����F���[�h
#define UseEqual    0x20 // ��v���o�𗘗p
#define NotUseEqual 0x21 // ��v���o�𗘗p���Ȃ�
#define SoftLatch   0x22 // �\�t�g���b�`�B�R�}���h�𔭌������Ƃ��̃G���R�[�_�̃J�E���g�l��������
#define ExtLatch    0x24 // �O�����b�`�B�O�����烉�b�`�M�������͂��ꂽ�Ƃ��̒l��������
#define CountUp     0x40 // �A�b�v�����ɃJ�E���g
#define CountDown   0x80 // �_�E�������ɃJ�E���g
// ��Ԗڂ̈���: ���̓p���X
//���̓p���X���[�h
#define SinglePulse     0x10    // �Q�[�g���P���p���X���[�h
#define DiffPhasePulse  0x20    // �ʑ����p���X���[�h
#define TwoPulse        0x40    // �Q�p���X���[�h   (PCI-6204�n��̂݁j
#define UpDownCount     0x80    // �A�b�v�_�E���J�E���g���[�h�@(PCI-6201�n��̂݁j
#define Multi_1         0x00    // �W��
#define Multi_2         0x01    // �Q���{
#define Multi_4         0x02    // �S���{
// �ȉ��̐ݒ�͈ʑ����p���X���[�h�̂Ƃ��̂ݗL��
#define SyncClear       0x04    // �����N���A
#define ASyncClear      0x08    // �񓯊��N���A
// �O�Ԗڂ̈����FZ��
//Z���ݒ�
#define NotClear        0x00 // Z���N���A�����Ȃ�
#define ClearWithEveryZ 0x01 // ����Z���N���A����
#define ClearWithZLatch 0x02 // Z���ƃ��b�`�M���ŃN���A����
#define ClearWithOneZ   0x04 // �P�x����Z���ŃN���A����
#define PosiZ           0x00 // Z���ʏ�
#define NegaZ           0x10 // Z�����]


#define	SAFE_DELETE(x)		if(x){delete x; x = NULL;}
#define	SAFE_DELETE_ARRAY(x)	if(x){delete [] x; x = NULL;}

#define DA_MAXCHANNEL	16


//--------------------------------
//�C���^�[�t�F�[�X�Ѓ{�[�h���ʃN���X
//--------------------------------
class InterfaceBoard
{
protected:
    int nRet;                       //�֐��Ԃ��l
    char        DeviceName[16];     //�f�o�C�X��
    HANDLE      DeviceHandle;       //�f�o�C�X�n���h��
    int ChannelNum;                 //�`���l����
};

//--------------------------------
//�C���^�[�t�F�[�X��DA�{�[�h�N���X�FI/O�����ɂ̂ݑΉ�
//(�������X�g�A�����{�[�h���͕ʃN���X�Ƃ��Ē�`����j
//--------------------------------
class InterfaceDABoard :public InterfaceBoard
{
private:
    DASMPLREQ   DeviceConfig;       // �o�͐���\����
    DABOARDSPEC DeviceInfo;         // �{�[�h���
    float Volt[6];                  //���݂̏o�͓d��
    //�f�[�^�^�C�v
	BYTE  Data8[DA_MAXCHANNEL];					// �G���[����̂��߂ɁA���ߑł��錾(2007/7/27)
    WORD  Data12[DA_MAXCHANNEL],Data16[DA_MAXCHANNEL];
    DWORD Data24[DA_MAXCHANNEL];
    void DisplayErrorMessage(void); //�{�[�h�̃G���[�����o��
public:
    InterfaceDABoard(char *name,int ChNum, int Range = DA_10V);//�R���X�g���N�^�B�`���l�����ƃ����W���w��
	InterfaceDABoard(char *name);// �R���X�g���N�^�̃I�[�o�[���[�h(�ύX�FWatanabe)
	void initialize(int *ChArray, int ChNum, int Range=DA_10V); // �`�����l���ݒ蓙���R���X�g���N�^���番��(�ǉ��FWatanabe)
    ~InterfaceDABoard();
    void DAOut(int Channel,double Value); //Value(������[V])��Channel�ɏo��
    void DAOut(double* Value);            //�z��Value(������[V])���o��
    void Show();                          //�{�[�h�̏�Ԃ�\��
    void SetChannel(int Channel, int Range= DA_10V); //����`���l���̃����W��ݒ�B�������̃`���l�����J���Ă��Ȃ���ΊJ��
    int  TEX_DaInputDI(void){
      DWORD dwData;
      DaInputDI( DeviceHandle, &dwData );
      return dwData;
    };
    void TEX_DaOutputDO(DWORD value){
      DaOutputDO( DeviceHandle, value );
    }

	};

//--------------------------------
//�C���^�[�t�F�[�X�ЃG���R�[�_�{�[�h�N���X
//���PCI6204,6205�p
//--------------------------------
class InterfaceCNTBoard :public InterfaceBoard
{
private:
    void DisplayErrorMessage(void);      //�����֐��B�G���[�\��
public:
    InterfaceCNTBoard(char *name, int Channel);
    ~InterfaceCNTBoard();
    void InitMode(int nCh, int CountMode, int PulseMode, int ZPhaseMode);       // �`�����l���̏������ݒ�(nCh�`�����l����CountMode�Őݒ肳�ꂽ���[�h�ɂ��APulseMode�Őݒ肳�ꂽ�p���X��ǂݍ���.Z���̐ݒ��ZMode�ɂ��w��j
    void InitMode(int CountMode, int PulseMode, int ZPhaseMode);                // �`�����l���̏������ݒ�(�S�`�����l����CountMode�Őݒ肳�ꂽ���[�h�ɂ��APulseMode�Őݒ肳�ꂽ�p���X��ǂݍ���.Z���̐ݒ��ZMode�ɂ��w��j
    void Show();                                                                //�{�[�h�̏�Ԃ�\��
    DWORD Get(int Channel);                                                     //�w��Channel���T���v�����O���ĕԂ�
    void Get(DWORD *EncValue);                                                  //�S�`���l�������T���v�����O
    void SetCount(DWORD Offset);                                                //�S�`���l���̒l��Offset�ɕύX�B
    void SetCount(int Channel, DWORD Offset);                                   //�w��`���l���̒l��Offset�ɕύX�B
};

//--------------------------------
//�C���^�[�t�F�[�X��ML�{�[�h�N���X�F
//--------------------------------
class InterfaceMLBoard :public InterfaceBoard
{
private:
	void DisplayErrorMessage(void);		//�{�[�h�̃G���[�����o��
	int nWriteSlaveNo,nReadSlaveNo;//�X���[�u�ԍ�
	DWORD dwWriteOffset,dwReadOffset;//�������݁A�ǂݍ��݃I�t�Z�b�g
public:
	InterfaceMLBoard(char *name);//�R���X�g���N�^�B�`���l�����A�����W�A���̓��[�h(single or diff)���w��
	~InterfaceMLBoard();
	void MLRead(float* read,DWORD offset,int datasize);
	void MLWrite(float* write,DWORD offset,int datasize);
	void MLRead(double* read,DWORD offset,int datasize);
	void MLWrite(double* write,DWORD offset,int datasize);
};

//--------------------------------
//�C���^�[�t�F�[�X��DIO�{�[�h�N���X�F
//--------------------------------
class InterfaceDIOBoard :public InterfaceBoard
{
private:
	void DisplayErrorMessage(void);		//�{�[�h�̃G���[�����o��
public:
	InterfaceDIOBoard(char *name);//�R���X�g���N�^�B�`���l�����A�����W�A���̓��[�h(single or diff)���w��
	~InterfaceDIOBoard();
	void DInput(int *Value, int offset, int num);
	void DOutput(BYTE value);
};

#endif