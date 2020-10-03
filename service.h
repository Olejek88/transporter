extern	byte	WorkEnable;		// work flag
extern	byte	ConvStopped;	// conveer stopped
extern	short	currentregim;	// 0 - test, 1-3 scene selection

extern	uint	totalDetected;	// ����� �������� �������
extern	uint	totalLoaded;	// ����� ��������� �������
extern	uint	todayDetected;	// �������� ������� �������
extern	uint	todayLoaded;	// ��������� ������� �������

// IO variables
// [0] i-87017
extern	float MO1_LG;		// ���������� ���� ��� ����������� (MO1/LG)
extern	float MOD1_1;		// ������ 1
extern	float MOD1_2;		// ������ 2
extern	float MOD1_3;		// ������ 3
// [1] i-87024
extern	float VC_LG;		// ���������� ���������� (VC/LG)
extern	float MOD2_1;		// ������ 1
extern	float MOD2_2;		// ������ 2
extern	float MOD2_3;		// ������ 3
// [2] i-87053
extern	byte  FQ1_ERROR;	// ������ ������ ������� ������� FQ (ERROR)
extern	byte  FQ1_BUSY;		// ������ ������ ������� ������� FQ (BUSY)
extern	byte  FQ1_OUTPUT;	// ������ ������� ������� ������� FQ (OUTPUT)
extern	byte  FQ2_ERROR;	// ������ ������ ������� ������� FQ (ERROR)
extern	byte  FQ2_BUSY;		// ������ ������ ������� ������� FQ (BUSY)
extern	byte  FQ2_OUTPUT;	// ������ ������� ������� ������� FQ (OUTPUT)
extern	byte  XUK1_ALARM;	// ������ "������" ������� XUK (ALARM)
extern	byte  XUK1_OUTPUT;	// ������ "�������" ������� XUK (OUTPUT)
extern	byte  XUK2_ALARM;	// ������ "������" ������� XUK (ALARM)
extern	byte  XUK2_OUTPUT;	// ������ "�������" ������� XUK (OUTPUT)
// [3] i-87053
extern	byte  SERV_SU;		// ����������� � ������ "��������� ����������"
extern	byte  SERV_READY;	// ���������� ������������ (RD/DICOM)
extern	byte  SERV_ALARM;	// ������ ������������ (ALM/DICOM)
extern	byte  SERV_CON_VKL;	// ��������� ������ ������������ �������
extern	byte  SERV_UACTIVE;	// ������� ����������
extern	byte  DOZA1_READY;	// ���������� �������� �1
extern	byte  DOZA2_READY;	// ���������� �������� �2
extern	byte  CONV_STOP;	// ��������� ��������� ���������
extern	byte  DOZA1_ERROR;	// ������ �������� �1
extern	byte  DOZA2_ERROR;	// ������ �������� �2
// [4] i-87064
extern	byte  FQ1_TRIG;		// ������ ������� ������� FQ (TRIG)
extern	byte  FQ1_IN0;		// ����� ������� ������� #1 (IN0)
extern	byte  FQ1_IN1;		// ����� ������� ������� #1 (IN1)
extern	byte  FQ1_IN2;		// ����� ������� ������� #1 / �������� ERROR (IN2)
extern	byte  FQ1_IN3;		// ����� ������� ������� #1 (IN3)
extern	byte  FQ1_IN4;		// ����� ������� ������� #1 (IN4)
extern	byte  FQ1_IN5;		// ������� �� ���������� ���������  / ����� ����� (IN5)
// [5] i-87064
extern	byte  SERV_SON;		// �������� ����������� (SON)
extern	byte  SERV_SP1;		// ��������������� ����� �1 (SP1)
extern	byte  SERV_SP2;		// ��������������� ����� �2 (SP2)
extern	byte  SERV_ST1;		// ���� ������������ (ST1)
extern	byte  SERV_RES;		// ����� ������ ������������ (RES)
extern	byte  DOZA1_START;	// �������� ������� �1
extern	byte  DOZA2_START;	// �������� ������� �2
extern	byte  SERV_ST2;		// ���� � �������� �����������
// [6] i-87061
extern	byte  FQ2_TRIG;		// ������ ������� ������� FQ (TRIG)
extern	byte  FQ2_IN0;		// ����� ������� ������� #2 (IN0)
extern	byte  FQ2_IN1;		// ����� ������� ������� #2 (IN1)
extern	byte  FQ2_IN2;		// ����� ������� ������� #2 / �������� ERROR (IN2)
extern	byte  FQ2_IN3;		// ����� ������� ������� #2 (IN3)
extern	byte  FQ2_IN4;		// ����� ������� ������� #2 (IN4)
extern	byte  FQ2_IN5;		// ������� �� ����. ��������� / ����� ����� (IN5)

// servoprivod
extern	float	SRV_REGIM;	// ������� ����� ������������
extern	uint	SRV_FACT;	// ����������� ��������� 
extern	uint	SRV_FREQ;	// ������� ��������
extern	uint	SRV_COUNT;	// ���������� �������
extern	uint	SRV_RASS;	// ���������������
extern	float	SRV_MOMENT;	// ������� ������
extern	uint	SRV_FREQ_R;	// �������� �������
extern	uint	SRV_POS;	// ���������� ���������
extern	float	SRV_SPEED;	// ������� ��������

extern	byte	SRV_SON;	// ����������� �������
extern	byte	SRV_LSP;	// �������� ����������� ������� ��������
extern	byte	SRV_LSN;	// �������� ����������� ��������� ��������
extern	byte	SRV_TL;		// ����������� ��������� �������
extern	byte	SRV_TL1;	
extern	byte	SRV_PC;		// ������������ �� �-���������
extern	byte	SRV_RES;	// �����
extern	byte	SRV_CR;		// �������� ������������ ��������
extern	byte	SRV_SP1;	// ����� ������������� ������� �������� 1
extern	byte	SRV_SP2;	// ����� ������������� ������� �������� 2
extern	byte	SRV_SP3;	// ����� ������������� ������� �������� 3
extern	byte	SRV_ST1;	// ���� � ������ �����������
extern	byte	SRV_ST2;	// ���� � �������� �����������
extern	byte	SRV_CM1;	// ����������� ��������, ����� 1
extern	byte	SRV_CM2;	// ����������� ��������, ����� 2
extern	byte	SRV_LOP;	// ����� ����� ����������
extern	byte	SRV_STAB2;	// ������ ����� ���������/����������
extern	byte	SRV_CDP;	//
extern	byte	SRV_RD;		// ����������
extern	byte	SRV_SA;		// ���������� ������� ��������
extern	byte	SRV_ZSP;	// ����������� ���������
extern	byte	SRV_TLC;	// ��������� ������ ��������� �������
extern	byte	SRV_VLC;	// ���������� ����������� ������� ��������
extern	byte	SRV_INP;	// � �������
extern	byte	SRV_WNG;	// ��������������
extern	byte	SRV_ALM;	// ��������� ������������
extern	byte	SRV_OP;		// ������� ���� Z-��������
extern	byte	SRV_MBR;	// ���������� ������������ ��������
extern	byte	SRV_ACD0;	// ��� ��������� ������������
extern	byte	SRV_ACD1;	// ��� ��������� ������������
extern	byte	SRV_ACD2;	// ��� ��������� ������������
extern	byte	SRV_BWNG;	// �������������� � �������
extern	byte	SRV_CDPS;	// ������ CDPS
extern	byte	SRV_ABSV;	// ���� ������ �� ���������� ����������� �����

extern	byte	SRV_SON;		// ����������� �������
extern  byte	SRV_LSP;		// �������� ����������� ������� ��������
extern  byte	SRV_LSN;		// �������� ����������� ��������� ��������
extern	byte	SRV_ST1;		// ���� � ������ �����������
extern	uint	SRV_ACCEL;		// ����� ������� / ����������
extern	uint	SRV_FREQ;		// ������� ��������


