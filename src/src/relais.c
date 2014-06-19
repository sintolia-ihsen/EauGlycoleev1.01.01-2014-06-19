
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "relais.h"
#include "timers.h"
#include "slvxsbus.h"
#include "can.h"
#include "tor.h"
#include "clock.h"
#include "variables.h"
#include "pwm.h"
#include "main.h"


void InitRelais()
{
	if(GetlReg(RNV_TEMPS_ACTIVATION_SORTIE) == -1)
	{
		SetlReg(RNV_TEMPS_ACTIVATION_SORTIE,1);
	}
	
	if(GetlReg(RNV_TEMPS_ENTRE_SORTIE) == -1)
	{
		SetlReg(RNV_TEMPS_ENTRE_SORTIE,0);	
	}
	
	if(GetlReg(RNV_NBR_REPETITION) == -1)
	{
		SetlReg(RNV_NBR_REPETITION,50);	
	}	
}

unsigned int MAE_RodageRelais = 0;

unsigned int CptSorties;
unsigned int CptRepetition;

void GestionRelais()
{
	int Cpt;
	if((!GetFlag(FV_MARCHE_ARRET_RELAIS)) && (MAE_RodageRelais > 1))
	{
		MAE_RodageRelais = 0;
	}
		
	switch(MAE_RodageRelais)
	{
		case 0:
			
			for(Cpt = 0; Cpt < TOR_NBR_SORTIE; Cpt++)
				SetFlag(FV_S(Cpt), 0);
				
			MAE_RodageRelais = 1;	
		
		break;
		
		case 1:
		
			if(GetFlag(FV_MARCHE_ARRET_RELAIS))
			{
				CptSorties = 0;
				CptRepetition = 0;
				MAE_RodageRelais = 2;
			}	
		
		break;
		
		case 2:
		
			SetlReg(RV_SORTIE_EN_COURS,CptSorties+1);
			SetlReg(RV_NUM_REPITION,CptRepetition+1);
			
			SetFlag(FV_S(CptSorties), 1);
			SetTimer(TIMER_RODAGE_ACTIVATION_SORTIE, GetlReg(RNV_TEMPS_ACTIVATION_SORTIE));
			MAE_RodageRelais = 3;
		
		break;

		case 3:
			if(!GetTimer(TIMER_RODAGE_ACTIVATION_SORTIE))
			{
				SetFlag(FV_S(CptSorties), 0);
				SetTimer(TIMER_RODAGE_ENTRE_SORTIE, GetlReg(RNV_TEMPS_ENTRE_SORTIE));
				CptSorties ++;
				MAE_RodageRelais = 4;
			}	
		break;
		
		case 4:
			if(!GetTimer(TIMER_RODAGE_ENTRE_SORTIE))
			{
				if(CptSorties >= TOR_NBR_SORTIE)
				{
					CptSorties = 0;
					CptRepetition++;	 
				}
				
				if(CptRepetition >= GetlReg(RNV_NBR_REPETITION))
				{
					SetFlag(FV_MARCHE_ARRET_RELAIS, 0);
					break;
				}
				
				MAE_RodageRelais = 2;	 	
			}	
		break;
		
	}	
}


