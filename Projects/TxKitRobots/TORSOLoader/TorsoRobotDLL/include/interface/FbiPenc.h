// -----------------------------------------------------------------------
//
//		GPC-6204	��`�t�@�C��
//
//		FbiPenc.h
//
//		Copyright 1999-2000 Interface Corporation. All rights reserved.
// -----------------------------------------------------------------------
#include	<Winioctl.h>

#if !defined( _FBIPENC_H_ )
#define _FBIPENC_H_

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------
//	�R�[���o�b�N�֐�
//
//		LPENCCALLBACK		PencSetEvent �ɂ�麰��ޯ��ւ��߲��
//		LPENCCALLBACKEX		PencSetEventEx �ɂ�麰��ޯ��ւ��߲��
//
// -----------------------------------------------------------------------
typedef void (CALLBACK PENCCALLBACK)(DWORD dwPencEvent, DWORD dwUser);
typedef PENCCALLBACK *LPPENCCALLBACK;

typedef void (CALLBACK PENCCALLBACKEX)(INT nChannel,DWORD dwPencEvent,DWORD dwUser);
typedef PENCCALLBACKEX *LPPENCCALLBACKEX;

// -----------------------------------------------------------------------
//	�I�[�v���t���O���ʎq
// -----------------------------------------------------------------------
#define	PENC_FLAG_NORMAL		0x0000
#define	PENC_FLAG_SHARE			0x0002

// -----------------------------------------------------------------------
//		�f�o�C�X���
// -----------------------------------------------------------------------
typedef struct {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test
    union {
        struct {
            ULONG   BaseAddresses[6];
            ULONG   CardBusCISPointer;
			ULONG	SubsystemVenderID;
			ULONG	SubsystemID;
            ULONG   ROMBaseAddress;
            ULONG   Reserved2[2];
            UCHAR   InterruptLine;      //
            UCHAR   InterruptPin;       // (ro)
            UCHAR   MinimumGrant;       // (ro)
            UCHAR   MaximumLatency;     // (ro)
        } type0;
    } u;
    UCHAR   DeviceSpecific[192];
} FBIPENCINFORMATION, *PFBIPENCINFORMATION;

// -----------------------------------------------------------------------
//	�߂�l
// -----------------------------------------------------------------------
#define	PENC_ERROR_SUCCESS				0			// ����I��
#define PENC_ERROR_NOT_DEVICE			0xC0000001	// �w�肳�ꂽ�f�o�C�X�������邱�Ƃ��ł��܂���
#define	PENC_ERROR_NOT_OPEN				0xC0000002	// �V�X�e�����f�o�C�X���I�[�v���ł��܂���
#define PENC_ERROR_INVALID_HANDLE		0xC0000003	// �f�o�C�X�n���h��������������܂���
#define PENC_ERROR_ALREADY_OPEN			0xC0000004  // ���ł�OPEN���Ă���f�o�C�X��OPEN���悤�Ƃ��܂���
#define PENC_ERROR_HANDLE_EOF			0xC0000005	// EOF�ɒB���܂���
#define PENC_ERROR_MORE_DATA			0xC0000006	// ����ɑ����̃f�[�^�����p�\�ł�
#define PENC_ERROR_INSUFFICIENT_BUFFER	0xC0000007	// �V�X�e���R�[���ɓn���ꂽ�f�[�^�̈悪���������܂�
#define PENC_ERROR_IO_PENDING			0xC0000008	// �񓯊�I/O���삪�i�s���ł�
#define PENC_ERROR_NOT_SUPPORTED		0xC0000009	// �T�|�[�g����Ă��Ȃ��@�\�ł�
#define PENC_ERROR_INITIALIZE_IRQ		0xC0001000	// ���荞�݂̏������Ɏ��s���܂���
#define	PENC_ERROR_INVALID_CHANNEL		0xC0001001	// �s���ȃ`�����l���ԍ����w�肵�܂���
#define	PENC_ERROR_INVALID_MODE			0xC0001002	// �s���ȃ��[�h���w�肵�܂���
#define	PENC_ERROR_INVALID_DIRECT		0xC0001003	// �s���ȃJ�E���^�������w�肵�܂���
#define	PENC_ERROR_INVALID_EQUALS		0xC0001004	// �s���Ȉ�v���o�t���O���w�肵�܂���
#define	PENC_ERROR_INVALID_LATCH		0xC0001005	// �s���ȃ��b�`�ݒ�l���w�肵�܂���
#define	PENC_ERROR_INVALID_COUNTER		0xC0001006	// �s���ȃJ�E���^�l���w�肵�܂���
#define	PENC_ERROR_INVALID_COMPARATOR	0xC0001007	// �s���Ȕ�r�J�E���^�l���w�肵�܂���
#define	PENC_ERROR_INVALID_ZMODE		0xC0001008	// �s���Ȃy���_���ݒ�l���w�肵�܂���
#define	PENC_ERROR_INVALID_MASK			0xC0001009	// �s���ȃC�x���g�}�X�N���w�肵�܂���
#define	PENC_ERROR_INVALID_ITIMER		0xC000100A	// �s���ȃC���^�[�o���^�C�}�ݒ�l���w�肵�܂���
#define PENC_ERROR_ALREADY_REGISTRATION	0xC000100B	// �C�x���g�͂��łɓo�^�ς݂ł�
#define PENC_ERROR_ALREADY_DELETE		0xC000100C	// �C�x���g�͂��łɍ폜����Ă��܂�
#define	PENC_ERROR_MEMORY_NOTALLOCATED	0xC000100D	// ��Ɨp�������̊m�ۂɎ��s���܂���	
#define	PENC_ERROR_MEMORY_FREE			0xC000100E	// �������̉���Ɏ��s���܂���	
#define	PENC_ERROR_TIMER				0xC000100F	// �^�C�}���\�[�X�̎擾�Ɏ��s���܂���
#define	PENC_ERROR_DRVCAL				0xC0001010	// �h���C�o���Ăяo���܂���
#define	PENC_ERROR_NULL_POINTER			0xC0001011	// �h���C�o�ADLL�Ԃ�NULL�|�C���^���n����܂���
#define PENC_ERROR_PARAMETER			0xC0001012	// �����p�����[�^�̒l���s���ł�

#if !defined(_FBIPENCLIB_)
#define FBIPENCAPI
#else
#define FBIPENCAPI __declspec(dllexport)
#endif

FBIPENCAPI HANDLE WINAPI PencOpen(LPCTSTR lpszName,DWORD fdwFlags);
FBIPENCAPI INT WINAPI PencClose(HANDLE hDevice);
FBIPENCAPI INT WINAPI PencSetMode(HANDLE hDevice,INT nChannel,INT nMode,INT nDirection,INT nEqual,INT nLatch);
FBIPENCAPI INT WINAPI PencGetMode(HANDLE hDevice,INT nChannel,PINT pnMode,PINT pnDirection,PINT nEqual,PINT nLatch);
FBIPENCAPI INT WINAPI PencSetCounter(HANDLE hDevice,INT nChannel,DWORD dwCounter);
FBIPENCAPI INT WINAPI PencGetCounter(HANDLE hDevice,INT nChannel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencSetCounterEx(HANDLE hDevice,DWORD dwChSel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencGetCounterEx(HANDLE hDevice,DWORD dwChSel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencSetComparator(HANDLE hDevice,INT nChannel,DWORD dwCounter);
FBIPENCAPI INT WINAPI PencGetComparator(HANDLE hDevice,INT nChannel,PDWORD pdwCounter);
FBIPENCAPI INT WINAPI PencSetZMode(HANDLE hDevice,INT nChannel,INT nZMode);
FBIPENCAPI INT WINAPI PencGetZMode(HANDLE hDevice,INT nChannel,PINT pnZMode);
FBIPENCAPI INT WINAPI PencGetStatus(HANDLE hDevice,INT nChannel,PINT pnStatus);
FBIPENCAPI INT WINAPI PencGetStatusEx(HANDLE hDevice,DWORD dwChSel,PDWORD pdwCounter,PDWORD pdwStatus);
FBIPENCAPI INT WINAPI PencEnableCount(HANDLE hDevice,DWORD dwChSel,INT nEnable);
FBIPENCAPI INT WINAPI PencReset(HANDLE hDevice,INT nChannel);
FBIPENCAPI INT WINAPI PencSetEventMask(HANDLE hDevice,INT nChannel,INT nEventMask,INT nTimerMask);
FBIPENCAPI INT WINAPI PencGetEventMask(HANDLE hDevice,INT nChannel,PINT pnEventMask,PINT pnTimerMask);
FBIPENCAPI INT WINAPI PencSetEvent(HANDLE hDevice,LPPENCCALLBACK lpEventProc,DWORD dwUser);
FBIPENCAPI INT WINAPI PencSetEventEx(HANDLE hDevice,LPPENCCALLBACKEX lpEventProcEx,DWORD dwUser);
FBIPENCAPI INT WINAPI PencKillEvent(HANDLE hDevice);
FBIPENCAPI INT WINAPI PencEventRequestPending(HANDLE hDevice,INT nChannel,DWORD dwEventMask,PDWORD pdwEventBuf,LPOVERLAPPED lpOverlapped);
FBIPENCAPI INT WINAPI PencSetTimerConfig(HANDLE hDevice,BYTE bTimerConfig);
FBIPENCAPI INT WINAPI PencGetTimerConfig(HANDLE hDevice,PBYTE pbTimerConfig);
FBIPENCAPI INT WINAPI PencGetTimerCount(HANDLE hDevice,PBYTE pbTimerCount);

#ifdef __cplusplus
}
#endif

#endif

//.FBIPENC.H
