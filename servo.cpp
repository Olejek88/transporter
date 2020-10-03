#include <windows.h>
#include "Modbus_ARM.h"
#include "winpacsdk.h"
#include "servo.h"

extern byte  SRV_SU;		// ����������� � ������ "��������� ����������"
extern byte  SRV_READY;		// ���������� ������������ (RD/DICOM)
extern byte  SRV_ALARM;		// ������ ������������ (ALM/DICOM)
extern byte  SRV_CON_VKL;	// ��������� ������ ������������ �������
extern byte  SRV_UACTIVE;	// ������� ����������

extern byte  FQ1_OUTPUT;	// ������ ������� ������� ������� FQ (OUTPUT)
extern byte  FQ2_OUTPUT;	// ������ ������� ������� ������� FQ (OUTPUT)
extern byte  XUK1_OUTPUT;	// ������ "�������" ������� XUK1 (OUTPUT)
extern byte  XUK2_OUTPUT;	// ������ "�������" ������� XUK2 (OUTPUT)

extern byte  DOZA1_START;	// �������� ������� �1
extern byte  DOZA2_START;	// �������� ������� �2
extern byte  DOZA1_READY;	// ������� �1 �����
extern byte  DOZA2_READY;	// ������� �2 �����

extern byte  SERV_ST1;		// ���� ������������ � �������� �����������
extern byte  SERV_ST2;		// ���� ������������ � ������ �����������
extern byte  SERV_SON;		// �������� ����������� (SON)

extern byte	WorkEnable;		// ���� �������� ������
extern byte	TestRegim;		// ���� ��������� ������

extern uint	PANEL_FREQ;		// �������/�������� ��������, �������� � ������
extern uint	PANEL_TIME;		// ����� ����������, �������� � ������
extern uint	PANEL_TYPE;		// ��� ������, �������� � ������

extern uint	DOZE1_TIME;		// �������� �������� 1
extern uint	DOZE2_TIME;		// �������� �������� 2
extern uint	DOZE1_TIME2;	// �������� �������� 1 �� ��������
extern uint	DOZE2_TIME2;	// �������� �������� 2 �� ��������
extern uint	WAIT_TIME;		// ��������

extern byte	PANEL_LSPN;		// ����������� ��������, �������� � ������
extern byte	PANEL_SON;		// ������� �������, �������� � ������
extern byte	PANEL_ST1;		// ����� �������, �������� � ������
extern uint	PANEL_CYCLE;	// ���� ���������, �������� � ������
extern byte	PANEL_REGIM;	// ����� ������, �������� � ������
extern byte	ST_ZERO;		// ����������� ����
extern byte	ST_FWD;			// ������ ������ ������		

extern float MO1_LG;		// ���������� ���� ��� ����������� (MO1/LG)
extern float VC_LG;			// ���������� ���������� (VC/LG)

extern	byte	ConvStopped;// ���� ����, ��� ������� ����������
extern	byte	Dozation1;	// ���� ������� �������� ������� 1
extern	byte	Dozation2;	// ���� ������� �������� ������� 2
extern	byte	StekloFound;// ������� ������������
extern	byte	ShapkaFound;// ������� �����

byte	prevStekloFound=0;	// ������� ������������
byte	prevShapkaFound=0;	// ������� �����


extern	VOID ULOGW (CHAR* string, ...);				// log file function
VOID	OutServError (byte iInitSuccess);			// out in console and log SRV error
byte	CRC (char* in, byte count, byte* crc);		// crc calculation function
byte	SendData (int type, int adr, byte* func, byte* ndata, byte* data, byte nbytes);	// send/recieve data function
VOID	ReadRegisters (int adr);					// read all input/output signals
VOID	OutSRVError (byte iInitSuccess);			// ����� ������ ������������
float	ConvertDataToValue	(byte* data, byte len);	// ����������� ��������� ������ � ��������
uint	ConvertHexValue	(byte* data, byte len);		// ����������� ��������� ������ � 0x16
int		CALLBACK TimeFunction (void);				// ������� ������� �� ������	

HANDLE	hPort3;					// SRV port handler

static	byte	start_stop=0;	// start-stop
static	byte	timer1=0;		// own timer
static	UINT	timer2=0;		// own timer
static  TCHAR	errorBuf[32];	// buffer for error messages
static	int		result=0;		// read/write result
//----------------------------------------------------------------------------------------------------
// [Thread] Get/Set all input/output values from/to panel
VOID ServoConnection (LPVOID lpParam)
{
 int	port = *(int *) lpParam;	// port number
 int	adr=0;						// address on modbus
 int	att=0;						// attempt stop conveer
 byte	data[100];					// input/output buffer
 byte	data2[100];					// input/output buffer 2
 byte	ret=0;						// return code
 float	value=0;
 int	waitSteklo=0;				// ������� ������������
 int	waitShapka=0;				// ������� �����
 int	pwaitSteklo=0;				// ������� ������������ �� �������� ��������
 int	pwaitShapka=0;				// ������� ����� �� �������� ��������

 uint	srv_req=0;					// �������� �������
 
 hPort3 = uart_Open("COM6:,115200,E,8,1");
 uart_EnableCheckSum(hPort3, false);
 uart_SetTimeOut(hPort3, 50, CTO_TIMEOUT_ALL);

 if (hPort3) ULOGW ("[srv] initialize servo driver on port [COM6:115200:Even:8bit:1sb] for [srv] success (hPort3=%x)",hPort3);
 else
	{
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
 	 ULOGW ("[srv] error open COM6 internal port on speed 115200 [8-E-1] [0x%x]: %ls",pac_GetLastError(), errorBuf);
	}
 // ����� ������� ������������ ���������
 SERV_ST2=0; SERV_SON=0;
 // ��� ����������
 if (SendData (RECV, adr, (byte *)"02", (byte *)"20", data, 0)) ULOGW ("[srv] ��������� ������: [%s]",data);
 
 sprintf ((char*)data,"0000");
 if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0000] PA"); 
 sprintf ((char*)data,"0002");
 if (SendData (SEND, adr, (byte *)"8B", (byte *)"00", data, 4)) ULOGW ("[srv] set regim: positioning [0002]");
 sprintf ((char*)data,"0000");
 if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0000] PA"); 
 sprintf ((char*)data,"30000002");
 if (SendData (SEND, adr, (byte *)"84", (byte *)"01", data, 8)) ULOGW ("[srv] ������ ������: 0002");
 sprintf ((char*)data,"0002");
 if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0002] PC"); 

 if (SendData (SEND, adr, (byte *)"00", (byte *)"12", data, 4))
	{
	 ULOGW ("[srv] current regim: [%s]",data); SRV_REGIM=(byte)atoi((char *)data);
	 if (!strstr((char *)data,"0002")) ULOGW ("[srv] wrong regim! [%s]!=[0002]",data);
	}

 if (!TestRegim) 
	{
	 //sprintf ((char*)data,"0002");
	 //if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0002] PC"); 
	 pac_SetBPTimer (2,10000,TimeFunction);
	}
 else
	{
	 PANEL_FREQ=1200; sprintf ((char*)data,"3000%04X",PANEL_FREQ);
	 if (SendData (SEND, adr, (byte *)"84", (byte *)"06", data,8)) ULOGW ("[srv] ������ ��������: %ld",PANEL_FREQ);
	 if (SendData (RECV, adr, (byte *)"08", (byte *)"06", data,0)) _snprintf (srv_speed_name,19,"%s",data);
	 sprintf ((char*)data,"03BC");				// ������� ��������
	 SendData (SEND, adr, (byte *)"A0", (byte *)"10", data, 4);		// ������ ������� �������� ��/���

	 //sprintf ((char*)data,"0002");
	 //if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0002] PC"); 
	 //if (SendData (RECV, adr, (byte *)"08", (byte *)"01", data,0)) _snprintf (srv_acc_name,19,"%s",data);
	 //if (SendData (RECV, adr, (byte *)"08", (byte *)"02", data,0)) _snprintf (srv_dec_name,19,"%s",data);
	 //if (SendData (RECV, adr, (byte *)"08", (byte *)"06", data,0)) _snprintf (srv_speed_name,19,"%s",data);
	 pac_SetBPTimer (2,10000,TimeFunction);
	}
 sprintf ((char*)data,"CLR ");
 SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
 sprintf ((char*)data,"03BC");				// ������� ��������
 SendData (SEND, adr, (byte *)"A0", (byte *)"10", data, 4);		// ������ ������� �������� ��/���
 //ULOGW ("[srv] write start/stop time");
 sprintf ((char*)data,"000002E8");			// �����/���������
 SendData (SEND, adr, (byte *)"A0", (byte *)"11", data, 8);		// ������ �������/���������

 //ULOGW ("[srv] write path %d (0x%08X)",WAIT_TIME,WAIT_TIME);
 //sprintf ((char*)data,"%08X",WAIT_TIME);			// ���� (���������� ���������)
 //sprintf ((char*)data,"000047FA");			// ���� (���������� ���������)
 //SendData (SEND, adr, (byte *)"A0", (byte *)"20", data, 8);	// ������ ����

 //sprintf ((char*)data,"0001");
 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);
 //sprintf ((char*)data,"%08X",WAIT_TIME);			// ���� (���������� ���������)
 //SendData (SEND, adr, (byte *)"02", (byte *)"91", data, 8);	// ������ ����
 //sprintf ((char*)data,"%08X",WAIT_TIME);			// ���� (���������� ���������)
 //SendData (SEND, adr, (byte *)"02", (byte *)"90", data, 8);	// ������ ����
 //sprintf ((char*)data,"0001");
 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);

// if (!TestRegim) SERV_SON=1;
 SERV_SON=1; //SERV_ST2=1; // SERV_SP2=1;

 // ������� ����
 while (WorkEnable)				// ���� �������� / ������� ����
	{
	 ConvStopped=0; att=0;
	 while (WorkEnable)		// ���� �������� / ������� ������
		{									
		 if (PANEL_FREQ<1) PANEL_FREQ=1200;
		 if (WAIT_TIME<1) WAIT_TIME=196850;
		 timer2=20;
			//WAIT_TIME=12898200;		 
		 //ULOGW ("[mt] in FREQ=%d TIME=%d LSPN=%d SON=%d ST1=%d CYC=%d [REG=%d] | cu FREQ=%d TIME=%d LSP/N=%d|%d SON=%d ST1=%d",PANEL_FREQ,PANEL_TIME,PANEL_LSPN,PANEL_SON,PANEL_ST1,PANEL_CYCLE,PANEL_REGIM,SRV_FREQ_R,SRV_ACCEL,SRV_LSP,SRV_LSN,SRV_SON,SRV_ST1);
		 sprintf ((char*)data,"0002");
		 if (SendData (SEND, adr, (byte *)"8B", (byte *)"00", data, 4)) ULOGW ("[srv] set regim: positioning [0002]"); 
		 //sprintf ((char*)data,"CLR ");
		 //SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
		 sprintf ((char*)data,"03BC");				// ������� ��������
		 SendData (SEND, adr, (byte *)"A0", (byte *)"10", data, 4);		// ������ ������� �������� ��/���
		 sprintf ((char*)data,"000002E8");			// �����/���������
		 SendData (SEND, adr, (byte *)"A0", (byte *)"11", data, 8);		// ������ �������/���������
		 SERV_SON=1;

		 //sprintf ((char*)data,"GO  ");
		 //SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
		 //sprintf ((char*)data,"GO  ");
		 //SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
		 //ULOGW ("[srv] clear indication 1EA5");
		 //sprintf ((char*)data,"1EA5");
		 //SendData (SEND, adr, (byte *)"81", (byte *)"00", data, 4);
		 //sprintf ((char*)data,"0000");
		 //SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4);
		 //sprintf ((char*)data,"0101");
		 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);

		 ULOGW ("[srv] write path %d (0x%08X)",WAIT_TIME,WAIT_TIME);
		 sprintf ((char*)data,"%08X",WAIT_TIME);			// ���� (���������� ���������)
		 SendData (SEND, adr, (byte *)"A0", (byte *)"20", data, 8);	// ������ ����
		 sprintf ((char*)data,"0001");
		 SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);
 
		 sprintf ((char*)data,"00000007");			// �������� SON, LSP, LSN
		 SendData (SEND, adr, (byte *)"92", (byte *)"00", data, 8);	// ��� ���������� ��������������
		 Sleep (100);	// 100
		 ULOGW ("[srv] start positioning");
		 sprintf ((char*)data,"1EA5");				// ������ ����������������
		 SendData (SEND, adr, (byte *)"A0", (byte *)"40", data, 4);	// ������	 

		 //sprintf ((char*)data,"GO  ");
		 //SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
		 //SERV_SON=1;
		 Sleep (300);	// 300
		 ReadRegisters (adr);
		 //SRV_ST2
		 //!SRV_INP
		 while (!SRV_INP && timer2)
			{
			 //SERV_ST2=0;
			 ReadRegisters (adr);
			 //sprintf ((char*)data,"CLR ");
			 //SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);

			 if (SendData (RECV, adr, (byte *)"01", (byte *)"81", data2, 0))
				{
				 value=ConvertDataToValue	(data2, 8);
				 if (value>=0) SRV_SPEED=value; else SRV_SPEED=(-1)*value;
				 SRV_SPEED=ConvertDataToValue	(data2, 8);
				 //ULOGW ("[srv] engine RPM: %s [%s] %f","SRV_SPEED",data2,SRV_SPEED);
				}
			 if (SendData (RECV, adr, (byte *)"01", (byte *)"84", data2,0))
				{
				 value=ConvertDataToValue	(data2, 8);
				 if (value>=0) SRV_FREQ_R=(uint)value; else SRV_FREQ_R=(uint)((-1)*value);
				 if (SRV_FREQ_R<0) SRV_FREQ_R*=(-1);
		 		 //ULOGW ("[srv] requested FREQ: %s [%s] %ld","SRV_FREQ_R",data2,SRV_FREQ_R);
				}
			 if (SendData (RECV, adr, (byte *)"01", (byte *)"82", data2,0))
				{
				 value=ConvertDataToValue	(data2, 8);
				 if (value>=0) posit=(uint)value; else posit=(uint)((-1)*value);
				}
			 if (SendData (RECV, adr, (byte *)"01", (byte *)"83", data2,0))
				{		
				 value=ConvertDataToValue	(data2, 8);
				 rass=(uint)value;
				}
			 if (SendData (RECV, adr, (byte *)"02", (byte *)"91", data2,0))
				{
				 value=ConvertDataToValue	(data2, 8);
				 rass2=(uint)value;
				}			 
			 //ULOGW ("[srv] SRV_INP=%d speed:%f | freq=%f | ras=%d | imp=%d | rass2=%d",SRV_INP,SRV_SPEED,SRV_FREQ_R,posit,rass,rass2);
			 /*
			 if (PANEL_FREQ!=SRV_FREQ_R && PANEL_FREQ>0 && PANEL_FREQ<3000)
		 		{
		 		 sprintf ((char*)data,"3000%04X",PANEL_FREQ);
		 		 if (SendData (SEND, adr, (byte *)"84", (byte *)"06", data,8)) ULOGW ("[srv] ser frequency: %ld ��/���",PANEL_FREQ);
		 		}*/
			 if (ST_ZERO)
				{
				 SERV_SON=0; SERV_ST2=0;
				 sprintf ((char*)data,"CLR ");
				 SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
				 sprintf ((char*)data,"000C");				// ������� ��������
				 SendData (SEND, adr, (byte *)"A0", (byte *)"10", data, 4);		// ������ ������� �������� ��/���
				 ULOGW ("[srv] write start/stop time");
				 sprintf ((char*)data,"00000028");			// �����/���������
				 SendData (SEND, adr, (byte *)"A0", (byte *)"11", data, 8);		// ������ �������/���������
				 sprintf ((char*)data,"0000");
				 if (SendData (SEND, adr, (byte *)"8B", (byte *)"00", data, 4)) ULOGW ("[srv] set regim: speed control [0001]"); 
				 sprintf ((char*)data,"0002");
				 if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0002] PC"); 
				 sprintf ((char*)data,"30000010");
		 		 if (SendData (SEND, adr, (byte *)"84", (byte *)"06", data,8)) ULOGW ("[srv] ser frequency: %ld ��/���",10);
				 SERV_SON=1;
				 while (ST_ZERO && WorkEnable)
					{					 
					 if (ST_FWD) SERV_ST2=1;
					 else SERV_ST2=0;
					 Sleep (10);
					}
				 sprintf ((char*)data,"0000");
				 if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0000] PA"); 
				 sprintf ((char*)data,"0002");
				 if (SendData (SEND, adr, (byte *)"8B", (byte *)"00", data, 4)) ULOGW ("[srv] set regim: positioning [0002]"); 
				 //sprintf ((char*)data,"0000");
				 //if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0000] PA"); 
				 //sprintf ((char*)data,"30000002");
				 //if (SendData (SEND, adr, (byte *)"84", (byte *)"01", data, 8)) ULOGW ("[srv] ������ ������: 0002");
				 //sprintf ((char*)data,"0002");
				 //if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0002] PC"); 
				 sprintf ((char*)data,"CLR ");
				 SendData (SEND, adr, (byte *)"A0", (byte *)"41", data, 4);
				 sprintf ((char*)data,"03BC");				// ������� ��������
				 SendData (SEND, adr, (byte *)"A0", (byte *)"10", data, 4);		// ������ ������� �������� ��/���
				 ULOGW ("[srv] write start/stop time");
				 sprintf ((char*)data,"000002E8");			// �����/���������
				 //	 WAIT_TIME=196500;
				 SendData (SEND, adr, (byte *)"A0", (byte *)"11", data, 8);		// ������ �������/���������
				 ULOGW ("[srv] write path %d (0x%08X)",WAIT_TIME,WAIT_TIME);
				 sprintf ((char*)data,"%08X",WAIT_TIME);			// ���� (���������� ���������)
				 SendData (SEND, adr, (byte *)"A0", (byte *)"20", data, 8);	// ������ ����
				 //sprintf ((char*)data,"0101");
				 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);
				 //sprintf ((char*)data,"0001");
				 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);
				 SERV_SON=1;
				 break;
				}
			 timer2--;
			 ULOGW ("[srv] timer %d",timer2);
			 Sleep (5);
			}
		 //sprintf ((char*)data,"0000");
		 //if (SendData (SEND, adr, (byte *)"8B", (byte *)"00", data, 4)) ULOGW ("[srv] set regim: positioning [0002]"); 
		 ULOGW ("[srv] SRV_INP=%d speed:%f | freq=%f | ras=%d | imp=%d | rass2=%d",SRV_INP,SRV_SPEED,SRV_FREQ_R,posit,rass,rass2);
		 FQ1_OUTPUT=1; FQ2_OUTPUT=1;

		 // ����������� ����� �� �������
		 if (SRV_INP) { ULOGW ("[srv] servo in position - stop"); break; }
		 // ���� ��� ������������ ������� ��
		 if (XUK1_OUTPUT) { ULOGW ("[srv] �������� ������ ��, ������� ���������");  break; }
		 if (XUK2_OUTPUT) { ULOGW ("[srv] �������� ������ ��, ������� ���������");  break; }

		 Sleep (5);
		 //att++;
		}
	  // ��������� ������� �� ����� ����������������
	  if (XUK1_OUTPUT || XUK2_OUTPUT) 
		{
		 ULOGW ("[srv] stop servo");
		 SERV_ST2=0; att=0;
		 while (SRV_ST2 && WorkEnable)
			{
			 ReadRegisters (adr);
			 att++;
			 Sleep (200);
			}
		 ULOGW ("[srv] attempt [%d] to stop conveer",att);

		 while ((XUK1_OUTPUT || XUK2_OUTPUT) && WorkEnable)
			{
			 // ������ ��� �������/�������� �������
			 ReadRegisters (adr);
			 Sleep (200);
			}
		 SERV_ST2=1; //SERV_SON=1;
		}
 	  // ��������� �������1 ��� �������2
	  //if (StekloFound || ShapkaFound)
	  if (FQ1_OUTPUT || FQ2_OUTPUT)
		{
		 /*att=0; 
		 while (SRV_ST2 && WorkEnable)
			{
			 //SERV_ST2=0;
			 ReadRegisters (adr);
			 Sleep (5); att++;
			}
		 ULOGW ("[srv] attempt [%d] for conveer stopped (SRV_ST2=%d)",att,SRV_ST2);
		 */
		 //if (1)
		 if (!SRV_ST2)
			{
			 ULOGW ("[srv] conveer stopped [Dz1=%d Dz2=%d]",Dozation1,Dozation2);
			 ConvStopped=1; 
			 ULOGW ("[srv] waiting for dozation [%d]ms [Dz1=%d Dz2=%d]",(20-att)*100,Dozation1,Dozation2);
			 Sleep (700);
			 /*att=20;
			 while ((Dozation1 || Dozation2) && att)
				{
				 Sleep (100); att--;
				}*/
			 att=140;
			 while (WorkEnable && (Dozation1 && Dozation2) && att)
				{
				 // ������ ��� �������/�������� �������
				 ReadRegisters (adr);
				 Sleep (100); att--;
				}
			 ULOGW ("[srv] dozation finished, conveer started [Dz1=%d Dz2=%d]",Dozation1,Dozation2);
			 ConvStopped=0; //SERV_ST2=1; 

			 // ������ ��� �������/�������� �������
			 //ReadRegisters (adr);
			 //Sleep (1000);
			}
		 FQ1_OUTPUT=0; FQ2_OUTPUT=0;
		}
	}
 ULOGW ("[srv] close modbusRTU COM6 [srv] port");
 if (hPort3) uart_Close(hPort3); hPort3=NULL;
 ExitThread(0); 
}
//------------------------------------------------------------------------------------------
// function send data to SRVousilitel 
byte SendData (int type, int adr, byte* func, byte* ndata, byte* data, byte nbytes)
{
 char out[100],in[100];
 byte crc[4];
 byte count=3;
 DWORD bytes=0;
 bool	res=FALSE;
 //char	outs[50];
 //ULOGW ("[srv] SendData (%d, %d, %s, %s, %s, %d)",type, adr, func, ndata, data, nbytes);

 out[0]=SOH;
 out[1]=adr+0x30;
 _snprintf ((char *)out+2,2,"%s",func);
 out[4]=STX;
 _snprintf ((char *)out+5,2,"%s",ndata);
 
 if (type==RECV) 
	{
	 out[7]=ETX;
	 CRC (out+1, 7, crc);
	 out[8]=crc[0];
	 out[9]=crc[1];
	 //ULOGW ("[srv] send (%d bytes) %x %x %x %x %x %x %x %x %x %x",10,out[0],out[1],out[2],out[3],out[4],out[5],out[6],out[7],out[8],out[9]);
	}
 if (type==SEND) 
	{
	 if (nbytes>10) { ULOGW ("[srv] [error!] nbytes>0"); nbytes=10; }
	 for (byte i=0; i<nbytes; i++)	out[7+i]=data[i];
	 out[7+nbytes]=ETX;
	 CRC (out+1, 7+nbytes, crc);
	 out[8+nbytes]=crc[0];
	 out[9+nbytes]=crc[1];
	 //ULOGW ("[srv] send (%d bytes) %x %x %x %x %x %x %x %x %x %x %x %x %x %x",10+nbytes,out[0],out[1],out[2],out[3],out[4],out[5],out[6],out[7],out[8],out[9],out[10],out[11],out[12],out[13]);
	}

 while (count)
	{
	 if (type==RECV) res=uart_BinSend (hPort3,out,10);
	 if (type==SEND) res=uart_BinSend (hPort3,out,10+nbytes);

	 if (!res)
		{
		 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
		 ULOGW ("[srv] error send to SRV | error [0x%x]: %ls",pac_GetLastError(), errorBuf);
		 count--;
		 continue;
		}	 
	 Sleep (50);
	 bytes=uart_GetDataSize(hPort3,IN_DATA);
 	 //ULOGW ("[SRV] recieve %d bytes",bytes);

	 res=0;
	 if (bytes<50) res=uart_BinRecv (hPort3,(char *)in,bytes);
	 if (!res) 
		{
		 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
		 ULOGW ("[srv] error recieve from SRV. error [0x%x]: %ls",pac_GetLastError(), errorBuf);
		 if (hPort3) uart_Close(hPort3);
		 hPort3 = uart_Open("COM6:,115200,E,8,1");
 		 count--;
		 continue;
		}
	 break;
	}
 if (!res) 
	{
	 out[0]=EOT;
	 uart_BinSend (hPort3,(char *)out,1);
	 ULOGW ("[srv] send EOT to SRV");
	}
 else
 if (bytes>3 && bytes<50)
	{
 	 CRC (in+1, (byte)bytes-3, crc);
	 //if (type==RECV) ULOGW ("[srv] recieve (%d bytes) %x %x %x %x %x %x %x %x %x %x %x %x %x [crc [%x %x]=[%x %x]] ",bytes,in[0],in[1],in[2],in[3],in[4],in[5],in[6],in[7],in[8],in[9],in[10],in[11],in[12],in[bytes-2],in[bytes-1],crc[0],crc[1]);
	 //if (type==SEND) ULOGW ("[srv] recieve (%d bytes) %x %x %x %x %x %x [crc [%x %x]=[%x %x]] ",bytes,in[0],in[1],in[2],in[3],in[4],in[5],in[bytes-2],in[bytes-1],crc[0],crc[1]); 
	 if (in[2]!='a' && in[2]!='A') { OutSRVError (in[2]); return 0; }
	 if (in[bytes-2]==crc[0] && in[bytes-1]==crc[1]) { memcpy (data,in+3,bytes-6); data[bytes-6]=0; return 1; }	
	 else { ULOGW ("[srv] wrong crc [crc [%x %x]=[%x %x]]",in[bytes-2],in[bytes-1],crc[0],crc[1]); return 0; }
	}
 else 
	{
	 ULOGW ("[srv] unknown recieve error (bytes=%d)",bytes); return 0;
	}
 return 0;
}
//----------------------------------------------------------------------------------------------------
// decode SRV error code
byte CRC (char* in, byte count, byte* crc)
{
 byte crcs=0;
 for (byte i=0; i<count; i++) crcs+=in[i];
 //ULOGW ("[SRV] %x +%x",crcs,in[i]);
 
 if (crcs/16>=10) crc[0]=crcs/16-10+0x41;
 else crc[0]=crcs/16+0x30;
 if (crcs%16>=10) crc[1]=crcs%16-10+0x41;
 else crc[1]=crcs%16+0x30;

 return crcs;
}
//----------------------------------------------------------------------------------------------------
VOID ReadRegisters (int	adr)
{
 byte	dat[100];
 uint	data;
 // ������ ������� ��������
 if (SendData (RECV, adr, (byte *)"12", (byte *)"00", dat, 4))
	{
	 data=ConvertHexValue(dat, 8);
	 SRV_SON=(byte)((data&0x1));		// ����������� �������
	 SRV_LSP=(byte)((data&0x2)>>1);		// �������� ����������� ������� ��������
	 SRV_LSN=(byte)((data&0x4)>>2);		// �������� ����������� ��������� ��������
	 SRV_TL=(byte)((data&0x8)>>3);		// ����������� ��������� �������
	 SRV_TL1=(byte)((data&0x10)>>4);	
	 SRV_PC=(byte)((data&0x20)>>5);		// ������������ �� �-���������
	 SRV_RES=(byte)((data&0x40)>>6);	// �����
	 SRV_CR=(byte)((data&0x80)>>7);		// �������� ������������ ��������
	 SRV_SP1=(byte)((data&0x100)>>8);	// ����� ������������� ������� �������� 1
	 SRV_SP2=(byte)((data&0x200)>>9);	// ����� ������������� ������� �������� 2
	 SRV_SP3=(byte)((data&0x400)>>10);	// ����� ������������� ������� �������� 3
	 SRV_ST1=(byte)((data&0x800)>>11);	// ���� � ������ �����������
	 SRV_ST2=(byte)((data&0x1000)>>12);	// ���� � �������� �����������
	 SRV_CM1=(byte)((data&0x2000)>>13);	// ����������� ��������, ����� 1
	 SRV_CM2=(byte)((data&0x4000)>>14);	// ����������� ��������, ����� 2
	 SRV_LOP=(byte)((data&0x8000)>>15);	// ����� ����� ����������
	 SRV_STAB2=(byte)((data&0x100000)>>19);	// ������ ����� ���������/����������
	 SRV_CDP=(byte)((data&0x8000000)>>27);		
	 //ULOGW ("[srv] %x [%d %d %d %d %d %d %d %d] [%d %d %d %d %d %d %d %d] STAB2[%d] CDP[%d]",data,SRV_SON,SRV_LSP,SRV_LSN,SRV_TL,SRV_TL1,SRV_PC,SRV_RES,SRV_CR,SRV_SP1,SRV_SP2,SRV_SP3,SRV_ST1,SRV_ST2,SRV_CM1,SRV_CM2,SRV_LOP,SRV_STAB2,SRV_CDP); 
	}
 // ������ �������� ��������
 //if (0)
 if (SendData (RECV, adr, (byte *)"12", (byte *)"80", dat, 4))
	{
	 data=ConvertHexValue(dat, 8);
	 SRV_RD=(byte)((data&0x1));			// ����������
	 SRV_SA=(byte)((data&0x2)>>1);		// ���������� ������� ��������
	 SRV_ZSP=(byte)((data&0x4)>>2);		// ����������� ���������
	 SRV_TLC=(byte)((data&0x8)>>3);		// ��������� ������ ��������� �������
	 SRV_VLC=(byte)((data&0x10)>>4);	// ���������� ����������� ������� ��������
	 SRV_INP=(byte)((data&0x20)>>5);	// � �������
	 SRV_WNG=(byte)((data&0x80)>>7);	// ��������������
	 SRV_ALM=(byte)((data&0x100)>>8);	// ��������� ������������
	 SRV_OP=(byte)((data&0x200)>>9);	// ������� ���� Z-��������
	 SRV_MBR=(byte)((data&0x400)>>10);	// ���������� ������������ ��������
	 SRV_ACD0=(byte)((data&0x1000)>>11);	// ��� ��������� ������������
	 SRV_ACD1=(byte)((data&0x2000)>>12);	// ��� ��������� ������������
	 SRV_ACD2=(byte)((data&0x4000)>>13);	// ��� ��������� ������������
	 SRV_BWNG=(byte)((data&0x8000)>>14);	// �������������� � �������
	 SRV_CDPS=(byte)((data&0x2000000)>>21);	// ������ CDPS
	 SRV_ABSV=(byte)((data&0x8000000)>>23);	// ���� ������ �� ���������� ����������� �����
	 //ULOGW ("[srv] %x [%d %d %d %d %d %d - %d] [%d %d %d - %d %d %d %d] CDPS[%d] ABSV[%d]",data,SRV_RD,SRV_SA,SRV_ZSP,SRV_TLC,SRV_VLC,SRV_INP,SRV_WNG,SRV_ALM,SRV_OP,SRV_MBR,SRV_ACD0,SRV_ACD1,SRV_ACD2,SRV_BWNG,SRV_CDPS,SRV_ABSV); 
	}
}
//----------------------------------------------------------------------------------------------------
// decode SRV error code
VOID OutSRVError (byte iInitSuccess)
{
 switch (iInitSuccess)
	{
	  case	OK:				ULOGW ("[SRV] communication error [0x%x (%c)] OK",iInitSuccess,iInitSuccess); break;
	  case	PARITY_ERROR:	ULOGW ("[SRV] communication error [0x%x (%c)] parity error",iInitSuccess,iInitSuccess); break;
	  case	CRC_ERROR:		ULOGW ("[SRV] communication error [0x%x (%c)] crc error",iInitSuccess,iInitSuccess); break;
	  case  ERROR_SIGN:		ULOGW ("[SRV] communication error [0x%x (%c)] character error",iInitSuccess,iInitSuccess); break;
	  case  ERROR_FUNC:		ULOGW ("[SRV] communication error [0x%x (%c)] wrong funtion",iInitSuccess,iInitSuccess); break;
	  case  ERROR_DATA:		ULOGW ("[SRV] communication error [0x%x (%c)] wrong data",iInitSuccess,iInitSuccess); break;

	  case	A_OK:			ULOGW ("[SRV] communication error [0x%x (%c)] OK",iInitSuccess,iInitSuccess); break;
	  case	A_PARITY_ERROR:	ULOGW ("[SRV] communication error [0x%x (%c)] parity error",iInitSuccess,iInitSuccess); break;
	  case	A_CRC_ERROR:	ULOGW ("[SRV] communication error [0x%x (%c)] crc error",iInitSuccess,iInitSuccess); break;
	  case  A_ERROR_SIGN:	ULOGW ("[SRV] communication error [0x%x (%c)] character error",iInitSuccess,iInitSuccess); break;
	  case  A_ERROR_FUNC:	ULOGW ("[SRV] communication error [0x%x (%c)] wrong funtion",iInitSuccess,iInitSuccess); break;
	  case  A_ERROR_DATA:	ULOGW ("[SRV] communication error [0x%x (%c)] wrong data",iInitSuccess,iInitSuccess); break;

	  default: ULOGW ("[SRV] error [0x%x] illegal function",iInitSuccess);
	}
 }
//----------------------------------------------------------------------------------------------------
float	ConvertDataToValue	(byte* data, byte len)
{
 float	value=0;
 __int32	val=0,k=0x10000000,k2=10000000;
 byte	ch;
 //ULOGW ("[SRV] ConvertDataToValue (%s, %d)",data, len);
 for (byte i=0; i<len; i++)
	{
	 if (data[4+i]>='0' && data[4+i]<='9') ch=data[4+i]-0x30;
	 if (data[4+i]>='A' && data[4+i]<='F') ch=data[4+i]-0x41+10;

	 if (data[3]=='0')	{ val+=ch*k;  k/=0x10;  }
	 if (data[3]=='1')	{ val+=ch*k2; k2/=10;	}
	 //ULOGW ("[SRV] 3=(%c) 4=(%c) [%x] val=%ld k=0x%x",data[3],data[4+i],ch,val,k);
	}
 if (data[2]>='0' && data[2]<'7') 
	{ 
	 value=(float)val/(float)(pow (10,data[2]-0x30)); 
	}
 else value=(float)val;
 //ULOGW ("[srv] %f",value);
 return value;
}
//----------------------------------------------------------------------------------------------------
int	CALLBACK TimeFunction (void)
{
 timer1--;
 //ULOGW ("[srv] timer [%d]",timer1);
 //if (PANEL_REGIM && TestRegim)
 if (PANEL_REGIM)
	{
	 if (timer1<=0) 
		{
		 ULOGW ("[srv] timer ");
		 if (start_stop) 
			{
			 if (PANEL_LSPN==1) { SERV_ST1=1; SERV_ST2=0; }
			 else  { SERV_ST1=0; SERV_ST2=1; }
			 start_stop=0;
			 timer1=PANEL_CYCLE/100;
			}
		 else 
			{
			 SERV_ST1=0; SERV_ST2=0;
			 start_stop=1;
			 timer1=PANEL_CYCLE/100;
			}
		}
	}
 else
	{
	 pac_KillBPTimer(2);
	}
 return 0;
}
//----------------------------------------------------------------------------------------------------
uint	ConvertHexValue	(byte* data, byte len)
{
 uint	value=0;
 __int32	val=0,k=0x1000000;
 byte	ch;
 //ULOGW ("[SRV] ConvertDataToValue (%s, %d)",data, len);
 for (byte i=1; i<len; i++)
	{
	 if (data[i]>='0' && data[i]<='9') ch=data[i]-0x30;
	 if (data[i]>='A' && data[i]<='F') ch=data[i]-0x41+10;

	 val+=ch*k;
	 k/=0x10;
	 //ULOGW ("[SRV] [%x] val=%ld k=0x%x",ch,val,k);
	}
 value=val;
 //ULOGW ("[srv] %d",value);
 return value;
}
//---------------------------------------------------------------------------------------------------- 
 // ����������������
 //ULOGW ("[SRV] ������ ���� (���������� ���������) - 400000 ���������");
 //sprintf ((char*)data,"00AA1A80");			// ���� (���������� ���������)
 //SendData (SEND, adr, (byte *)"A0", (byte *)"20", data, 8);	// ������ ����
 //ULOGW ("[SRV] ����� ����������� �������� - ������");
 //sprintf ((char*)data,"0000");				// ������ ��������
 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);	// ����� ����������� ��������
 //ULOGW ("[SRV] ������ ����������������");
 //sprintf ((char*)data,"1EA5");				// ������ ����������������
 //SendData (SEND, adr, (byte *)"A0", (byte *)"40", data, 4);	// ������

	 //ULOGW ("[srv] ������ ������� �������� 200 ��/���");
	 //sprintf ((char*)data,"00C8");								// ������� ��������
	 //SendData (SEND, adr, (byte *)"A0", (byte *)"10", data, 4);	// ������ ������� �������� ��/���
	 //ULOGW ("[srv] ������ �������/��������� 1 �������");
	 //sprintf ((char*)data,"000003E8");							// �����/���������
	 //SendData (SEND, adr, (byte *)"A0", (byte *)"11", data, 8);	// ������ �������/���������
	 //ULOGW ("[srv] �������� SON, LSP, LSN ��� ���������� ��������������");
	 //sprintf ((char*)data,"00000007");							// �������� SON, LSP, LSN
	 //SendData (SEND, adr, (byte *)"92", (byte *)"00", data, 8);	// ��� ���������� ��������������

 //ULOGW ("[srv] ����������� ��������");
 //sprintf ((char*)data,"0000");
 //if (SendData (SEND, adr, (byte *)"85", (byte *)"00", data, 4)) ULOGW ("[srv] ����� ������ ���������� [0000] PA"); 
 //ULOGW ("[srv] ����������� �������� ������");
 //sprintf ((char*)data,"0000");
 //SendData (SEND, adr, (byte *)"A0", (byte *)"21", data, 4);
 //sprintf ((char*)data,"00000000");
 //if (SendData (SEND, adr, (byte *)"84", (byte *)"0E", data,8)) ULOGW ("[srv] ����������� �������� ������");
