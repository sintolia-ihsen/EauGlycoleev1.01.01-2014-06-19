//////////////////////////////////////////////////////////////
// Date de création : 14 Février 2007						//
// Auteur			: Faouzi								//
// Description		: Fonctions pour gérer les timers		//
//////////////////////////////////////////////////////////////

////////////////////////	TIMER 2 (16Bits)	//////////////////////
#include "main.h"
#include "timers.h"
#include "variables.h"
#include "uart.h"
//#include "tor.h"
//
//#include "slvxsbus.h"
//#include "mtrxsbus.h"
//#include "radio.h"
//#include "ledetat.h"

#ifdef TMR_USE_TIMER2

unsigned char timers2_Tick;

void timer2_Init()
{
	int Perametre = T2_ON | T2_SOURCE_INT | T2_PS_1_256;

	if(GetSystemClock() == 20000000)
	{
		Perametre = T2_ON | T2_SOURCE_INT | T2_PS_1_64;
	}

    OpenTimer2(Perametre, 31250);

    // set up the timer interrupt with a priority of 2
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
}

//----------------------------------------------------------------------
// Fonction :
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void timer2_Stop(void)
{
	T2CONbits.TON = 0;
}

//----------------------------------------------------------------------
// Fonction :
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void timer2_Start(void)
{
	T2CONbits.TON = 1;
}

//----------------------------------------------------------------------
// Fonction : Code de l'interruption du timer 2
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void)
{
    // clear the interrupt flag
    mT2ClearIntFlag();
    int i;
	for(i = 0; i < NB_TIMERS_TOTAL; i++)
	{
		if(Timers[i]) Timers[i]--;
	}
	
	/*****************************************/
	if(EtatCompteurs.octet[0])
	{
		if(EtatCompteurs.flags.f0) Compteurs[0] += PR2;
		if(EtatCompteurs.flags.f1) Compteurs[1] += PR2;
		if(EtatCompteurs.flags.f2) Compteurs[2] += PR2;
		if(EtatCompteurs.flags.f3) Compteurs[3] += PR2;
		if(EtatCompteurs.flags.f4) Compteurs[4] += PR2;
		if(EtatCompteurs.flags.f5) Compteurs[5] += PR2;
		if(EtatCompteurs.flags.f6) Compteurs[6] += PR2;
		if(EtatCompteurs.flags.f7) Compteurs[7] += PR2;
	}
	
	if(EtatCompteurs.octet[1])
	{
		if(EtatCompteurs.flags.f8)  Compteurs[8]  += PR2;
		if(EtatCompteurs.flags.f9)  Compteurs[9]  += PR2;
		if(EtatCompteurs.flags.f10) Compteurs[10] += PR2;
		if(EtatCompteurs.flags.f11) Compteurs[11] += PR2;
		if(EtatCompteurs.flags.f12) Compteurs[12] += PR2;
		if(EtatCompteurs.flags.f13) Compteurs[13] += PR2;
		if(EtatCompteurs.flags.f14) Compteurs[14] += PR2;
		if(EtatCompteurs.flags.f15) Compteurs[15] += PR2;
	}		
	/*****************************************/
	
	tor_Echant();
/*
    timers2_Tick++;
    if(timers2_Tick >= 10)
    {
	timers2_Tick = 0;
        clock_MAJ();
    }
*/
}

#endif

#ifdef TMR_USE_TIMER4

unsigned short timer4_TV[TIMER4_NBR_TV];

void timer4_Init()
{
	int prs = 10000;

	if(GetSystemClock() == 20000000)
	{
		prs = 2500;
	}

    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_8, prs); //1ms

    // set up the timer interrupt with a priority of 4
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_2);

}

//----------------------------------------------------------------------
// Fonction :
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
unsigned short timer4_GetTimer(int Index)
{
	return timer4_TV[Index];
}

//----------------------------------------------------------------------
// Fonction :
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void timer4_SetTimer(int Index, unsigned short Val)
{
	timer4_TV[Index] = Val;
	T4CONbits.TON = 1;
}

//----------------------------------------------------------------------
// Fonction : Code de l'interruption du timer 4
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void __ISR(_TIMER_4_VECTOR, ipl2) Timer4Handler(void)
{
    // clear the interrupt flag
    mT4ClearIntFlag();

    int i;
	for(i = 0; i < TIMER4_NBR_TV; i++)
	{
		if(timer4_TV[i]) timer4_TV[i]--;
	}
}
#endif

#ifdef TMR_USE_TIMER5

void timer5_Init()
{
	if(GetSystemClock() == 20000000)
	{
		OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_8, 1250); //10ms
	}
	else
	{
		OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_8, 5000); //10ms
	}
	
    // set up the timer interrupt with a priority of 2
    ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_2);
    
    timer5_Stop();
}

//----------------------------------------------------------------------
// Fonction :
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void timer5_Stop(void)
{
	if(T5CONbits.TON)
	{
		T5CONbits.TON = 0;
	}
}

//----------------------------------------------------------------------
// Fonction :
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void timer5_Start(void)
{
	if(!T5CONbits.TON)
	{
		T5CONbits.TON = 1;
	}
}

//----------------------------------------------------------------------
// Fonction : Code de l'interruption du timer 2
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void __ISR(_TIMER_5_VECTOR, ipl2) Timer5Handler(void)
{
    // clear the interrupt flag
    mT5ClearIntFlag();
	tor_Timer10ms();
}

#endif

