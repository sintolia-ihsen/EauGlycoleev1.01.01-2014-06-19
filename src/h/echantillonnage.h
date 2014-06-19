//////////////////////////////////////////////////
// Date de création : 08/02/2010				//
// Auteur			: Faouzi					//
// Description		: Fichier Type 1 ANA		//
//					  							//
//////////////////////////////////////////////////


#define AL_PASSE_BAS			0
#define AL_PASSE_HAUT			1
#define AL_PASSE_BANDE			2
#define AL_HORS_BANDE			3


typedef struct
{
	int AdrRegPoint;
	int AdrBaseRV;
	int AdrBaseRNV;
	int AdrBaseFV;
	int AdrBaseFNV;
	int AdrBaseT;
	int TailleFifoMax;
	double *pFifo;
}EchantParam;

// Les variable registres volatile
#define RV_ECHANT_DIRECTE						(pParam->AdrBaseRV + 0)
#define RV_ECHANT_MOYENNE_FIFO					(pParam->AdrBaseRV + 1)
#define RV_ECHANT_CPT_INSERTION_FIFO           	(pParam->AdrBaseRV + 2)
#define RV_ECHANT_MOY_ANA                      	(pParam->AdrBaseRV + 3)
#define RV_ECHANT_MIN_MOY_ANA                  	(pParam->AdrBaseRV + 4)
#define RV_ECHANT_MAX_MOY_ANA                  	(pParam->AdrBaseRV + 5)
#define RV_ECHANT_SOMME_MOYENNE_HEURE          	(pParam->AdrBaseRV + 6)
#define RV_ECHANT_CPT_MOYENNE_HEURE            	(pParam->AdrBaseRV + 7)
#define RV_ECHANT_MOYENNE_HEURE_ACTUELLE       	(pParam->AdrBaseRV + 8)
#define RV_ECHANT_MAX_MOY_HEURE_ACTUELLE       	(pParam->AdrBaseRV + 9)
#define RV_ECHANT_MIN_MOY_HEURE_ACTUELLE       	(pParam->AdrBaseRV + 10)
#define RV_ECHANT_MOYENNE_HEURE_PRECEDENTE     	(pParam->AdrBaseRV + 11)
#define RV_ECHANT_MIN_MOY_HEURE_PRECEDENTE     	(pParam->AdrBaseRV + 12)
#define RV_ECHANT_MAX_MOY_HEURE_PRECEDENTE     	(pParam->AdrBaseRV + 13)
#define RV_ECHANT_TENTATIVE_INSR_FIFO          	(pParam->AdrBaseRV + 14)
#define RV_ECHANT_KALMAN_VAL_N          		(pParam->AdrBaseRV + 15)
#define RV_ECHANT_KALMAN_VARIATION_N			(pParam->AdrBaseRV + 16)
#define RV_ECHANT_ECARTYPE						(pParam->AdrBaseRV + 17)

// Les variable registres non volatile
#define RNV_ECHANT_TEMPS_INSERTION_FIFO     	(pParam->AdrBaseRNV + 0)
#define RNV_ECHANT_FILTRE_MIN            	    (pParam->AdrBaseRNV + 1)
#define RNV_ECHANT_FILTRE_MAX	                (pParam->AdrBaseRNV + 2)
#define RNV_ECHANT_TAILLE_FIFO                 	(pParam->AdrBaseRNV + 3)
#define RNV_ECHANT_COEFF                      	(pParam->AdrBaseRNV + 4)
#define RNV_ECHANT_OFFSET                   	(pParam->AdrBaseRNV + 5)
#define RNV_ECHANT_SEUIL_ECARTYPE          	    (pParam->AdrBaseRNV + 6)
#define RNV_ECHANT_CONSIGNE                 	(pParam->AdrBaseRNV + 7)
#define RNV_ECHANT_TOLERANCE_HAUT           	(pParam->AdrBaseRNV + 8)
#define RNV_ECHANT_TOLERANCE_BASSE         	    (pParam->AdrBaseRNV + 9)
#define RNV_ECHANT_HYSTERESIS                   (pParam->AdrBaseRNV + 10)
#define RNV_ECHANT_TEMPS_FILTRE_ALARME          (pParam->AdrBaseRNV + 11)
#define RNV_ECHANT_MODE_ALARME 		        	(pParam->AdrBaseRNV + 12)
#define RNV_ECHANT_TEMPS_MOY_HEURE 	        	(pParam->AdrBaseRNV + 13)
#define RNV_ECHANT_NBR_MOY_HEURE    	        (pParam->AdrBaseRNV + 14)
#define RNV_ECHANT_VALEUR_ABERRANTE             (pParam->AdrBaseRNV + 15)
#define RNV_ECHANT_KALMAN_Q                     (pParam->AdrBaseRNV + 16)
#define RNV_ECHANT_KALMAN_R                     (pParam->AdrBaseRNV + 17)

			
// Les variable flags volatile
#define FV_ECHANT_PREMIER_VALEUR				(pParam->AdrBaseFV + 0)
#define FV_ECHANT_DEMANDE_RAZ_MIN_MAX           (pParam->AdrBaseFV + 1)
#define FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG        (pParam->AdrBaseFV + 2)
#define FV_ECHANT_ALARME_ANA_SUP_CNSG           (pParam->AdrBaseFV + 3)
#define FV_ECHANT_ETAT_REEL_ANA_INF_CNSG        (pParam->AdrBaseFV + 4)
#define FV_ECHANT_ALARME_ANA_INF_CNSG           (pParam->AdrBaseFV + 5)
#define FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TH     (pParam->AdrBaseFV + 6)
#define FV_ECHANT_ALARME_ANA_SUP_CNSG_TH        (pParam->AdrBaseFV + 7)
#define FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TH     (pParam->AdrBaseFV + 8)
#define FV_ECHANT_ALARME_ANA_INF_CNSG_TH        (pParam->AdrBaseFV + 9)
#define FV_ECHANT_ETAT_REEL_ANA_INF_CNSG_TB     (pParam->AdrBaseFV + 10)
#define FV_ECHANT_ALARME_ANA_INF_CNSG_TB        (pParam->AdrBaseFV + 11)
#define FV_ECHANT_ETAT_REEL_ANA_SUP_CNSG_TB     (pParam->AdrBaseFV + 12)
#define FV_ECHANT_ALARME_ANA_SUP_CNSG_TB        (pParam->AdrBaseFV + 13)
#define FV_ECHANT_ALARME            			(pParam->AdrBaseFV + 14)
#define FV_ECHANT_MOYENNE_HEURE_OK              (pParam->AdrBaseFV + 15)

// Les variable flags non volatile
#define FNV_ECHANT_ACTIVER_KALMAN	            (pParam->AdrBaseFNV + 0)
#define FNV_ECHANT_ACTIVER_ALARME				(pParam->AdrBaseFNV + 1)

// Les variable timers
#define TIMER_ECHANT_INSERTION_FIFO				(pParam->AdrBaseT + 0)
#define TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG     (pParam->AdrBaseT + 1)
#define TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG     (pParam->AdrBaseT + 2)
#define TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TH  (pParam->AdrBaseT + 3)
#define TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TH  (pParam->AdrBaseT + 4)
#define TIMER_ECHANT_TEMPS_FILTRE_ANA_SUP_CNSG_TB  (pParam->AdrBaseT + 5)
#define TIMER_ECHANT_TEMPS_FILTRE_ANA_INF_CNSG_TB  (pParam->AdrBaseT + 6)
#define TIMER_ECHANT_MAJ_MOYENNE_HEURE				(pParam->AdrBaseT + 7)

void Echant_Init(EchantParam *pParam);
void Echant_Gestion(EchantParam *pParam);
