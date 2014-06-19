#include <plib.h>
#include "tor.h"
#include "timers.h"
#include "main.h"
#include "variables.h"


unsigned int tor_NbrImp[TOR_NBR_ENTREE_INT];
unsigned int tor_CnsgNImp[TOR_NBR_ENTREE_INT];
unsigned long tor_TempsMinEntreImp[TOR_NBR_ENTREE_INT];
unsigned long tor_TempsMaxEntreImp[TOR_NBR_ENTREE_INT];
unsigned long tor_CurVal[TOR_NBR_ENTREE_INT];
unsigned long tor_TempsEchant[TOR_NBR_ENTREE_INT];
unsigned long tor_CptIt[TOR_NBR_ENTREE_INT];


char tor_MAESortie[TOR_NBR_SORTIE];
bittor torbit;

void tor_init()
{
	int i;
	
	TOR_TRIS_E01 = TOR_INPUT;	
	//INT3
    INTSetVectorPriority( INT_EXTERNAL_1_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority( INT_EXTERNAL_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_INT1);
    INTEnable(INT_INT1, INT_ENABLED);
    
	TOR_TRIS_E02 = TOR_INPUT;	
	INTSetVectorPriority( INT_EXTERNAL_2_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority( INT_EXTERNAL_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_INT2);
    INTEnable(INT_INT2, INT_ENABLED);

	TOR_TRIS_E03 = TOR_INPUT;
	INTSetVectorPriority( INT_EXTERNAL_3_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority( INT_EXTERNAL_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_INT3);
    INTEnable(INT_INT3, INT_ENABLED);
    
	TOR_TRIS_E04 = TOR_INPUT;
	INTSetVectorPriority( INT_EXTERNAL_4_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority( INT_EXTERNAL_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_INT4);
    INTEnable(INT_INT4, INT_ENABLED);

	TOR_TRIS_E05 = TOR_INPUT;
	TOR_TRIS_E06 = TOR_INPUT;
	TOR_TRIS_E07 = TOR_INPUT;
	TOR_TRIS_E08 = TOR_INPUT;
	TOR_TRIS_E09 = TOR_INPUT;
	TOR_TRIS_E10 = TOR_INPUT;
	TOR_TRIS_E11 = TOR_INPUT;
	TOR_TRIS_E12 = TOR_INPUT;

	///////////////// Sortie ////////////////////////
	// Initialisation des sortie avant l'initialisation des tris (Etat zero par defaut)
	TOR_S01 = 0;
	TOR_S02 = 0;
	TOR_S03 = 0;
	TOR_S04 = 0;
	TOR_S05 = 0;
	TOR_S06 = 0;
	TOR_S07 = 0;
	TOR_S08 = 0;
	TOR_S09 = 0;
	TOR_S10 = 0;
	TOR_S11 = 0;
	TOR_S12 = 0;

	TOR_TRIS_S01 = TOR_OUTPUT;
	TOR_TRIS_S02 = TOR_OUTPUT;
	TOR_TRIS_S03 = TOR_OUTPUT;
	TOR_TRIS_S04 = TOR_OUTPUT;
	TOR_TRIS_S05 = TOR_OUTPUT;
	TOR_TRIS_S06 = TOR_OUTPUT;
	TOR_TRIS_S07 = TOR_OUTPUT;
	TOR_TRIS_S08 = TOR_OUTPUT;
	TOR_TRIS_S09 = TOR_OUTPUT;
	TOR_TRIS_S10 = TOR_OUTPUT;
	TOR_TRIS_S11 = TOR_OUTPUT;
	TOR_TRIS_S12 = TOR_OUTPUT;
		
	for(i = 0; i < TOR_NBR_ENTREE_INT; i++)
	{
		if(GetlReg(TOR_TEMPS_MIN_ENTRE_IMP(i)) == 0xFFFFFFFF)
		{
			SetlReg(TOR_TEMPS_MIN_ENTRE_IMP(i), 1);
		}
		
		if(GetlReg(TOR_TEMPS_MAX_ENTRE_IMP(i)) == 0xFFFFFFFF)
		{
			SetlReg(TOR_TEMPS_MAX_ENTRE_IMP(i), 20);
		}
		
		if(GetlReg(TOR_NBR_IMP(i)) == 0xFFFFFFFF)
		{
			SetlReg(TOR_NBR_IMP(i), 0);
		}

		if(GetlReg(TOR_TEMPS_ECHANT(i)) == 0xFFFFFFFF)
		{
			SetlReg(TOR_TEMPS_ECHANT(i), 1);
		}

		tor_TempsMinEntreImp[i] = GetlReg(TOR_TEMPS_MIN_ENTRE_IMP(i))*312.5;
		tor_TempsMaxEntreImp[i] = GetlReg(TOR_TEMPS_MAX_ENTRE_IMP(i));
		tor_CnsgNImp[i] = GetlReg(TOR_NBR_IMP(i));
		tor_TempsEchant[i] = GetlReg(TOR_TEMPS_ECHANT(i));
	}
	
	if(	GetlReg(RNV_TEMP_AV_ACTIVATION_S(0)) == 0xFFFFFFFF &&
		GetlReg(RNV_TEMP_ACTIVATION_S(0)) == 0xFFFFFFFF
	)
	{
		for(i = 0; i < TOR_NBR_SORTIE; i++)
		{
			SetlReg(RNV_TEMP_AV_ACTIVATION_S(i), 0);
			SetlReg(RNV_TEMP_ACTIVATION_S(i), 0);
		}
	}
	
	if(	GetlReg(RNV_TEMP_AV_ACTIVATION_E(0)) == 0xFFFFFFFF &&
		GetlReg(RNV_TEMP_AV_DESACTIVATION_E(0)) == 0xFFFFFFFF
	)
	{
		for(i = 0; i < TOR_NBR_ENTREE; i++)
		{
			SetlReg(RNV_TEMP_AV_ACTIVATION_E(i), 20);
			SetlReg(RNV_TEMP_AV_DESACTIVATION_E(i), 5);
		}

		SetlReg(RNV_TEMP_AV_DESACTIVATION_E11, 0);
	}
}

void tor_SetSorties(int num, int val)
{
	SetFlag(TOR_ADR_FLAG_IMG_S(num), val);
}

void tor_gestion()
{
	int i;
	unsigned long Periode;
	unsigned long DutyCycle;
						
	for(i = 0; i < TOR_NBR_ENTREE_INT; i++)
	{
		
		tor_TempsMinEntreImp[i] = GetlReg(TOR_TEMPS_MIN_ENTRE_IMP(i))*312.5;
		tor_TempsMaxEntreImp[i] = GetlReg(TOR_TEMPS_MAX_ENTRE_IMP(i));
		tor_CnsgNImp[i] = GetlReg(TOR_NBR_IMP(i));
		tor_TempsEchant[i] = GetlReg(TOR_TEMPS_ECHANT(i));
		
		if(!Timers[TOR_TIMER_TEMPS_MAX_TOR(i)] && tor_TempsMaxEntreImp[i]) // changer min par max
		{
			SetlReg(TOR_ECHANT_POINT(i), 0);
			SetlReg(TOR_TEMPS_N_IMP_POINT(i), 0);
			SetlReg(TOR_CPT_NBR_IMP(i), 0);
			switch(i)
			{
				case TOR_ENTREE01	:
				{
					EtatCompteurs.flags.f0 = 0;
					EtatCompteurs.flags.f1 = 0;
					break;
				}
				case TOR_ENTREE02	:
				{
					EtatCompteurs.flags.f2 = 0;
					EtatCompteurs.flags.f3 = 0;
					break;
				}
				case TOR_ENTREE03	:
				{
					EtatCompteurs.flags.f4 = 0;
					EtatCompteurs.flags.f5 = 0;
					break;
				}
				case TOR_ENTREE04	:
				{
					EtatCompteurs.flags.f6 = 0;
					EtatCompteurs.flags.f7 = 0;
					break;
				}
			}
		}
	}
	INTCONbits.INT1EP = !GetFlag(FNV_ETAT_ACTIF_E(0));
	INTCONbits.INT2EP = !GetFlag(FNV_ETAT_ACTIF_E(1));
	INTCONbits.INT3EP = !GetFlag(FNV_ETAT_ACTIF_E(2));
	INTCONbits.INT4EP = !GetFlag(FNV_ETAT_ACTIF_E(3));
	

	
	for(i = 0; i < TOR_NBR_ENTREE; i++)
	{
		torbit.bInvert = GetFlag(FNV_ETAT_ACTIF_E(i));
		
		switch(i)
		{
			case TOR_ENTREE01	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E01;
				}
				else
				{
					torbit.bEntree = !TOR_E01;
				}
				
				if(torbit.bImageE1 != torbit.bEntree)
				{
					torbit.bImageE1 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE02	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E02;
				}
				else
				{
					torbit.bEntree = !TOR_E02;
				}
				
				if(torbit.bImageE2 != torbit.bEntree)
				{
					torbit.bImageE2 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE03	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E03;
				}
				else
				{
					torbit.bEntree = !TOR_E03;
				}
				
				if(torbit.bImageE3 != torbit.bEntree)
				{
					torbit.bImageE3 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE04	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E04;
				}
				else
				{
					torbit.bEntree = !TOR_E04;
				}
				
				if(torbit.bImageE4 != torbit.bEntree)
				{
					torbit.bImageE4 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE05	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E05;
				}
				else
				{
					torbit.bEntree = !TOR_E05;
				}
				
				if(torbit.bImageE5 != torbit.bEntree)
				{
					torbit.bImageE5 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE06	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E06;
				}
				else
				{
					torbit.bEntree = !TOR_E06;
				}
				
				if(torbit.bImageE6 != torbit.bEntree)
				{
					torbit.bImageE6 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE07	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E07;
				}
				else
				{
					torbit.bEntree = !TOR_E07;
				}
				
				if(torbit.bImageE7 != torbit.bEntree)
				{
					torbit.bImageE7 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE08	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E08;
				}
				else
				{
					torbit.bEntree = !TOR_E08;
				}
				
				if(torbit.bImageE8 != torbit.bEntree)
				{
					torbit.bImageE8 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE09	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E09;
				}
				else
				{
					torbit.bEntree = !TOR_E09;
				}
				
				if(torbit.bImageE9 != torbit.bEntree)
				{
					torbit.bImageE9 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE10	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E10;
				}
				else
				{
					torbit.bEntree = !TOR_E10;
				}
				
				if(torbit.bImageE10 != torbit.bEntree)
				{
					torbit.bImageE10 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE11	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E11;
				}
				else
				{
					torbit.bEntree = !TOR_E11;
				}
				
				if(torbit.bImageE11 != torbit.bEntree)
				{
					torbit.bImageE11 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
			case TOR_ENTREE12	:
			{
				if(torbit.bInvert)
				{
					torbit.bEntree = TOR_E12;
				}
				else
				{
					torbit.bEntree = !TOR_E12;
				}
				
				if(torbit.bImageE12 != torbit.bEntree)
				{
					torbit.bImageE12 = torbit.bEntree;
					torbit.bChargeTimer = 1;
				}	
				break;
			}
		}
		
		if(torbit.bChargeTimer)
		{
			torbit.bChargeTimer = 0;
			if(torbit.bEntree)
			{
				SetTimer(TIMER_E(i), GetlReg(RNV_TEMP_AV_ACTIVATION_E(i)));
			}
			else
			{
				SetTimer(TIMER_E(i), GetlReg(RNV_TEMP_AV_DESACTIVATION_E(i)));
			}
		}
		
		if(!GetTimer(TIMER_E(i)))
		{
			SetFlag(TOR_ADR_FLAG_IMG_E(i), torbit.bEntree);
		}			
	}
	
	for(i = 0; i < TOR_NBR_SORTIE; i++)
	{
		if(GetFlag(TOR_ADR_FLAG_IMG_S(i)))
		{
			switch(i)
			{
				case TOR_SORTIE01	:
				{
					torbit.bSortie = TOR_S01;
					break;
				}
				case TOR_SORTIE02	:
				{
					torbit.bSortie = TOR_S02;
					break;
				}
				case TOR_SORTIE03	:
				{
					torbit.bSortie = TOR_S03;
					break;
				}
				case TOR_SORTIE04	:
				{
					torbit.bSortie = TOR_S04;
					break;
				}
				case TOR_SORTIE05	:
				{
					torbit.bSortie = TOR_S05;
					break;
				}
				case TOR_SORTIE06	:
				{
					torbit.bSortie = TOR_S06;
					break;
				}
				case TOR_SORTIE07	:
				{
					torbit.bSortie = TOR_S07;
					break;
				}
				case TOR_SORTIE08	:
				{
					torbit.bSortie = TOR_S08;
					break;
				}
				case TOR_SORTIE09	:
				{
					torbit.bSortie = TOR_S09;
					break;
				}
				case TOR_SORTIE10	:
				{
					torbit.bSortie = TOR_S10;
					break;
				}
				case TOR_SORTIE11	:
				{
					torbit.bSortie = TOR_S11;
					break;
				}
				case TOR_SORTIE12	:
				{
					torbit.bSortie = TOR_S12;
					break;
				}
			}
		
			
			if(GetFlag(FNV_MODE_SORTIE(i))) // Marche/Arret
			{				
				switch(tor_MAESortie[i])
				{
					case TOR_AV_ACTIVATION_SORTIE :
					{
						if(GetlReg(RNV_TEMP_AV_ACTIVATION_S(i)))
						{
							SetTimer(TIMER_S(i), GetlReg(RNV_TEMP_AV_ACTIVATION_S(i)));
							tor_MAESortie[i] = TOR_TEMPO_AV_ACTIVATION_SORTIE;
						}
					}	
					case TOR_TEMPO_AV_ACTIVATION_SORTIE :
					{
						if(GetTimer(TIMER_S(i)))
						{
							break;
						}
						
						torbit.bSortie = 1;
						
						if(GetlReg(RNV_TEMP_ACTIVATION_S(i)))
						{
							SetTimer(TIMER_S(i), GetlReg(RNV_TEMP_ACTIVATION_S(i)));
							tor_MAESortie[i] = TOR_TEMPO_ACTIVATION_SORTIE;
						}
						else
						{
							tor_MAESortie[i] = TOR_AV_ACTIVATION_SORTIE;
							break;
						}
					}	
					case TOR_TEMPO_ACTIVATION_SORTIE :
					{
						if(!GetTimer(TIMER_S(i)))
						{
							SetFlag(TOR_ADR_FLAG_IMG_S(i), 0);
							tor_MAESortie[i] = TOR_AV_ACTIVATION_SORTIE;
						}
						break;
					}
				}
			}
			else //Frequence
			{
				switch(tor_MAESortie[i])
				{
					case TOR_SORTIE_EN_ETAT_ON :
					{
						if(GetTimer(TIMER_S(i)))
						{
							break;
						}
						
						torbit.bSortie = 1;

						Periode = GetlReg(RNV_TEMP_AV_ACTIVATION_S(i));
						DutyCycle =  GetlReg(RNV_TEMP_ACTIVATION_S(i));
						
						if(DutyCycle >= Periode)
						{
							break;
						}
										
						SetTimer(TIMER_S(i), DutyCycle);
					}
						
					case TOR_SORTIE_EN_ETAT_OFF :
					{
						if(!GetTimer(TIMER_S(i)))
						{
							torbit.bSortie = 0;
							
							Periode = GetlReg(RNV_TEMP_AV_ACTIVATION_S(i));
							DutyCycle =  GetlReg(RNV_TEMP_ACTIVATION_S(i));
						
							SetTimer(TIMER_S(i), Periode - DutyCycle);
							tor_MAESortie[i] = TOR_SORTIE_EN_ETAT_ON;
						}
						break;
					}
				}
			}
		}
		else
		{
			torbit.bSortie = 0;
			SetTimer(TIMER_S(i), 0);
			tor_MAESortie[i] = TOR_AV_ACTIVATION_SORTIE;
		}
		
		
		switch(i)
		{
			case TOR_SORTIE01	:
			{
				TOR_S01 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE02	:
			{
				TOR_S02 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE03	:
			{
				TOR_S03 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE04	:
			{
				TOR_S04 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE05	:
			{
				TOR_S05 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE06	:
			{
				TOR_S06 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE07	:
			{
				TOR_S07 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE08	:
			{
				TOR_S08 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE09	:
			{
				TOR_S09 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE10	:
			{
				TOR_S10 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE11	:
			{
				TOR_S11 = torbit.bSortie;
				break;
			}
			case TOR_SORTIE12	:
			{
				TOR_S12 = torbit.bSortie;
				break;
			}
		}
	}

}

void tor_Echant()
{
	int i;

	for(i = 0; i < TOR_NBR_ENTREE_INT; i++)
	{
		tor_CptIt[i]++;	// incrémentation de temp 
		if(tor_CptIt[i] >= tor_TempsEchant[i]) // si l'heure d'extraire l'echatiant
		{
			Registres[TOR_ECHANT_POINT(i)] = tor_CurVal[i]; 
			tor_CurVal[i] = 0;
			tor_CptIt[i] = 0;
		}
	}
}
	

void __ISR(_EXTERNAL_1_VECTOR , ipl3) INT1InterruptHandler(void) 
{ 
	unsigned long TimerPh;
	
    mINT1ClearIntFlag();
    
    TimerPh = TMR2;
		
	EtatCompteurs.flags.f0 = 1;
	EtatCompteurs.flags.f1 = 1;
	if(Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(0)] +  TimerPh > tor_TempsMinEntreImp[0])
	{
		Timers[TOR_TIMER_TEMPS_MAX_TOR(0)] = tor_TempsMaxEntreImp[0];
		
		Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(0)] = -TimerPh;
		
		tor_CurVal[0]++;
		Registres[TOR_TOTAL_POINT(0)]++;
		
		Registres[TOR_CPT_NBR_IMP(0)]++;
		if(Registres[TOR_CPT_NBR_IMP(0)] >= tor_CnsgNImp[0])
		{
			Registres[TOR_CPT_NBR_IMP(0)] = 0;
		
			Registres[TOR_TEMPS_N_IMP_POINT(0)] = (Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(0)] + TimerPh) * 3.2;
			
			Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(0)] = -TimerPh;
		}
	}
}

void __ISR(_EXTERNAL_2_VECTOR , ipl3) INT2InterruptHandler(void) 
{
	unsigned long TimerPh;
	
    mINT2ClearIntFlag();
    
    TimerPh = TMR2;
		
	EtatCompteurs.flags.f2 = 1;
	EtatCompteurs.flags.f3 = 1;
	if(Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(1)] +  TimerPh > tor_TempsMinEntreImp[1])
	{
		Timers[TOR_TIMER_TEMPS_MAX_TOR(1)] = tor_TempsMaxEntreImp[1];
		
		Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(1)] = -TimerPh;
		
		tor_CurVal[1]++;
		Registres[TOR_TOTAL_POINT(1)]++;
		
		Registres[TOR_CPT_NBR_IMP(1)]++;
		if(Registres[TOR_CPT_NBR_IMP(1)] >= tor_CnsgNImp[1])
		{
			Registres[TOR_CPT_NBR_IMP(1)] = 0;
		
			Registres[TOR_TEMPS_N_IMP_POINT(1)] = (Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(1)] + TimerPh) * 3.2;
			
			Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(1)] = -TimerPh;
		}
	}
}

void __ISR(_EXTERNAL_3_VECTOR , ipl3) INT3InterruptHandler(void)
{
	unsigned long TimerPh;
	
    mINT3ClearIntFlag();
    
    TimerPh = TMR2;
		
	EtatCompteurs.flags.f4 = 1;
	EtatCompteurs.flags.f5 = 1;
	if(Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(2)] +  TimerPh > tor_TempsMinEntreImp[2])
	{
		Timers[TOR_TIMER_TEMPS_MAX_TOR(2)] = tor_TempsMaxEntreImp[2];
		
		Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(2)] = -TimerPh;
		
		tor_CurVal[2]++;
		Registres[TOR_TOTAL_POINT(2)]++;
		
		Registres[TOR_CPT_NBR_IMP(2)]++;
		if(Registres[TOR_CPT_NBR_IMP(2)] >= tor_CnsgNImp[2])
		{
			Registres[TOR_CPT_NBR_IMP(2)] = 0;
		
			Registres[TOR_TEMPS_N_IMP_POINT(2)] = (Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(2)] + TimerPh) * 3.2;
			
			Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(2)] = -TimerPh;
		}
	}
}

void __ISR(_EXTERNAL_4_VECTOR , ipl3) INT4InterruptHandler(void)
{
	unsigned long TimerPh;

    mINT4ClearIntFlag();

    TimerPh = TMR2;

	EtatCompteurs.flags.f6 = 1;
	EtatCompteurs.flags.f7 = 1;
	if(Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(3)] +  TimerPh > tor_TempsMinEntreImp[3])
	{
		Timers[TOR_TIMER_TEMPS_MAX_TOR(3)] = tor_TempsMaxEntreImp[3];

		Compteurs[TOR_CPT_TEMPS_MIN_ENTRE_IMP(3)] = -TimerPh;

		tor_CurVal[3]++;
		Registres[TOR_TOTAL_POINT(3)]++;

		Registres[TOR_CPT_NBR_IMP(3)]++;
		if(Registres[TOR_CPT_NBR_IMP(3)] >= tor_CnsgNImp[3])
		{
			Registres[TOR_CPT_NBR_IMP(3)] = 0;

			Registres[TOR_TEMPS_N_IMP_POINT(3)] = (Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(3)] + TimerPh) * 3.2;

			Compteurs[TOR_CPT_TEMPS_ENTRE_N_IMP(3)] = -TimerPh;
		}
	}
}
