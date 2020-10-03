// Modbus_ARM.h : main header file for the Modbus_ARM DLL
//#if !defined(AFX_Modbus_ARM_H__94D9CFBC_56CE_4C36_B846_FA74648487CF__INCLUDED_)
//#define AFX_Modbus_ARM_H__94D9CFBC_56CE_4C36_B846_FA74648487CF__INCLUDED_
//#endif
//
//#if _MSC_VER >= 1000
//#pragma once
//#endif // _MSC_VER >= 1000

//#include  "MBDLLTool.h"   // fun. define
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WINCONSDK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WINCONSDK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef Modbus_ARM_API_EXPORTS
#define Modbus_ARM_API extern "C" __declspec(dllexport)
#else
#define Modbus_ARM_API extern "C" __declspec(dllimport)
#endif

//Modbus Master RTU functions
Modbus_ARM_API int CALLBACK MBRTUInit(int iPortNumber, int iBaudrate,int iParity, int iDataBit, int iStopBit, int iTimeOut); 
Modbus_ARM_API int CALLBACK MBRTU_R_Coils(int iPortNumber, int iSlaveNumber, int iStartAddress, int iCount, unsigned char *iRecv, int iFuncNumber /*1/2 up to the modbus device*/);
Modbus_ARM_API int CALLBACK MBRTU_W_Coil(int iPortNumber, int iSlaveNumber, int iCoilAddress, int iCoilStatus); // Fun 5
Modbus_ARM_API int CALLBACK MBRTU_R_Registers(int iPortNumber, int iSlaveNumber, int iStartAddress, int iCount, short *iRecv, int iFuncNumber /*3/4 up to the modbus device*/);	
Modbus_ARM_API int CALLBACK MBRTU_W_Register(int iPortNumber, int iSlaveNumber, int iRegAddress, short iRegStatus); // Fun 6
Modbus_ARM_API void CALLBACK MBRTUClose(int iPortNumber);
Modbus_ARM_API int CALLBACK MBRTU_W_Multi_Coils(int iPortNumber, int iSlaveNumber, int iCoilAddress, int iCount, unsigned char *iCoilStatus); // Fun 15
Modbus_ARM_API int CALLBACK MBRTU_W_Multi_Registers(int iPortNumber, int iSlaveNumber, int iRegAddress, int iCount, short *iRegStatus); // Fun 16
//Modbus Master ASCII functions
Modbus_ARM_API int CALLBACK MBASCInit(int iPortNumber, int iBaudrate,int iParity, int iDataBit, int iStopBit, int iTimeOut); 
Modbus_ARM_API int CALLBACK MBASC_R_Coils(int iPortNumber, int iSlaveNumber, int iStartAddress, int iCount, unsigned char *iRecv, int iFuncNumber /*1/2 up to the modbus device*/);
Modbus_ARM_API int CALLBACK MBASC_W_Coil(int iPortNumber, int iSlaveNumber, int iCoilAddress, int iCoilStatus); // Fun 5
Modbus_ARM_API int CALLBACK MBASC_R_Registers(int iPortNumber, int iSlaveNumber, int iStartAddress, int iCount, short *iRecv, int iFuncNumber /*3/4 up to the modbus device*/);	
Modbus_ARM_API int CALLBACK MBASC_W_Register(int iPortNumber, int iSlaveNumber, int iRegAddress, short iRegStatus); // Fun 6
Modbus_ARM_API void CALLBACK MBASCClose(int iPortNumber);
Modbus_ARM_API int CALLBACK MBASC_W_Multi_Coils(int iPortNumber, int iSlaveNumber, int iCoilAddress, int iCount, unsigned char *iCoilStatus); // Fun 15
Modbus_ARM_API int CALLBACK MBASC_W_Multi_Registers(int iPortNumber, int iSlaveNumber, int iRegAddress, int iCount, short *iRegStatus); // Fun 16
//Modbus Master TCP functions
Modbus_ARM_API int CALLBACK MBTCPInit(int iSocketNumber, char *tcpipaddr, int tcpipport, int iTimeOut); // Return 1==>True 0==>False
Modbus_ARM_API int CALLBACK MBTCP_R_Coils(int iSocketNumber,int iSlaveNumber, int iStartAddress, int iCount, unsigned char *iRecv, int iFuncNumber /*1/2 up to the modbus device*/);	//Function 1/2 Return 1==>True 0==>False
Modbus_ARM_API int CALLBACK MBTCP_W_Coil(int iSocketNumber, int iSlaveNumber, int iCoilAddress, int iCoilStatus);	//Function 15 Return 1==>True 0==>False
Modbus_ARM_API int CALLBACK MBTCP_R_Registers(int iSocketNumber, int iSlaveNumber, int iStartAddress, int iCount, short *iRecv, int iFuncNumber /*3/4 up to the modbus device*/);	//Function 3/4 Return 1==>True 0==>False
Modbus_ARM_API int CALLBACK MBTCP_W_Register(int iSocketNumber, int iSlaveNumber, int iRegAddress, short iRegStatus);	//Function 16 iRegStatus==>-32768~32767 Return 1==>True 0==>False 
Modbus_ARM_API void CALLBACK MBTCPClose(int iSocketNumber);	//Return 1==>True 0==>False
Modbus_ARM_API int CALLBACK MBTCP_W_Multi_Coils(int iPortNumber, int iSlaveNumber, int iCoilAddress, int iCount, unsigned char *iCoilStatus); // Fun 15
Modbus_ARM_API int CALLBACK MBTCP_W_Multi_Registers(int iPortNumber, int iSlaveNumber, int iRegAddress, int iCount, short *iRegStatus); // Fun 16
//Bit Pattern sub.
Modbus_ARM_API int CALLBACK Coil_Bit_Read(unsigned char *coil, int bitno); 
Modbus_ARM_API int CALLBACK Coil_Bit_Write(unsigned char *coil, int bitno, int onoff); 

#define    iPARITY_NONE   0
#define    iPARITY_ODD    1
#define    iPARITY_EVEN   2

// Modbus exception code 
#define    MB_RTC_OK                      0
#define    MB_ILLEGAL_FUNCTION            1
#define    MB_ILLEGAL_DATA_ADDRESS        2
#define    MB_ILLEGAL_DATA_VALUE          3
#define    MB_SLAVE_DEVICE_FAILURE        4
#define    MB_ACKNOWLEDGE                 5
#define    MB_SLAVE_DEVICE_BUSY           6
#define    MB_NEGATIVE_ACKNOWLEDGE        7
#define    MB_MEMORY_PARITY_ERROR         8
//#define    MB_ILLEGAL_FUNCTION          9    // undefine code
#define    MB_GATEWAY_PATH_UNAVAILABLE    10
#define    MB_GATEWAY_FAILED_TO_RESPOND   11
// DLL sub. parameter error
#define    MB_OPEN_PORT_ERROR   101  // Open COM/TCP Port error
#define    MB_PORTNO_OVER       102  // COM Port is 1 - 8
#define    MB_PORT_NOT_OPEN     103  // COM/TCP Port does not open yet
#define    MB_FUN_ERROR         104  // Modbus Fun. No. error
#define    MB_READ_COUNT_OVER   105  // reading Count of Register or Bits is over range
                                     // RTU: 120 register, 1920 coils
                                     // ASCII: 60 register, 960 coils
                                     // TCP: 120 register, 1920 coils
#define    MB_SLAVENO_OVER      106  // Modbus Slave No. must be 1 - 247
#define    MB_ADDRESS_OVER      107  // Register or Coil Address must count from 1
#define    MB_COMM_TIMEOUT      108  // Comm. timeout
#define    MB_CRC_ERROR         109  // RTU CRC Check error
#define    MB_LRC_ERROR         110  // ASCII LRC Check error
#define    MB_INVALID_SOCKET    111  // Initial Socket error
#define    MB_TCP_CONNECT_ERROR 112  // Connect Remote Modbus Server error
#define    MB_TCP_SEND_ERROR    113  // Send TCP Data error
#define    MB_TCP_TIMEOUT       114  // Waiting Modbus Response Timeout
#define    MB_WSA_INIT_ERROR    115  // WSA Startup error
#define    MB_TCP_SOCKET_ERROR  116  // Create Socket error
#define    MB_TCP_BIND_ERROR    117  // TCP Server Bind error
#define    MB_TCP_LISTEN_ERROR  118  // TCP Server Listen error
#define    MB_TCP_HAS_DATA      119  // it has data from remote Modbus Master
#define    MB_WRITE_COUNT_OVER  120  // reading Count of Register or Bits is over range
                                     // RTU: 120 register, 1920 coils
                                     // ASCII: 60 register, 960 coils
                                     // TCP: 120 register, 1920 coils

// delete some shit below