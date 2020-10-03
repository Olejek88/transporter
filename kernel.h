// constants

// global variables
byte	WorkEnable=1;	// work flag
byte	ConvStopped=1;	// conveer stopped
byte	Dozation1=0;	// ���� ������� �������� ������� 1
byte	Dozation2=0;	// ���� ������� �������� ������� 2
byte	TestRegim=0;	// �������� �����
byte	StekloFound=0;	// ������� ������������
byte	ShapkaFound=0;	// ������� �����
byte	pStekloFound=0;	// ������� ������������ ������ (��� ��������� ������)
byte	pShapkaFound=0;	// ������� ����� ������ (��� ��������� ������)

uint	totalDetected=0;	// ����� �������� �������
uint	totalLoaded=0;		// ����� ��������� �������
uint	todayDetected=0;	// �������� ������� �������
uint	todayLoaded=0;		// ��������� ������� �������

uint	currentregim;			// 0 - test, 1-3 scene selection
uint	currentregim2;			// 0 - test, 1-3 scene selection
extern	uint	PANEL_TYPE;		// regim from panel
extern	uint	PANEL_TYPE2;	// regim from panel
extern	byte	PANEL_REGIM;
extern	byte	PANEL_STOP;
extern	byte	PANEL_EXIT;

extern	uint	DOZE1_TIME;
extern	uint	DOZE2_TIME;
extern	uint	DOZE1_TIME2;
extern	uint	DOZE2_TIME2;

byte	IZnal[50]={0};	// ������� ���������� �� �������

HANDLE	hPort;			// internal port handler
TCHAR	errorBuf[32];	// buffer for error messages

HANDLE	hThrdIO;		// I/O thread handle
DWORD	dwThrdIDIO;		// I/O thread ID value
HANDLE	hReadEventsIO;	// I/O thread 

HANDLE	hThrdMT;		// Panel thread handle
DWORD	dwThrdIDMT;		// Panel thread ID value
HANDLE	hReadEventsMT;	// Panel thread 

HANDLE	hThrdServ;		// Servoprivod thread handle
DWORD	dwThrdIDServ;	// Servoprivod thread ID value
HANDLE	hReadEventsServ;// Servoprivod thread 

HANDLE	hThrdAlg;		// Algorithm thread handle
DWORD	dwThrdIDAlg;	// Algorithm thread ID value
HANDLE	hReadEventsAlg; // Algorithm thread 

HANDLE	hThrdService;		// Service handle
DWORD	dwThrdIDService;	// Service thread ID value
HANDLE	hReadEventsService; // Service thread 

// IO variables
// [0] i-87017
float MO1_LG;		// ���������� ���� ��� ����������� (MO1/LG)
float MOD1_1;		// ������ 1
float MOD1_2;		// ������ 2
float MOD1_3;		// ������ 3
// [1] i-87024
float VC_LG;		// ���������� ���������� (VC/LG)
float MOD2_1;		// ������ 1
float MOD2_2;		// ������ 2
float MOD2_3;		// ������ 3
// [2] i-87053
byte  FQ1_ERROR=0;		// ������ ������ ������� ������� FQ (ERROR)
byte  FQ1_BUSY=0;		// ������ ������ ������� ������� FQ (BUSY)
byte  FQ1_OUTPUT=0;		// ������ ������� ������� ������� FQ (OUTPUT)
byte  FQ2_ERROR=0;		// ������ ������ ������� ������� FQ (ERROR)
byte  FQ2_BUSY=0;		// ������ ������ ������� ������� FQ (BUSY)
byte  FQ2_OUTPUT=0;		// ������ ������� ������� ������� FQ (OUTPUT)
byte  XUK1_ALARM=0;		// ������ "������" ������� XUK (ALARM)
byte  XUK1_OUTPUT=0;	// ������ "�������" ������� XUK (OUTPUT)
byte  XUK2_ALARM=0;		// ������ "������" ������� XUK (ALARM)
byte  XUK2_OUTPUT=0;	// ������ "�������" ������� XUK (OUTPUT)
// [3] i-87053
byte  PRESENT1=0;		// ������� ������ ��� ��������� 1
byte  PRESENT2=0;		// ������� ������ ��� ��������� 2
byte  ZERO_INST=0;		// ����
byte  SERV_SU=0;		// ����������� � ������ "��������� ����������"
byte  SERV_READY=0;		// ���������� ������������ (RD/DICOM)
byte  SERV_ALARM=0;		// ������ ������������ (ALM/DICOM)
byte  SERV_CON_VKL=0;	// ��������� ������ ������������ �������
byte  SERV_UACTIVE=0;	// ������� ����������
byte  DOZA1_READY=0;	// ���������� �������� �1
byte  DOZA2_READY=0;	// ���������� �������� �2
byte  CONV_STOP=0;		// ��������� ��������� ���������
byte  DOZA1_ERROR=0;	// ������ �������� �1
byte  DOZA2_ERROR=0;	// ������ �������� �2
// [4] i-87064
byte  FQ1_TRIG=0;		// ������ ������� ������� FQ (TRIG)
byte  FQ1_IN0=0;		// ����� ������� ������� #1 (IN0)
byte  FQ1_IN1=0;		// ����� ������� ������� #1 (IN1)
byte  FQ1_IN2=0;		// ����� ������� ������� #1 / �������� ERROR (IN2)
byte  FQ1_IN3=0;		// ����� ������� ������� #1 (IN3)
byte  FQ1_IN4=0;		// ����� ������� ������� #1 (IN4)
byte  FQ1_IN5=0;		// ������� �� ���������� ���������  / ����� ����� (IN5)
// [5] i-87064
byte  SERV_SON=0;		// �������� ����������� (SON)
byte  SERV_SP1=0;		// ��������������� ����� �1 (SP1)
byte  SERV_SP2=0;		// ��������������� ����� �2 (SP2)
byte  SERV_ST1=0;		// ���� ������������ (ST1)
byte  SERV_RES=0;		// ����� ������ ������������ (RES)
byte  DOZA1_START=0;	// �������� ������� �1
byte  DOZA2_START=0;	// �������� ������� �2
byte  SERV_ST2=0;		// ���� ������������ (ST2)
// [6] i-87061
byte  FQ2_TRIG=0;		// ������ ������� ������� FQ (TRIG)
byte  FQ2_IN0=0;		// ����� ������� ������� #2 (IN0)
byte  FQ2_IN1=0;		// ����� ������� ������� #2 (IN1)
byte  FQ2_IN2=0;		// ����� ������� ������� #2 / �������� ERROR (IN2)
byte  FQ2_IN3=0;		// ����� ������� ������� #2 (IN3)
byte  FQ2_IN4=0;		// ����� ������� ������� #2 (IN4)
byte  FQ2_IN5=0;		// ������� �� ����. ��������� / ����� ����� (IN5)

byte  ZERO_INST1=0;		// ����

//byte  FQ1_CHANGED=0;	// �������  FQ (OUTPUT)

DWORD do_value4_prev=0xff,do_value6_prev=0xff,do_value7_prev=0xff;
DWORD di_value2_prev=0xff,di_value3_prev=0xff;
