//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                    GLOBAL CONSTANTS RTCC - ADDRESSES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define  RTC_TRIS_SCL           _TRISA2
#define  RTC_TRIS_SDA           _TRISA3
#define  RTC_TRIS_MFP           _TRISD11

#define  RTC_SCL           _LATA2
#define  RTC_SDA           _LATA3
#define  RTC_MFP           _LATD11


     #define  ADDR_EEPROM_WRITE 0xae       //  DEVICE ADDR for EEPROM (writes)
     #define  ADDR_EEPROM_READ  0xaf       //  DEVICE ADDR for EEPROM (reads)
     #define  ADDR_RTCC_WRITE   0xde       //  DEVICE ADDR for RTCC MCHP  (writes)
     #define  ADDR_RTCC_READ    0xdf       //  DEVICE ADDR for RTCC MCHP  (reads)
//.................................................................................
     #define  SRAM_PTR          0x20       //  pointer of the SRAM area (RTCC)
     #define  ADDR_EEPROM_SR    0xff       //  STATUS REGISTER in the  EEPROM
//.................................................................................
     #define  ADDR_SEC          0x00       //  address of SECONDS      register
     #define  ADDR_MIN          0x01       //  address of MINUTES      register
     #define  ADDR_HOUR         0x02       //  address of HOURS        register
     #define  ADDR_DAY          0x03       //  address of DAY OF WK    register
     #define  ADDR_STAT         0x03       //  address of STATUS       register
     #define  ADDR_DATE         0x04       //  address of DATE         register
     #define  ADDR_MNTH         0x05       //  address of MONTH        register
     #define  ADDR_YEAR         0x06       //  address of YEAR         register
     #define  ADDR_CTRL         0x07       //  address of CONTROL      register
     #define  ADDR_CAL          0x08       //  address of CALIB        register
     #define  ADDR_ULID         0x09       //  address of UNLOCK ID    register



#define  ADDR_SEC_ALARM_1          0x0A
#define  ADDR_MIN_ALARM_1          0x0B
#define  ADDR_HOUR_ALARM_1         0x0C
#define  ADDR_DAY_ALARM_1          0x0D
#define  ADDR_DATE_ALARM_1         0x0E
#define  ADDR_MNTH_ALARM_1         0x0F


#define  ADDR_SEC_ALARM_2          0x11
#define  ADDR_MIN_ALARM_2          0x12
#define  ADDR_HOUR_ALARM_2         0x13
#define  ADDR_DAY_ALARM_2          0x14
#define  ADDR_DATE_ALARM_2         0x15
#define  ADDR_MNTH_ALARM_2         0x16


//.................................................................................
     #define  ADDR_ALM0SEC      0x0a       //  address of ALARMO SEC   register
     #define  ADDR_ALM0MIN      0x0b       //  address of ALARMO MIN   register
     #define  ADDR_ALM0HR       0x0c       //  address of ALARMO HOUR  register
     #define  ADDR_ALM0CTL      0x0d       //  address of ALARM0 CONTR register
     #define  ADDR_ALM0DAT      0x0e       //  address of ALARMO DATE  register
     #define  ADDR_ALM0MTH      0x0f       //  address of ALARMO MONTH register
//.................................................................................
     #define  ADDR_ALM1SEC      0x11       //  address of ALARM1 SEC   register
     #define  ADDR_ALM1MIN      0x12       //  address of ALARM1 MIN   register
     #define  ADDR_ALM1HR       0x13       //  address of ALARM1 HOUR  register
     #define  ADDR_ALM1CTL      0x14       //  address of ALARM1 CONTR register
     #define  ADDR_ALM1DAT      0x15       //  address of ALARM1 DATE  register
     #define  ADDR_ALM1MTH      0x16       //  address of ALARM1 MONTH register
//.................................................................................
     #define  ADDR_SAVtoBAT_MIN 0x18       //  address of T_SAVER MIN(VDD->BAT)
     #define  ADDR_SAVtoBAT_HR  0x19       //  address of T_SAVER HR (VDD->BAT)
     #define  ADDR_SAVtoBAT_DAT 0x1a       //  address of T_SAVER DAT(VDD->BAT)
     #define  ADDR_SAVtoBAT_MTH 0x1b       //  address of T_SAVER MTH(VDD->BAT)
//..................................................................................
     #define  ADDR_SAVtoVDD_MIN 0x1c       //  address of T_SAVER MIN(BAT->VDD)
     #define  ADDR_SAVtoVDD_HR  0x1d       //  address of T_SAVER HR (BAT->VDD)
     #define  ADDR_SAVtoVDD_DAT 0x1e       //  address of T_SAVER DAT(BAT->VDD)
     #define  ADDR_SAVtoVDD_MTH 0x1f       //  address of T_SAVER MTH(BAT->VDD)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                  GLOBAL CONSTANTS RTCC - INITIALIZATION
//..................................................................................
     #define  START_32KHZ  0x80            //  start crystal: ST = b7 (ADDR_SEC)
     #define  LP           0x20            //  mask for the leap year bit(MONTH REG)
     #define  HOUR_12      0x40            //  12 hours format   (ADDR_HOUR)
     #define  HOUR_24      0x00            //  12 hours format   (ADDR_HOUR)
     #define  PM           0x20            //  post-meridian bit (ADDR_HOUR)
     #define  OUT_PIN      0x80            //  = b7 (ADDR_CTRL)
     #define  SQWE         0x40            //  SQWE = b6 (ADDR_CTRL)
     #define  ALM_NO       0x00            //  no alarm activated        (ADDR_CTRL)
     #define  ALM_0        0x10            //  ALARM0 is       activated (ADDR_CTRL)
     #define  ALM_1        0x20            //  ALARM1 is       activated (ADDR_CTRL)
     #define  ALM_01       0x30            //  both alarms are activated (ADDR_CTRL)
     #define  MFP_01H      0x00            //  MFP = SQVAW(01 HERZ)      (ADDR_CTRL)
     #define  MFP_04K      0x01            //  MFP = SQVAW(04 KHZ)       (ADDR_CTRL)
     #define  MFP_08K      0x02            //  MFP = SQVAW(08 KHZ)       (ADDR_CTRL)
     #define  MFP_32K      0x03            //  MFP = SQVAW(32 KHZ)       (ADDR_CTRL)
     #define  MFP_64H      0x04            //  MFP = SQVAW(64 HERZ)      (ADDR_CTRL)
     #define  ALMx_POL     0x80            //  polarity of MFP on alarm  (ADDR_ALMxCTL)
     #define  ALMxC_SEC    0x00            //  ALARM compare on SEC      (ADDR_ALMxCTL)
     #define  ALMxC_MIN    0x10            //  ALARM compare on MIN      (ADDR_ALMxCTL)
     #define  ALMxC_HR     0x20            //  ALARM compare on HOUR     (ADDR_ALMxCTL)
     #define  ALMxC_DAY    0x30            //  ALARM compare on DAY      (ADDR_ALMxCTL)
     #define  ALMxC_DAT    0x40            //  ALARM compare on DATE     (ADDR_ALMxCTL)
     #define  ALMxC_ALL    0x70            //  ALARM compare on all param(ADDR_ALMxCTL)
     #define  ALMx_IF      0x08            //  MASK of the ALARM_IF      (ADDR_ALMxCTL)
     #define  OSCON        0x20            //  state of the oscillator(running or not)
     #define  VBATEN       0x08            //  enable battery for back-up


	#define MASK_SEC			0x7F
	#define MASK_MIN			0x7F
	#define MASK_HOUR			0x3F
	#define MASK_DA				0x07
	#define MASK_DATE			0x3F
	#define MASK_MNTH			0x1F
	#define MASK_YEAR			0xFF
	#define MASK_ALARM_FLAG		0x08

#define I2C_ERR_START       0x01
#define I2C_ERR_RESTART     0x02
#define I2C_ERR_STOP        0x04
#define I2C_ERR_WR          0x08
#define I2C_ERR_RD          0x10
#define I2C_ERR_NACK        0x20
#define I2C_ERR_ACK         0x40

int clock_GetYear(void);
int clock_GetMonth(void);
int clock_GetDay(void);
int clock_GetHour(void);
int clock_GetMinute(void);
int clock_GetSecond(void);
int clock_GetDayOfWeek(void);
int clock_GetDayOfYear(void);
int clock_SetDateHeure(char Heure, char Minute, char Seconde, char Jour, char Mois, int Annee);


void rtcc_wr(unsigned char time_var, unsigned char rtcc_reg);
unsigned char rtcc_rd(unsigned char rtcc_reg);
void rtc_init(long DateHeureDefault);
void rtc_gestion(void);
int rtc_SetAlarme();
