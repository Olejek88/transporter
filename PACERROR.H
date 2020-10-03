#ifndef __PACERROR_H
#define __PACERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#define PAC_ERR_SUCCESS						0x00000
#define PAC_ERR_UNKNOWN						0x00001

//Basic=====================================================================
#define PAC_ERR_BASE						0x10000
#define PAC_ERR_SLOT_REGISTERED				(PAC_ERR_BASE + 1)
#define PAC_ERR_SLOT_NOT_REGISTERED			(PAC_ERR_BASE + 2)
#define PAC_ERR_UNKNOWN_MODULE				(PAC_ERR_BASE + 3)
#define PAC_ERR_INVALID_MAC					(PAC_ERR_BASE + 4)
#define PAC_ERR_INVALID_COMPORT_NUMBER		(PAC_ERR_BASE + 5)
#define PAC_ERR_FUNCTION_NOT_SUPPORT		(PAC_ERR_BASE + 6)

//Memory Access=============================================================
#define PAC_ERR_MEMORY_BASE					0x11000
#define PAC_ERR_EEP_ACCESS_RESTRICTION		(PAC_ERR_MEMORY_BASE + 1)
#define PAC_ERR_SRAM_INVALID_ADDRESS        (PAC_ERR_MEMORY_BASE + 2)
#define PAC_ERR_SRAM_INVALID_TYPE			(PAC_ERR_MEMORY_BASE + 3)
#define PAC_ERR_NVRAM_INVALID_ADDRESS		(PAC_ERR_MEMORY_BASE + 4)
#define PAC_ERR_EEP_WRITE_PROTECT           (PAC_ERR_MEMORY_BASE + 5) 
#define PAC_ERR_EEP_WRITE_FAIL              (PAC_ERR_MEMORY_BASE + 6) 
#define PAC_ERR_EEP_READ_FAIL               (PAC_ERR_MEMORY_BASE + 7) 
//Watch Dog=================================================================
#define PAC_ERR_WDT_BASE					0x12000
#define PAC_ERR_WDT_INVALID_VALUE			(PAC_ERR_WDT_BASE + 1)
#define PAC_ERR_WDT_UNEXIST					(PAC_ERR_WDT_BASE + 2)
#define PAC_ERR_WDT_BASE_NOT_SET			(PAC_ERR_WDT_BASE + 3)

//Registry==================================================================
#define PAC_ERR_REG_BASE					0x13000

//UART======================================================================
#define PAC_ERR_UART_BASE					0x14000
#define PAC_ERR_UART_CHECKSUM				(PAC_ERR_UART_BASE+1)
#define PAC_ERR_UART_READ_TIMEOUT			(PAC_ERR_UART_BASE+2)
#define PAC_ERR_UART_RESPONSE				(PAC_ERR_UART_BASE+3)
#define PAC_ERR_UART_UNDER_INPUT_RANGE		(PAC_ERR_UART_BASE+4)
#define PAC_ERR_UART_EXCEED_INPUT_RANGE		(PAC_ERR_UART_BASE+5)
#define PAC_ERR_UART_OPEN_FAILED		    (PAC_ERR_UART_BASE+6)
#define PAC_ERR_UART_GET_COMM_STATUS_ERROR	(PAC_ERR_UART_BASE+7)
#define PAC_ERR_UART_INVALID_VALUE			(PAC_ERR_UART_BASE+8)

//IO========================================================================
#define PAC_ERR_IO_BASE						0x15000
#define PAC_ERR_IO_NOT_SUPPORT				(PAC_ERR_IO_BASE+1)
#define PAC_ERR_IO_ID						(PAC_ERR_IO_BASE+2)
#define PAC_ERR_IO_SLOT						(PAC_ERR_IO_BASE+3)
#define PAC_ERR_IO_CHANNEL					(PAC_ERR_IO_BASE+4)
#define PAC_ERR_IO_GAIN						(PAC_ERR_IO_BASE+5)
#define PAC_ERR_IO_INT_MODE  				(PAC_ERR_IO_BASE+6)
#define PAC_ERR_IO_VALUE_OUT_OF_RANGE		(PAC_ERR_IO_BASE+7)
#define PAC_ERR_IO_CHANNEL_OUT_OF_RANGE		(PAC_ERR_IO_BASE+8)

//Backplane Timer==================================================================
#define PAC_ERR_BPTIMER_BASE					0x16000

#define PAC_ERR_BPTIMER_REGISTERED				(PAC_ERR_BPTIMER_BASE + 1)
#define PAC_ERR_BPTIMER_NOT_REGISTERED			(PAC_ERR_BPTIMER_BASE + 2)

#ifdef __cplusplus
}
#endif

#endif
