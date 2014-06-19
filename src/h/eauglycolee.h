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


#define DMR_AUX_AUCUN						0		// Aucun D�marrage Pompe Aux
#define DMR_AUX_DEFAUT						1		// D�marrage Pompe Aux suite � un defaut pompe
#define DMR_AUX_CYCLE1						2		// D�marrage Pompe Aux suite � CYCLE1
#define DMR_AUX_CYCLE2						3		// D�marrage Pompe Aux suite � CYCLE2
#define DMR_AUX_CYCLE3						4		// D�marrage Pompe Aux suite � CYCLE3
#define DMR_AUX_OBJECTIF_NON_ATTEINT		5		// D�marrage Pompe Aux suite � Objectif non atteint
#define DMR_AUX_DELTA_DEMARRAGE				6		// D�marrage Pompe Aux suite � un delta demarrage
#define DMR_AUX_ECHEC_DEMARRAGE				7		// D�marrage Pompe Aux suite � un echec d�marrage

#define ARRET_AUX_AUCUN						0		// Aucun Arr�t Pompe Aux
#define ARRET_AUX_TOL_HAUT_FILTREE			1		// Arr�t Pompe Aux suite � une tol�rance haute filtr�e
#define ARRET_AUX_TOL_HAUT_NON_FILTREE		2		// Arr�t Pompe Aux suite � une tol�rance haute non filtr�e
#define ARRET_AUX_DELTA_ARRET				3		// Arr�t Pompe Aux suite � un delta arr�t
#define ARRET_AUX_SEUIL_DEBIT				4		// Arr�t Pompe Aux suite � seuil d�bit
#define ARRET_AUX_SEUIL_MIN_FERM_VANNE		5		// Arr�t Pompe Aux suite � seuil min fermeture vanne
#define ARRET_AUX_ABSENCE_FREQ_VARIATEUR_2	6		// Arr�t Pompe Aux suite � abscence freq variateur 2

#define DMR_AUCUN							0		// Aucun D�marrage
#define DMR_IMMEDIAT						1		// D�marrage Imm�diat
#define DMR_DIFFERE							2		// D�marrage Diff�r�
#define DMR_PRESS_MIN						3		// D�marrage Pression Min R�seau
#define DMR_MANUEL							4		// D�marrage Manuel
#define DMR_NON_ACK							5		// D�faut Non Acquit�

#define ARRET_AUCUN							0		// Aucun Arr�t
#define ARRET_IMMEDIAT						1		// Arr�t Imm�diat
#define ARRET_DIFFERE						2		// Arr�t Diff�r�
#define ARRET_TOL_HAUT						3		// Arr�t Tol�rance Haute
#define ARRET_TOL_BASSE						4		// Arr�t Tol�rance Basse
#define ARRET_ABSCENCE_REGIME				5		// Arr�t Abscence R�gime
#define ARRET_REGIME_BASSE					6		// Arr�t R�gime Basse
#define ARRET_DEFAUT_HP						7		// Arr�t D�faut HP
#define ARRET_DEFAUT_PRESS_AMONT			8		// Arr�t D�faut Pression Amont
#define ARRET_DEFAUT_HUILE					9		// Arr�t D�faut Huile
#define ARRET_DEFAUT_E5						10		// Arr�t D�faut E5
#define ARRET_BOUTON_ARRET_MANUEL			11		// Arr�t Bouton Arr�t Manuel
#define ARRET_BOUTON_ARRET_AUTO				12		// Arr�t Bouton Arr�t Auto
#define ARRET_DEBIT_MINI					13		// Arr�t D�bit Mini
#define ARRET_DEFAUT_TEMPERATURE			14		// Arr�t D�faut Temp�rature
#define ARRET_ECHEC_DMR						15		// Arr�t Echec D�marrage
#define ARRET_DEBIT_SECURITE				16		// Arr�t D�bit S�curit�
#define ARRET_DELTA_PRESSION				17		// Arr�t Delta Pression
#define ARRET_DEFAUT_SEUIL_TEMPERATURE		18		// Arr�t Temp�rature
#define ARRET_ENTREE_MODE_NORMAL			19		// Arr�t Entr�e Mode Normal
#define ARRET_ENTREE_MODE_BRUTAL			20		// Arr�t Entr�e Mode Brutal
#define ARRET_VANNE_FERMEE					21		// Arr�t Vanne Ferm�e
#define ARRET_DEFAUT_VARIATEUR_2			22		// Arr�t Defaut Variateur 2
#define ARRET_ABSCENCE_FREQ_VARIATEUR_2		23		// Arr�t Abscence Freq Variateur 2

// D�finitions des Ent�es
#define ENTREE_TEMP_MOTEUR					GetFlag(FV_E03)							// D�faut temp�rature 1
#define ENTREE_MANQUE_HUILE					GetFlag(FV_E04)							// D�faut huile 2
#define ENTREE_E04_SEUL_BAS					GetFlag(FV_E04)							// Seuil Bas
#define ENTREE_E04_VANNE_FERMEE				GetFlag(FV_E04)							// Vanne Ferm�e
#define ENTREE_E08_VANNE_OUVERTE			GetFlag(FV_E08)							// Vanne Ouverte
#define ENTREE_DEFAUT_E5					GetFlag(FV_E05)							// Pressosta
#define ENTREE_DEFAUT_POMPE(Num)			GetFlag(FV_E06 + Num)
#define ENTREE_DEFAUT_VARIATEUR_2			GetFlag(FV_E08)							// D�faut Variateur 2
#define ENTREE_MODE_ARRET					(!GetFlag(FV_E09) && !GetFlag(FV_E10))
#define ENTREE_MODE_AUTO					(GetFlag(FV_E09) && !GetFlag(FV_E10))
#define ENTREE_MODE_MANUEL					(!GetFlag(FV_E09) && GetFlag(FV_E10))
#define ENTREE_DEMARRAGE_CYCLE				GetFlag(FV_E11)							// D�marrage Moteur en mode mannuel
#define ENTREE_MODE							GetFlag(FV_E12)

#define VAL_PRESSION_NON_FILTREE_1			GetfReg(RV_ANA_DIRECTE_A1)				// Pression Nom Filtr�e
#define VAL_PRESSION_FILTREE_1				GetfReg(RV_MOY_ANA_A1)					// Pression Filtr�e
#define VAL_PRESSION_FILTREE_2				GetfReg(RV_MOY_ANA_A2)					// Pression
#define VAL_FREQ_VARIATEUR					GetfReg(RV_MOY_ANA_A3)					// Freq variateur
#define VAL_REGIME_MOTEUR					GetfReg(RV_MOY_E1)						// R�gime Moteur
#define VAL_DEBIT_IMPULSION					GetfReg(RV_MOY_E2)						// D�bit Impulsion
#define VAL_DEBIT_ANALOGIQUE				GetfReg(RV_MOY_ANA_A4)					// D�bit Analogique
#define VAL_REGIME_MOTEUR_SIMULE			GetfReg(RV_REGIME_SIMULE_IRRIG)			// R�gime Moteur Simule
#define VAL_REGIME_REEL						1500									// Regime r�el
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
