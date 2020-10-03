#define ReadHoldingRegisters	0x3
#define ReadInputRegisters		0x4
#define PresetSingleRegister  	0x5
#define PresetMultiRegister  	0x6

byte	ST_ZERO=0;
byte	ST_FWD=0;

uint	PANEL_FREQ=700;
uint	PANEL_TIME=0;
uint	PANEL_CYCLE=0;
uint	PANEL_TYPE=0;
uint	DOZE1_TIME=1800;
uint	DOZE2_TIME=2200;
uint	DOZE1_TIME2=3600;
uint	DOZE2_TIME2=4400;
uint	WAIT_TIME=12900000;

byte	PANEL_LSPN=0;
byte	PANEL_SON=1;
byte	PANEL_ST1=1;
byte	PANEL_REGIM=0;
byte	PANEL_STOP=0;
byte	PANEL_EXIT=0;

extern	byte	SRV_SON;		// ����������� �������
extern  byte	SRV_LSP;		// �������� ����������� ������� ��������
extern  byte	SRV_LSN;		// �������� ����������� ��������� ��������
extern	byte	SRV_ST1;		// ���� � ������ �����������
extern	uint	SRV_ACCEL;		// ����� ������� / ����������
extern	uint	SRV_FREQ;		// ������� ��������

extern	byte	SRV_REGIM;		// ������� ����� ������������
extern	uint	SRV_FACT;		// ����������� ��������� 
extern	uint	SRV_COUNT;		// ���������� �������
extern	uint	SRV_RASS;		// ���������������
extern	float	SRV_MOMENT;		// ������� ������
extern	uint	SRV_FREQ_R;		// �������� �������
extern	uint	SRV_POS;		// ���������� ���������
extern	float	SRV_SPEED;		// ������� ��������

extern	byte	SRV_SON;		// ����������� �������
extern	byte	SRV_RD;			// ����������

extern	byte  SERV_READY;		// ���������� ������������ (RD/DICOM)
extern	byte  SERV_ALARM;		// ������ ������������ (ALM/DICOM)
extern	byte  SERV_CON_VKL;		// ��������� ������ ������������ �������
extern	byte  SERV_UACTIVE;		// ������� ����������
extern	byte  DOZA1_READY;		// ���������� �������� �1
extern	byte  DOZA2_READY;		// ���������� �������� �2
extern	byte  CONV_STOP;		// ��������� ��������� ���������
extern	byte  DOZA1_ERROR;		// ������ �������� �1
extern	byte  DOZA2_ERROR;		// ������ �������� �2

extern	byte  FQ1_ERROR;		// ������ ������ ������� ������� FQ (ERROR)
extern	byte  FQ1_BUSY;			// ������ ������ ������� ������� FQ (BUSY)
extern	byte  FQ1_OUTPUT;		// ������ ������� ������� ������� FQ (OUTPUT)
extern	byte  FQ2_ERROR;		// ������ ������ ������� ������� FQ (ERROR)
extern	byte  FQ2_BUSY;			// ������ ������ ������� ������� FQ (BUSY)
extern	byte  FQ2_OUTPUT;		// ������ ������� ������� ������� FQ (OUTPUT)
extern	byte  XUK1_ALARM;		// ������ "������" ������� XUK (ALARM)
extern	byte  XUK1_OUTPUT;		// ������ "�������" ������� XUK (OUTPUT)
extern	byte  XUK2_ALARM;		// ������ "������" ������� XUK (ALARM)
extern	byte  XUK2_OUTPUT;		// ������ "�������" ������� XUK (OUTPUT)

extern	uint	SRV_FACT;		// ����������� ��������� [80]
extern	uint	SRV_FACT_PREV;	// ����������� ��������� [80]
extern	uint	SRV_2P;			// ����������� ��������� [80]
extern	uint	CORRECT;
extern	uint	WAIT_TIME;
extern	uint	WAIT_TIME2;

extern	byte	IZnal[50];		// ������� ���������� �� �������
extern	byte	sensor_pos;		

extern	byte	DOZA1_START;
extern	byte	DOZA2_START;
extern	byte	PANEL_STOP;