/*******************************************************************************

	Project        : TexART NCord-System

	Component      : NCord-System PCI Driver

	Module         : TexARTNCorddriverif.h

	Function       : interface header

-------------------------------------------------------------------------------
	[history]
	2009-06-01	Ver 1.00   M.Shiroma    �V�K�쐬

******************************************************************************/
#ifndef		_TEXARTNCORDDRIVERIF_H_				//�Q�d��`�֎~
#define		_TEXARTNCORDDRIVERIF_H_
//*******************************************/
//*    include								*/
//*******************************************/
#ifndef	DRIVER
#include	<windows.h>
#include	<winioctl.h>
#endif
//*******************************************/
//*    define								*/
//*******************************************/
// �f�o�C�X��
#define		NCORD_DEVICE_NAME		"\\\\.\\NCord-%d"
// �f�[�^�T�C�Y
#define		NCORD_DATA_SIZE			(32)			// �]���f�[�^�T�C�Y
#define		MAX_TERMINAL			(8)				// �ő�^�[�~�i����

// �����J�E���g���O�p
#define		LOG_AREA_SIZE			(64*1024)		// LOG Area Size(128Kbyte)

// �R�}���h�y�у��X�|���X
#define		NCORD_CMND_GAIN			(0x0001)		// ���Q�C���R�}���h(���[�^����p�p�����[�^)
#define		NCORD_CMND_DRV			(0x0002)		// �h���C�o�R�}���h
#define		NCORD_CMND_TRM_RST		(0x0003)		// �^�[�~�i�����Z�b�g�R�}���h
#define		NCORD_CMND_CNT_RST		(0x0004)		// �J�E���^���Z�b�g�R�}���h
#define		NCORD_CMND_TRM_SRT		(0x0010)		// �^�[�~�i���X�^�[�g�R�}���h
#define		NCORD_CMND_TRM_STP		(0x0020)		// �^�[�~�i���X�g�b�v�R�}���h
#define		NCORD_RESP_GAIN			(0x8001)		// ���Q�C�����X�|���X
#define		NCORD_RESP_DRV			(0x8002)		// �h���C�o���X�|���X
#define		NCORD_RESP_TRM_RST		(0x8003)		// �^�[�~�i�����Z�b�g���X�|���X
#define		NCORD_RESP_CNT_RST		(0x8004)		// �J�E���^���Z�b�g���X�|���X
#define		NCORD_RESP_TRM_SRT		(0x8010)		// �^�[�~�i���X�^�[�g���X�|���X
#define		NCORD_RESP_TRM_STP		(0x8020)		// �^�[�~�i���X�g�b�v���X�|���X

#define		BIT_CYCLE_INTR			(0x10000000)	// ���������ݗp�r�b�g
#define		BIT_RECV_COMPLETE		(0x00000100)	// ��M�����ݑS�R���g���[���[����
#define		BIT_RECV_CH1			(0x00000001)	// CH1 ��M����
#define		BIT_RECV_CH2			(0x00000002)	// CH2 ��M����
#define		BIT_RECV_CH3			(0x00000004)	// CH3 ��M����
#define		BIT_RECV_CH4			(0x00000008)	// CH4 ��M����
#define		BIT_RECV_CH5			(0x00000010)	// CH5 ��M����
#define		BIT_RECV_CH6			(0x00000020)	// CH6 ��M����
#define		BIT_RECV_CH7			(0x00000040)	// CH7 ��M����
#define		BIT_RECV_CH8			(0x00000080)	// CH8 ��M����
#define		BIT_CH1_CRC_ERR			(0x00001000)	// CH1 CRC ERROR����
#define		BIT_CH2_CRC_ERR			(0x00002000)	// CH2 CRC ERROR����
#define		BIT_CH3_CRC_ERR			(0x00004000)	// CH3 CRC ERROR����
#define		BIT_CH4_CRC_ERR			(0x00008000)	// CH4 CRC ERROR����
#define		BIT_CH5_CRC_ERR			(0x00010000)	// CH5 CRC ERROR����
#define		BIT_CH6_CRC_ERR			(0x00020000)	// CH6 CRC ERROR����
#define		BIT_CH7_CRC_ERR			(0x00040000)	// CH7 CRC ERROR����
#define		BIT_CH8_CRC_ERR			(0x00080000)	// CH8 CRC ERROR����
#define		BIT_CH1_OVRFLOW			(0x00100000)	// CH1 �I�[�o�[�t���[����
#define		BIT_CH2_OVRFLOW			(0x00200000)	// CH2 �I�[�o�[�t���[����
#define		BIT_CH3_OVRFLOW			(0x00400000)	// CH3 �I�[�o�[�t���[����
#define		BIT_CH4_OVRFLOW			(0x00800000)	// CH4 �I�[�o�[�t���[����
#define		BIT_CH5_OVRFLOW			(0x01000000)	// CH5 �I�[�o�[�t���[����
#define		BIT_CH6_OVRFLOW			(0x02000000)	// CH6 �I�[�o�[�t���[����
#define		BIT_CH7_OVRFLOW			(0x04000000)	// CH7 �I�[�o�[�t���[����
#define		BIT_CH8_OVRFLOW			(0x08000000)	// CH8 �I�[�o�[�t���[����
//*******************************************/
//*    struct								*/
//*******************************************/
// �����J�E���g���O�p M.S
typedef	struct	_CYCLE_COUNTER_LOG
{
	USHORT				Location;			// Log�����݈ʒu
	USHORT				Log[LOG_AREA_SIZE];	// Log�o�b�t�@
}
CYCLE_COUNTER_LOG,*PCYCLE_COUNTER_LOG;
typedef	struct	_CYCLE_COUNTER
{
	USHORT				Counter;
	CYCLE_COUNTER_LOG	CountLog;
}
CYCLE_COUNTER,*PCYCLE_COUNTER;

typedef	struct
{
	ULONG			value;				// �f�[�^
	ULONG			offset;				// offset
#define		REG_SEND_CH1_MEM		(0x0000)	// Ch1(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH1_MEM		(0x0040)	// Ch1(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH2_MEM		(0x0080)	// Ch2(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH2_MEM		(0x00C0)	// Ch2(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH3_MEM		(0x0100)	// Ch3(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH3_MEM		(0x0140)	// Ch3(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH4_MEM		(0x0180)	// Ch4(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH4_MEM		(0x01C0)	// Ch4(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH5_MEM		(0x0200)	// Ch1(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH5_MEM		(0x0240)	// Ch1(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH6_MEM		(0x0280)	// Ch2(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH6_MEM		(0x02C0)	// Ch2(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH7_MEM		(0x0300)	// Ch3(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH7_MEM		(0x0340)	// Ch3(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_SEND_CH8_MEM		(0x0380)	// Ch4(RW) ���M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define		REG_RECV_CH8_MEM		(0x03C0)	// Ch4(R ) ��M�������J�n�I�t�Z�b�g (�����l0x00��fill)
#define			OFFSET_MEM_NEXT			(0x0080)	// Next Ch Offset
#define		REG_SEND_CTRL			(0x0800)	// ���M���䃌�W�X�^
#define			BIT_CH1_SEND			(0x01)		// CH1 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH2_SEND			(0x02)		// CH2 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH3_SEND			(0x04)		// CH3 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH4_SEND			(0x08)		// CH4 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH5_SEND			(0x10)		// CH5 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH6_SEND			(0x20)		// CH6 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH7_SEND			(0x40)		// CH7 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define			BIT_CH8_SEND			(0x80)		// CH8 ���M�J�n�w�� (W) 1:���M�J�n 0:���M��~ / (R) 1:���M��
#define		REG_RECV_STATUS			(0x0840)	// ��M�X�e�[�^�X���W�X�^
#define			BIT_CH1_DTRDY			(0x00000001)	// 1:CH1 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH1_CRCERR			(0x00000002)	// 1:CH1 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH1_OVERR			(0x00000004)	// 1:CH1 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH2_DTRDY			(0x00000010)	// 1:CH2 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH2_CRCERR			(0x00000020)	// 1:CH2 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH2_OVERR			(0x00000040)	// 1:CH2 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH3_DTRDY			(0x00000100)	// 1:CH3 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH3_CRCERR			(0x00000200)	// 1:CH3 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH3_OVERR			(0x00000400)	// 1:CH3 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH4_DTRDY			(0x00001000)	// 1:CH4 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH4_CRCERR			(0x00002000)	// 1:CH4 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH4_OVERR			(0x00004000)	// 1:CH4 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH5_DTRDY			(0x00010000)	// 1:CH5 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH5_CRCERR			(0x00020000)	// 1:CH5 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH5_OVERR			(0x00040000)	// 1:CH5 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH6_DTRDY			(0x00100000)	// 1:CH6 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH6_CRCERR			(0x00200000)	// 1:CH6 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH6_OVERR			(0x00400000)	// 1:CH6 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH7_DTRDY			(0x01000000)	// 1:CH7 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH7_CRCERR			(0x02000000)	// 1:CH7 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH7_OVERR			(0x04000000)	// 1:CH7 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH8_DTRDY			(0x10000000)	// 1:CH8 ��M�f�[�^�������� '1'���C�g�Ŏ�M���荞�݃N���A
#define			BIT_CH8_CRCERR			(0x20000000)	// 1:CH8 ��M�f�[�^CRC ERROR���� '1'���C�g�ŃG���[�N���A
#define			BIT_CH8_OVERR			(0x40000000)	// 1:CH8 ��M�f�[�^�I�[�o�[�t���[ ERROR���� '1'���C�g�ŃG���[�N���A
#define		REG_READ_END			(0x0844)	// �ǂݎ�芮�����W�X�^
#define			BIT_CH1_RDEND			(0x0001)	// 1:CH1��M�f�[�^�ǂݎ�芮��
#define			BIT_CH2_RDEND			(0x0002)	// 1:CH2��M�f�[�^�ǂݎ�芮��
#define			BIT_CH3_RDEND			(0x0004)	// 1:CH3��M�f�[�^�ǂݎ�芮��
#define			BIT_CH4_RDEND			(0x0008)	// 1:CH4��M�f�[�^�ǂݎ�芮��
#define			BIT_CH5_RDEND			(0x0010)	// 1:CH5��M�f�[�^�ǂݎ�芮��
#define			BIT_CH6_RDEND			(0x0020)	// 1:CH6��M�f�[�^�ǂݎ�芮��
#define			BIT_CH7_RDEND			(0x0040)	// 1:CH7��M�f�[�^�ǂݎ�芮��
#define			BIT_CH8_RDEND			(0x0080)	// 1:CH8��M�f�[�^�ǂݎ�芮��
#define		REG_ALARM_CH1			(0x0850)	// Ch1 �A���[�����W�X�^
#define		REG_ALARM_CH2			(0x0854)	// Ch2 �A���[�����W�X�^
#define		REG_ALARM_CH3			(0x0858)	// Ch3 �A���[�����W�X�^
#define		REG_ALARM_CH4			(0x085C)	// Ch4 �A���[�����W�X�^
#define		REG_ALARM_CH5			(0x0860)	// Ch5 �A���[�����W�X�^
#define		REG_ALARM_CH6			(0x0864)	// Ch6 �A���[�����W�X�^
#define		REG_ALARM_CH7			(0x0868)	// Ch7 �A���[�����W�X�^
#define		REG_ALARM_CH8			(0x086C)	// Ch8 �A���[�����W�X�^
#define			BIT_MTALM1				(0x00000001)	// 1:���[�^1�ŃA���[������
#define			BIT_MTALM2				(0x00000002)	// 1:���[�^2�ŃA���[������
#define			BIT_MTALM3				(0x00000004)	// 1:���[�^3�ŃA���[������
#define			BIT_MTALM4				(0x00000008)	// 1:���[�^4�ŃA���[������
#define			BIT_MTALM5				(0x00000010)	// 1:���[�^5�ŃA���[������
#define			BIT_MTALM6				(0x00000020)	// 1:���[�^6�ŃA���[������
#define			BIT_MTALM7				(0x00000040)	// 1:���[�^7�ŃA���[������
#define			BIT_MTALM8				(0x00000080)	// 1:���[�^8�ŃA���[������
#define			BIT_MTALM9				(0x00000100)	// 1:���[�^9�ŃA���[������
#define			BIT_MTALM10				(0x00000200)	// 1:���[�^10�ŃA���[������
#define			BIT_MTALM11				(0x00000400)	// 1:���[�^11�ŃA���[������
#define			BIT_MTALM12				(0x00000800)	// 1:���[�^12�ŃA���[������
#define			BIT_MTALM13				(0x00001000)	// 1:���[�^13�ŃA���[������
#define			BIT_MTALM14				(0x00002000)	// 1:���[�^14�ŃA���[������
#define			BIT_MTALM15				(0x00004000)	// 1:���[�^15�ŃA���[������
#define			BIT_MTALM16				(0x00008000)	// 1:���[�^16�ŃA���[������
#define			BIT_TMNL_CRCERR			(0x00010000)	// 1:Terminal���Ŏ�MCRC ERROR����
#define		REG_TIMER_SET			(0x0880)	// �^�C�}���W�X�^ 100��s�^�C�}�̃J�E���g�A�b�v���Ԃ�ݒ�
#define			BIT_TIMER_STOP			(0x0000)	// �^�C�}��~
#define			BIT_TIMER_100			(0x0001)		// 100��s 0x0002:200��s 0x0003:300��s 0xFFFF:6553500��s
#define		REG_TIMER_STATUS		(0x0884)	// �^�C�}�[���荞�݃X�e�[�^�X���W�X�^
#define			BIT_TIMINTON			(0x0001)	// 1:�^�C�}���荞�݃N���A
#define		REG_INTR_STATUS			(0x0900)	//���荞�݃X�e�[�^�X���W�X�^
#define			BIT_CH1_TXINT			(0x00000001)	// 1:CH1�f�[�^���M�I�����荞��
#define			BIT_CH2_TXINT			(0x00000002)	// 1:CH2�f�[�^���M�I�����荞��
#define			BIT_CH3_TXINT			(0x00000004)	// 1:CH3�f�[�^���M�I�����荞��
#define			BIT_CH4_TXINT			(0x00000008)	// 1:CH4�f�[�^���M�I�����荞��
#define			BIT_CH5_TXINT			(0x00000010)	// 1:CH5�f�[�^���M�I�����荞��
#define			BIT_CH6_TXINT			(0x00000020)	// 1:CH6�f�[�^���M�I�����荞��
#define			BIT_CH7_TXINT			(0x00000040)	// 1:CH7�f�[�^���M�I�����荞��
#define			BIT_CH8_TXINT			(0x00000080)	// 1:CH8�f�[�^���M�I�����荞��
#define			BIT_CH1_RXINT			(0x00000100)	// 1:CH1�f�[�^��M�I�����荞��
#define			BIT_CH2_RXINT			(0x00000200)	// 1:CH2�f�[�^��M�I�����荞��
#define			BIT_CH3_RXINT			(0x00000400)	// 1:CH3�f�[�^��M�I�����荞��
#define			BIT_CH4_RXINT			(0x00000800)	// 1:CH4�f�[�^��M�I�����荞��
#define			BIT_CH5_RXINT			(0x00001000)	// 1:CH5�f�[�^��M�I�����荞��
#define			BIT_CH6_RXINT			(0x00002000)	// 1:CH6�f�[�^��M�I�����荞��
#define			BIT_CH7_RXINT			(0x00004000)	// 1:CH7�f�[�^��M�I�����荞��
#define			BIT_CH8_RXINT			(0x00008000)	// 1:CH8�f�[�^��M�I�����荞��
#define			BIT_TIMINT				(0x80000000)	// 1:�^�C�}���荞��
#define		REG_INTR_MASK			(0x0904)	// ���荞�݃}�X�N���W�X�^
#define			BIT_CH1_TXINTMSK		(0x00000001)	// 0:CH1�f�[�^���M�I�����荞�ݗL�� 1:CH1�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH2_TXINTMSK		(0x00000002)	// 0:CH2�f�[�^���M�I�����荞�ݗL�� 1:CH2�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH3_TXINTMSK		(0x00000004)	// 0:CH3�f�[�^���M�I�����荞�ݗL�� 1:CH3�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH4_TXINTMSK		(0x00000008)	// 0:CH4�f�[�^���M�I�����荞�ݗL�� 1:CH3�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH5_TXINTMSK		(0x00000010)	// 0:CH5�f�[�^���M�I�����荞�ݗL�� 1:CH1�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH6_TXINTMSK		(0x00000020)	// 0:CH6�f�[�^���M�I�����荞�ݗL�� 1:CH2�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH7_TXINTMSK		(0x00000040)	// 0:CH7�f�[�^���M�I�����荞�ݗL�� 1:CH3�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH8_TXINTMSK		(0x00000080)	// 0:CH8�f�[�^���M�I�����荞�ݗL�� 1:CH3�f�[�^���M�I�����荞�݃}�X�N
#define			BIT_CH1_RXINTMSK		(0x00000100)	// 0:CH1�f�[�^��M�I�����荞�ݗL�� 1:CH1�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH2_RXINTMSK		(0x00000200)	// 0:CH2�f�[�^��M�I�����荞�ݗL�� 1:CH2�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH3_RXINTMSK		(0x00000400)	// 0:CH3�f�[�^��M�I�����荞�ݗL�� 1:CH3�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH4_RXINTMSK		(0x00000800)	// 0:CH4�f�[�^��M�I�����荞�ݗL�� 1:CH3�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH5_RXINTMSK		(0x00001000)	// 0:CH5�f�[�^��M�I�����荞�ݗL�� 1:CH1�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH6_RXINTMSK		(0x00002000)	// 0:CH6�f�[�^��M�I�����荞�ݗL�� 1:CH2�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH7_RXINTMSK		(0x00004000)	// 0:CH7�f�[�^��M�I�����荞�ݗL�� 1:CH3�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_CH8_RXINTMSK		(0x00008000)	// 0:CH8�f�[�^��M�I�����荞�ݗL�� 1:CH3�f�[�^��M�I�����荞�݃}�X�N
#define			BIT_TIMINTMSK			(0x80000000)	// 0:�^�C�}���荞�ݗL�� 1�^�C�}���荞�݃}�X�N
#define		REG_DIPSWITCH			(0x0A00)	// DIP SWITCH���W�X�^
#define			BIT_DIP1				(0x0001)	// DIP SW1 1: ON 0:OFF
#define			BIT_DIP2				(0x0002)	// DIP SW2 1: ON 0:OFF
#define			BIT_DIP3				(0x0004)	// DIP SW3 1: ON 0:OFF
#define			BIT_DIP4				(0x0008)	// DIP SW4 1: ON 0:OFF
#define			BIT_DIP5				(0x0010)	// DIP SW5 1: ON 0:OFF
#define			BIT_DIP6				(0x0020)	// DIP SW6 1: ON 0:OFF
#define			BIT_DIP7				(0x0040)	// DIP SW7 1: ON 0:OFF
#define			BIT_DIP8				(0x0080)	// DIP SW8 1: ON 0:OFF
#define		REG_LED					(0x0A04)	// LED���W�X�^
#define		REG_DUMMY_COM			(0x0FF0)	// �_�~�[�ʐM���W�X�^
#define		REG_RESET				(0x0FFC)	// RESET���W�X�^
}
NCORD_REG;

typedef	struct
{
	USHORT				CmndResp[NCORD_DATA_SIZE];
}
COMMAND_RESP,*PCOMMAND_RESP;
typedef	struct
{
	USHORT			flag;							// �����^�C�}�[�ɓ�������ꍇ��TRUE��ݒ�
	USHORT			terminal_count;					// �R�}���h���X�|���X�̈�̌�
	COMMAND_RESP	cmnd_resp[MAX_TERMINAL];		// �R�}���h���X�|���X�̈�
}
NCORD_COMMAND,*PNCORD_COMMAND;
//*******************************************/
//*    control code							*/
//*******************************************/
#define		DEV_TYPE				430226
//IOCTL�̃x�[�X�ԍ�
#define		NCORD_IOCTL_BASE	0x1000
#define	IOCTL_COMMAND \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+0,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define	IOCTL_GETLOG \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+1,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define	IOCTL_WRITE_REG \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define	IOCTL_READ_REG \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+3,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define	IOCTL_READ_DEV_EXT \
	CTL_CODE(DEV_TYPE,NCORD_IOCTL_BASE+4,METHOD_BUFFERED,FILE_ANY_ACCESS)

#endif
