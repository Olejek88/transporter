#include <windows.h>
#include "winpacsdk.h"
#include "panel.h"
#include "Modbus_ARM.h"

extern byte  FQ1_ERROR;		// Сигнал ошибки первого датчика FQ (ERROR)
extern byte  SERV_SU;		// Сервопривод в режиме "системное управление"
extern byte  FQ1_TRIG;		// Запуск первого датчика FQ (TRIG)

extern byte	WorkEnable;		// work flag
extern short currentregim;	// regim selection
extern byte	TestRegim;		// test regim

extern float MO1_LG;		// Аналоговый вход для мониторинга (MO1/LG)
extern float VC_LG;			// Аналоговый управление (VC/LG)
extern float MOD1_1;
extern float MOD1_2;
extern float MOD1_3;
extern float MOD2_1;
extern float MOD2_2;
extern float MOD2_3;

HANDLE	hPort2;				// Panel port handler
HANDLE	hPort5;				// Panel port handler

extern	VOID ULOGW (CHAR* string, ...);		// log file function
extern	VOID OutError (int iInitSuccess);	// out in console and log modbus error
extern	VOID OutModbusError (int iInitSuccess);

int ModbusRTU_Read(int adr, int start, int count, byte *iRecv, int func);
int ModbusRTU_Write(int adr, int start, int count, byte len, byte *iSend, int func);
int ModbusRTU_Answer(int adr, byte count);

UINT CRC (const BYTE* const Data, const BYTE DataSize);
unsigned int CalcCRC(unsigned int c,unsigned int crc);
WORD CRC16 (const BYTE *nData, WORD wLength);
VOID ReadCOM (LPVOID lpParam);

static	TCHAR	errorBuf[32];	// buffer for error messages
static	int		result=0;		// read/write result
//----------------------------------------------------------------------------------------------------
// [Thread] Get/Set all input/output values from/to panel
VOID PanelConnection (LPVOID lpParam)
{
 //int port = *(int *) lpParam;	// port number
 int adr=0x1;					// address on modbus
 int speed=115200;
 int iReadSuccess;
 byte	data[150];				// output buffer
 short	in_reg=0;
 byte	count=0;
 int port=3;

 int iInitSuccess = MBRTUInit(port,speed,1,8,1,100);
 if (!iInitSuccess) ULOGW ("[mt] initialize modbus driver on port [COM%d:115200:Even:8bit:1sb] for [panel] success",port);
 else OutModbusError (iInitSuccess);

 while (WorkEnable)
	{
	 Sleep (1000);
	 if (TestRegim)
		{
		 if (!MBRTU_R_Registers(port,adr,50+1,1,&in_reg,0x3)) { PANEL_FREQ=(uint)in_reg; ULOGW ("[mt] read frequency from panel [%d]",PANEL_FREQ); }
		 if (!MBRTU_R_Registers(port,adr,52+1,1,&in_reg,0x3)) { PANEL_TIME=(uint)in_reg; ULOGW ("[mt] read accel/deccel time from panel [%d]",PANEL_TIME); }
		 if (!MBRTU_R_Registers(port,adr,54+1,1,&in_reg,0x3)) { PANEL_CYCLE=(uint)in_reg; ULOGW ("[mt] read cycle time from panel [%d]",PANEL_CYCLE); }
		 if (!MBRTU_R_Registers(port,adr,56+1,1,&in_reg,0x3)) { PANEL_TYPE=(uint)in_reg; ULOGW ("[mt] read type from panel [%d]",PANEL_TYPE); }

		 if (!MBRTU_R_Coils(port,adr,50+1,5,&PANEL_LSPN,0x1)) ULOGW ("[mt] read current LSP/LSN from panel [%d]",PANEL_LSPN);
		}

	 if (!MBRTU_R_Registers(port,adr,50+1,1,&in_reg,0x3)) { PANEL_FREQ=(uint)in_reg; /*ULOGW ("[mt] read frequency from panel [%d]",PANEL_FREQ); */ }
	 if (!MBRTU_R_Registers(port,adr,54+1,1,&in_reg,0x3)) { PANEL_CYCLE=(uint)in_reg; /* ULOGW ("[mt] read cycle time from panel [%d]",PANEL_CYCLE); */}
	 if (!MBRTU_R_Registers(port,adr,56+1,1,&in_reg,0x3)) { PANEL_TYPE=(uint)in_reg; /* ULOGW ("[mt] read type from panel [%d]",PANEL_TYPE); */}

	 if (!MBRTU_R_Registers(port,adr,64+1,1,&in_reg,0x3)) { DOZE1_TIME=(uint)in_reg; /* ULOGW ("[mt] read type from panel [%d]",PANEL_TYPE); */}
	 if (!MBRTU_R_Registers(port,adr,66+1,1,&in_reg,0x3)) { DOZE2_TIME=(uint)in_reg; /* ULOGW ("[mt] read type from panel [%d]",PANEL_TYPE); */}
	 if (!MBRTU_R_Registers(port,adr,68+1,1,&in_reg,0x3)) { DOZE1_TIME2=(uint)in_reg; /* ULOGW ("[mt] read type from panel [%d]",PANEL_TYPE); */}
	 if (!MBRTU_R_Registers(port,adr,70+1,1,&in_reg,0x3)) { DOZE2_TIME2=(uint)in_reg; /* ULOGW ("[mt] read type from panel [%d]",PANEL_TYPE); */}

	 // read all coils - commands from panel
	 MBRTU_R_Coils(port,adr,53+1,3,&PANEL_REGIM,0x1);
	 MBRTU_R_Coils(port,adr,155+1,2,&ST_ZERO,0x1);
	 
	 // send all values to panel	 	
	 memcpy (data,&SRV_REGIM,sizeof(SRV_REGIM));				// 400 
	 memcpy (data+4,&SRV_FACT,sizeof(SRV_FACT));				// 402
 	 memcpy (data+8,&SRV_FREQ,sizeof(SRV_FREQ));				// 404	
 	 memcpy (data+12,&SRV_COUNT,sizeof(SRV_COUNT));				// 406
 	 memcpy (data+16,&SRV_RASS,sizeof(SRV_RASS));				// 408
 	 memcpy (data+20,&SRV_MOMENT,sizeof(SRV_MOMENT));			// 410
 	 memcpy (data+24,&SRV_FREQ_R,sizeof(SRV_FREQ_R));			// 412
 	 memcpy (data+28,&SRV_POS,sizeof(SRV_POS));					// 414
 	 memcpy (data+32,&SRV_SPEED,sizeof(SRV_SPEED));				// 414
	 MBRTU_W_Multi_Registers(port,adr,400+1,20,(short *)data);

	 memcpy (data,&SRV_REGIM,sizeof(SRV_REGIM));				// 400 
	 memcpy (data+4,&SRV_FACT,sizeof(SRV_FACT));				// 402
 	 memcpy (data+8,&SRV_FREQ,sizeof(SRV_FREQ));				// 404	
 	 memcpy (data+12,&SRV_COUNT,sizeof(SRV_COUNT));				// 406
	 MBRTU_W_Multi_Registers(port,adr,400+1,20,(short *)data);

	 memcpy (data, &IZnal, 50);
	 if (PANEL_REGIM)
		{
		 data[50]=DOZA2_START; data[51]=0;
		 data[52]=DOZA1_START; data[53]=0;
		}
	 else
		{
		 data[50]=0; data[51]=DOZA2_START; 
		 data[52]=0; data[53]=DOZA1_START;
		}
	 iReadSuccess =MBRTU_W_Multi_Coils(port,adr,800+1,54,data);

	 memcpy (data, &sensor_pos, 1);							// 860
	 memcpy (data+4,&SRV_FACT,sizeof(SRV_FACT));			// 862
 	 memcpy (data+8,&SRV_FACT_PREV,sizeof(SRV_FACT_PREV));	// 864	
 	 memcpy (data+12,&SRV_2P,sizeof(SRV_2P));				// 866
 	 memcpy (data+16,&CORRECT,sizeof(CORRECT));				// 868
 	 memcpy (data+20,&WAIT_TIME,sizeof(WAIT_TIME));			// 870
 	 memcpy (data+24,&WAIT_TIME2,sizeof(WAIT_TIME2));		// 872

	 MBRTU_W_Multi_Registers(port,adr,860+1,14,(short *)data);

	 memcpy (data, &FQ1_ERROR, sizeof(FQ1_ERROR));
	 memcpy (data+2, &FQ1_BUSY, sizeof(FQ1_BUSY));
	 memcpy (data+4, &FQ1_OUTPUT, sizeof(FQ1_OUTPUT));
	 memcpy (data+6, &FQ2_ERROR, sizeof(FQ2_ERROR));
	 memcpy (data+8, &FQ2_BUSY, sizeof(FQ2_BUSY));
	 memcpy (data+10, &FQ2_OUTPUT, sizeof(FQ2_OUTPUT));
	 memcpy (data+12, &XUK1_ALARM, sizeof(XUK1_ALARM));
	 memcpy (data+14, &XUK1_OUTPUT, sizeof(XUK1_OUTPUT));
	 memcpy (data+16, &XUK2_ALARM, sizeof(XUK2_ALARM));
	 memcpy (data+18, &XUK2_OUTPUT, sizeof(XUK2_OUTPUT));
	 MBRTU_W_Multi_Registers(port,adr,10+1,20,(short *)data);

	 memcpy (data, &SERV_SU, 10);
	 iReadSuccess =MBRTU_W_Multi_Coils(port,adr,120+1,10,data);

	 memcpy (data, &FQ1_TRIG, 21); 	 
	 iReadSuccess =MBRTU_W_Multi_Coils(port,adr,100+1,8,data);
	 iReadSuccess =MBRTU_W_Multi_Coils(port,adr,110+1,8,data+7);
	 iReadSuccess =MBRTU_W_Multi_Coils(port,adr,120+1,8,data+14);

 	 if (count%10==0)
		{
		 memcpy (data,&VC_LG,sizeof(VC_LG));
 		 memcpy (data+4,&MOD2_1,sizeof(MOD2_1));
 		 memcpy (data+8,&MOD2_2,sizeof(MOD2_2));
 		 memcpy (data+12,&MOD2_3,sizeof(MOD2_3));
		 iReadSuccess =MBRTU_W_Multi_Registers(port,adr,1,8,(short *)data);
		 //if (!iReadSuccess) ULOGW ("[mt] write all AO signals success [AO: %f %f %f %f]",VC_LG,MOD2_1,MOD2_2,MOD2_3);
		 //else ULOGW ("[mt] write all AO signals failed error code: %d",iReadSuccess);
		 memcpy (data,&MO1_LG,sizeof(MO1_LG));
 		 memcpy (data+4,&MOD1_1,sizeof(MOD1_1));
 		 memcpy (data+8,&MOD1_2,sizeof(MOD1_2));
 		 memcpy (data+12,&MOD1_3,sizeof(MOD1_3));
		 iReadSuccess =MBRTU_W_Multi_Registers(port,adr,101,8,(short *)data);
		}	 
	 count++;	 
	}
 MBRTUClose(port);
 // recieve scene number from panel
 // service: recieve in/out command
 ExitThread(0);
}
//--------------------------------------------------------------------------------------------------------------------
// answer on modbus master request
int ModbusRTU_Answer(int adr, byte count)
{
 byte	data[40];		// data array
 UINT	crc=0;		//(* CRC checksum *)
 bool	res=FALSE;
 DWORD	bytes=0;

 res=uart_BinRecv (hPort2,(char *)data,count);
 if (!res) 
	{
	 ULOGW ("[MT] error recieve from panel");
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
	 ULOGW ("[MT] error [0x%x]: %ls",pac_GetLastError(), errorBuf);
	}
 else
	{
	 ULOGW ("[MT] RD (0x%x) (0x%x) (0x%x 0x%x) (0x%x 0x%x) [0x%x 0x%x]",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
	 if (data[2]>2)
		{
		 crc = CRC16 (data, data[2]+3);
	 	 ULOGW ("[MT] crc=%x [%x][%x] data=[%x][%x]",crc,crc/256,crc%256,data[data[2]+3],data[data[2]+4]);
		}
	}
 
 data[0]=adr;
 //data[1]=func;
 if (data[1]==3) count=data[5];
 else count=data[5]*2;
 data[2]=count;
 for (int i=0; i<count; i++) data[3+i]=0;
 crc = CRC16 (data, 3+count);
 data[3+count]=crc%256;
 data[4+count]=crc/256; 
 res = uart_BinSend(hPort2, (char *)data, 5+count);
 if (data[1]==3) ULOGW ("[MT] SD (0x%x) (0x%x) (0x%x) (0x%x) [0x%x 0x%x]",data[0],data[1],data[2],data[3],data[4],data[5]);
 return 0;
}
//--------------------------------------------------------------------------------------------------------------------
// Modbus read coils/registers function
int ModbusRTU_Read(int adr, int start, int count, byte *iRecv, int func)
{
 byte data[40];		// data array
 UINT	crc=0;		//(* CRC checksum *)
 bool	res=FALSE;
 DWORD bytes=0;

 data[0]=adr;
 data[1]=func;
 data[2]=start/256;
 data[3]=start%256;
 data[4]=count/256;
 data[5]=count%256;
 crc = CRC16 (data, 6);

 data[6]=crc%256;
 data[7]=crc/256; 
 res = uart_BinSend(hPort2, (char *)data, 9);
 ULOGW ("[MT] (0x%x) (0x%x) (0x%x 0x%x) (0x%x 0x%x) [0x%x 0x%x]",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);

 if (res) ULOGW ("[MT] send to panel success");
 else  
	{
	 ULOGW ("[MT] error send to panel");
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
	 ULOGW ("[MT] error [0x%x]: %ls",pac_GetLastError(), errorBuf);
	}	 
 Sleep (30);
 bytes=uart_GetDataSize(hPort2,IN_DATA);
 ULOGW ("[MT] recieve %d bytes",bytes);
 res=uart_BinRecv (hPort2,(char *)iRecv,10);
 if (!res) 
	{
	 ULOGW ("[MT] error recieve from panel");
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
	 ULOGW ("[MT] error [0x%x]: %ls",pac_GetLastError(), errorBuf);
	}
 if (bytes)
	{
	 if (iRecv[2]>2)
		{
		 crc = CRC16 (data, iRecv[2]+3);
	 	 ULOGW ("[MT] crc=%x [%x][%x] data=[%x][%x]",crc,crc/256,crc%256,iRecv[iRecv[2]+3],iRecv[iRecv[2]+4]);
		}
	}
 return 0;
}
//--------------------------------------------------------------------------------------------------------------------
// Modbus write coils/registers function
int ModbusRTU_Write(int adr, int start, int count, byte len, byte *iSend, int func)
{
 byte data[40];		// data array
 UINT	crc=0;		//(* CRC checksum *)
 bool	res=FALSE;
 DWORD bytes=0;

 data[0]=adr;
 data[1]=func;
 if (func==PresetMultiRegister)
	{
	 data[2]=start/256;
	 data[3]=start%256;
	 data[4]=count/256;
	 data[5]=count%256;
     data[6]=len;
     for (byte i=0; i<len; i++) data[7+i]=iSend[i];	 
     crc = CRC16 (data, 7+len); 
	 data[7+len]=crc%256;
	 data[7+len+1]=crc/256; 
     ULOGW ("[MT] (0x%x) (0x%x) (0x%x 0x%x) (0x%x 0x%x) (0x%x) 0x%x [0x%x 0x%x]",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9]);
     res = uart_BinSend(hPort2, (char *)data, len+9);
	}
 if (func==PresetSingleRegister) 
	{
 	 data[2]=start/256;
	 data[3]=start%256;
	 for (byte i=0; i<len; i++) data[4+i]=iSend[i];
     crc = CRC16 (data, 4+len); 
	 data[4+len]=crc%256;
	 data[4+len+1]=crc/256; 
	 ULOGW ("[MT] (0x%x) (0x%x) (0x%x 0x%x) (0x%x 0x%x) [0x%x 0x%x]",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
	 res = uart_BinSend(hPort2, (char *)data, len+6);
	}

 if (res) ULOGW ("[MT] send to panel success");
 else  
	{
	 ULOGW ("[MT] error send to panel");
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
	 ULOGW ("[MT] error [0x%x]: %ls",pac_GetLastError(), errorBuf);
	}	 
 Sleep (100);
 bytes=uart_GetDataSize(hPort2,IN_DATA);
 ULOGW ("[MT] recieve %d bytes",bytes);
 res=uart_BinRecv (hPort2,(char *)iSend,bytes);
 if (!res) 
	{
	 ULOGW ("[MT] error recieve from panel");
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
	 ULOGW ("[MT] error [0x%x]: %ls",pac_GetLastError(), errorBuf);
	}
 if (bytes)
	{
     ULOGW ("[MT] RD (0x%x) (0x%x) (0x%x 0x%x) (0x%x 0x%x) (0x%x 0x%x 0x%x 0x%x ... )",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9]);	 
	 if (iSend[5]>2)
		{
		 crc = CRC16 (data, iSend[2]*2+3);
	 	 ULOGW ("[MT] crc=%x [%x][%x] data=[%x][%x]",crc,crc/256,crc%256,iSend[iSend[2]+3],iSend[iSend[2]+4]);
		}
	}
 return 0;
}
//---------------------------------------------------------------------------------------------------------------------
int crc16 (byte a)
{
 UINT reg,tmp;
 UINT i;
    
 reg = 0xFFFF;                 
 reg ^= (UINT)a;    
    
 for(i=0;i<8;i++)
    {
     tmp = reg;
     reg >>= 1;        
     if((tmp <<=15)==0x8000) reg ^= 0xA001;
    }
 return reg;
}
//--------------------------------------------------------------------------------------------------------------------
UINT CRC (const BYTE* const Data, const BYTE DataSize)
{
 UINT _CRC = 0;
 BYTE* _Data = (BYTE*)Data;
 for(unsigned int i = 0; i < DataSize; i++) 
    {
     _CRC = CalcCRC(*_Data, _CRC);
     _Data++;
    }

 return _CRC;
}
//--------------------------------------------------------------------------------------------------------------------
unsigned int CalcCRC(unsigned int c,unsigned int crc)
{
 int count,flg;
 for (count=0;count<8;count++)
    {
     flg=crc&0x8000;
     crc<<=1;
     if(((c)&0x80)!=0)crc+=1;
	 if(flg!=0)crc^=0x1021;
	    c<<=1;
    }
 return crc;
}

WORD CRC16 (const BYTE *nData, WORD wLength)
{
static const WORD wCRCTable[] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

BYTE nTemp;
WORD wCRCWord = 0xFFFF;

   while (wLength--)
   {
      nTemp = *nData++ ^ wCRCWord;
      wCRCWord >>= 8;
      wCRCWord ^= wCRCTable[nTemp];
   }
   return wCRCWord;

}

VOID ReadCOM (LPVOID lpParam)
{
 bool	res=FALSE;
 DWORD bytes=0;
 byte	iRecv[50];
 while (WorkEnable)
	{
	 Sleep (50);
	 bytes=uart_GetDataSize(hPort5,IN_DATA);
	 ULOGW ("[MT] recieve %d bytes",bytes);
	 res=uart_BinRecv (hPort5,(char *)iRecv,10);
	 if (!res) 
		{
		 ULOGW ("[MT] error recieve from panel");
		 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
		 ULOGW ("[MT] error [0x%x]: %ls",pac_GetLastError(), errorBuf);
		}
	 else ULOGW ("[MT] RECV (0x%x) (0x%x) (0x%x 0x%x) (0x%x 0x%x) (0x%x 0x%x) [0x%x 0x%x]",iRecv[0],iRecv[1],iRecv[2],iRecv[3],iRecv[4],iRecv[5],iRecv[6],iRecv[7],iRecv[8],iRecv[9]);
	}
}
