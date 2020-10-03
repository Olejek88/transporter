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

extern	byte	SRV_SON;		// Сервопривод включен
extern  byte	SRV_LSP;		// Концевой выключатель прямого вращения
extern  byte	SRV_LSN;		// Концевой выключатель обратного вращения
extern	byte	SRV_ST1;		// Пуск в прямом направлении
extern	uint	SRV_ACCEL;		// Время разгона / торможения
extern	uint	SRV_FREQ;		// Частота вращения

extern	byte	SRV_REGIM;		// Текущий режим сервопривода
extern	uint	SRV_FACT;		// Фактическое положение 
extern	uint	SRV_COUNT;		// Абсолютный счетчик
extern	uint	SRV_RASS;		// Рассогласование
extern	float	SRV_MOMENT;		// Текущий момент
extern	uint	SRV_FREQ_R;		// Заданная частота
extern	uint	SRV_POS;		// Абсолютное положение
extern	float	SRV_SPEED;		// Задание скорости

extern	byte	SRV_SON;		// Сервопривод включен
extern	byte	SRV_RD;			// Готовность

extern	byte  SERV_READY;		// Готовность сервопривода (RD/DICOM)
extern	byte  SERV_ALARM;		// Авария сервопривода (ALM/DICOM)
extern	byte  SERV_CON_VKL;		// Контактор работы сервопривода включен
extern	byte  SERV_UACTIVE;		// Наличие напряжения
extern	byte  DOZA1_READY;		// Готовность дозатора №1
extern	byte  DOZA2_READY;		// Готовность дозатора №2
extern	byte  CONV_STOP;		// Аварийная остановка конвейера
extern	byte  DOZA1_ERROR;		// Авария дозатора №1
extern	byte  DOZA2_ERROR;		// Авария дозатора №2

extern	byte  FQ1_ERROR;		// Сигнал ошибки первого датчика FQ (ERROR)
extern	byte  FQ1_BUSY;			// Сигнал занято первого датчика FQ (BUSY)
extern	byte  FQ1_OUTPUT;		// Сигнал решения первого датчика FQ (OUTPUT)
extern	byte  FQ2_ERROR;		// Сигнал ошибки второго датчика FQ (ERROR)
extern	byte  FQ2_BUSY;			// Сигнал занято второго датчика FQ (BUSY)
extern	byte  FQ2_OUTPUT;		// Сигнал решения второго датчика FQ (OUTPUT)
extern	byte  XUK1_ALARM;		// Сигнал "Авария" датчика XUK (ALARM)
extern	byte  XUK1_OUTPUT;		// Сигнал "Решение" датчика XUK (OUTPUT)
extern	byte  XUK2_ALARM;		// Сигнал "Авария" датчика XUK (ALARM)
extern	byte  XUK2_OUTPUT;		// Сигнал "Решение" датчика XUK (OUTPUT)

extern	uint	SRV_FACT;		// Фактическое положение [80]
extern	uint	SRV_FACT_PREV;	// Фактическое положение [80]
extern	uint	SRV_2P;			// Фактическое положение [80]
extern	uint	CORRECT;
extern	uint	WAIT_TIME;
extern	uint	WAIT_TIME2;

extern	byte	IZnal[50];		// наличие изоляторов на позиции
extern	byte	sensor_pos;		

extern	byte	DOZA1_START;
extern	byte	DOZA2_START;
extern	byte	PANEL_STOP;