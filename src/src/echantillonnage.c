//////////////////////////////////////////////////
// Date de création : 08/02/2010				//
// Auteur			: Faouzi					//
// Description		: Fichier Type 1 ANA		//
//					  							//
//////////////////////////////////////////////////
#include <math.h>
#include "variables.h"
#ifdef UTILISIE_KALMAN
#include "kalman.h"
#endif
#include "echantillonnage.h"

void Echant_Init(EchantParam *pParam)
{
	if(	GetlReg(RNV_ECHANT_TEMPS_INSERTION_FIFO) == 0xFFFFFFFF &&
		GetlReg(RNV_ECHANT_FILTRE_MIN) == 0xFFFFFFFF &&
		GetlReg(RNV_ECHANT_COEFF) == 0xFFFFFFFF
	)
	{
		SetlReg(RNV_ECHANT_TEMPS_INSERTION_FIFO, 1);
		SetfReg(RNV_ECHANT_FILTRE_MIN, 0);
		SetfReg(RNV_ECHANT_FILTRE_MAX, 0);
		SetlReg(RNV_ECHANT_TAILLE_FIFO,  0);
		SetfReg(RNV_ECHANT_COEFF, 1);
		SetfReg(RNV_ECHANT_OFFSET,  0);
		SetfReg(RNV_ECHANT_SEUIL_ECARTYPE, 10);
		SetfReg(RNV_ECHANT_CONSIGNE, 0);
		SetfReg(RNV_ECHANT_TOLERANCE_HAUT, 0);
		SetfReg(RNV_ECHANT_TOLERANCE_BASSE, 0);
		SetfReg(RNV_ECHANT_HYSTERESIS, 0);
		SetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME, 0);
		SetlReg(RNV_ECHANT_MODE_ALARME, 0);
		SetlReg(RNV_ECHANT_TEMPS_MOY_HEURE, 6);
		SetlReg(RNV_ECHANT_NBR_MOY_HEURE, 10);
		SetfReg(RNV_ECHANT_VALEUR_ABERRANTE, -99);
		SetfReg(RNV_ECHANT_KALMAN_Q, 0.01);
		SetfReg(RNV_ECHANT_KALMAN_R, 1);
	}
}

//-------------------------------------------------------------------
// Fonction :	Gestion echantillonnage
// Entrée 	:	structure de type echant
// Sortie 	:	rien
//------------------------------------------------------------------------------
void Echant_Gestion(EchantParam *pParam)
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

	// Conversion de la valeur point
	ValConv = ((GetlReg(pParam->AdrRegPoint) * GetfReg(RNV_ECHANT_COEFF)) + GetfReg(RNV_ECHANT_OFFSET));
	
	// validation de la valeur convertie
	if(	(ValConv >= GetfReg(RNV_ECHANT_FILTRE_MIN) && ValConv <= GetfReg(RNV_ECHANT_FILTRE_MAX)) ||
		(GetfReg(RNV_ECHANT_FILTRE_MIN) == GetfReg(RNV_ECHANT_FILTRE_MAX))
	)
	{
		ValMoy = GetfReg(RV_ECHANT_MOY_ANA);
		// Verife Taille FIFO
		TailleFifo = GetlReg(RNV_ECHANT_TAILLE_FIFO);
		if(TailleFifo > 1)
		{
			// Temps insertion
			if(!GetTimer(TIMER_ECHANT_INSERTION_FIFO))
			{
				
				SetTimer(TIMER_ECHANT_INSERTION_FIFO, GetlReg(RNV_ECHANT_TEMPS_INSERTION_FIFO));// base 100ms
				
				//verife tolerance
				CptInsertion = GetlReg(RV_ECHANT_CPT_INSERTION_FIFO);
				CptInsertion++;
				if(CptInsertion >= TailleFifo || CptInsertion >= pParam->TailleFifoMax)
				{
					CptInsertion = 0;
				}
				pParam->pFifo[CptInsertion] = ValConv;
				SetlReg(RV_ECHANT_CPT_INSERTION_FIFO, CptInsertion);
				
				if(!GetFlag(FV_ECHANT_PREMIER_VALEUR))
				{
					SetFlag(FV_ECHANT_PREMIER_VALEUR, 1);
					SetTimer(TIMER_ECHANT_MAJ_MOYENNE_HEURE, GetlReg(RNV_ECHANT_TEMPS_MOY_HEURE)*600);
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
				SetfReg(RV_ECHANT_MOYENNE_FIFO, ValMoyFifo);

				if(GetfReg(RNV_ECHANT_SEUIL_ECARTYPE))
				{
					Ecartype = 0;
					for(CptFifo = 0; CptFifo < TailleFifo && CptFifo < pParam->TailleFifoMax; CptFifo++)
					{
						Ecartype += pow((pParam->pFifo[CptFifo] - ValMoyFifo), 2);
					}
					Ecartype /= CptFifo;
					Ecartype = sqrt(Ecartype);
					
					SetfReg(RV_ECHANT_ECARTYPE, Ecartype);
					
					if(Ecartype < GetfReg(RNV_ECHANT_SEUIL_ECARTYPE))
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
		if(GetFlag(FNV_ECHANT_ACTIVER_KALMAN))
		{
			KalmanVal = GetfReg(RV_ECHANT_KALMAN_VAL_N);
			KalmanVar = GetfReg(RV_ECHANT_KALMAN_VARIATION_N);
			FiltreKalman(ValMoy, &KalmanVal, &KalmanVar, GetfReg(RNV_ECHANT_KALMAN_Q), GetfReg(RNV_ECHANT_KALMAN_R));
			ValMoy = KalmanVal;
			SetfReg(RV_ECHANT_KALMAN_VAL_N, KalmanVal);
			SetfReg(RV_ECHANT_KALMAN_VARIATION_N, KalmanVar);
		}
		#endif
		
		SetfReg(RV_ECHANT_MOY_ANA, ValMoy);
		// Min Max Moyenne Fifo
		// SI RAZ MIN MAX
		if(GetFlag(FV_ECHANT_DEMANDE_RAZ_MIN_MAX))
		{
			SetFlag(FV_ECHANT_DEMANDE_RAZ_MIN_MAX, 0);
			SetfReg(RV_ECHANT_MIN_MOY_ANA, ValMoy);
			SetfReg(RV_ECHANT_MAX_MOY_ANA, ValMoy);
		}
		else
		{
			if(GetfReg(RV_ECHANT_MIN_MOY_ANA) > ValMoy) SetfReg(RV_ECHANT_MIN_MOY_ANA, ValMoy);
			if(GetfReg(RV_ECHANT_MAX_MOY_ANA) < ValMoy) SetfReg(RV_ECHANT_MAX_MOY_ANA, ValMoy);
		}
		
		// Gestion Alarme
		/////////////////
		// ANA > Consigne
		if(ValMoy > GetfReg(RNV_ECHANT_CONSIGNE))
		{
			if(!GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG)) //Etat et faux
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy < GetfReg(RNV_ECHANT_CONSIGNE) - GetfReg(RNV_ECHANT_HYSTERESIS))
		{
			if(GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}

		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG))
		{
			SetFlag(FV_ECHANT_ALARME_ANA_SUP_CNSG, GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG));
		}

		/////////////////
		// ANA < Consigne
		if(ValMoy < GetfReg(RNV_ECHANT_CONSIGNE))
		{
			if(!GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy > GetfReg(RNV_ECHANT_CONSIGNE) + GetfReg(RNV_ECHANT_HYSTERESIS))
		{
			if(GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG))
		{
			SetFlag(FV_ECHANT_ALARME_ANA_INF_CNSG, GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG));
		}

		/////////////////
		// ANA < Consigne + Tol Haute
		if(ValMoy < GetfReg(RNV_ECHANT_CONSIGNE) + GetfReg(RNV_ECHANT_TOLERANCE_HAUT))
		{
			if(!GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TH))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TH, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TH, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy > GetfReg(RNV_ECHANT_CONSIGNE) + GetfReg(RNV_ECHANT_TOLERANCE_HAUT) + GetfReg(RNV_ECHANT_HYSTERESIS))
		{
			if(GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TH))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TH, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TH, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TH))
		{
			SetFlag(FV_ECHANT_ALARME_ANA_INF_CNSG_TH, GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TH));
		}

		/////////////////
		// ANA > Consigne + Tol Haute
		if(ValMoy > GetfReg(RNV_ECHANT_CONSIGNE) + GetfReg(RNV_ECHANT_TOLERANCE_HAUT))
		{
			if(!GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TH))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TH, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TH, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy < GetfReg(RNV_ECHANT_CONSIGNE) + GetfReg(RNV_ECHANT_TOLERANCE_HAUT) - GetfReg(RNV_ECHANT_HYSTERESIS))
		{
			if(GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TH))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TH, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TH, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TH))
		{
			SetFlag(FV_ECHANT_ALARME_ANA_SUP_CNSG_TH, GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TH));
		}

		/////////////////
		// ANA < Consigne + Tol basse
		if(ValMoy < GetfReg(RNV_ECHANT_CONSIGNE) - GetfReg(RNV_ECHANT_TOLERANCE_BASSE))
		{
			if(!GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TB))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TB, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TB, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy > GetfReg(RNV_ECHANT_CONSIGNE) - GetfReg(RNV_ECHANT_TOLERANCE_BASSE) + GetfReg(RNV_ECHANT_HYSTERESIS))
		{
			if(GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TB))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TB, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TB, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TB))
		{
			SetFlag(FV_ECHANT_ALARME_ANA_INF_CNSG_TB, GetFlag(FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TB));
		}

		/////////////////
		// ANA > Consigne + Tol basse
		if(ValMoy > GetfReg(RNV_ECHANT_CONSIGNE) - GetfReg(RNV_ECHANT_TOLERANCE_BASSE))
		{
			if(!GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TB))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TB, 1);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TB, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		else if(ValMoy < GetfReg(RNV_ECHANT_CONSIGNE) - GetfReg(RNV_ECHANT_TOLERANCE_BASSE) - GetfReg(RNV_ECHANT_HYSTERESIS))
		{
			if(GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TB))
			{
				SetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TB, 0);
				// Chargement du timer filtre
				SetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TB, GetlReg(RNV_ECHANT_TEMPS_FILTRE_ALARME));
			}
		}
		
		// Si le timer est vide alors l'etat du ana par raport au consigne ne pas changer pendant le temps filtre
		if(!GetTimer(TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TB))
		{
			SetFlag(FV_ECHANT_ALARME_ANA_SUP_CNSG_TB, GetFlag(FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TB));
		}
		
		// Alarme Basse Haut, Basse Bas, Basse Bonde et Hor Bonde
		Etat = 0;
		if(GetFlag(FNV_ECHANT_ACTIVER_ALARME))
		{
			switch(GetlReg(RNV_ECHANT_MODE_ALARME))
			{
				case AL_PASSE_BAS :
				{
					Etat = GetFlag(FV_ECHANT_ALARME_ANA_INF_CNSG_TB);
					break;
				}
				case AL_PASSE_HAUT :
				{
					Etat = GetFlag(FV_ECHANT_ALARME_ANA_SUP_CNSG_TH);
					break;
				}
				case AL_PASSE_BANDE :
				{
					Etat = GetFlag(FV_ECHANT_ALARME_ANA_INF_CNSG_TH) && GetFlag(FV_ECHANT_ALARME_ANA_SUP_CNSG_TB); 
					break;
				}
				case AL_HORS_BANDE :
				{
					Etat = GetFlag(FV_ECHANT_ALARME_ANA_SUP_CNSG_TH) || GetFlag(FV_ECHANT_ALARME_ANA_INF_CNSG_TB);
					break;
				}
				default : 
				{
					Etat = 0;
					break;
				}
			}
		}
		
		SetFlag(FV_ECHANT_ALARME, Etat);
			
		// Moyenne Heure
		// Si temps mise a jour moyenne heure est écoulé
		if(!GetTimer(TIMER_ECHANT_MAJ_MOYENNE_HEURE))
		{
			// Recharge temps mise a jour moyenne heure
			SetTimer(TIMER_ECHANT_MAJ_MOYENNE_HEURE, GetlReg(RNV_ECHANT_TEMPS_MOY_HEURE)*600);
			
			// 
			SetfReg(RV_ECHANT_SOMME_MOYENNE_HEURE, GetfReg(RV_ECHANT_SOMME_MOYENNE_HEURE) + ValMoy);
			
			SetlReg(RV_ECHANT_CPT_MOYENNE_HEURE, GetlReg(RV_ECHANT_CPT_MOYENNE_HEURE) + 1);
			if(GetlReg(RV_ECHANT_CPT_MOYENNE_HEURE) > GetlReg(RNV_ECHANT_NBR_MOY_HEURE))
			{
				// Mise a jour moy, Min et Max heure precedente
				SetfReg(RV_ECHANT_MOYENNE_HEURE_PRECEDENTE, GetfReg(RV_ECHANT_MOYENNE_HEURE_ACTUELLE));
				SetfReg(RV_ECHANT_MIN_MOY_HEURE_PRECEDENTE, GetfReg(RV_ECHANT_MIN_MOY_HEURE_ACTUELLE));
				SetfReg(RV_ECHANT_MAX_MOY_HEURE_PRECEDENTE, GetfReg(RV_ECHANT_MAX_MOY_HEURE_ACTUELLE));
				
				// init moy, Min et Max heure actuelle 
				SetfReg(RV_ECHANT_SOMME_MOYENNE_HEURE, ValMoy);
				SetfReg(RV_ECHANT_MIN_MOY_HEURE_ACTUELLE, ValMoy);
				SetfReg(RV_ECHANT_MAX_MOY_HEURE_ACTUELLE, ValMoy);
				SetlReg(RV_ECHANT_CPT_MOYENNE_HEURE, 1);
				
				// indication fin moyenne en cours
				SetFlag(FV_ECHANT_MOYENNE_HEURE_OK, 1);
			}
			
			SetfReg(RV_ECHANT_MOYENNE_HEURE_ACTUELLE, GetfReg(RV_ECHANT_SOMME_MOYENNE_HEURE) / GetlReg(RV_ECHANT_CPT_MOYENNE_HEURE));
			if(GetfReg(RV_ECHANT_MIN_MOY_HEURE_ACTUELLE) > GetfReg(RV_ECHANT_MOYENNE_HEURE_ACTUELLE)) 
			{
				SetfReg(RV_ECHANT_MIN_MOY_HEURE_ACTUELLE, GetfReg(RV_ECHANT_MOYENNE_HEURE_ACTUELLE));
			}
			if(GetfReg(RV_ECHANT_MAX_MOY_HEURE_ACTUELLE) < GetfReg(RV_ECHANT_MOYENNE_HEURE_ACTUELLE))
			{
				SetfReg(RV_ECHANT_MAX_MOY_HEURE_ACTUELLE, GetfReg(RV_ECHANT_MOYENNE_HEURE_ACTUELLE));
			}

		}
	}
	else // si la valeur est érroné
	{
		// Initialisation
		ValConv = GetfReg(RNV_ECHANT_VALEUR_ABERRANTE);
		SetfReg(RV_ECHANT_MOY_ANA, ValConv);
		//Min, Max, Alarme, ...?
		
	}
	SetfReg(RV_ECHANT_DIRECTE, ValConv);

}
