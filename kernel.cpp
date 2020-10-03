#define MAIN_LOG	"\\Micro_SD\\Temp\\FTP\\log3"

#include <windows.h>
#include <urlmon.h>
#include <oledb.h>

#include "PACERROR.H"
#include "Modbus_ARM.h"
#include "winpacsdk.h"
#include "kernel.h"

bool Init (VOID);					// initialize function
bool deInit (VOID);					// deinitialize function
VOID dispatcher (VOID);				// dispatcher 
VOID ULOGW (CHAR* string, ...);		// log file function
VOID GetInfo (VOID);				// get system information

VOID GetAllIOStatus (LPVOID lpParam);	// [Thread] Get all input/output status/values 
VOID MainAlgorithm (LPVOID lpParam);	// [Thread] Main algorithm thread 
extern VOID PanelConnection (LPVOID lpParam);	// [Thread] Get/Set all input/output values from/to panel
extern VOID ServoConnection (LPVOID lpParam);	// [Thread] Get/Set all input/output values from/to servoprivod
extern VOID ServiceThread (LPVOID lpParam);		// [Thread] Service thread

extern	bool WriteDataFile (byte format);		// write data in file
extern	bool LoadDataFile (byte format);		// load data from file

VOID OutModbusError (int iInitSuccess);	// out in console and log modbus error
VOID OutError (int iInitSuccess);		// out in console and log winpac errors
VOID ConveyerCycle(uint step);			// cycle conveyer

int CALLBACK slot2_callback_proc();
int CALLBACK slot3_callback_proc();

extern	HANDLE	hPort3;					// SRV port handler

extern FILE *Log;
//----------------------------------------------------------------------------------------------------
INT _tmain(INT argc, CHAR *argv[])
{  
  Log = fopen(MAIN_LOG,"w");
  // if (!Log) { printf ("critical! error open port, maybe MicroSD card not inserted"); Sleep (300000); return -1; }
  if (fclose (Log))	printf ("not closed");			// close log file
  Log=NULL;
  printf ("start");
  ULOGW ("uaiz conveer kernel v0.5 started");
  LoadDataFile (0);
  GetInfo ();			// get system info
  if (Init ()) ULOGW ("initialize success");
  else { ULOGW ("initialize error! please check out all hardware and software"); Sleep (30000); return -1; }
  dispatcher ();		// start dispatcher
  deInit ();			// deinitialize all
  ULOGW ("uaiz kernel finished");
  return 0; 
}
//----------------------------------------------------------------------------------------------------
// initialise 
bool Init (VOID)
{
 hPort = uart_Open("COM0:,115200,N,8,1");
 if (hPort) ULOGW ("open COM0 internal port on speed 115200 [8-N-1] success (hPort=%x)",hPort);
 else 
	{
	 pac_GetErrorMessage(pac_GetLastError(), errorBuf);
	 ULOGW ("error open COM0 internal port on speed 115200 [8-N-1] error [0x%x]: %s",pac_GetLastError(), errorBuf);
	 return FALSE;
	}
 return TRUE;
}
//----------------------------------------------------------------------------------------------------
// deinitialise function 
bool deInit (VOID)
{
 if (hPort) { uart_Close(hPort); ULOGW ("close COM0 internal port"); }
 WorkEnable=0;			// disable all work 
 if (hPort3) { uart_Close(hPort3); ULOGW ("close modbusRTU COM1 [panel] port"); }
 MBRTUClose(3);
 //pac_EnableSlotInterrupt(2, false); // disable slot2 [87053W] interrupt
 //pac_UnregisterSlotInterrupt(2);
 //MBRTUClose (1);		// close panel interface
 return TRUE;
}
//----------------------------------------------------------------------------------------------------
VOID dispatcher (VOID)
{
 WorkEnable=1;			// work enable
 UINT	count=100000,stopbytime=0;
 TCHAR  InBuffer[300]; CHAR   buf[100];

 MEMORYSTATUS lpBuffer;
 DWORD	AvailPhys=0;
 hThrdMT = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) PanelConnection, &hReadEventsMT, 0, &dwThrdIDMT);
 if (hThrdMT==NULL) ULOGW ("[mt] Panel thread start error");
 if (hThrdMT) CeSetThreadPriority(hThrdMT, 20); 

 hThrdServ = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) ServoConnection, &hReadEventsServ, 0, &dwThrdIDServ);
 if (hThrdServ==NULL) ULOGW ("[srv] Servo thread start error");
 if (hThrdServ) CeSetThreadPriority(hThrdServ, 1); 

 Sleep (2000);
 hThrdIO = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) GetAllIOStatus, &hReadEventsIO, 0, &dwThrdIDIO);
 if (hThrdIO==NULL) ULOGW ("[io] Get IO thread start error");
 if (hThrdIO) CeSetThreadPriority(hThrdIO, 2); 

 hThrdAlg = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) MainAlgorithm, &hReadEventsAlg, 0, &dwThrdIDAlg);
 if (hThrdAlg==NULL) ULOGW ("[alg] Algorithm thread start error");
 if (hThrdAlg) CeSetThreadPriority(hThrdAlg, 3); 

 //hThrdService = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE) ServiceThread, &hReadEventsService, 0, &dwThrdIDService); 
 //if (hThrdService==NULL) ULOGW ("[serv] Service thread start error");
 //if (hThrdService) CeSetThreadPriority(hThrdService, 3); 

 GlobalMemoryStatus(&lpBuffer); ULOGW ("memory: [%d] %ld %ld",lpBuffer.dwMemoryLoad, lpBuffer.dwAvailPhys, lpBuffer.dwAvailVirtual); 
 AvailPhys=lpBuffer.dwAvailPhys;
// while (!PANEL_STOP && count)
 while (1)
	{
	 //GlobalMemoryStatus(&lpBuffer); ULOGW ("memory: [%d] %ld %ld",lpBuffer.dwMemoryLoad, lpBuffer.dwAvailPhys, lpBuffer.dwAvailVirtual);
	 if (PANEL_EXIT) break;
 	 if (count%5==0)
		{
		 GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, TEXT("hh:mm:ss"), InBuffer, sizeof(InBuffer));
		 WideCharToMultiByte(CP_ACP, 0, InBuffer, sizeof(InBuffer), buf, sizeof(buf), NULL,NULL);
		 if (strstr (buf,"22:00:")) { stopbytime=1; break; }
		}

	 Sleep (2000); count--;
	}
 GlobalMemoryStatus(&lpBuffer); ULOGW ("memory: [%d] %ld %ld [eaten=%ld]",lpBuffer.dwMemoryLoad, lpBuffer.dwAvailPhys, lpBuffer.dwAvailVirtual,AvailPhys-lpBuffer.dwAvailPhys);
// printf ("memory: [%d] %ld %ld [eaten=%ld]",lpBuffer.dwMemoryLoad, lpBuffer.dwAvailPhys, lpBuffer.dwAvailVirtual,AvailPhys-lpBuffer.dwAvailPhys);
 Sleep (15000);			// waiting for the stop
 if (stopbytime) pac_Reboot();
 WorkEnable=0;			// disable all work
 Sleep (10000);			// waiting for the stop
}
//----------------------------------------------------------------------------------------------------
// get system information
VOID GetInfo (VOID)
{
  DWORD mem_size,eeprom_size;
  char CPU[32],MAC[32],OS[32],SN[32],DLL[32];
  char slotCard[10];
  byte LAN = 1;
  byte slot = 1;
  int type = 0, nBattery1=0, nBattery2=0;
  bool bExist;

  bExist = pac_SDExists();
  pac_GetMacAddress(LAN,MAC);
  mem_size = pac_GetMemorySize(WINPAC_MEM_SRAM);
  eeprom_size = pac_GetMemorySize(WINPAC_MEM_EEPROM);
  pac_GetCPUVersion(CPU);
  pac_GetOSVersion(OS);
  pac_GetSerialNumber(SN);
  pac_GetSDKVersion(DLL);
  type=pac_GetRotaryID();
  nBattery1 = pac_GetBatteryLevel(1);
  nBattery2 = pac_GetBatteryLevel(2);
  ULOGW ("PAC seial number %s",SN);
  ULOGW ("CPU: %s | memory size: %ldKb | eeprom size: %ldKb",CPU,mem_size,eeprom_size);
  ULOGW ("OS installed: %s | SDK version: %s",OS, DLL);
  ULOGW ("Rotary switch in %d position", type);
  ULOGW ("mac adress %s",MAC);
  ULOGW ("battery level [1]=%d | [2]=%d",nBattery1,nBattery2);
  ULOGW ("device booted %ld seconds ago",pac_GetBPTimerTimeTick_ms()/1000);

  if (bExist) ULOGW ("micro SD card is installed"); else ULOGW ("micro SD card is not installed");
  for (slot=0; slot<=7; slot++)
	{
	 pac_GetModuleName(slot, slotCard);
     type=pac_GetModuleType(slot);
	 switch (type)
		{
		 case PAC_PARALLEL: ULOGW ("[%d] module in slot %s [0x%x] | general I-8000W module",slot,slotCard,type); break;
		 case PAC_PARALLEL_RW: ULOGW ("[%d] module in slot %s [0x%x] | I-8000RW module", slot,slotCard,type); break;
		 case PAC_SCAN: ULOGW ("[%d] module in slot %s [0x%x] | scan module", slot,slotCard,type); break;
		 case PAC_DI32: ULOGW ("[%d] module in slot %s [0x%x] | I-8000W module with 32 DI channels", slot,slotCard,type); break;
		 case PAC_32BIT: ULOGW ("[%d] module in slot %s [0x%x] | bit32 module", slot,slotCard,type); break;
		 case PAC_DO32: ULOGW ("[%d] module in slot %s [0x%x] | I-8000W module with 32 DO channels", slot,slotCard,type); break;
		 case PAC_DI16DO16: ULOGW ("[%d] module in slot %s [0x%x] | I-8000W module with 16 DI channels and 16 DO channels", slot,slotCard,type); break;
		 case PAC_DI16: ULOGW ("[%d] module in slot %s [0x%x] | I-8000W module with 16 DI channels",slot,slotCard,type); break;
		 case PAC_DO16: ULOGW ("[%d] module in slot %s [0x%x] | I-8000W module with 16 DO channels",slot,slotCard,type); break;
		 case PAC_DI8DO8: ULOGW ("[%d] module in slot %s [0x%x] | I-8000W module with 8 DI channels and 8 DO channels", slot,slotCard,type); break;
		 case PAC_8K: ULOGW ("[%d] module in slot %s [0x%x] | 8K series module",slot,slotCard,type); break;
		 default: ULOGW ("[%d] no module defined [0x%x]", slot,type);
		}
	}
}
//----------------------------------------------------------------------------------------------------
// [Thread] Get all input/output status/values 
VOID GetAllIOStatus (LPVOID lpParam)
{
 DWORD do_value,di_value,do_value4=0,do_value6,do_value7=0;
 bool rs=0; byte cnt=0;
 // float ao_value;
 while (WorkEnable)
	{
	 // [2] i-87053	 	 
	 pac_ReadDI(hPort,2,16, &di_value);
	 if (di_value!=di_value2_prev)
		{
		 SERV_SU=(byte)((di_value&0x800)>>11);		// Сервопривод в режиме "системное управление"
		 ULOGW ("[wp][2] (0)%d|(1)%d|(2)%d|(3)%d|(4)%d|(5)%d|(6)%d|(7)%d|(8)%d|(9)%d",FQ1_ERROR,FQ1_BUSY,FQ1_OUTPUT,FQ2_ERROR,FQ2_BUSY,FQ2_OUTPUT,XUK1_ALARM,XUK1_OUTPUT,XUK2_ALARM,XUK2_OUTPUT);
		 di_value2_prev=di_value;
		}
	 // [3] i-87053
	 pac_ReadDI(hPort,3,16, &di_value);
	 if (di_value!=di_value3_prev)
		{
		 PRESENT1=(byte)(di_value&0x1);				// Наличие детали под дозатором 1
		 PRESENT2=(byte)((di_value&0x2)>>1);		// Наличие детали под дозатором 2
		 ZERO_INST=(byte)((di_value&0x4)>>2);		// Ноль
		 if (ZERO_INST) ZERO_INST1=1;
		 //SERV_SU=(byte)(di_value&0x8)>>3;			// Сервопривод в режиме "системное управление"
		 SERV_READY=(byte)((di_value&0x10)>>4);		// Готовность сервопривода (RD/DICOM)
		 SERV_ALARM=(byte)((di_value&0x20)>>5);		// Авария сервопривода (ALM/DICOM)
		 SERV_CON_VKL=(byte)((di_value&0x40)>>6);		// Контактор работы сервопривода включен
		 SERV_UACTIVE=(byte)((di_value&0x80)>>7);		// Наличие напряжения
		 DOZA1_READY=(byte)((di_value&0x100)>>8);		// Готовность дозатора №1
		 DOZA2_READY=(byte)((di_value&0x200)>>9);		// Готовность дозатора №2	 
		 //DOZA1_READY=DOZA2_READY=1;
		 CONV_STOP=(byte)((di_value&0x400)>>10);		// Аварийная остановка конвейера
		 DOZA1_ERROR=(byte)((di_value&0x800)>>11);	// Авария дозатора №1
		 DOZA2_ERROR=(byte)((di_value&0x1000)>>12);	// Авария дозатора №2
		 di_value3_prev=di_value;
		 //ULOGW ("[wp][3] (0)%d|(1)%d|(2)%d|(3)%d|(4)%d|(5)%d|(6)%d|(7)%d|(8)%d|(9)%d",SERV_SU,SERV_READY,SERV_ALARM,SERV_CON_VKL,SERV_UACTIVE,DOZA1_READY,DOZA2_READY,CONV_STOP,DOZA1_ERROR,DOZA2_ERROR);
		}

	 // [6] i-87064
	 do_value6=SERV_ST2*128+DOZA2_START*64+DOZA1_START*32+SERV_RES*16+SERV_ST1*8+SERV_SP2*4+SERV_SP1*2+SERV_SON;
	 if (do_value6!=do_value6_prev)
		{
		 if (1) // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		 //if (SERV_SU) 
			{
			 rs=pac_WriteDO(hPort,6,8, do_value6);
			 do_value6_prev=do_value6;
			 ULOGW ("[wp] write [4]0x%x [6!]0x%x [7]0x%x (res=%d)",do_value4,do_value6,do_value7,rs);
			}
		}
	 Sleep (10);
	 cnt++;
	}
 do_value=0x0; pac_WriteDO(hPort,4,8, do_value);
 do_value=0x0; pac_WriteDO(hPort,6,8, do_value);
 do_value=0x0; pac_WriteDO(hPort,7,16, do_value);
 ExitThread(0);
}
//----------------------------------------------------------------------------------------------------
// main algorithm
VOID MainAlgorithm (LPVOID lpParam)
{
 DWORD	do_value=0,di_value=0,di_latch=0;
 bool	res=0;
 uint	cnt=10;

 while (WorkEnable)
	{
	 if (FQ1_OUTPUT || FQ2_OUTPUT)
		{
		 ConveyerCycle(1); // LAST24.04
		 if (PRESENT1) { IZnal[2]=1; ULOGW ("[alg] [FQ1] shapka found on pos [2]"); StekloFound=1; totalDetected++; }
		 else IZnal[2]=0;
		 //if (PRESENT2) { IZnal[14]=1; ULOGW ("[alg] [FQ2] steklodetal found [14]"); ShapkaFound=1; totalDetected++; }
		 //else IZnal[14]=0;

		 cnt=2000; // время ожидания / доводки конвеера (20мс)
		 while (!ConvStopped && cnt)
			{
			 Sleep (20); cnt--;
			}
		 ULOGW ("[alg] waiting time for conveer stopped [%d]ms [%d|%d|%d|%d]",cnt*20,DOZE1_TIME,DOZE2_TIME,DOZE1_TIME2,DOZE2_TIME2);
		 if (DOZE1_TIME<100) DOZE1_TIME=2000;
		 if (DOZE2_TIME<100) DOZE2_TIME=2000;

		 //if (!Dozation1 && !Dozation2 && ConvStopped && 0)
		 if (ConvStopped)
			{
			 //if (FQ1_OUTPUT && IZnal[4+PANEL_REGIM]) { DOZA1_START=1; ULOGW ("[alg] start dozation 1 (shapka)"); Dozation1=1; }
			 //if (FQ2_OUTPUT && IZnal[16+PANEL_REGIM]) { DOZA2_START=1; ULOGW ("[alg] start dozation 2 (steklo)"); Dozation2=1; }
			 Dozation2=1;
			 Dozation1=1;
			 //Sleep (200);

			 //if (FQ1_OUTPUT && IZnal[4+PANEL_REGIM]) 
			 if (FQ1_OUTPUT) 
				{
				 //cnt=1; // 600!
				 //while (DOZA1_START && !DOZA1_READY && cnt)
				 //while (cnt)
					//{
					 Sleep (DOZE1_TIME); //cnt--;
					//}
				}
			 /*
			 if (FQ2_OUTPUT && IZnal[16+PANEL_REGIM]) 
				{
				 cnt=1;
				 while (DOZA2_START && !DOZA2_READY && cnt)
					{				 
					 Sleep (DOZE2_TIME); cnt--;
					}
				}*/
			 //Sleep (1500);	// waiting for dozator1
			 //if (DOZA2_START) { Dozation2=0; ULOGW ("[alg] dozation finished"); }
			 //if (DOZA1_START) { Dozation1=0; ULOGW ("[alg] dozation finished"); }
			 //DOZA1_START=0; DOZA2_START=0;	// temporary za dozator			 			 

			 // если установлен и первый и второй, сбрасываем второй (нет пробелов)
			 //StekloFound=0; 
			 //ShapkaFound=0;			 
			 Dozation2=0;
			 Dozation1=0;
			 Sleep (2000); totalLoaded++;
			}
		}
	 Sleep (50);
	}
 //ULOGW ("set FQ1_IN5 to zero for stop continuous measurements"); 
 ExitThread(0); 
}
//----------------------------------------------------------------------------------------------------
// decode WinPAC error code
VOID OutError (int iInitSuccess)
{
 switch (iInitSuccess)
	{
	 case 0x00001: ULOGW ("[error %x] Unknow Error",iInitSuccess); break;
	 case 0x10001: ULOGW ("[error %x] Slot registered error",iInitSuccess); break;
	 case 0x10002: ULOGW ("[error %x] Slot not registered error",iInitSuccess); break;
	 case 0x10003: ULOGW ("[error %x] Unknown Module",iInitSuccess); break;
	 case 0x10004: ULOGW ("[error %x] Module doesn't exist",iInitSuccess); break;
	 case 0x10005: ULOGW ("[error %x] Invalid COM port number",iInitSuccess); break;
	 case 0x10006: ULOGW ("[error %x] Function not supported",iInitSuccess); break;
	 case 0x11001: ULOGW ("[error %x] EEPROM accesses invalid address",iInitSuccess); break;
	 case 0x11002: ULOGW ("[error %x] SRAM accesses invalid address",iInitSuccess); break;
	 case 0x11003: ULOGW ("[error %x] SRAM accesses invalid type",iInitSuccess); break;
	 case 0x11004: ULOGW ("[error %x] NVRAM accesses invalid address",iInitSuccess); break;
	 case 0x11005: ULOGW ("[error %x] EEPROM write protection",iInitSuccess); break;
	 case 0x11006: ULOGW ("[error %x] EEPROM write fail",iInitSuccess); break;
	 case 0x11007: ULOGW ("[error %x] EEPROM read fail",iInitSuccess); break;
	 case 0x12001: ULOGW ("[error %x] The input value is invalid",iInitSuccess); break;
	 case 0x12002: ULOGW ("[error %x] The wdt doesn't exist",iInitSuccess); break;
	 case 0x12003: ULOGW ("[error %x] The wdt init error",iInitSuccess); break;
	 case 0x13001: ULOGW ("[error %x] Create interrupt's event failure",iInitSuccess); break;
	 case 0x14001: ULOGW ("[error %x] Uart check sum error",iInitSuccess); break;
	 case 0x14002: ULOGW ("[error %x] Uart read timeout",iInitSuccess); break;
	 case 0x14003: ULOGW ("[error %x] Uart response error",iInitSuccess); break;
	 case 0x14004: ULOGW ("[error %x] Uart under input range",iInitSuccess); break;
	 case 0x14005: ULOGW ("[error %x] Uart exceed input range",iInitSuccess); break;
	 case 0x14006: ULOGW ("[error %x] Uart open filed",iInitSuccess); break;
	 case 0x14007: ULOGW ("[error %x] Uart get Comm Modem status error",iInitSuccess); break;
	 case 0x14008: ULOGW ("[error %x] Uart get wrong line status",iInitSuccess); break;
	 case 0x15001: ULOGW ("[error %x] IO card does not support this API function",iInitSuccess); break;
	 case 0x15002: ULOGW ("[error %x] API unsupport this IO card",iInitSuccess); break;
	 case 0x15003: ULOGW ("[error %x] Slot's value exceeds its range",iInitSuccess); break;
	 case 0x15004: ULOGW ("[error %x] Channel's value exceeds its range",iInitSuccess); break;
	 case 0x15005: ULOGW ("[error %x] Gain's value exceeds its range",iInitSuccess); break;
	 case 0x15006: ULOGW ("[error %x] Unsupported interrupt mode",iInitSuccess); break;
	 case 0x15007: ULOGW ("[error %x] I/O value is out of the range",iInitSuccess); break;
	 case 0x15008: ULOGW ("[error %x] I/O channel is out of the range",iInitSuccess); break;
	 default: ULOGW ("[error] unknown error code [0x%x]",iInitSuccess);
	}
}
//----------------------------------------------------------------------------------------------------
// decode WinPAC error code
VOID OutModbusError (int iInitSuccess)
{
 switch (iInitSuccess)
	{
	 case	MB_RTC_OK:				ULOGW ("[modbus] error [%d] RTC OK",iInitSuccess); break;
	 case	MB_ILLEGAL_FUNCTION:	ULOGW ("[modbus] error [%d] illegal function",iInitSuccess); break;
	 case	MB_ILLEGAL_DATA_ADDRESS:ULOGW ("[modbus] error [%d] illegal data address",iInitSuccess); break;
	 case	MB_ILLEGAL_DATA_VALUE:	ULOGW ("[modbus] error [%d] illegal data value",iInitSuccess); break;
	 case	MB_SLAVE_DEVICE_FAILURE:ULOGW ("[modbus] error [%d] slave device failure",iInitSuccess); break;
	 case	MB_ACKNOWLEDGE:			ULOGW ("[modbus] error [%d] acknowledge",iInitSuccess); break;
	 case	MB_SLAVE_DEVICE_BUSY:	ULOGW ("[modbus] error [%d] slave device busy",iInitSuccess); break;
	 case	MB_NEGATIVE_ACKNOWLEDGE:ULOGW ("[modbus] error [%d] negative acknowledge",iInitSuccess); break;
	 case	MB_MEMORY_PARITY_ERROR:	ULOGW ("[modbus] error [%d] memory parity error",iInitSuccess); break;
	 case	MB_GATEWAY_PATH_UNAVAILABLE:	ULOGW ("[modbus] error [%d] gateway path unavailable",iInitSuccess); break;
	 case	MB_GATEWAY_FAILED_TO_RESPOND:	ULOGW ("[modbus] error [%d] gateway failed to respond",iInitSuccess); break;
	 case	MB_OPEN_PORT_ERROR:		ULOGW ("[modbus] error [%d] open COM/TCP Port error",iInitSuccess); break;
	 case	MB_PORTNO_OVER:			ULOGW ("[modbus] error [%d] COM Port is 1-8",iInitSuccess); break;
	 case	MB_PORT_NOT_OPEN:		ULOGW ("[modbus] error [%d] COM/TCP Port does not open yet",iInitSuccess); break;
	 case	MB_FUN_ERROR:			ULOGW ("[modbus] error [%d] modbus Fun. No. error",iInitSuccess); break;
	 case	MB_READ_COUNT_OVER:		ULOGW ("[modbus] error [%d] reading Count of Register or Bits is over range (RTU: 120 register, 1920 coils)",iInitSuccess); break;
	 case	MB_SLAVENO_OVER:		ULOGW ("[modbus] error [%d] modbus Slave No. must be 1 - 247",iInitSuccess); break;
	 case	MB_ADDRESS_OVER:		ULOGW ("[modbus] error [%d] register or Coil Address must count from 1",iInitSuccess); break;
	 case	MB_COMM_TIMEOUT:		ULOGW ("[modbus] error [%d] communication timeout",iInitSuccess); break;
	 case	MB_CRC_ERROR:			ULOGW ("[modbus] error [%d] RTU CRC Check error",iInitSuccess); break;
	 case	MB_LRC_ERROR:			ULOGW ("[modbus] error [%d] ASCII LRC Check error",iInitSuccess); break;
	 case	MB_INVALID_SOCKET:		ULOGW ("[modbus] error [%d] Initial Socket error",iInitSuccess); break;
	 case	MB_TCP_CONNECT_ERROR:   ULOGW ("[modbus] error [%d] connect Remote Modbus Server error",iInitSuccess); break;
	 case	MB_TCP_SEND_ERROR:      ULOGW ("[modbus] error [%d] send TCP Data error",iInitSuccess); break;
	 case	MB_TCP_TIMEOUT:         ULOGW ("[modbus] error [%d] Waiting Modbus Response Timeout",iInitSuccess); break; 
	 case	MB_WSA_INIT_ERROR:      ULOGW ("[modbus] error [%d] WSA Startup error",iInitSuccess); break;
	 case	MB_TCP_SOCKET_ERROR:    ULOGW ("[modbus] error [%d] Create Socket error",iInitSuccess); break;
	 case	MB_TCP_BIND_ERROR:      ULOGW ("[modbus] error [%d] TCP Server Bind error",iInitSuccess); break;
	 case	MB_TCP_LISTEN_ERROR:    ULOGW ("[modbus] error [%d] TCP Server Listen error",iInitSuccess); break;
	 case	MB_TCP_HAS_DATA:		ULOGW ("[modbus] error [%d] it has data from remote Modbus Master",iInitSuccess); break;
	 case	MB_WRITE_COUNT_OVER:    ULOGW ("[modbus] error [%d] reading Count of Register or Bits is over range (RTU: 120 register, 1920 coils)",iInitSuccess); break;
	 default: ULOGW ("[modbus] unknown error code [0x%x]",iInitSuccess);
	}
}
//----------------------------------------------------------------------------------------------------
void ConveyerCycle(uint step)
{
 IZnal[0]=0;
 for (int r=sizeof(IZnal); r>0; r--)
	IZnal[r]=IZnal[r-1];
 IZnal[0]=IZnal[48]=IZnal[49]=IZnal[47]=0;
 //ULOGW ("[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]",IZnal[0],IZnal[1],IZnal[2],IZnal[3],IZnal[4],IZnal[5],IZnal[6],IZnal[7],IZnal[8],IZnal[9],IZnal[10],IZnal[11],IZnal[12],IZnal[13],IZnal[14],IZnal[15],IZnal[16],IZnal[17],IZnal[18],IZnal[19]);
}
//----------------------------------------------------------------------------------------------------
#define	DATABASE_DSS "uaiz"
UINT ConnectDatabase ()
{
 HRESULT	hr = NOERROR;
 return hr;
}
//----------------------------------------------------------------------------------------------------
int CALLBACK slot2_callback_proc()
{
 //HANDLE hd=pac_GetSlotInterruptEvent(2);  // This function retrieves the slot event handle which registered by pac_InterruptInitialize
 //DWORD pac_GetSlotInterruptID(BYTE Slot); // This function retrieves the slot interrupt ID
 ULOGW ("slot2 [87053W] interrupt recieved");
 DWORD di_value;
 pac_ReadDI(hPort,2,16, &di_value);
 FQ1_OUTPUT=(byte)((di_value&0x1));			// Сигнал ошибки первого датчика FQ (ERROR)
 FQ1_BUSY=(byte)((di_value&0x2)>>1);		// Сигнал занято первого датчика FQ (BUSY)
 FQ1_ERROR=(byte)((di_value&0x4)>>2);		// Сигнал решения первого датчика FQ (OUTPUT)
 FQ2_OUTPUT=(byte)((di_value&0x8)>>3);		// Сигнал ошибки второго датчика FQ (ERROR)
 FQ2_BUSY=(byte)((di_value&0x10)>>4);		// Сигнал занято второго датчика FQ (BUSY)
 FQ2_ERROR=(byte)((di_value&0x20)>>5);		// Сигнал решения второго датчика FQ (OUTPUT)
 XUK1_ALARM=(byte)((di_value&0x40)>>6);		// Сигнал "Авария" датчика XUK (ALARM)
 XUK1_OUTPUT=(byte)((di_value&0x80)>>7);	// Сигнал "Решение" датчика XUK (OUTPUT)
 XUK2_ALARM=(byte)((di_value&0x100)>>8);	// Сигнал "Авария" датчика XUK (ALARM)
 XUK2_OUTPUT=(byte)((di_value&0x200)>>9);	// Сигнал "Решение" датчика XUK (OUTPUT)
 SERV_SU=(byte)((di_value&0x800)>>11);		// Сервопривод в режиме "системное управление"
 pac_InterruptDone(2);
 ULOGW ("slot2 [87053W] interrupt close");
 return 1;
}
//----------------------------------------------------------------------------------------------------
int CALLBACK slot3_callback_proc()
{
 ULOGW ("slot3 [87053W] interrupt recieved");
 pac_InterruptDone(3);
 return 1;
}
