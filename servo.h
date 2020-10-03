// направление передачи
#define		RECV	0x00		// прием
#define		SEND	0x01		// передача
// управляющие символы
#define		SOH		0x01
#define		STX		0x02
#define		ETX		0x03
#define		EOT		0x04
// коды ошибок
#define		OK					0x41
#define		PARITY_ERROR		0x42
#define		CRC_ERROR			0x43
#define		ERROR_SIGN			0x44
#define		ERROR_FUNC			0x45
#define		ERROR_DATA			0x46

#define		A_OK				0x61
#define		A_PARITY_ERROR		0x62
#define		A_CRC_ERROR			0x63
#define		A_ERROR_SIGN		0x64
#define		A_ERROR_FUNC		0x65
#define		A_ERROR_DATA		0x66

// коды команд
#define		STATUS				0x01
#define		CURRENT_ERROR		0x02
#define		COM4		0x04
#define		COM5		0x05
#define		COM6		0x06
#define		COM7		0x07
#define		COM8		0x08
#define		COM9		0x09
#define		COMIO		0x12			// внешние входные и выходные сигналы
#define		ARCHIVE_ERROR		0x33

// параметры
#define		DAT1_FACT		0x00		// фактическое положение
#define		DAT1_FREQ		0x01		// частота вращения двигателя
#define		DAT1_RASS		0x02		// рассогласование (в импульсах)
#define		DAT1_IMPZ		0x03		// накопленное импульсное задание
#define		DAT1_FREQ_Z		0x04		// заданная частота
#define		DAT1_SPEED		0x05		// напряжение на аналоговом входе задания скорости
#define		DAT1_MOMENT		0x06		// напряжение на аналоговом входе задания момента
#define		DAT1_7		0x07			// нагрузка тормозного контура
#define		DAT1_8		0x08			// действующее значение нагрузки
#define		DAT1_9		0x09			// пиковое значение нагрузки
#define		DAT1_A		0x0A			// текущий крутящий момент
#define		DAT1_B		0x0B			// абсолютное значение в пределах оборота
#define		DAT1_C		0x0C			// абсолютный счетчик
#define		DAT1_D		0x0D			// соотношение моментов инерции
#define		DAT1_E		0x0E			// напряжение звена постоянного тока

#define		COM12_IN1	0x00		// состояние входных клемм
#define		COM12_IN2	0x40		// состояние внешних входных сигналов
#define		COM12_IN3	0x60		// состояние выходных клемм, включенных в результате коммуникации
#define		COM12_OUT1	0x80		// состояние выходных клемм
#define		COM12_OUT2	0xC0		// состояние внешних выходных сигналов 

#define		COM33_ERROR0	0x10		// номер аварийной сигнализации (текущий)
#define		COM33_ERROR1	0x11		// номер аварийной сигнализации (текущий-1)
#define		COM33_ERROR2	0x12		// номер аварийной сигнализации (текущий-2)
#define		COM33_ERROR3	0x13		// номер аварийной сигнализации (текущий-3)
#define		COM33_ERROR4	0x14		// номер аварийной сигнализации (текущий-4)
#define		COM33_ERROR5	0x15		// номер аварийной сигнализации (текущий-5)
#define		COM33_TIME0		0x20		// время аварийной сигнализации (текущий)
#define		COM33_TIME1		0x21		// время аварийной сигнализации (текущий-1)
#define		COM33_TIME2		0x22		// время аварийной сигнализации (текущий-2)
#define		COM33_TIME3		0x23		// время аварийной сигнализации (текущий-3)
#define		COM33_TIME4		0x24		// время аварийной сигнализации (текущий-4)
#define		COM33_TIME5		0x25		// время аварийной сигнализации (текущий-5)

//--------------------------------------------------------------------------------------------------------
byte	SRV_REGIM=0;	// Текущий режим сервопривода
uint	SRV_FACT=0;		// Фактическое положение 
uint	SRV_FREQ=0;		// Частота вращения
uint	SRV_COUNT=0;	// Абсолютный счетчик
uint	SRV_RASS=0;		// Рассогласование
float	SRV_MOMENT=0;	// Текущий момент
uint	SRV_FREQ_R=0;	// Заданная частота
uint	SRV_POS=0;		// Абсолютное положение
float	SRV_SPEED=0;	// Задание скорости
uint	SRV_ACCEL=0;	// время разгона

uint	posit=0;
uint	rass=0;
uint	rass2=0;

uint	SRV_CYCLE=0;	// цикл разгона/торможения

char	srv_acc_name[20];
char	srv_dec_name[20];
char	srv_speed_name[20];

byte	SRV_SON=0;		// Сервопривод включен
byte	SRV_LSP=0;		// Концевой выключатель прямого вращения
byte	SRV_LSN=0;		// Концевой выключатель обратного вращения
byte	SRV_TL=0;		// Ограничение крутящего момента
byte	SRV_TL1=0;	
byte	SRV_PC=0;		// Переключение на П-регулятор
byte	SRV_RES=0;		// Сброс
byte	SRV_CR=0;		// Стирание позиционного счетчика
byte	SRV_SP1=0;		// Выбор фиксированной частоты вращения 1
byte	SRV_SP2=0;		// Выбор фиксированной частоты вращения 2
byte	SRV_SP3=0;		// Выбор фиксированной частоты вращения 3
byte	SRV_ST1=0;		// Пуск в прямом направлении
byte	SRV_ST2=0;		// Пуск в обратном направлении
byte	SRV_CM1=0;		// Электронный редуктор, выбор 1
byte	SRV_CM2=0;		// Электронный редуктор, выбор 2
byte	SRV_LOP=0;		// Смена режим управления
byte	SRV_STAB2=0;	// Второе время ускорения/замедления
byte	SRV_CDP=0;		//
byte	SRV_RD=0;		// Готовность
byte	SRV_SA=0;		// Достигнута частота вращения
byte	SRV_ZSP=0;		// Неподвижное состояние
byte	SRV_TLC=0;		// Достигнут предел крутящего момента
byte	SRV_VLC=0;		// Достигнуто ограничение частоты вращения
byte	SRV_INP=0;		// В позиции
byte	SRV_RR1=0;		// резерв 1
byte	SRV_WNG=0;		// Предупреждение
byte	SRV_ALM=0;		// Аварийная сигнализация
byte	SRV_OP=0;		// Импульс фазы Z-энкодера
byte	SRV_MBR=0;		// Управление удерживающим тормозом
byte	SRV_RR2=0;		// резерв 2
byte	SRV_ACD0=0;		// Код аварийной сигнализации
byte	SRV_ACD1=0;		// Код аварийной сигнализации
byte	SRV_ACD2=0;		// Код аварийной сигнализации
byte	SRV_BWNG=0;		// Предупреждение о батарее
byte	SRV_CDPS=0;		// Сигнал CDPS
byte	SRV_ABSV=0;		// Если данные по абсолютным координатам стерт

