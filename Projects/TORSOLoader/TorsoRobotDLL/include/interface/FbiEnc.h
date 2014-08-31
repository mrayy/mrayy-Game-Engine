///////////////////////////////////////////////////////////////////////////////
//																		       
//  Windows NT/95     FbiEnc.h    Include file for Encorder Driver
//				   
//	 	    ������`�t�@�C��
//																		       
///////////////////////////////////////////////////////////////////////////////
#if !defined( _FBIENC_H_ )
#define _FBIENC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (CALLBACK ENCCALLBACK)(DWORD dwEncEvent, DWORD dwUser);
typedef ENCCALLBACK FAR *LPENCCALLBACK;

// -----------------------------------------------------------------------
//	���ʎq
// -----------------------------------------------------------------------
#define MAX_COUNTER		0xffffff


// -----------------------------------------------------------------------
//	�I�[�v���t���O
// -----------------------------------------------------------------------
#define ENC_FLAG_NORMAL	0x0000		//EncOpen�Ɏw��\�ȃt���O�ł��B
#define ENC_FLAG_SHARE	0x0002		//EncOpen�Ɏw��\�ȃt���O�ł��B�f�o�C�X�̏d���I�[�v���������܂��B


// -----------------------------------------------------------------------
//	�߂�l
// -----------------------------------------------------------------------
#define	ENC_ERROR_SUCCESS				0			// ����I��
#define ENC_ERROR_NOT_DEVICE			0xC0000001	// �w�肳�ꂽ�f�o�C�X�������邱�Ƃ��ł��܂���
#define	ENC_ERROR_NOT_OPEN				0xC0000002	// �V�X�e�����f�o�C�X���I�[�v���ł��܂���
#define ENC_ERROR_INVALID_HANDLE		0xC0000003	// �f�o�C�X�n���h��������������܂���
#define ENC_ERROR_ALREADY_OPEN			0xC0000004  // ���ł�OPEN���Ă���f�o�C�X��OPEN���悤�Ƃ��܂���
#define ENC_ERROR_HANDLE_EOF			0xC0000005	// EOF�ɒB���܂���
#define ENC_ERROR_MORE_DATA				0xC0000006	// ����ɑ����̃f�[�^�����p�\�ł�
#define ENC_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// �V�X�e���R�[���ɓn���ꂽ�f�[�^�̈悪���������܂�
#define ENC_ERROR_IO_PENDING			0xC0000008	// �񓯊�I/O���삪�i�s���ł�
#define ENC_ERROR_NOT_SUPPORTED			0xC0000009	// �T�|�[�g����Ă��Ȃ��@�\�ł�
#define ENC_ERROR_INITIALIZE_IRQ		0xC0001000	// ���荞�݂̏������Ɏ��s���܂���
#define	ENC_ERROR_INVALID_CHANNEL		0xC0001001	// �s���ȃ`�����l���ԍ����w�肵�܂���
#define	ENC_ERROR_INVALID_MODE			0xC0001002	// �s���ȃ��[�h���w�肵�܂���
#define	ENC_ERROR_INVALID_DIRECT		0xC0001003	// �s���ȃJ�E���^�������w�肵�܂���
#define	ENC_ERROR_INVALID_COUNTER		0xC0001004	// �s���ȃJ�E���^�l���w�肵�܂���
#define ENC_ERROR_INVALID_COMPARATOR	0xC0001005	// �s���Ȕ�r�J�E���^���w�肵�܂���
#define	ENC_ERROR_INVALID_ZMODE			0xC0001006	// �s���Ȃy���_���l���w�肵�܂���
#define	ENC_ERROR_INVALID_FILTER		0xC0001007	// �s���ȃt�B���^�l���w�肵�܂���
#define	ENC_ERROR_INVALID_MASK			0xC0001008	// �s���ȃC�x���g�}�X�N�l���w�肵�܂���
#define	ENC_ERROR_INVALID_ITIMER		0xC0001009	// �s���ȃC���^�[�o���^�C�}�ݒ�l���w�肵�܂���
#define ENC_ERROR_ALREADY_REGISTRATION	0xC000100A	// �C�x���g�͂��łɓo�^�ς݂ł�
#define ENC_ERROR_ALREADY_DELETE		0xC000100B	// �C�x���g�͂��łɍ폜����Ă��܂�
#define	ENC_ERROR_MEMORY_NOTALLOCATED	0xC000100C	// ��Ɨp�������̊m�ۂɎ��s���܂���	
#define	ENC_ERROR_MEMORY_FREE			0xC000100D	// �������̉���Ɏ��s���܂���	
#define	ENC_ERROR_TIMER					0xC000100E	// �^�C�}���\�[�X�̎擾�Ɏ��s���܂���
#define	ENC_ERROR_DRVCAL				0xC000100F	// �h���C�o���Ăяo���܂���
#define	ENC_ERROR_NULL_POINTER			0xC0001010	// �h���C�o�ADLL�Ԃ�NULL�|�C���^���n����܂���
#define ENC_ERROR_PARAMETER				0xC0001011	// �����p�����[�^�̒l���s���ł�


#if !defined(_FBIENCLIB_)
#define FBIENCAPI
#else
#define FBIENCAPI __declspec(dllexport)
#endif

FBIENCAPI	HANDLE WINAPI	EncOpen( LPCTSTR lpszName, DWORD  fdwFlags );
FBIENCAPI	INT WINAPI	EncClose( HANDLE hDeviceHandle );
FBIENCAPI	INT WINAPI	EncSetSystemMode( HANDLE hDeviceHandle, INT nChannel, INT nMode );
FBIENCAPI	INT WINAPI	EncSetMode( HANDLE hDeviceHandle, INT nChannel, INT nMode, INT nDirect, INT nPulseEvent );
FBIENCAPI	INT WINAPI	EncGetMode( HANDLE hDeviceHandle, INT nChannel, PINT pnMode, PINT pnDirect, PINT pnPulseEvent );
FBIENCAPI	INT WINAPI	EncSetCounter( HANDLE hDeviceHandle, INT nChannel, DWORD dwCounter );
FBIENCAPI	INT WINAPI	EncGetCounter( HANDLE hDeviceHandle, INT nChannel, PDWORD pdwCounter );
FBIENCAPI	INT WINAPI	EncSetCounterEx( HANDLE hDeviceHandle, INT nChannel, PDWORD pdwCounter );
FBIENCAPI	INT WINAPI	EncGetCounterEx( HANDLE hDeviceHandle, INT nChannel, PDWORD pdwCounter );
FBIENCAPI	INT WINAPI	EncSetComparator( HANDLE hDeviceHandle, INT nChannel, INT nCounter, DWORD dwComparator );
FBIENCAPI	INT WINAPI	EncGetComparator( HANDLE hDeviceHandle, INT nChannel, INT nCounter, PDWORD pdwComparator );
FBIENCAPI	INT WINAPI	EncSetZMode( HANDLE hDeviceHandle, INT nChannel, INT nZMode );
FBIENCAPI	INT WINAPI	EncGetZMode( HANDLE hDeviceHandle, INT nChannel, PINT pnZMode );
FBIENCAPI	INT WINAPI	EncSetFilter( HANDLE hDeviceHandle, INT nFilter );
FBIENCAPI	INT WINAPI	EncGetFilter( HANDLE hDeviceHandle, PINT pnFilter );
FBIENCAPI	INT WINAPI	EncSetEventMask( HANDLE hDeviceHandle, INT nChannel, INT nEventMask );
FBIENCAPI	INT WINAPI	EncGetEventMask( HANDLE hDeviceHandle, INT nChannel, PINT pnEventMask );
FBIENCAPI	INT WINAPI	EncSetEvent( HANDLE hDeviceHandle, LPENCCALLBACK lpEventProc, DWORD  dwUser );
FBIENCAPI	INT WINAPI	EncKillEvent( HANDLE hDeviceHandle );
FBIENCAPI	INT WINAPI	EncSetTimerConfig( HANDLE hDeviceHandle,	BYTE bTimerConfigValue );
FBIENCAPI	INT WINAPI	EncGetTimerConfig( HANDLE hDeviceHandle, PBYTE pbTimerConfigValue );
FBIENCAPI	INT WINAPI	EncGetTimerCount( HANDLE hDeviceHandle, PBYTE pbTimerCount );
FBIENCAPI	INT WINAPI	EncGetStatus( HANDLE hDeviceHandle, INT nChannel, PINT pnStatus );
FBIENCAPI	INT WINAPI	EncReset( HANDLE hDeviceHandle );

#ifdef __cplusplus
}
#endif

#endif

//.FBIENC.H
