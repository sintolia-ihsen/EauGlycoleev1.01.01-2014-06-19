#include "main.h"

#define true								1
#define false								0

#define MAE_CYCLE_INIT						0		// Cycle Initial


#define TOR_S_POMPE_1						TOR_SORTIE07
#define TOR_S_POMPE(Num)					(TOR_S_POMPE_1 + Num)
#define TOR_S_CONTACT_MOTEUR				TOR_SORTIE01
#define TOR_S_ALTERNATEUR					TOR_SORTIE02
#define TOR_S_PRECHAUFFAGE					TOR_SORTIE03
#define TOR_S_POMPE_1_RELEVAGE				TOR_SORTIE02
#define TOR_S_POMPE_2_RELEVAGE				TOR_SORTIE03
#define TOR_S_DEMARRAGE_MOTEUR				TOR_SORTIE04
//#define TOR_S_ACCELERATION_MOTEUR			TOR_SORTIE05
//#define TOR_S_DECELERATION_MOTEUR			TOR_SORTIE06
#define TOR_S_MODE							TOR_SORTIE10
#define TOR_VANNE_S1						TOR_SORTIE01
#define TOR_VANNE_S2						TOR_SORTIE02
#define TOR_VANNE_S3						TOR_SORTIE03
#define TOR_ACK_VARIATEUR_2					TOR_SORTIE08
#define TOR_MARCHE_VARIATEUR_2				TOR_SORTIE09


#define NBR_TOUR_DEMARRAGE					200
#define FILTRE_REGIME_MOTEUR				15		// 15 secondes
//#define REGIME_MOTEUR_MARCHE				400
#define REGIME_MOTEUR_MARCHE				15
#define FILTRE_DEFAUT_MOTEUR				2		// 2 secondes
#define LIMITE_REGIME_BAS					5
#define LIMITE_REGIME_HAUT					3000

#define TYPE_DMR_AUTRE						0
#define TYPE_DMR_STATORIQUE					1

#define MODE_AUTO							0
#define MODE_ARRET							1
#define MODE_MANUEL							2

#define ENTREE_MODE_SANS_ARRET				0
#define ENTREE_MODE_ARRET_NORMAL			1
#define ENTREE_MODE_ARRET_BRUTAL			2

#define FILTRE_HP							2		// 2 secondes
#define FILTRE_PRESS_MIN_AMONT				2		// 2 secondes
#define MAX_POMPE							3
#define PWM_DECELERATION					0
#define PWM_ACCELERATION					4000
#define PWM_PAS_DECELERATION				4
#define PWM_PAS_ACCELERATION				1

#define SENS_AUCUN							0
#define SENS_ACC							1
#define SENS_DECEL							2

#define ACC_TOR								0
#define ACC_SORTIE_ANA						1

#define DEBIT_ANALOGIQUE					0
#define DEBIT_IMPULSION						1

#define REGULATION_FREQUENCE				0
#define REGULATION_DEBIT					1

#define MOTEUR_DIESEL						0
#define MOTEUR_ELEC							1
#define DEMARREUR_PROGRESSIF				2
#define VANNE								3

#define VARIATEUR2_POMPE_1					0
#define VARIATEUR2_50_50					1
#define VARIATEUR2_PERMUTATION				2
#define VARIATEUR2_MTR_SLV					3
#define VARIATEUR2_POMPE_2					4

#define INDEX_VARIATEUR_1					0
#define INDEX_VARIATEUR_2					1


#define DMR_AUX_AUCUN						0		// Aucun Démarrage Pompe Aux
#define DMR_AUX_DEFAUT						1		// Démarrage Pompe Aux suite à un defaut pompe
#define DMR_AUX_CYCLE1						2		// Démarrage Pompe Aux suite à CYCLE1
#define DMR_AUX_CYCLE2						3		// Démarrage Pompe Aux suite à CYCLE2
#define DMR_AUX_CYCLE3						4		// Démarrage Pompe Aux suite à CYCLE3
#define DMR_AUX_OBJECTIF_NON_ATTEINT		5		// Démarrage Pompe Aux suite à Objectif non atteint
#define DMR_AUX_DELTA_DEMARRAGE				6		// Démarrage Pompe Aux suite à un delta demarrage
#define DMR_AUX_ECHEC_DEMARRAGE				7		// Démarrage Pompe Aux suite à un echec démarrage

#define ARRET_AUX_AUCUN						0		// Aucun Arrêt Pompe Aux
#define ARRET_AUX_TOL_HAUT_FILTREE			1		// Arrêt Pompe Aux suite à une tolérance haute filtrée
#define ARRET_AUX_TOL_HAUT_NON_FILTREE		2		// Arrêt Pompe Aux suite à une tolérance haute non filtrée
#define ARRET_AUX_DELTA_ARRET				3		// Arrêt Pompe Aux suite à un delta arrêt
#define ARRET_AUX_SEUIL_DEBIT				4		// Arrêt Pompe Aux suite à seuil débit
#define ARRET_AUX_SEUIL_MIN_FERM_VANNE		5		// Arrêt Pompe Aux suite à seuil min fermeture vanne
#define ARRET_AUX_ABSENCE_FREQ_VARIATEUR_2	6		// Arrêt Pompe Aux suite à abscence freq variateur 2

#define DMR_AUCUN							0		// Aucun Démarrage
#define DMR_IMMEDIAT						1		// Démarrage Immédiat
#define DMR_DIFFERE							2		// Démarrage Différé
#define DMR_PRESS_MIN						3		// Démarrage Pression Min Réseau
#define DMR_MANUEL							4		// Démarrage Manuel
#define DMR_NON_ACK							5		// Défaut Non Acquité

#define ARRET_AUCUN							0		// Aucun Arrêt
#define ARRET_IMMEDIAT						1		// Arrêt Immédiat
#define ARRET_DIFFERE						2		// Arrêt Différé
#define ARRET_TOL_HAUT						3		// Arrêt Tolérance Haute
#define ARRET_TOL_BASSE						4		// Arrêt Tolérance Basse
#define ARRET_ABSCENCE_REGIME				5		// Arrêt Abscence Régime
#define ARRET_REGIME_BASSE					6		// Arrêt Régime Basse
#define ARRET_DEFAUT_HP						7		// Arrêt Défaut HP
#define ARRET_DEFAUT_PRESS_AMONT			8		// Arrêt Défaut Pression Amont
#define ARRET_DEFAUT_HUILE					9		// Arrêt Défaut Huile
#define ARRET_DEFAUT_E5						10		// Arrêt Défaut E5
#define ARRET_BOUTON_ARRET_MANUEL			11		// Arrêt Bouton Arrêt Manuel
#define ARRET_BOUTON_ARRET_AUTO				12		// Arrêt Bouton Arrêt Auto
#define ARRET_DEBIT_MINI					13		// Arrêt Débit Mini
#define ARRET_DEFAUT_TEMPERATURE			14		// Arrêt Défaut Température
#define ARRET_ECHEC_DMR						15		// Arrêt Echec Démarrage
#define ARRET_DEBIT_SECURITE				16		// Arrêt Débit Sécurité
#define ARRET_DELTA_PRESSION				17		// Arrêt Delta Pression
#define ARRET_DEFAUT_SEUIL_TEMPERATURE		18		// Arrêt Température
#define ARRET_ENTREE_MODE_NORMAL			19		// Arrêt Entrée Mode Normal
#define ARRET_ENTREE_MODE_BRUTAL			20		// Arrêt Entrée Mode Brutal
#define ARRET_VANNE_FERMEE					21		// Arrêt Vanne Fermée
#define ARRET_DEFAUT_VARIATEUR_2			22		// Arrêt Defaut Variateur 2
#define ARRET_ABSCENCE_FREQ_VARIATEUR_2		23		// Arrêt Abscence Freq Variateur 2

// Définitions des Entées
#define ENTREE_TEMP_MOTEUR					GetFlag(FV_E03)							// Défaut température 1
#define ENTREE_MANQUE_HUILE					GetFlag(FV_E04)							// Défaut huile 2
#define ENTREE_E04_SEUL_BAS					GetFlag(FV_E04)							// Seuil Bas
#define ENTREE_E04_VANNE_FERMEE				GetFlag(FV_E04)							// Vanne Fermée
#define ENTREE_E08_VANNE_OUVERTE			GetFlag(FV_E08)							// Vanne Ouverte
#define ENTREE_DEFAUT_E5					GetFlag(FV_E05)							// Pressosta
#define ENTREE_DEFAUT_POMPE(Num)			GetFlag(FV_E06 + Num)
#define ENTREE_DEFAUT_VARIATEUR_2			GetFlag(FV_E08)							// Défaut Variateur 2
#define ENTREE_MODE_ARRET					(!GetFlag(FV_E09) && !GetFlag(FV_E10))
#define ENTREE_MODE_AUTO					(GetFlag(FV_E09) && !GetFlag(FV_E10))
#define ENTREE_MODE_MANUEL					(!GetFlag(FV_E09) && GetFlag(FV_E10))
#define ENTREE_DEMARRAGE_CYCLE				GetFlag(FV_E11)							// Démarrage Moteur en mode mannuel
#define ENTREE_MODE							GetFlag(FV_E12)

#define VAL_PRESSION_NON_FILTREE_1			GetfReg(RV_ANA_DIRECTE_A1)				// Pression Nom Filtrée
#define VAL_PRESSION_FILTREE_1				GetfReg(RV_MOY_ANA_A1)					// Pression Filtrée
#define VAL_PRESSION_FILTREE_2				GetfReg(RV_MOY_ANA_A2)					// Pression
#define VAL_FREQ_VARIATEUR					GetfReg(RV_MOY_ANA_A3)					// Freq variateur
#define VAL_REGIME_MOTEUR					GetfReg(RV_MOY_E1)						// Régime Moteur
#define VAL_DEBIT_IMPULSION					GetfReg(RV_MOY_E2)						// Débit Impulsion
#define VAL_DEBIT_ANALOGIQUE				GetfReg(RV_MOY_ANA_A4)					// Débit Analogique
#define VAL_REGIME_MOTEUR_SIMULE			GetfReg(RV_REGIME_SIMULE_IRRIG)			// Régime Moteur Simule
#define VAL_REGIME_REEL						1500									// Regime réel
#define VAL_TEMPERATURE						GetfReg(RV_MOY_ANA_AN01)				// Temperature
#define VAL_FREQ_VARIATEUR2					GetfReg(RV_MOY_ANA_A5)					// Freq variateur 2

// Flags Volatiles Irrigation
#define FV_DEMARRE_IMMEDIAT_CYCLE_IRRIG		600



// Flags Non Volatiles Irrigation
#define FV_ACTIVE_DMR_DIFFERE_IRRIG			640


// Flags Non Volatiles Irrigation
#define FNV_ACTIVE_EGL						30300


// Registres Volatiles Irrigation
#define RV_PRSS_FILTREE_IRRIG				600


// Registres Non Volatiles Irrigation
#define RNV_HEURE_DEMARRE_CYCLE_IRRIG		30000


#define RNV_VAL_HEURE_DEMARRE				30200


// Timers Irrigation
#define TMR_TEMPS_MINI_DMR_POMPE_AUX		400


typedef struct
{
	unsigned gbInitConsigne:1;

}MCEGLBit;


// Prototypes des fonctions Irrigation
void InitEauGlycolee(void);
void GestionEauGlycolee(void);
