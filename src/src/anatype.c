//////////////////////////////////////////////////
// Date de création : 08/02/2010				//
// Auteur			: Faouzi					//
// Description		: Fichier Type 1 ANA		//
//					  							//
//////////////////////////////////////////////////
#include <math.h>
#include "variables.h"
#include "anatype.h"
#ifdef UTILISIE_KALMAN
#include "kalman.h"
#endif

const short can_AnaPoint[]={82,84,87,90,93,95,98,101,104,107,110,114,118,122,126,130,134,138,142,146,150,154,159,164,169,174,179,184,189,194,199,205,212,219,226,233,240,247,254,261,268,276,284,292,300,308,317,326,335,344,353,362,371,381,391,401,411,421,431,442,453,464,475,486,497,508,519,530,541,553,565,577,589,600,611,622,633,644,655,666,677,688,699,710,720,730,740,750,760,770,779,788,797,806,815,824,832,840,848,856,863,870,877,884,890,896,902,908,914,919,924,929,934,938,942,946,950,954,958,962,965,};

void AnaType1_Init(AnaType1Param *pParam)
{
	if(	GetlReg(RNV_ANA_TEMPS_INSERTION_FIFO) == 0xFFFFFFFF &&
	GetlReg(RNV_ANA_FILTRE_MIN) == 0xFFFFFFFF &&
	GetlReg(RNV_ANA_COEFF) == 0xFFFFFFFF
	)
	{
		SetlReg(RNV_ANA_TEMPS_INSERTION_FIFO, 1);
		SetfReg(RNV_ANA_FILTRE_MIN, 0);
		SetfReg(RNV_ANA_FILTRE_MAX, 0);
		SetlReg(RNV_ANA_TAILLE_FIFO,  10);
		SetfReg(RNV_ANA_COEFF, 0.09);
		SetfReg(RNV_ANA_OFFSET,  0);
		SetfReg(RNV_ANA_SEUIL_ECARTYPE, 0);
		SetfReg(RNV_ANA_CONSIGNE, 0);
		SetfReg(RNV_ANA_TOLERANCE_HAUT, 0);
		SetfReg(RNV_ANA_TOLERANCE_BASSE, 0);
		SetfReg(RNV_ANA_HYSTERESIS, 0);
		SetlReg(RNV_ANA_TEMPS_FILTRE_ALARME, 0);
		SetlReg(RNV_ANA_MODE_ALARME, 0);
		SetlReg(RNV_ANA_TEMPS_MOY_HEURE, 6);
		SetlReg(RNV_ANA_NBR_MOY_HEURE, 10);
		SetfReg(RNV_ANA_VALEUR_ABERRANTE, -99);
		SetfReg(RNV_ANA_KALMAN_Q, 0.01);
		SetfReg(RNV_ANA_KALMAN_R, 1);
		SetFlag(FNV_ANA_CAPTEUR_LINEAIRE, 0);
		SetFlag(FNV_ANA_ACTIVER_KALMAN, 0);
	}
}


double AnaType1_PointToTemps(int PointAna, int Offset)
{
	int cpt;

	double PointN; 
	double PointNPlus1;
	double Temps;
	
	PointAna -= Offset;
	
	if(PointAna <= can_AnaPoint[0])
	{
		Temps = 90;
	}
	else if(PointAna >= can_AnaPoint[120])
	{
		Temps = -30;
	}
	else
	{
		for(cpt = 0; cpt < 120; cpt++)
		{
			PointN = can_AnaPoint[cpt];
			PointNPlus1 = can_AnaPoint[cpt+1];
			
			if((PointN <= PointAna) && (PointAna < PointNPlus1))
			{				
				Temps = (90. - cpt) - ((PointAna - PointN)/(PointNPlus1 - PointN));
				break;
			}
		}
	}
	
	return Temps;
}

//-------------------------------------------------------------------
// Fonction :	Gestion Ana type 1
// Entrée 	:	Adresse Reg Point, ....
// Sortie 	:	rien
//------------------------------------------------------------------------------
void AnaType1_Gestion(AnaType1Param *pParam)
{
	int CptFifo;
	int CptInsertion;
	int Etat;
	double ValConv;
	double ValMoyFifo;
	double ValMoy;
	double Ecartype;
	double KalmanVal;
	double KalmanVar;
	int TailleFifo;
	
	if(GetFlag(FNV_ANA_CAPTEUR_LINEAIRE))
	{
		// Conversion de la valeur point
		ValConv = ((GetlReg(pParam->AdrRegPoint) * GetfReg(RNV_ANA_COEFF)) + GetfReg(RNV_ANA_OFFSET));
	}
	else
	{
		ValConv = AnaType1_PointToTemps(GetlReg(pParam->AdrRegPoint), GetfReg(RNV_ANA_OFFSET));
	}
	// validation de la valeur convertie
	if(	(ValConv >= GetfReg(RNV_ANA_FILTRE_MIN) && ValConv <= GetfReg(RNV_ANA_FILTRE_MAX)) ||
		(GetfReg(RNV_ANA_FILTRE_MIN) == GetfReg(RNV_ANA_FILTRE_MAX))
	)
	{
		ValMoy = GetfReg(RV_ANA_MOY_ANA);
		
		// Verife Taille FIFO
		TailleFifo = GetlReg(RNV_ANA_TAILLE_FIFO);
		if(TailleFifo > 1)
		{
			// Temps insertion
			if(!GetTimer(TIMER_ANA_INSERTION_FIFO))
			{
				
				SetTimer(TIMER_ANA_INSERTION_FIFO, GetlReg(RNV_ANA_TEMPS_INSERTION_FIFO));// base 100ms
				
				//verife tolerance
				CptInsertion = GetlReg(RV_ANA_CPT_INSERTION_FIFO);
				CptInsertion++;
				if(CptInsertion >= TailleFifo || CptInsertion >= pParam->TailleFifoMax)
				{
					CptInsertion = 0;
				}
				
				pParam->pFifo[CptInsertion] = ValConv;
				SetlReg(RV_ANA_CPT_INSERTION_FIFO, CptInsertion);
				
				if(!GetFlag(FV_ANA_PREMIER_VALEUR))
				{
					SetFlag(FV_ANA_PREMIER_VALEUR, 1);
					SetTimer(TIMER_ANA_MAJ_MOYENNE_HEURE, GetlReg(RNV_ANA_TEMPS_MOY_HEURE)*600);
					for(CptFifo = 0; CptFifo < TailleFifo && CptFifo < pParam->TailleFifoMax; CptFifo++)
					{
						pParam->pFifo[CptFifo] = ValConv;
					}
				}
			
				//Calcule moyenne FIFO
				ValMoyFifo = 0;
				for(CptFifo = 0; CptFifo < TailleFifo && CptFifo < pParam->TailleFifoMax; CptFifo++)
				{
					ValMoyFifo += pParam->pFifo[CptFifo];
				}

				ValMoyFifo /= CptFifo;
				SetfReg(RV_ANA_MOYENNE_FIFO, ValMoyFifo);

				if(GetfReg(RNV_ANA_SEUIL_ECARTYPE))
				{
					Ecartype = 0;
					for(CptFifo = 0; CptFifo < TailleFifo && CptFifo < pParam->TailleFifoMax; CptFifo++)
					{
						Ecartype += pow((pParam->pFifo[CptFifo] - ValMoyFifo), 2);
					}
					Ecartype /= CptFifo;
					Ecartype = sqrt(Ecartype);
					
					SetfReg(RV_ANA_ECARTYPE, Ecartype);
					
					if(Ecartype < GetfReg(RNV_ANA_SEUIL_ECARTYPE))
					{
						ValMoy = ValMoyFifo;
					}
				}
				else
				{
					ValMoy = ValMoyFifo;
				}
			}
		}
		else
		{
			ValMoy = ValConv;
		}
		
		#ifdef UTILISIE_KALMAN
		//Filtre Kalman
		if(GetFlag(FNV_ANA_ACTIVER_KALMAN))
		{
			KalmanVal = GetfReg(RV_ANA_KALMAN_VAL_N);
			KalmanVar = GetfReg(RV_ANA_KALMAN_VARIATION_N);
			FiltreKalman(ValMoy, &KalmanVal, &KalmanVar, GetfReg(RNV_ANA_KALMAN_Q), GetfReg(RNV_ANA_KALMAN_R));
			ValMoy = KalmanVal;
			SetfReg(RV_ANA_KALMAN_VAL_N, KalmanVal);
			SetfReg(RV_ANA_KALMAN_VARIATION_N, KalmanVar);
		}
		#endif
		
		SetfReg(RV_ANA_MOY_ANA, ValMoy);
		// Min Max Moyenne Fifo
		// SI RAZ MIN MAX
		if(GetFlag(FV_ANA_DEMANDE_RAZ_MIN_MAX))
		{
			SetFlag(FV_ANA_DEMANDE_RAZ_MIN_MAX, 0);
			SetfReg(RV_ANA_MIN_MOY_ANA, ValMoy);
			SetfReg(RV_ANA_MAX_MOY_ANA, ValMoy);
		}
		else
		{
			if(GetfReg(RV_ANA_MIN_MOY_ANA) > ValMoy) SetfReg(RV_ANA_MIN_MOY_ANA, ValMoy);
			if(GetfReg(RV_ANA_MAX_MOY_ANA) < ValMoy) SetfReg(RV_ANA_MAX_MOY_ANA, ValMoy);
		}
		
		// Gestion Alarme
		/////////////////
		// ANA > Consigne
		if(ValMoy > GetfReg(RNV_ANA_CONSIGNE))
		{
			if(!GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG)) //Etat et faux
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy < GetfReg(RNV_ANA_CONSIGNE) - GetfReg(RNV_ANA_HYSTERESIS))
		{
			if(GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}

		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG))
		{
			SetFlag(FV_ANA_ALARME_ANA_SUP_CNSG, GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG));
		}

		/////////////////
		// ANA < Consigne
		if(ValMoy < GetfReg(RNV_ANA_CONSIGNE))
		{
			if(!GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy > GetfReg(RNV_ANA_CONSIGNE) + GetfReg(RNV_ANA_HYSTERESIS))
		{
			if(GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG))
		{
			SetFlag(FV_ANA_ALARME_ANA_INF_CNSG, GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG));
		}

		/////////////////
		// ANA < Consigne + Tol Haute
		if(ValMoy < GetfReg(RNV_ANA_CONSIGNE) + GetfReg(RNV_ANA_TOLERANCE_HAUT))
		{
			if(!GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TH))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TH, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG_TH, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy > GetfReg(RNV_ANA_CONSIGNE) + GetfReg(RNV_ANA_TOLERANCE_HAUT) + GetfReg(RNV_ANA_HYSTERESIS))
		{
			if(GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TH))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TH, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG_TH, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG_TH))
		{
			SetFlag(FV_ANA_ALARME_ANA_INF_CNSG_TH, GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TH));
		}

		/////////////////
		// ANA > Consigne + Tol Haute
		if(ValMoy > GetfReg(RNV_ANA_CONSIGNE) + GetfReg(RNV_ANA_TOLERANCE_HAUT))
		{
			if(!GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TH))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TH, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG_TH, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy < GetfReg(RNV_ANA_CONSIGNE) + GetfReg(RNV_ANA_TOLERANCE_HAUT) - GetfReg(RNV_ANA_HYSTERESIS))
		{
			if(GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TH))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TH, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG_TH, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG_TH))
		{
			SetFlag(FV_ANA_ALARME_ANA_SUP_CNSG_TH, GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TH));
		}

		/////////////////
		// ANA < Consigne + Tol basse
		if(ValMoy < GetfReg(RNV_ANA_CONSIGNE) - GetfReg(RNV_ANA_TOLERANCE_BASSE))
		{
			if(!GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TB))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TB, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG_TB, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy > GetfReg(RNV_ANA_CONSIGNE) - GetfReg(RNV_ANA_TOLERANCE_BASSE) + GetfReg(RNV_ANA_HYSTERESIS))
		{
			if(GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TB))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TB, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG_TB, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_INF_CNSG_TB))
		{
			SetFlag(FV_ANA_ALARME_ANA_INF_CNSG_TB, GetFlag(FV_ANA_ETAT_REEL_ANA_INF_CNSG_TB));
		}

		/////////////////
		// ANA > Consigne + Tol basse
		if(ValMoy > GetfReg(RNV_ANA_CONSIGNE) - GetfReg(RNV_ANA_TOLERANCE_BASSE))
		{
			if(!GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TB))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TB, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG_TB, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy < GetfReg(RNV_ANA_CONSIGNE) - GetfReg(RNV_ANA_TOLERANCE_BASSE) - GetfReg(RNV_ANA_HYSTERESIS))
		{
			if(GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TB))
			{
				SetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TB, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG_TB, GetlReg(RNV_ANA_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ANA_TEMPS_FILTRE_ANA_SUP_CNSG_TB))
		{
			SetFlag(FV_ANA_ALARME_ANA_SUP_CNSG_TB, GetFlag(FV_ANA_ETAT_REEL_ANA_SUP_CNSG_TB));
		}
		
		// Alarme Basse Haut, Basse Bas, Basse Bonde et Hor Bonde
		Etat = 0;
		if(GetFlag(FNV_ANA_ACTIVER_ALARME))
		{
			switch(GetlReg(RNV_ANA_MODE_ALARME))
			{
				case AL_PASSE_BAS :
				{
					Etat = GetFlag(FV_ANA_ALARME_ANA_SUP_CNSG_TH);
					break;
				}
				case AL_PASSE_HAUT :
				{
					Etat = GetFlag(FV_ANA_ALARME_ANA_INF_CNSG_TB);
					break;
				}
				case AL_PASSE_BANDE :
				{
					Etat = GetFlag(FV_ANA_ALARME_ANA_INF_CNSG_TH) && GetFlag(FV_ANA_ALARME_ANA_SUP_CNSG_TB); 
					break;
				}
				case AL_HORS_BANDE :
				{
					Etat = GetFlag(FV_ANA_ALARME_ANA_SUP_CNSG_TH) || GetFlag(FV_ANA_ALARME_ANA_INF_CNSG_TB);
					break;
				}
				default : 
				{
					Etat = 0;
					break;
				}
			}
		}
		
		SetFlag(FV_ANA_ALARME, Etat);
			
		// Moyenne Heure
		// Si temps mise a jour moyenne heure est écoulé
		if(!GetTimer(TIMER_ANA_MAJ_MOYENNE_HEURE))
		{
			// Recharge temps mise a jour moyenne heure
			SetTimer(TIMER_ANA_MAJ_MOYENNE_HEURE, GetlReg(RNV_ANA_TEMPS_MOY_HEURE)*600);
			
			// 
			SetfReg(RV_ANA_SOMME_MOYENNE_HEURE, GetfReg(RV_ANA_SOMME_MOYENNE_HEURE) + ValMoy);
			
			SetlReg(RV_ANA_CPT_MOYENNE_HEURE, GetlReg(RV_ANA_CPT_MOYENNE_HEURE) + 1);
			if(GetlReg(RV_ANA_CPT_MOYENNE_HEURE) > GetlReg(RNV_ANA_NBR_MOY_HEURE))
			{
				// Mise a jour moy, Min et Max heure precedente
				SetfReg(RV_ANA_MOYENNE_HEURE_PRECEDENTE, GetfReg(RV_ANA_MOYENNE_HEURE_ACTUELLE));
				SetfReg(RV_ANA_MIN_MOY_HEURE_PRECEDENTE, GetfReg(RV_ANA_MIN_MOY_HEURE_ACTUELLE));
				SetfReg(RV_ANA_MAX_MOY_HEURE_PRECEDENTE, GetfReg(RV_ANA_MAX_MOY_HEURE_ACTUELLE));
				
				// init moy, Min et Max heure actuelle 
				SetfReg(RV_ANA_SOMME_MOYENNE_HEURE, ValMoy);
				SetfReg(RV_ANA_MIN_MOY_HEURE_ACTUELLE, ValMoy);
				SetfReg(RV_ANA_MAX_MOY_HEURE_ACTUELLE, ValMoy);
				SetlReg(RV_ANA_CPT_MOYENNE_HEURE, 1);
				
				// indication fin moyenne en cours
				SetFlag(FV_ANA_MOYENNE_HEURE_OK, 1);
			}
			
			SetfReg(RV_ANA_MOYENNE_HEURE_ACTUELLE, GetfReg(RV_ANA_SOMME_MOYENNE_HEURE) / GetlReg(RV_ANA_CPT_MOYENNE_HEURE));
			if(GetfReg(RV_ANA_MIN_MOY_HEURE_ACTUELLE) > GetfReg(RV_ANA_MOYENNE_HEURE_ACTUELLE)) 
			{
				SetfReg(RV_ANA_MIN_MOY_HEURE_ACTUELLE, GetfReg(RV_ANA_MOYENNE_HEURE_ACTUELLE));
			}
			if(GetfReg(RV_ANA_MAX_MOY_HEURE_ACTUELLE) < GetfReg(RV_ANA_MOYENNE_HEURE_ACTUELLE))
			{
				SetfReg(RV_ANA_MAX_MOY_HEURE_ACTUELLE, GetfReg(RV_ANA_MOYENNE_HEURE_ACTUELLE));
			}

		}
	}
	else // si la valeur est érroné
	{
		// Initialisation
		ValConv = GetfReg(RNV_ANA_VALEUR_ABERRANTE);
		SetfReg(RV_ANA_MOY_ANA, ValConv);
		//Min, Max, Alarme, ...?
		
	}
	SetfReg(RV_ANA_DIRECTE, ValConv);

}
