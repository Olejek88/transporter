extern	byte	WorkEnable;		// work flag
extern	byte	ConvStopped;	// conveer stopped
extern	short	currentregim;	// 0 - test, 1-3 scene selection

extern	uint	totalDetected;	// всего опознано деталей
extern	uint	totalLoaded;	// всего заполнено деталей
extern	uint	todayDetected;	// опознано деталей сегодня
extern	uint	todayLoaded;	// заполнено деталей сегодня

// IO variables
// [0] i-87017
extern	float MO1_LG;		// Аналоговый вход для мониторинга (MO1/LG)
extern	float MOD1_1;		// резерв 1
extern	float MOD1_2;		// резерв 2
extern	float MOD1_3;		// резерв 3
// [1] i-87024
extern	float VC_LG;		// Аналоговый управление (VC/LG)
extern	float MOD2_1;		// резерв 1
extern	float MOD2_2;		// резерв 2
extern	float MOD2_3;		// резерв 3
// [2] i-87053
extern	byte  FQ1_ERROR;	// Сигнал ошибки первого датчика FQ (ERROR)
extern	byte  FQ1_BUSY;		// Сигнал занято первого датчика FQ (BUSY)
extern	byte  FQ1_OUTPUT;	// Сигнал решения первого датчика FQ (OUTPUT)
extern	byte  FQ2_ERROR;	// Сигнал ошибки второго датчика FQ (ERROR)
extern	byte  FQ2_BUSY;		// Сигнал занято второго датчика FQ (BUSY)
extern	byte  FQ2_OUTPUT;	// Сигнал решения второго датчика FQ (OUTPUT)
extern	byte  XUK1_ALARM;	// Сигнал "Авария" датчика XUK (ALARM)
extern	byte  XUK1_OUTPUT;	// Сигнал "Решение" датчика XUK (OUTPUT)
extern	byte  XUK2_ALARM;	// Сигнал "Авария" датчика XUK (ALARM)
extern	byte  XUK2_OUTPUT;	// Сигнал "Решение" датчика XUK (OUTPUT)
// [3] i-87053
extern	byte  SERV_SU;		// Сервопривод в режиме "системное управление"
extern	byte  SERV_READY;	// Готовность сервопривода (RD/DICOM)
extern	byte  SERV_ALARM;	// Авария сервопривода (ALM/DICOM)
extern	byte  SERV_CON_VKL;	// Контактор работы сервопривода включен
extern	byte  SERV_UACTIVE;	// Наличие напряжения
extern	byte  DOZA1_READY;	// Готовность дозатора №1
extern	byte  DOZA2_READY;	// Готовность дозатора №2
extern	byte  CONV_STOP;	// Аварийная остановка конвейера
extern	byte  DOZA1_ERROR;	// Авария дозатора №1
extern	byte  DOZA2_ERROR;	// Авария дозатора №2
// [4] i-87064
extern	byte  FQ1_TRIG;		// Запуск первого датчика FQ (TRIG)
extern	byte  FQ1_IN0;		// Выбор образца датчика #1 (IN0)
extern	byte  FQ1_IN1;		// Выбор образца датчика #1 (IN1)
extern	byte  FQ1_IN2;		// Выбор образца датчика #1 / стирание ERROR (IN2)
extern	byte  FQ1_IN3;		// Выбор образца датчика #1 (IN3)
extern	byte  FQ1_IN4;		// Выбор образца датчика #1 (IN4)
extern	byte  FQ1_IN5;		// Команда на постоянное измерение  / выбор сцены (IN5)
// [5] i-87064
extern	byte  SERV_SON;		// Включить сервопривод (SON)
extern	byte  SERV_SP1;		// Технологический режим №1 (SP1)
extern	byte  SERV_SP2;		// Технологический режим №2 (SP2)
extern	byte  SERV_ST1;		// Пуск сервопривода (ST1)
extern	byte  SERV_RES;		// Сброс аварии сервопривода (RES)
extern	byte  DOZA1_START;	// Включить дозатор №1
extern	byte  DOZA2_START;	// Включить дозатор №2
extern	byte  SERV_ST2;		// Пуск в обратном направлении
// [6] i-87061
extern	byte  FQ2_TRIG;		// Запуск второго датчика FQ (TRIG)
extern	byte  FQ2_IN0;		// Выбор образца датчика #2 (IN0)
extern	byte  FQ2_IN1;		// Выбор образца датчика #2 (IN1)
extern	byte  FQ2_IN2;		// Выбор образца датчика #2 / стирание ERROR (IN2)
extern	byte  FQ2_IN3;		// Выбор образца датчика #2 (IN3)
extern	byte  FQ2_IN4;		// Выбор образца датчика #2 (IN4)
extern	byte  FQ2_IN5;		// Команда на пост. измерение / выбор сцены (IN5)

// servoprivod
extern	float	SRV_REGIM;	// Текущий режим сервопривода
extern	uint	SRV_FACT;	// Фактическое положение 
extern	uint	SRV_FREQ;	// Частота вращения
extern	uint	SRV_COUNT;	// Абсолютный счетчик
extern	uint	SRV_RASS;	// Рассогласование
extern	float	SRV_MOMENT;	// Текущий момент
extern	uint	SRV_FREQ_R;	// Заданная частота
extern	uint	SRV_POS;	// Абсолютное положение
extern	float	SRV_SPEED;	// Задание скорости

extern	byte	SRV_SON;	// Сервопривод включен
extern	byte	SRV_LSP;	// Концевой выключатель прямого вращения
extern	byte	SRV_LSN;	// Концевой выключатель обратного вращения
extern	byte	SRV_TL;		// Ограничение крутящего момента
extern	byte	SRV_TL1;	
extern	byte	SRV_PC;		// Переключение на П-регулятор
extern	byte	SRV_RES;	// Сброс
extern	byte	SRV_CR;		// Стирание позиционного счетчика
extern	byte	SRV_SP1;	// Выбор фиксированной частоты вращения 1
extern	byte	SRV_SP2;	// Выбор фиксированной частоты вращения 2
extern	byte	SRV_SP3;	// Выбор фиксированной частоты вращения 3
extern	byte	SRV_ST1;	// Пуск в прямом направлении
extern	byte	SRV_ST2;	// Пуск в обратном направлении
extern	byte	SRV_CM1;	// Электронный редуктор, выбор 1
extern	byte	SRV_CM2;	// Электронный редуктор, выбор 2
extern	byte	SRV_LOP;	// Смена режим управления
extern	byte	SRV_STAB2;	// Второе время ускорения/замедления
extern	byte	SRV_CDP;	//
extern	byte	SRV_RD;		// Готовность
extern	byte	SRV_SA;		// Достигнута частота вращения
extern	byte	SRV_ZSP;	// Неподвижное состояние
extern	byte	SRV_TLC;	// Достигнут предел крутящего момента
extern	byte	SRV_VLC;	// Достигнуто ограничение частоты вращения
extern	byte	SRV_INP;	// В позиции
extern	byte	SRV_WNG;	// Предупреждение
extern	byte	SRV_ALM;	// Аварийная сигнализация
extern	byte	SRV_OP;		// Импульс фазы Z-энкодера
extern	byte	SRV_MBR;	// Управление удерживающим тормозом
extern	byte	SRV_ACD0;	// Код аварийной сигнализации
extern	byte	SRV_ACD1;	// Код аварийной сигнализации
extern	byte	SRV_ACD2;	// Код аварийной сигнализации
extern	byte	SRV_BWNG;	// Предупреждение о батарее
extern	byte	SRV_CDPS;	// Сигнал CDPS
extern	byte	SRV_ABSV;	// Если данные по абсолютным координатам стерт

extern	byte	SRV_SON;		// Сервопривод включен
extern  byte	SRV_LSP;		// Концевой выключатель прямого вращения
extern  byte	SRV_LSN;		// Концевой выключатель обратного вращения
extern	byte	SRV_ST1;		// Пуск в прямом направлении
extern	uint	SRV_ACCEL;		// Время разгона / торможения
extern	uint	SRV_FREQ;		// Частота вращения


