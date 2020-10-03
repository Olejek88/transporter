// ����������� ��������
#define		RECV	0x00		// �����
#define		SEND	0x01		// ��������
// ����������� �������
#define		SOH		0x01
#define		STX		0x02
#define		ETX		0x03
#define		EOT		0x04
// ���� ������
#define		OK					0x41
#define		PARITY_ERROR		0x42
#define		CRC_ERROR			0x43
#define		ERROR_SIGN			0x44
#define		ERROR_FUNC			0x45
#define		ERROR_DATA			0x46

#define		A_OK				0x61
#define		A_PARITY_ERROR		0x62
#define		A_CRC_ERROR			0x63
#define		A_ERROR_SIGN		0x64
#define		A_ERROR_FUNC		0x65
#define		A_ERROR_DATA		0x66

// ���� ������
#define		STATUS				0x01
#define		CURRENT_ERROR		0x02
#define		COM4		0x04
#define		COM5		0x05
#define		COM6		0x06
#define		COM7		0x07
#define		COM8		0x08
#define		COM9		0x09
#define		COMIO		0x12			// ������� ������� � �������� �������
#define		ARCHIVE_ERROR		0x33

// ���������
#define		DAT1_FACT		0x00		// ����������� ���������
#define		DAT1_FREQ		0x01		// ������� �������� ���������
#define		DAT1_RASS		0x02		// ��������������� (� ���������)
#define		DAT1_IMPZ		0x03		// ����������� ���������� �������
#define		DAT1_FREQ_Z		0x04		// �������� �������
#define		DAT1_SPEED		0x05		// ���������� �� ���������� ����� ������� ��������
#define		DAT1_MOMENT		0x06		// ���������� �� ���������� ����� ������� �������
#define		DAT1_7		0x07			// �������� ���������� �������
#define		DAT1_8		0x08			// ����������� �������� ��������
#define		DAT1_9		0x09			// ������� �������� ��������
#define		DAT1_A		0x0A			// ������� �������� ������
#define		DAT1_B		0x0B			// ���������� �������� � �������� �������
#define		DAT1_C		0x0C			// ���������� �������
#define		DAT1_D		0x0D			// ����������� �������� �������
#define		DAT1_E		0x0E			// ���������� ����� ����������� ����

#define		COM12_IN1	0x00		// ��������� ������� �����
#define		COM12_IN2	0x40		// ��������� ������� ������� ��������
#define		COM12_IN3	0x60		// ��������� �������� �����, ���������� � ���������� ������������
#define		COM12_OUT1	0x80		// ��������� �������� �����
#define		COM12_OUT2	0xC0		// ��������� ������� �������� �������� 

#define		COM33_ERROR0	0x10		// ����� ��������� ������������ (�������)
#define		COM33_ERROR1	0x11		// ����� ��������� ������������ (�������-1)
#define		COM33_ERROR2	0x12		// ����� ��������� ������������ (�������-2)
#define		COM33_ERROR3	0x13		// ����� ��������� ������������ (�������-3)
#define		COM33_ERROR4	0x14		// ����� ��������� ������������ (�������-4)
#define		COM33_ERROR5	0x15		// ����� ��������� ������������ (�������-5)
#define		COM33_TIME0		0x20		// ����� ��������� ������������ (�������)
#define		COM33_TIME1		0x21		// ����� ��������� ������������ (�������-1)
#define		COM33_TIME2		0x22		// ����� ��������� ������������ (�������-2)
#define		COM33_TIME3		0x23		// ����� ��������� ������������ (�������-3)
#define		COM33_TIME4		0x24		// ����� ��������� ������������ (�������-4)
#define		COM33_TIME5		0x25		// ����� ��������� ������������ (�������-5)

//--------------------------------------------------------------------------------------------------------
byte	SRV_REGIM=0;	// ������� ����� ������������
uint	SRV_FACT=0;		// ����������� ��������� 
uint	SRV_FREQ=0;		// ������� ��������
uint	SRV_COUNT=0;	// ���������� �������
uint	SRV_RASS=0;		// ���������������
float	SRV_MOMENT=0;	// ������� ������
uint	SRV_FREQ_R=0;	// �������� �������
uint	SRV_POS=0;		// ���������� ���������
float	SRV_SPEED=0;	// ������� ��������
uint	SRV_ACCEL=0;	// ����� �������

uint	posit=0;
uint	rass=0;
uint	rass2=0;

uint	SRV_CYCLE=0;	// ���� �������/����������

char	srv_acc_name[20];
char	srv_dec_name[20];
char	srv_speed_name[20];

byte	SRV_SON=0;		// ����������� �������
byte	SRV_LSP=0;		// �������� ����������� ������� ��������
byte	SRV_LSN=0;		// �������� ����������� ��������� ��������
byte	SRV_TL=0;		// ����������� ��������� �������
byte	SRV_TL1=0;	
byte	SRV_PC=0;		// ������������ �� �-���������
byte	SRV_RES=0;		// �����
byte	SRV_CR=0;		// �������� ������������ ��������
byte	SRV_SP1=0;		// ����� ������������� ������� �������� 1
byte	SRV_SP2=0;		// ����� ������������� ������� �������� 2
byte	SRV_SP3=0;		// ����� ������������� ������� �������� 3
byte	SRV_ST1=0;		// ���� � ������ �����������
byte	SRV_ST2=0;		// ���� � �������� �����������
byte	SRV_CM1=0;		// ����������� ��������, ����� 1
byte	SRV_CM2=0;		// ����������� ��������, ����� 2
byte	SRV_LOP=0;		// ����� ����� ����������
byte	SRV_STAB2=0;	// ������ ����� ���������/����������
byte	SRV_CDP=0;		//
byte	SRV_RD=0;		// ����������
byte	SRV_SA=0;		// ���������� ������� ��������
byte	SRV_ZSP=0;		// ����������� ���������
byte	SRV_TLC=0;		// ��������� ������ ��������� �������
byte	SRV_VLC=0;		// ���������� ����������� ������� ��������
byte	SRV_INP=0;		// � �������
byte	SRV_RR1=0;		// ������ 1
byte	SRV_WNG=0;		// ��������������
byte	SRV_ALM=0;		// ��������� ������������
byte	SRV_OP=0;		// ������� ���� Z-��������
byte	SRV_MBR=0;		// ���������� ������������ ��������
byte	SRV_RR2=0;		// ������ 2
byte	SRV_ACD0=0;		// ��� ��������� ������������
byte	SRV_ACD1=0;		// ��� ��������� ������������
byte	SRV_ACD2=0;		// ��� ��������� ������������
byte	SRV_BWNG=0;		// �������������� � �������
byte	SRV_CDPS=0;		// ������ CDPS
byte	SRV_ABSV=0;		// ���� ������ �� ���������� ����������� �����

