//////////////////////////////////////////////////////////////
// Date de création : 14 Février 2007						//
// Auteur			: Faouzi								//
// Description		: Fonctions pour gérer l'horloge		//
//////////////////////////////////////////////////////////////

#include <plib.h>
#include <time.h>
#include <stdlib.h>
#include "clock.h"
#include "rtc.h"
#include "main.h"
#include "variables.h"


clockbit bitclock;

unsigned int clock_year;
unsigned int clock_mon;
unsigned int clock_mday;
unsigned int clock_hour;
unsigned int clock_min;
unsigned int clock_sec;
unsigned int clock_wday;
unsigned int clock_yday;
unsigned int clock_isdst;

long clock_Lastdt;
long Clock_OldDateHeure;

unsigned char err_code; // global error flag

//--------------------------------------------------------------------------------------------
// Fonction : clock_init
// Utilité : Initialisation du module clock
//--------------------------------------------------------------------------------------------
void clock_GetTimeCompile(struct tm	*pTime)
{
	//"18:02:51"
	char StrDateTime[12];
	char *gsm_pValTime;

	strcpy(StrDateTime, __TIME__);

	gsm_pValTime = strtok(StrDateTime, ":");
	pTime->tm_hour = atoi(gsm_pValTime);
	gsm_pValTime = strtok(NULL, ":");
	pTime->tm_min = atoi(gsm_pValTime);
	gsm_pValTime = strtok(NULL, ":");
	pTime->tm_sec = atoi(gsm_pValTime);
}

const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void clock_GetDateCompile(struct tm	*pTime)
{
	//"Jul  5 2012"
	char i;
	char StrDateTime[12];
	char *gsm_pValTime;

	strcpy(StrDateTime	,__DATE__);

	gsm_pValTime = strtok(StrDateTime, " ");
	for (i = 0; i < 12; i++)
	{
		if (!strcmp(gsm_pValTime, months[i]))
		{
			pTime->tm_mon = i;
			break;
		}
	}
	gsm_pValTime = strtok(NULL, " ");
	pTime->tm_mday = atoi(gsm_pValTime);
	gsm_pValTime = strtok(NULL, " ");
	pTime->tm_year = atoi(gsm_pValTime) - 1900;
}

void clock_init()
{
    struct tm	STime;
	long DateHeure;

    DateHeure = GetlReg(RNV_DATE_COMPILE);
    if(DateHeure == 0xFFFFFFFF)
	{
		clock_GetTimeCompile(&STime);
		clock_GetDateCompile(&STime);

    	DateHeure = mktime(&STime);

        SetlReg(RNV_DATE_COMPILE, DateHeure);
	}

    rtc_init(DateHeure);
	DateHeure = rtc_Read();
	Clock_OldDateHeure = DateHeure;
	Registres[0] = DateHeure;
   
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_MAJDateTime
// Utilité : mise a jour des variables date et heure system (heure, minutes, secondes ...) chaque seconde
//			 et corrige l'heure system par rapport a l'heure GPS si cette dernière est valide
//--------------------------------------------------------------------------------------------

void clock_Gestion()
{
	struct tm	*pSTime;

	// Chaque seconde ou changement de de la date/heure
	if(clock_Lastdt != Registres[0])
	{
		clock_Lastdt = Registres[0];
		pSTime = localtime(&clock_Lastdt);
		clock_year	= pSTime->tm_year + 1900; // tm_year = nombre des année depuis 1900 (ex pour l'année 2007 on a tm_year = 107)
		clock_mon	= pSTime->tm_mon + 1;
		clock_mday	= pSTime->tm_mday;
		clock_hour	= pSTime->tm_hour;
		clock_min	= pSTime->tm_min;
		clock_sec	= pSTime->tm_sec;
		clock_wday	= pSTime->tm_wday;
		clock_yday	= pSTime->tm_yday;
		clock_isdst = pSTime->tm_isdst;
	}
	
	if(bitclock.bSave)
	{
		bitclock.bSave = 0;
		rtc_Write(Registres[0]);
		SetFlag(FV_DATE_HEURE_ERREUR, 0);
	}

    SetFlag(FV_RTC_ERREUR, err_code!=0);
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_Stop
// Utilité : Arrêt du timer1 ==> arrêt module horloge
//--------------------------------------------------------------------------------------------

void clock_Stop(void)
{
	T1CONbits.TON = 0;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_Start
// Utilité : démarrage du timer1 ==> démarrage module horloge
//--------------------------------------------------------------------------------------------

void clock_Start(void)
{
	T1CONbits.TON = 1;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetYear
// Utilité : renvoi l'année de l'horloge system
//--------------------------------------------------------------------------------------------

int  clock_GetYear()
{
	return clock_year;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetMonth
// Utilité : renvoi le moi de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetMonth() 
{
	return clock_mon;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetDay
// Utilité : renvoi le jour de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetDay()
{
	return clock_mday;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetHour
// Utilité : renvoi l'heure de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetHour()
{
	return clock_hour;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetMinute
// Utilité : renvoi les minutes de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetMinute()
{
	return clock_min;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetSecond
// Utilité : renvoi les secondes de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetSecond()
{
	return clock_sec;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetDayOfWeek
// Utilité : renvoi le jour de la semaine de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetDayOfWeek()
{
	return clock_wday;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_GetDayOfYear
// Utilité : renvoi le jour de l'année de l'horloge system
//--------------------------------------------------------------------------------------------
int clock_GetDayOfYear()
{
	return clock_yday;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_IsBisextile
// Utilité : renvoi si l'année est Bisextile
//--------------------------------------------------------------------------------------------
int clock_IsBisextile()
{
	return clock_isdst;
}

//--------------------------------------------------------------------------------------------
// Fonction : clock_SetDateHeure
// Utilité : changer l'heure system
//--------------------------------------------------------------------------------------------
int clock_SetDateHeure(char Heure, char Minute, char Seconde, char Jour, char Mois, int Annee)
{
	struct tm	STime;
	unsigned long TimeTempo;
	
	if(	Heure > 23 ||	Minute > 59 ||	Seconde > 59 || 
		Jour < 1 || Jour > 31 ||
		Mois < 1 || Mois > 12 ||
		Annee < 1970 || Annee > 2020
	  ) 
		return 0;
		
	STime.tm_hour = Heure;
	STime.tm_min = Minute;
	STime.tm_sec = Seconde;
	STime.tm_year = Annee - 1900; // tm_year = nombre des année depuis 1900 (ex pour l'année 2007 on a tm_year = 107)
	STime.tm_mon = Mois - 1;
	STime.tm_mday = Jour;

	TimeTempo = mktime(&STime);
	if(TimeTempo == -1) return 0;
	Registres[0] = TimeTempo;
	return 1;
}




void clock_MAJ(void)
{ 
    if(Registres[0] != Clock_OldDateHeure)
    {
	    bitclock.bSave = 1;
	}
	
	Registres[0]++;
	Clock_OldDateHeure = Registres[0];
}

 
