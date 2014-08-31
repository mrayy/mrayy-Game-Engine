///////////////////////////////////////////////////////////////////////////////
//																		       
//  Windows NT/98/95     FbiMl.h    Include file for Memolink Driver
//				   
//	 	    ������`�t�@�C��
//																		       
///////////////////////////////////////////////////////////////////////////////
#if !defined( _FBIML_H_ )
#define _FBIML_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (CALLBACK MLCALLBACK)(DWORD dwMlEvent, DWORD dwUser);
typedef MLCALLBACK FAR *LPMLCALLBACK;

// -----------------------------------------------------------------------
//	�I�[�v���t���O���ʎq
// -----------------------------------------------------------------------
#define	ML_FLAG_NORMAL			0x0000
#define	ML_FLAG_SHARE			0x0002

// -----------------------------------------------------------------------
//	�p���e�B�G���[���ʎq
// -----------------------------------------------------------------------
#define ML_PARITY_NONE			0		// �p���e�B�G���[�Ȃ�
#define ML_PARITY_WRITE			1		// �������݃p���e�B�G���[����
#define ML_PARITY_READ			2		// �ǂݍ��݃p���e�B�G���[����
#define ML_PARITY_BOTH			3		// ��������/�ǂݍ��݃p���e�B�G���[����

// -----------------------------------------------------------------------
//	�߂�l
// -----------------------------------------------------------------------
#define	ML_ERROR_SUCCESS				0			// ����I��
#define ML_ERROR_NOT_DEVICE				0xC0000001	// �w�肳�ꂽ�f�o�C�X�������邱�Ƃ��ł��܂���
#define	ML_ERROR_NOT_OPEN				0xC0000002	// �V�X�e�����f�o�C�X���I�[�v���ł��܂���
#define ML_ERROR_INVALID_HANDLE			0xC0000003	// �f�o�C�X�n���h��������������܂���
#define ML_ERROR_ALREADY_OPEN			0xC0000004  // ���ł�OPEN���Ă���f�o�C�X��OPEN���悤�Ƃ��܂���
#define ML_ERROR_HANDLE_EOF				0xC0000005	// EOF�ɒB���܂���
#define ML_ERROR_MORE_DATA				0xC0000006	// ����ɑ����̃f�[�^�����p�\�ł�
#define ML_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// �V�X�e���R�[���ɓn���ꂽ�f�[�^�̈悪���������܂�
#define ML_ERROR_IO_PENDING				0xC0000008	// �񓯊�I/O���삪�i�s���ł�
#define ML_ERROR_NOT_SUPPORTED			0xC0000009	// �T�|�[�g����Ă��Ȃ��@�\�ł�

#define ML_ERROR_INITIALIZE_IRQ			0xC0001000	// ���荞�݂̏������Ɏ��s���܂���
#define	ML_ERROR_INVALID_SLAVENUM		0xC0001001	// �s���ȃX���[�u�ԍ����w�肵�܂���
#define	ML_ERROR_INVALID_OFFSET			0xC0001002	// �s���ȃI�t�Z�b�g���w�肵�܂���
#define	ML_ERROR_INVALID_VALUE			0xC0001003	// �s���ȏ������ݒl���w�肵�܂���
#define	ML_ERROR_INVALID_SIZE			0xC0001004	// �s���ȃT�C�Y���w�肵�܂���
#define	ML_ERROR_INVALID_ACCONTROL		0xC0001005	// �s���Ȃ`�b�R���g���[���ݒ�l���w�肵�܂���
#define	ML_ERROR_INVALID_MASK			0xC0001006	// �s���ȃC�x���g�}�X�N���w�肵�܂���
#define ML_ERROR_ALREADY_REGISTRATION	0xC0001007	// �C�x���g�͂��łɓo�^�ς݂ł�
#define ML_ERROR_ALREADY_DELETE			0xC0001008	// �C�x���g�͂��łɍ폜����Ă��܂�
#define	ML_ERROR_MEMORY_NOTALLOCATED	0xC0001009	// ��Ɨp�������̊m�ۂɎ��s���܂���	
#define	ML_ERROR_MEMORY_FREE			0xC000100A	// �������̉���Ɏ��s���܂���	
#define	ML_ERROR_TIMER					0xC000100B	// �^�C�}���\�[�X�̎擾�Ɏ��s���܂���
#define	ML_ERROR_DRVCAL					0xC000100C	// �h���C�o���Ăяo���܂���
#define	ML_ERROR_NULL_POINTER			0xC000100D	// �h���C�o�ADLL�Ԃ�NULL�|�C���^���n����܂���
#define ML_ERROR_PARAMETER				0xC000100E	// �����p�����[�^�̒l���s���ł�


#if !defined(_FBIMLLIB_)
#define FBIMLAPI
#else
#define FBIMLAPI __declspec(dllexport)
#endif


FBIMLAPI	HANDLE WINAPI	MlOpen( LPCTSTR lpszName, DWORD  fdwFlags );
FBIMLAPI	INT WINAPI		MlClose( HANDLE hDeviceHandle );
FBIMLAPI	INT WINAPI		MlGetInformation( HANDLE hDeviceHandle, PINT pnBoardType, PINT pnRswNo );
FBIMLAPI	INT WINAPI		MlReadByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PBYTE pbValue );
FBIMLAPI	INT WINAPI		MlReadWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PWORD pwValue );
FBIMLAPI	INT WINAPI		MlWriteByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, BYTE bValue );
FBIMLAPI	INT WINAPI		MlWriteWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, WORD wValue );
FBIMLAPI	INT WINAPI		MlFillByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, BYTE bValue, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlFillWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, WORD wValue, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlReadMemoryByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pDistination, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlReadMemoryWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pDistination, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlWriteMemoryByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pSource, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlWriteMemoryWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwOffset, PVOID pSource, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlCopyMemoryByte( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwSourceOffset, DWORD dwDistinationOffset, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlCopyMemoryWord( HANDLE hDeviceHandle, INT nSlaveNo, DWORD dwSourceOffset, DWORD dwDistinationOffset, DWORD dwSize );
FBIMLAPI	INT WINAPI		MlSetEventMask( HANDLE hDeviceHandle, INT nEventMask );
FBIMLAPI	INT WINAPI		MlGetEventMask( HANDLE hDeviceHandle, PINT pnEventMask );
FBIMLAPI	INT WINAPI		MlSetEvent( HANDLE hDeviceHandle, LPMLCALLBACK lpEventProc, DWORD  dwUser );
FBIMLAPI	INT WINAPI		MlKillEvent( HANDLE hDeviceHandle );
FBIMLAPI	INT WINAPI		MlGenerateIrq( HANDLE hDeviceHandle, INT nSlaveNo );
FBIMLAPI	INT WINAPI		MlSetAcControl( HANDLE hDeviceHandle, INT nSlaveNo, INT nAcControl );
FBIMLAPI	INT WINAPI		MlGetAcControl( HANDLE hDeviceHandle, INT nSlaveNo, PINT pnAcControl );
FBIMLAPI	INT WINAPI		MlSetSlaveNo( HANDLE hDeviceHandle, INT nSlaveNo );
FBIMLAPI	INT WINAPI		MlGetSlaveNo( HANDLE hDeviceHandle, PINT pnSlaveNo );
FBIMLAPI	INT WINAPI		MlParityCheck( HANDLE hDeviceHandle, PINT pnParityError );

FBIMLAPI	INT WINAPI		MlSetEventEx( HANDLE hDeviceHandle, HANDLE hEvent, HWND hWnd, UINT uMsg, LPMLCALLBACK lpEventProc, DWORD dwUser );
FBIMLAPI    INT WINAPI      MlGetEventStat( HANDLE hDeviceHandle, DWORD *dwEvent );

#ifdef __cplusplus
}
#endif

#endif

//.FBIML.H
