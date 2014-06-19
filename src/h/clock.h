#if !defined(CLOCK_H)
#define CLOCK_H


#define TRIS_ENTREE  _TRISD0

typedef struct
{
	unsigned bSaveClock:1;
	unsigned bAjusterClock:1;
	unsigned bGPSRaff:1;
	unsigned bSave:1;
}clockbit;

extern clockbit bitclock;

void clock_init(void);
void clock_Gestion(void);
void clock_MAJ(void);

void clock_Stop(void);
void clock_Start(void);
void clock_Save(int);
int clock_GetYear(void);
int clock_GetMonth(void);
int clock_GetDay(void);
int clock_GetHour(void);
int clock_GetMinute(void);
int clock_GetSecond(void);
int clock_GetDayOfWeek(void);
int clock_GetDayOfYear(void);
int clock_SetDateHeure(char Heure, char Minute, char Seconde, char Jour, char Mois, int Annee);

#endif
