#if !defined(VARIABLES_H)
#define VARIABLES_H

#include <time.h>

#define USE_FLASH	1

// Fonctions gestion des bit

// Nbre variable sbus
#define NB_TIMERS_TOTAL				1024
#define NB_REGS_TOTAL				3072
#define NB_FLAGS_TOTAL				3072
#define NB_COMPTEURS_TOTAL			16
#if (USE_FLASH)
#define NB_NVREGS_TOTAL1			1024
#define NB_NVREGS_TOTAL2			1024
#define NB_NVREGS_TOTAL3			768
#define NB_NVREGS_TOTAL4			1024

#define NB_NVFLAGS_TOTAL1			0
#define NB_NVFLAGS_TOTAL2			0
#define NB_NVFLAGS_TOTAL3			8192
#define NB_NVFLAGS_TOTAL4			0
#endif

#define BASE_TIMERS					0
#define BASE_REGS					0
#define BASE_FLAGS					0
#if (USE_FLASH)
#define BASE_NVREGS1				20000
#define BASE_NVREGS2				30000
#define BASE_NVREGS3				40000
#define BASE_NVREGS4				50000

#define BASE_NVFLAGS1				20000
#define BASE_NVFLAGS2				40000
#define BASE_NVFLAGS3				30000
#define BASE_NVFLAGS4				50000
#endif


typedef union
{
	struct
	{
		unsigned f0:1;
		unsigned f1:1;
		unsigned f2:1;
		unsigned f3:1;
		unsigned f4:1;
		unsigned f5:1;
		unsigned f6:1;
		unsigned f7:1;
		unsigned f8:1;
		unsigned f9:1;
		unsigned f10:1;
		unsigned f11:1;
		unsigned f12:1;
		unsigned f13:1;
		unsigned f14:1;
		unsigned f15:1;
		unsigned f16:1;
		unsigned f17:1;
		unsigned f18:1;
		unsigned f19:1;
		unsigned f20:1;
		unsigned f21:1;
		unsigned f22:1;
		unsigned f23:1;
		unsigned f24:1;
		unsigned f25:1;
		unsigned f26:1;
		unsigned f27:1;
		unsigned f28:1;
		unsigned f29:1;
		unsigned f30:1;
		unsigned f31:1;
	}flags;
	unsigned char octet[4];	
}octetflags;

typedef union
{
	octetflags of;
	unsigned long ul;
	long l;
	double d;
	time_t t;
	unsigned short us[2];
	short s[2];
	unsigned char c[4];
}ofsld;

typedef struct
{
	unsigned bTempbit:1;
	unsigned bWriteConsgne:1;
}variablebit;

extern volatile unsigned long	Timers[NB_TIMERS_TOTAL];
extern volatile unsigned long	Compteurs[NB_COMPTEURS_TOTAL];
extern volatile long 			Registres[NB_REGS_TOTAL];
extern volatile char			Flags[NB_FLAGS_TOTAL/8];
extern octetflags				EtatCompteurs;


// Prototypes des fonctions XSBus
int SetlReg(unsigned short Num,long Val);
int SetfReg(unsigned short Num,double Val);
long GetlReg(unsigned short Num);
double GetfReg(unsigned short Num);
int SetFlag(unsigned short Num,char Val);
int GetFlag(unsigned short Num);

int SetTimer(unsigned short Num,unsigned long Val);
unsigned long GetTimer(unsigned short Num);
int SetTimerIsr(unsigned short Num,unsigned long Val);
unsigned long GetTimerIsr(unsigned short Num);
#endif //!defined(VARIABLES_H)
