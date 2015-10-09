//
// InterfaceBoard.c: �C���^�[�t�F�[�X�Ѓ{�[�h�p�N���X��`
// 2007/7/27 Torso�p�ɑ啝��������(�폜)
//  New Delete�錾���Ȃ������܂������Ȃ������̂ō폜->���ߑł��Ő錾
//

//#define WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <windows.h>
#include "InterfaceBoard.h"
#include "variables.h"
#include <string.h>
#include <math.h>
//#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

#pragma warning ( disable : 4996 )  // VS2005�ł͌x�������̂Ŗ�����

//--------------------------------
//�C���^�[�t�F�[�X��DA�{�[�h�N���X
//�����F�f�o�C�X��(��F"FBIDA1")�A�`���l����(��F4)�A�����W(��FDA_10V,FBIDA.h�Œ�`)
//--------------------------------
InterfaceDABoard::InterfaceDABoard(char *name, int ChNum, int Range)
{
    int n;
    ChannelNum = ChNum;

    strcpy(DeviceName,name);
    DeviceHandle=DaOpen((LPCTSTR)DeviceName);
    if(DeviceHandle == INVALID_HANDLE_VALUE){
        DaClose(DeviceHandle);
        cout << "Error. Can't Open Device" << DeviceName << endl;
        exit(-1);
    }
    DaGetDeviceInfo(DeviceHandle,&DeviceInfo);
    DaGetSamplingConfig(DeviceHandle, &DeviceConfig);
    for(n=0;n<ChannelNum;n++){
        Volt[n]=0.0;
    }
    //�`���l���ݒ�B�`���l�����A�����W,�T���v�����[�h�A���g��,�J��Ԃ��񐔁B

    DeviceConfig.ulChCount=ChNum;
    for(n=0 ; n<ChannelNum ; n++){
        DeviceConfig.SmplChReq[n].ulChNo = n+1;
        DeviceConfig.SmplChReq[n].ulRange = Range;
    }
    DeviceConfig.ulSamplingMode = DA_IO_SAMPLING;
    DeviceConfig.fSmplFreq =10000.0;
    DeviceConfig.ulSmplRepeat = 0;
    DeviceConfig.ulTrigMode = DA_FREERUN;
    DaSetSamplingConfig(DeviceHandle , &DeviceConfig );
}

// �R���X�g���N�^�̃I�[�o�[���[�h�쐬(�ύX�FWatanabe)
// 

InterfaceDABoard::InterfaceDABoard(char *name)
{
    strcpy(DeviceName,name);
    DeviceHandle=DaOpen((LPCTSTR)DeviceName);
    if(DeviceHandle == INVALID_HANDLE_VALUE){
        DaClose(DeviceHandle);
        cout << "Error. Can't Open Device" << DeviceName << endl;
        exit(-1);
    }
    DaGetDeviceInfo(DeviceHandle,&DeviceInfo);

}

// initialize���R���X�g���N�^���番��
// �`�����l���g�̓񎟌��z���^���āA�ꊇ�擾�������`�����l���g��
// �����\���̂��쐬����
// ����͕����Ȃ��Ȃ̂ňꎟ���z��ɂ��Ă��邱�Ƃɒ��ӁI�I
// (�ėp������������Ȃ��AD�Ɠ��l��**ChArray, *ChNum�Œ�`���ׂ�)
void InterfaceDABoard::initialize(int *ChArray, int ChNum, int Range)
{
    int n;
	ChannelNum = ChNum;

	for(n=0;n<ChannelNum;n++){
        Volt[n]=0.0;
    }
    //�`���l���ݒ�B�`���l�����A�����W,�T���v�����[�h�A���g��,�J��Ԃ��񐔁B

    DeviceConfig.ulChCount=ChNum;
    for(n=0 ; n<ChannelNum ; n++){
        DeviceConfig.SmplChReq[n].ulChNo = ChArray[n];
        DeviceConfig.SmplChReq[n].ulRange = Range;
    }
    DeviceConfig.ulSamplingMode = DA_IO_SAMPLING;
    DeviceConfig.fSmplFreq =10000.0;
    DeviceConfig.ulSmplRepeat = 0;
    DeviceConfig.ulTrigMode = DA_FREERUN;
    DaSetSamplingConfig(DeviceHandle , &DeviceConfig );
}


//����`���l���̃����W��ݒ�B�������̃`���l�����J���Ă��Ȃ���ΊJ��
void InterfaceDABoard::SetChannel(int Channel, int Range)
{
    if(Channel > ChannelNum){
        ChannelNum = Channel;
        DeviceConfig.ulChCount=ChannelNum;
    }
    DeviceConfig.SmplChReq[Channel].ulChNo = Channel+1;
    DeviceConfig.SmplChReq[Channel].ulRange = Range;
    DaSetSamplingConfig(DeviceHandle , &DeviceConfig );
}


InterfaceDABoard::~InterfaceDABoard(void)
{
    DaClose(DeviceHandle);
}


//�{�[�h�̏�Ԃ�\��
void InterfaceDABoard::Show(void)
{
    cout << "Board Information" << endl
         << "Type " <<DeviceInfo.ulBoardType << endl
         << "ID " << DeviceInfo.ulBoardID << endl // �{�[�h���ʔԍ�
         << "Channel Number " << DeviceInfo.ulChCount << endl// �{�[�h�A�i���O�o�̓`�����l����
         << "Resolution " << DeviceInfo.ulResolution << endl// �{�[�h����\
         << "Digital Input Number " << DeviceInfo.ulDi << endl// �{�[�h�f�W�^�����͓_��
         << "Digital Output Number " << DeviceInfo.ulDo << endl;// �{�[�h�f�W�^���o�͓_��
}

//
//Channel��Value[V]���o�͂��A����ȊO��Channel�̒l�͕ێ�����B
//
void InterfaceDABoard::DAOut(int Channel,double Value)
{
	UINT MaxCh=DeviceConfig.ulChCount;
	UINT MaxSmplNum=1;

	if(Channel>(int)MaxCh-1 ||Channel<0){
        cout << "No such DA Channel " << Channel << endl;
        exit(-1);
	}

	Volt[Channel]=(float)Value;

	//�o��
	switch(DeviceInfo.ulResolution){
		case 8 :
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN8,Data8,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data8);
			break;
		case 12:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN12,Data12,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data12);
			break;
		case 16:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN16,Data16,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data16);
			break;
		case 24:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN24,Data24,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data24);
			break;
	}

    if(nRet!=0){
        DisplayErrorMessage();
        exit(-1);
    }
}


//�z��Value(������[V])���o�́B�`���l������
//�z��T�C�Y�̓`���l������������΂Ȃ�Ȃ�
//(���ɃG���[�������Ă��Ȃ��̂ŗe�Ղ�Segmentation Fault����B���Ӂj
void InterfaceDABoard::DAOut(double* Value)
{
	UINT MaxCh=DeviceConfig.ulChCount;
	UINT MaxSmplNum=1;

    //Segmentation Fault�N���₷���Ƃ���B�`���l�������f�[�^���w�肵�Ă��邱�Ƃ��m�F����
    for(int n=0;n<(int)MaxCh;n++){
        Volt[n]=(float)Value[n];
    }

	//�o��
	switch(DeviceInfo.ulResolution){
		case 8 :
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN8,Data8,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data8);
			break;
		case 12:
			nRet=DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN12,Data12,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data12);
			break;
		case 16:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN16,Data16,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data16);
			break;
		case 24:
			DaDataConv(DA_DATA_PHYSICAL,Volt,MaxSmplNum,&DeviceConfig,
						DA_DATA_BIN24,Data24,&MaxSmplNum,&DeviceConfig,0,0,NULL);
			nRet=DaOutputDA(DeviceHandle,MaxCh,DeviceConfig.SmplChReq,Data24);
			break;
	}

    if(nRet!=0){
        DisplayErrorMessage();
        exit(-1);
    }
}

//�G���[�\���F���̕ӂ̒l��gpcda.h���ɒ�`����Ă���
void InterfaceDABoard::DisplayErrorMessage()
{
    cout <<"DA Error: ";
    switch(nRet) {
        case DA_ERROR_NOT_DEVICE:
            cout << "The device couldn't be found."<<endl;
            break;
        case DA_ERROR_NOT_OPEN:
            cout << "The system couldn't found the device."<<endl;
            break;
        case DA_ERROR_INVALID_HANDLE:
            cout << "Invalid device handle is specified."<<endl;
            break;
        case DA_ERROR_ALREADY_OPEN:
            cout << "The device has been already opened."<<endl;
            break;
        case DA_ERROR_NOT_SUPPORTED:
            cout << "It is not supported."<<endl;
            break;
        case DA_ERROR_NOW_SAMPLING:
            cout << "The analog output is running."<<endl;
            break;
        case DA_ERROR_STOP_SAMPLING:
            cout << "The analog output is stopped."<<endl;
            break;
        case DA_ERROR_START_SAMPLING:
            cout << "The analog output couldn't be performed."<<endl;
            break;
        case DA_ERROR_SAMPLING_TIMEOUT:
            cout << "The timeout interval elapsed while the analog output is running."<<endl;
            break;
        case DA_ERROR_INVALID_PARAMETER:
            cout << "Invalid parameters are specified."<<endl;
            break;
        case DA_ERROR_ILLEGAL_PARAMETER:
            cout << "Invalid analog output conditions are specified."<<endl;
            break;
        case DA_ERROR_NULL_POINTER:
            cout << "A NULL pointer is specified."<<endl;
            break;
        case DA_ERROR_SET_DATA:
            cout << "The analog output data couldn't be configured."<<endl;
            break;
        case DA_ERROR_FILE_OPEN:
            cout << "Opening the file failed."<<endl;
            break;
        case DA_ERROR_FILE_CLOSE:
            cout << "Closing the file failed."<<endl;
            break;
        case DA_ERROR_FILE_READ:
            cout << "Reading the file failed."<<endl;
            break;
        case DA_ERROR_FILE_WRITE:
            cout << "Writing the file failed."<<endl;
            break;
        case DA_ERROR_INVALID_DATA_FORMAT:
            cout << "Invalid data format is specified."<<endl;
            break;
        case DA_ERROR_INVALID_AVERAGE_OR_SMOOTHING:
            cout << "Invalid averaging configuration or invalid smoothing configuration is specified."<<endl;
            break;
        case DA_ERROR_INVALID_SOURCE_DATA:
            cout <<  "Invalid source data are specified."<<endl;
            break;
        case DA_ERROR_NOT_ALLOCATE_MEMORY:
            cout << "Enough memory couldn't be allocated."<<endl;
            break;
        case DA_ERROR_NOT_LOAD_DLL:
            cout << "Loading the DLL failed."<<endl;
            break;
        case DA_ERROR_CALL_DLL:
            cout << "Calling the DLL failed."<<endl;
            break;
        default:
            cout << "Unexpected error is occurred."<<endl;
            break;
    }

}


//�G���[�\���F���̕ӂ̒l��fbienc.h���ɒ�`����Ă���
void InterfaceCNTBoard::DisplayErrorMessage()
{
    cout << "CNTBoard::";
    switch(nRet){
        case PENC_ERROR_SUCCESS:   // ����I��
            cout << "����I��"<<endl;
            break;
        case PENC_ERROR_NOT_DEVICE:
            cout << "�w�肳�ꂽ�f�o�C�X�������邱�Ƃ��ł��܂���"<<endl;
            break;
        case PENC_ERROR_NOT_OPEN:
            cout << "�V�X�e�����f�o�C�X���I�[�v���ł��܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_HANDLE:
            cout << "�f�o�C�X�n���h��������������܂���"<<endl;
            break;
        case PENC_ERROR_ALREADY_OPEN:
            cout << "���ł�OPEN���Ă���f�o�C�X��OPEN���悤�Ƃ��܂���"<<endl;
            break;
        case PENC_ERROR_HANDLE_EOF:
            cout << "EOF�ɒB���܂���"<<endl;
            break;
        case PENC_ERROR_MORE_DATA:
            cout << "����ɑ����̃f�[�^�����p�\�ł�"<<endl;
            break;
        case PENC_ERROR_INSUFFICIENT_BUFFER:
            cout << "�V�X�e���R�[���ɓn���ꂽ�f�[�^�̈悪���������܂�"<<endl;
            break;
        case PENC_ERROR_IO_PENDING:
            cout << "�񓯊�I/O���삪�i�s���ł�"<<endl;
            break;
        case PENC_ERROR_NOT_SUPPORTED:
            cout << "�T�|�[�g����Ă��Ȃ��@�\�ł�"<<endl;
            break;
        case PENC_ERROR_INITIALIZE_IRQ:
            cout << "���荞�݂̏������Ɏ��s���܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_CHANNEL:
            cout << "�s���ȃ`�����l�����w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_MODE:
            cout << "�s���ȃ��[�h���w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_DIRECT:
            cout << "�s���ȃJ�E���^�������w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_COUNTER:
            cout << "�s���ȃJ�E���^�l���w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_COMPARATOR:
            cout << "�s���Ȕ�r�J�E���^���w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_ZMODE:
            cout << "�s���Ȃy���_���l���w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_MASK:
            cout << "�s���ȃC�x���g�}�X�N�l���w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_INVALID_ITIMER:
            cout << "�s���ȃC���^�[�o���^�C�}�ݒ�l���w�肵�܂���"<<endl;
            break;
        case PENC_ERROR_ALREADY_REGISTRATION:
            cout << "�C�x���g�͂��łɓo�^�ς݂ł�"<<endl;
            break;
        case PENC_ERROR_ALREADY_DELETE:
            cout << "�C�x���g�͂��łɍ폜����Ă��܂�"<<endl;
            break;
        case PENC_ERROR_MEMORY_NOTALLOCATED:
            cout << "��Ɨp�������̊m�ۂɎ��s���܂���"<<endl;
            break;
        case PENC_ERROR_MEMORY_FREE:
            cout << "�������̉���Ɏ��s���܂���"<<endl;
            break;
        case PENC_ERROR_TIMER:
            cout << "�^�C�}���\�[�X�̎擾�Ɏ��s���܂���"<<endl;
            break;
        case PENC_ERROR_DRVCAL:
            cout << "�h���C�o���Ăяo���܂���"<<endl;
            break;
        case PENC_ERROR_NULL_POINTER:
            cout << "�h���C�o�ADLL�Ԃ�NULL�|�C���^���n����܂���"<<endl;
            break;
        case PENC_ERROR_PARAMETER:
            cout << "�����p�����[�^�̒l���s���ł�"<<endl;
            break;
        default:
            cout << "�\�����Ȃ��G���[���������܂���"<<endl;
            break;
    }
}


//--------------------------------
//�C���^�[�t�F�[�X�ЃJ�E���^�{�[�h�N���X
//�����F�f�o�C�X��(��F"FBIPENC1")�A�`���l����(��F2)
//PCI-6204,PCI-6205�n���p
//--------------------------------
InterfaceCNTBoard::InterfaceCNTBoard(char *name, int Channel)
{
    ChannelNum = Channel;

    strcpy(DeviceName,name);
    DeviceHandle=PencOpen((LPCTSTR)DeviceName, PENC_FLAG_SHARE);
    if(DeviceHandle == INVALID_HANDLE_VALUE){
        PencClose(DeviceHandle);
        cout << "Error. Can't Open Device" << DeviceName << endl;
        exit(-1);
    }
    // �f�t�H���g�̏��������
    // �J�E���g�A�b�v�A��v���o���Ȃ��A�\�t�g���b�`�A�ʑ������̓p���X�A�S���{�AZ���N���A����
    InitMode(CountUp | NotUseEqual |SoftLatch , DiffPhasePulse | Multi_4 , NotClear);
}

InterfaceCNTBoard::~InterfaceCNTBoard(void)
{
    PencClose(DeviceHandle);
}

// �`�����l���̏������ݒ�(nCh�`�����l����CountMode�Őݒ肳�ꂽ���[�h�ɂ��A
// PulseMode�Őݒ肳�ꂽ�p���X��ǂݍ���.Z���̐ݒ��ZMode�ɂ��w��j
void InterfaceCNTBoard::InitMode(int nCh, int CountMode, int PulseMode, int ZPhaseMode){
//    assert(nCh < ChannelNum);       // nCH�Ŏw�肳���`�����l���́A�����������`�����l���������������K�v������B
    int CountDirection = 0;
    int EqualMode = 0;
    int LatchMode = 0;
    int InputPulseMode = 0x00;

    if((CountMode & 0xC0) == CountUp){
        CountDirection = 0;         // Up�����ɃJ�E���g
    }else if((CountMode & 0xC0) == CountDown){
        CountDirection = 1;         // Down�����ɃJ�E���g
    }
    if((CountMode & 0x21) == UseEqual){
        EqualMode = 1;          // ��v���o�L��
    }else if((CountMode & 0x21) == NotUseEqual){
        EqualMode = 0;          // ��v���o����
    }


    if((CountMode & 0x22) == SoftLatch){
        LatchMode = 0;          // �\�t�g���b�`
    }else if((CountMode & 0x24) == ExtLatch){
        LatchMode = 1;          // �O�����b�`
    }

    if((PulseMode & 0xF0) == SinglePulse){
        InputPulseMode = 0x00;              // �P���p���X���[�h�̂Ƃ�
        if((PulseMode & 0x01) == Multi_1){
            InputPulseMode += 0x00;         //�W��
        }else{
            InputPulseMode += 0x01;         // �Q���{�B�P���p���X���[�h�̂Ƃ��͂Q���{�܂�
        }
    }else if((PulseMode & 0xF0 ) == DiffPhasePulse){
        InputPulseMode = 0x04;              // �ʑ����p���X���[�h�̂Ƃ�
        if((PulseMode & 0x03) == Multi_1){
            InputPulseMode += 0x00;         //�W��
        }else if((PulseMode & 0x03) == Multi_2){
            InputPulseMode += 0x01;         // �Q���{
        }else if((PulseMode & 0x03) == Multi_4){
            InputPulseMode += 0x02;         // 4���{
        }
        if((PulseMode & 0x0C) == SyncClear){
            InputPulseMode += 0x08;         // �����N���A
        }else if((PulseMode & 0x0C) == ASyncClear){
            InputPulseMode += 0x00;         //�@�񓯊��N���A
        }
    }else if((PulseMode & 0xF0 ) == TwoPulse){
        InputPulseMode = 0x08;
    }

    if((nRet=PencReset(DeviceHandle,nCh+1)) != 0) DisplayErrorMessage();
    if((nRet=PencSetMode(DeviceHandle, nCh+1, InputPulseMode, CountDirection, EqualMode, LatchMode)) != 0) DisplayErrorMessage();
    if((nRet=PencSetZMode(DeviceHandle, nCh+1, ZPhaseMode) != 0)) DisplayErrorMessage();
    if((nRet=PencSetCounter(DeviceHandle,nCh+1,0) != 0)) DisplayErrorMessage();
}

// �`�����l���̏������ݒ�(�S�`�����l����CountMode�Őݒ肳�ꂽ���[�h�ɂ��A
// PulseMode�Őݒ肳�ꂽ�p���X��ǂݍ���.Z���̐ݒ��ZMode�ɂ��w��j
void InterfaceCNTBoard::InitMode(int CountMode, int PulseMode, int ZPhaseMode)
{
    int i;
    for(i=0;i<ChannelNum;i++)
        InitMode(i,CountMode,PulseMode,ZPhaseMode);
}

//�{�[�h��ԕ\��
void InterfaceCNTBoard::Show(void)
{
    int n;
    int multi;
    int Mode, Direction, Equal, Latch, ZMode;
    cout << "PCI-6204 compatible CNT Board" << endl;

    for(n=0;n<ChannelNum;n++){
        cout << "�`�����l��" << n << "  " ;
        PencGetMode(DeviceHandle, n+1, &Mode, &Direction, &Equal, &Latch);
        PencGetZMode(DeviceHandle, n+1, &ZMode);
        multi = (int)pow(2.0,(Mode & 0x03));
        switch(Mode & 0x0C){
        case 0x00:
            cout << "�Q�[�g���P���p���X  " << multi << "���{  " << "�񓯊��N���A" << endl;
            break;
        case 0x04:
            cout << "�ʑ����p���X�@�@�@�@�@" << multi << "���{  " << "�񓯊��N���A" << endl;
            break;
        case 0x0C:
            cout << "�ʑ����p���X�@�@�@�@�@" << multi << "���{  " << "  �����N���A" << endl;
            break;
        case 0x08:
            cout << "�Q�p���X�@�@�@�@�@�@�@" << multi << "���{  " << "�񓯊��N���A" << endl;
            break;
        }
        if(Direction == 0)
            cout << "   �J�E���g�A�b�v  " ;
        else
            cout << "   �J�E���g�_�E��  " ;

        if(Equal == 0)
            cout << "��v���o����  ";
        else
            cout << "��v���o�L��  ";

        if(Latch == 0)
            cout << "�\�t�g���b�`  ";
        else
            cout << "�O�����b�`    ";

        if((ZMode & 0x10) == 0x00)
            cout << "Z���ʏ�  ";
        else
            cout << "Z�����]  ";

        switch(ZMode & 0x03){
        case 0x00:
            cout << "Z���N���A����" << endl;
            break;
        case 0x01:
            cout << "Z���N���A�L��" << endl;
            break;
        case 0x02:
            cout << "�O�����b�`&Z���ɂ��N���A�L��" << endl;
            break;
        }

    }

}

//�G���R�[�_�l��Ԃ��֐����
//����`���l������̒l��ǂށB�`���l���w���0�`
DWORD InterfaceCNTBoard::Get(int Channel)
{
    static DWORD dwCounter;
    if((nRet=PencGetCounter(DeviceHandle,Channel+1,&dwCounter))!=0)DisplayErrorMessage();
    return dwCounter;

}
//���ׂẴ`���l������̒l�𓯎��ɓǂ݁A�����̔z��Ɋi�[
void InterfaceCNTBoard::Get(DWORD *EncValue)
{
    int bit = (1<<ChannelNum) - 1; //�`���l�������̃r�b�g�𗧂Ă�
    if((nRet=PencGetCounterEx(DeviceHandle, bit, &EncValue[0]))!=0)
		DisplayErrorMessage();
}

//���݂̂��ׂẴG���R�[�_�l�𓯎���0�ɐݒ�B
//�I�v�V�����Ƃ��ăI�t�Z�b�g�l�������Ƃ��Đݒ�E

void InterfaceCNTBoard::SetCount(DWORD Offset)
{
    //�G���R�[�_�̃I�t�Z�b�g�l��ݒ�
    DWORD *dw= new DWORD[ChannelNum];

    for(int n=0;n<ChannelNum;n++){
        dw[n] = Offset;
    }
    //���Z�b�g
    int bit = (1<<ChannelNum) - 1; //�`���l�������̃r�b�g�𗧂Ă�
    if((nRet=PencSetCounterEx(DeviceHandle, bit, dw))!=0)DisplayErrorMessage();
}

//�w��`���l���̒l��Offset�l�ɐݒ�B�`���l���ԍ��w��͂O�`
void InterfaceCNTBoard::SetCount(int Channel, DWORD Offset)
{
    //���Z�b�g
    if((nRet=PencSetCounter(DeviceHandle,Channel+1,Offset) != 0)) DisplayErrorMessage();
}



InterfaceDIOBoard::InterfaceDIOBoard(char *name)
{	
	strcpy(DeviceName,name);
	
    DeviceHandle = DioOpen((LPCTSTR)DeviceName, 0);
    if (DeviceHandle == INVALID_HANDLE_VALUE) {
		DioClose(DeviceHandle);
        printf("�f�o�C�X�� FBIDIO1�͎g�p�ł��܂���\n");
        exit(-1);             //�v���O�����I��
    }
}

InterfaceDIOBoard::~InterfaceDIOBoard(void)
{
	DioClose(DeviceHandle);
}

void InterfaceDIOBoard::DInput(int *Value, int offset, int num)
{

    int buffer[64];

	DioInputPoint(DeviceHandle, buffer, offset, num);

	for(int i=0; i<num; i++) Value[i] = buffer[i];

}

void InterfaceDIOBoard::DOutput(BYTE value)
{

}