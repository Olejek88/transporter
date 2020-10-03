#ifndef __WINPACSDK_H
#define __WINPACSDK_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WINPACSDK_EXPORTS
#define WINPAC_API extern "C" __declspec(dllexport) 
#else
#define WINPAC_API extern "C" __declspec(dllimport) 
#endif

#define WINPAC_PRIVATE_API WINPAC_API
#define WINPAC_SERAL_NUMBER_LENGTH		8


#define PAC_PARALLEL 0x80
#define PAC_PARALLEL_RW 0x81
#define PAC_SCAN     0x40
#define PAC_DI32     0xE3
#define PAC_32BIT    0x20
#define PAC_DO32     0xE0
#define PAC_DI16DO16 0xE2
#define PAC_DI16     0xC3
#define PAC_DO16     0xC0
#define PAC_DI8DO8   0xC2
#define PAC_8K		 8000


//=============================================================================
//  System Functions
//=============================================================================
WINPAC_API int	pac_GetModuleType(BYTE slot);
WINPAC_API int	pac_GetModuleName(BYTE slot, LPSTR strName);
WINPAC_API int	pac_GetRotaryID();
WINPAC_API void pac_EnableLED(bool bFlag);
WINPAC_API void pac_GetSerialNumber(LPSTR SerialNumber);
WINPAC_API void pac_GetCPUVersion(LPSTR cpu_version);
WINPAC_API void pac_GetOSVersion(LPSTR os_version);
WINPAC_API void pac_GetEbootVersion(LPSTR eboot_version);
WINPAC_API void pac_GetSDKVersion(LPSTR sdk_version);
WINPAC_API void pac_GetMacAddress(BYTE LAN, LPSTR MacAddr);
WINPAC_API void pac_ChangeSlot(BYTE slotNo);
WINPAC_API bool pac_BackwardCompatible();
WINPAC_API int  pac_GetComMapping(int ndx);
WINPAC_API void pac_Reboot();
WINPAC_API int pac_GetModuleVersion(BYTE slot, LPSTR strName);

//=============================================================================
//   Buzzer Functions
//=============================================================================
WINPAC_API void pac_BuzzerBeep(WORD count,DWORD milliseconds);
WINPAC_API void pac_StopBuzzer();
WINPAC_API void pac_SetBuzzerFreqDuty(int freq, int duty);
WINPAC_API void pac_GetBuzzerFreqDuty(int *freq, int *duty);

//=============================================================================
// Backplane API
//=============================================================================

WINPAC_API int	pac_GetDIPSwitch();
WINPAC_API WORD pac_GetSlotCount();

WINPAC_API void pac_RegistryHotPlug(DWORD hWnd, DWORD msgID);
WINPAC_API void pac_UnregistryHotPlug(DWORD hWnd);

WINPAC_API void pac_EnableHotPlug(bool bEnable); //undocumented API
WINPAC_API bool pac_GetHotPlugStatus(); //undocumented API
WINPAC_API void pac_ClearHotPlug(); //undocumented API
WINPAC_API bool pac_GetHotPlugIRQState(); //undocumented API

WINPAC_API void pac_ResetSystemToDefault(); //undocumented API
WINPAC_API BYTE pac_GetRisingEdgeIRQStatus(); //undocumented API
WINPAC_API BYTE pac_GetRisingEdgeEnableStatus(); //undocumented API
WINPAC_API void pac_ClearRisingEdgeIRQ(BYTE slots); //undocumented API
WINPAC_API void pac_EnableRisingEdgeIRQ(BYTE slots, bool bEnable); //undocumented API
WINPAC_API void pac_EOI(); //undocumented API
WINPAC_API void pac_EnableRetrigger(BYTE iValues);
WINPAC_API void pac_ResetBackplane(); //undocumented API
WINPAC_API void pac_GetBackplaneID(LPSTR backplane_version);

//=============================================================================
// Memory API
//=============================================================================
#define WINPAC_MEM_SRAM		0
#define WINPAC_MEM_EEPROM	1
#define WINPAC_MEM_NVRAM        2

WINPAC_API DWORD pac_GetMemorySize(int mem_type);
WINPAC_API bool	 pac_ReadMemory(DWORD address, LPBYTE lpBuffer, DWORD dwLength, int mem_type);
WINPAC_API bool	 pac_WriteMemory(DWORD address, LPBYTE lpBuffer, DWORD dwLength, int mem_type);
WINPAC_API void  pac_EnableEEPROM(bool bEnable);



//=============================================================================
// Watch Dog Timer Functions
//=============================================================================
#define WINPAC_WDT_HW		0
#define WINPAC_WDT_OS		1

WINPAC_API bool	 pac_EnableWatchDog(int wdt, DWORD value);
WINPAC_API void  pac_DisableWatchDog(int wdt);
WINPAC_API void  pac_RefreshWatchDog(int wdt);
WINPAC_API bool  pac_GetWatchDogState(int wdt);
WINPAC_API DWORD pac_GetWatchDogTime(int wdt);
WINPAC_API bool  pac_SetWatchDogTime(int wdt, DWORD value);



//=============================================================================
// Micro SD Management API
//=============================================================================

WINPAC_API bool pac_SDExists();
WINPAC_API bool pac_SDMount(LPTSTR szPartitionName);
WINPAC_API bool pac_SDUnmount();
WINPAC_API bool pac_SDOnside();


//=============================================================================
// Registry API
//=============================================================================

#define PKT_NONE	0
#define PKT_KEY		1
#define PKT_STRING	2
#define PKT_DWORD	3

WINPAC_API bool  pac_RegSave(LPCTSTR KeyName); 
WINPAC_API bool  pac_RegKeyExist(LPCTSTR KeyName);
WINPAC_API bool	 pac_RegDeleteKey(LPCTSTR KeyName);
WINPAC_API bool  pac_RegDeleteValue(LPCTSTR KeyName);
WINPAC_API bool  pac_RegSetString(LPCTSTR KeyName, LPCTSTR assignStr, DWORD dwLength);
WINPAC_API bool  pac_RegSetDWORD(LPCTSTR KeyName, DWORD assignVal);
WINPAC_API bool  pac_RegCreateKey(LPCTSTR KeyName);
WINPAC_API DWORD pac_RegCountKey(LPCTSTR KeyName);
WINPAC_API DWORD pac_RegCountValue(LPCTSTR KeyName);
WINPAC_API bool  pac_RegGetKeyByIndex(LPCTSTR KeyName, DWORD dwIndex, LPTSTR lpName);
WINPAC_API bool  pac_RegGetValueByIndex(LPCTSTR KeyName, DWORD dwIndex, LPTSTR lpName);
WINPAC_API DWORD pac_RegGetKeyInfo(LPCTSTR KeyName);
WINPAC_API bool  pac_RegGetString(LPCTSTR KeyName, LPTSTR lpData, DWORD dwLength);
WINPAC_API DWORD pac_RegGetDWORD(LPCTSTR KeyName);



//=============================================================================
// UART API
//=============================================================================
#define CTO_TIMEOUT_ALL    		0
#define CTO_READ_RETRY_TIMEOUT  1
#define CTO_READ_TOTAL_TIMEOUT	2
#define CTO_WRITE_TOTAL_TIMEOUT	3

#define CTS	0
#define DSR	1
#define RI	2
#define CD	3

#define IN_DATA		0
#define OUT_DATA	1

WINPAC_API HANDLE uart_Open(LPCSTR ConnectionString); 
WINPAC_API HANDLE uart_OpenEx(LPCSTR ConnectionString);

WINPAC_API bool   uart_Close(HANDLE hPort);
WINPAC_API bool   uart_Write(HANDLE hPort, LPCSTR buf);
WINPAC_API bool   uart_Read(HANDLE hPort, LPSTR buf);

WINPAC_API bool   uart_Send(HANDLE hPort, LPCSTR buf);
WINPAC_API bool	  uart_Recv(HANDLE hPort, LPSTR buf);
WINPAC_API bool	  uart_SendCmd(HANDLE hPort, LPCSTR cmd, LPSTR szResult);

WINPAC_API void   uart_SetTimeOut(HANDLE hPort, DWORD msec, int ctoType);
WINPAC_API void	  uart_EnableCheckSum(HANDLE hPort, bool bEnable);  // Disable by default
WINPAC_API void	  uart_SetTerminator(HANDLE hPort, LPCSTR szTerm); // CR by default

WINPAC_API bool	  uart_BinRecv(HANDLE hPort, LPSTR buf, DWORD in_Len);
WINPAC_API bool   uart_BinSend(HANDLE hPort, LPCSTR buf, DWORD in_Len);
WINPAC_API bool   uart_BinSendCmd(HANDLE hPort, LPCSTR ByteCmd, DWORD in_Len, LPSTR ByteResult, DWORD out_Len);

WINPAC_API DWORD  uart_GetDataSize(HANDLE hPort, int data_type);
WINPAC_API BOOL   uart_GetLineStatus(HANDLE hPort, int pin);
WINPAC_API DWORD uart_SetLineStatus(HANDLE port,int pin,int mode);
WINPAC_API DWORD uart_GetOutputLineStatus(HANDLE hPort);

//=============================================================================
// Slot Interrupt API
//=============================================================================

#define PAC_INTR_DONE 0
#define PAC_INTR_EXIT 1
#define PAC_INTR_FAIL 2

#define INTR_RISING_TRIG  0
#define INTR_LEVEL_TRIG   1
#define INTR_Falling_TRIG 2

typedef int (CALLBACK * PAC_CALLBACK_FUNC)();

WINPAC_API bool    pac_RegisterSlotInterrupt(BYTE slot, PAC_CALLBACK_FUNC f);
WINPAC_API bool    pac_UnregisterSlotInterrupt(BYTE slot);
WINPAC_API bool    pac_SetSlotInterruptPriority(BYTE slot, int nPriority);
WINPAC_API void    pac_EnableSlotInterrupt(BYTE slot, bool bEnable);
WINPAC_API void    pac_InterruptDone(BYTE slot);
WINPAC_API HANDLE  pac_GetSlotInterruptEvent(BYTE slot);                   
WINPAC_API DWORD   pac_GetSlotInterruptID(BYTE Slot);
WINPAC_API void	   pac_SetTriggerType(BYTE slot, int iType) ; //0: Rising edge trigger(default), 1: Level trigger, 2: Falling edge trigger
WINPAC_API bool    pac_InterruptInitialize( BYTE slot );
WINPAC_API void    pac_SetSlotInterruptEvent(BYTE slot, HANDLE hEvent);


//=============================================================================
//Backplane timer
//=============================================================================

#define TIMEROUT  0
#define TIMER1   	1
#define TIMER2   	2

typedef int (CALLBACK * PAC_TIMEROUT_CALLBACK_FUNC)();

WINPAC_API bool pac_SetBPTimerOut(unsigned int uHighElapse,unsigned int uLOwElapse, PAC_TIMEROUT_CALLBACK_FUNC f);
WINPAC_API bool pac_SetBPTimer(int type,unsigned int uElapse, PAC_TIMEROUT_CALLBACK_FUNC f);
WINPAC_API void pac_KillBPTimer(int type);
WINPAC_API DWORD pac_GetBPTimerTimeTick_ms(void);
WINPAC_API WORD pac_GetBPTimerTimeTick_us(void);

//=============================================================================
// Error Handling API
//=============================================================================
WINPAC_API DWORD	pac_GetLastError();
WINPAC_API void		pac_SetLastError(DWORD errno);
WINPAC_API void		pac_ClearLastError(DWORD errno);
WINPAC_API void     pac_GetErrorMessage(DWORD dwMessageID, LPTSTR lpBuffer);

//=============================================================================
// PAC Local/Remote IO API
//=============================================================================
#define PAC_REMOTE_IO_BASE		(0x1000)
#define PAC_REMOTE_IO(iAddress) (PAC_REMOTE_IO_BASE+iAddress)

#define pac_GetBit(v, ndx) (v & (1<<ndx))

WINPAC_API bool  pac_WriteDO(HANDLE hPort, int slot, int iDO_TotalCh, DWORD lDO_Value);
WINPAC_API bool  pac_WriteDOBit(HANDLE hPort, int slot, int iDO_TotalCh, int iChannel, int iBitValue);
WINPAC_API bool  pac_ReadDO(HANDLE hPort, int slot,int iDO_TotalCh, DWORD *lDO_Value);
WINPAC_API bool  pac_ReadDI(HANDLE hPort, int slot,int iDI_TotalCh, DWORD *lDI_Value);
WINPAC_API bool  pac_ReadDIO(HANDLE hPort, int slot,int iDI_TotalCh,int iDO_TotalCh, DWORD* lDI_Value, DWORD* lDO_Value);
WINPAC_API bool  pac_ReadDILatch(HANDLE hPort, int slot, int iDI_TotalCh,int iLatchType, DWORD *lDI_Latch_Value);
WINPAC_API bool  pac_ClearDILatch(HANDLE hPort, int slot);
WINPAC_API bool  pac_ReadDIOLatch(HANDLE hPort, int slot,int iDI_TotalCh,int iDO_TotalCh,int iLatchType, DWORD *lDI_Latch_Value,DWORD *lDO_Latch_Value);
WINPAC_API bool  pac_ClearDIOLatch(HANDLE hPort, int slot);
WINPAC_API bool  pac_ReadDICNT(HANDLE hPort, int slot,int iChannel,int iDI_TotalCh, DWORD *lCounter_Value);
WINPAC_API bool  pac_ClearDICNT(HANDLE hPort, int slot,int iChannel,int iDI_TotalCh);
WINPAC_API bool  pac_WriteAO(HANDLE hPort, int slot,int iChannel,int iAO_TotalCh,float fValue);
WINPAC_API bool  pac_ReadAO(HANDLE hPort, int slot,int iChannel,int iAO_TotalCh,float *fValue);
WINPAC_API bool  pac_ReadAI(HANDLE hPort, int slot,int iChannel, int iAI_TotalCh,float *fValue);
WINPAC_API bool  pac_ReadAIHex(HANDLE hPort, int slot,int iChannel, int iAI_TotalCh,int *iValue);
WINPAC_API bool  pac_ReadAIAll(HANDLE hPort, int slot,float fValue[]);
WINPAC_API bool  pac_ReadAIAllHex(HANDLE hPort, int slot, int iValue[]);
WINPAC_API bool  pac_ReadCNT(HANDLE hPort, int slot, int iChannel, DWORD *lCounter_Value);
WINPAC_API bool  pac_ClearCNT(HANDLE hPort, int slot,int iChannel);
WINPAC_API bool  pac_ReadCNTOverflow(HANDLE hPort, int slot, int iChannel, int *iOverflow);
//I8KR IO API

#ifdef __cplusplus
extern "C" {
#endif
WINPAC_API int i8KR_Init(int slot);
short i8041RW_GetLibVersion();

void i8041RW_GetLibDate(char *LibDate);
short i8041RW_GetFirmwareVersion(int slot);

void i8KR_WriteDO(int slot,unsigned long lData);
void i8KR_WriteDOBit(int slot,int iBitIndex,int iStatus);

unsigned long i8KR_ReadbackDO(int slot);
short i8KR_ReadbackDOBit(int slot,int iBitIndex );


WINPAC_API short pac_GetModuleLastOutputSource(int slot);
WINPAC_API short pac_GetModuleWDTStatus(int slot, short* wdtStatus, float *wdtTimeout);
WINPAC_API short pac_SetModuleWDTConfig(int slot, short enStatus, float wdtTimeout,int ifWDT_Overwrite);
//short i8041RW_GetWDTStatus(int slot); // base + 6 Read DO Status
WINPAC_API short pac_ReadModuleSafeValue(int slot, unsigned long *lValue);
WINPAC_API short pac_WriteModuleSafeValue(int slot,unsigned long lData);
WINPAC_API short pac_RefreshModuleWDT(int slot);

WINPAC_API short pac_SetModulePowerOnEnStatus(int slot, short enStatus);
WINPAC_API short pac_GetModulePowerOnEnStatus(int slot, short* enStatus);
WINPAC_API short pac_WriteModulePowerOnValue(int slot,unsigned long lData);
WINPAC_API short pac_ReadModulePowerOnValue(int slot, unsigned long *lValue);

#ifdef __cplusplus
}
#endif


//=============================================================================
// MISC API
//=============================================================================
WINPAC_API void pac_AnsiToWideString(const char *astr, LPTSTR wstr);
WINPAC_API void pac_WideStringToAnsi(const TCHAR *wstr, LPSTR astr);
WINPAC_API int  pac_GetBatteryLevel(int nBattery);
WINPAC_API void pac_DoEvent();


//=============================================================================
// Undocumented API
//=============================================================================
#define PAC_INP		_pac_inp
#define PAC_OUTP	_pac_outp
#define PAC_MAP		_pac_map


WINPAC_PRIVATE_API BYTE	 _pac_inp(int addr);
WINPAC_PRIVATE_API void	 _pac_outp(int addr, BYTE val);
WINPAC_PRIVATE_API PBYTE _pac_map(int addr);	
WINPAC_PRIVATE_API BYTE  _pac_slot_inp(BYTE slot, int addr);
WINPAC_PRIVATE_API void  _pac_slot_outp(BYTE slot, int addr, BYTE val);
WINPAC_PRIVATE_API PBYTE _pac_slot_map(BYTE slot, int addr);
WINPAC_PRIVATE_API DWORD _pac_get_slot_offset(BYTE slot);
WINPAC_PRIVATE_API PBYTE _pac_get_base_address();
WINPAC_PRIVATE_API void  _pac_set_mac_address(BYTE LAN, LPTSTR MacAddr);

/************************** 16-bit ********************************/
WINPAC_PRIVATE_API WORD	 _pac_inpw(int addr);
WINPAC_PRIVATE_API void	 _pac_outpw(int addr, WORD val);
WINPAC_PRIVATE_API WORD  _pac_slot_inpw(BYTE slot, int addr);
WINPAC_PRIVATE_API void  _pac_slot_outpw(BYTE slot, int addr, WORD val);
/******************************************************************/

WINPAC_PRIVATE_API void	 _pac_refresh_screen();
WINPAC_PRIVATE_API void  _pac_us_delay(DWORD microsecond);
WINPAC_PRIVATE_API void  _pac_ticks_delay(DWORD ticks);
WINPAC_PRIVATE_API bool  _pac_get_partition_info(LPTSTR szPartName,LPTSTR szFileSys, LPTSTR szVolName);
WINPAC_PRIVATE_API DWORD _pac_oscr0();

#ifdef __cplusplus
}
#endif

#pragma comment(lib, "winpacsdk.lib")


#endif // __WINPACSDK_H
									
