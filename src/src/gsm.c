#include <string.h>
#include "plib.h"
#include "gsm.h"
#include "gsm+.h"
#include "main.h"
#include "config.h"
#include "variables.h"
#include "slvxsbus.h"
#include "mtrxsbus.h"
#include "uart.h"
#include "comsbusmaitre.h"

#define TAILLE_MEMOIR_HESTORIQUE_H		1024
//#define MAX_NBR_SAVE_A_ENVOYER_UDPH		360
#define MAX_NBR_SAVE_A_ENVOYER_UDPH		225
long gsm_MemoirRam[TAILLE_MEMOIR_HESTORIQUE_H];
char gsm_MemoirPaquet[128];

char gsm_BufferTCPUDP[1536];

unsigned char gsm_MAESaveH;
unsigned int gsm_AddresseSaveRam;
unsigned int gsm_AddresseSaveRamFlash;
unsigned int gsm_AddresseSaveRamEnvoi;
unsigned int gsm_NbrSaveRAM;
unsigned int gsm_NbrRegSaveSecUDPH;
unsigned int gsm_NbrRegSaveRAMUDPH;
unsigned int gsm_NbrSaveRAMUDPH;

//UDP
unsigned char gsm_MAEUDP = GSM_UDP_INIT;
int gsm_aUDPCfg;
int gsm_CptVarUDP;
int gsm_UDPCptBit;
int gsm_UDPCptOctet;
uVarUDP gsm_DataVarUDP;

unsigned char gsm_MAE;
unsigned char gsm_MAEInit;
unsigned char gsm_MAEInitPwr;
unsigned char gsm_MAECmd;
unsigned char gsm_MAEInitCfg;
unsigned char gsm_MAETraittementSMS;
unsigned char gsm_MAERequette;

unsigned char gsm_NombreReset;
unsigned char gsm_NombreErreur;

bitgsm gsmbit;

//Num Téléphones
unsigned int gsm_aNumTelephone[4];
unsigned int gsm_aAdresseIP[4];
unsigned int gsm_aNomStation;
unsigned int gsm_aTCPPort;
unsigned int gsm_aUDPRPort;
unsigned int gsm_aUDPHPort;
unsigned int gsm_aTOSKT;
unsigned int gsm_aAPN;
unsigned int gsm_aUser;
unsigned int gsm_aPW;
unsigned int gsm_aHommetMort;
uHommetMort gsm_HommetMort;
unsigned int gsm_aSonneAvantEnvoiSMS;
uSonneAvantEnvoiSMS gsm_SonneAvantEnvoiSMS;
unsigned int gsm_aNomAna;
unsigned int gsm_aNomEntree;
unsigned int gsm_aNomSortie;
unsigned int gsm_aInitCfgAlrmeAna;
unsigned int gsm_aInitCfgAlrmeEntree;


unsigned char gsm_NbrSMSEntrant;
unsigned int gsm_aInitSMSEntrant;
unsigned char gsm_NbrAlarme;
unsigned int gsm_aInitEntetAlarme;

unsigned char gsm_MAEUDPR;
int gsm_MAECondEnvoiUDPR;
int gsm_OldEtatFrqEnvoiUDPR;

EntetSMSAlarme gsm_EntetAlarme;
unsigned int gsm_aInitAlarme;

unsigned char gsm_NbrSMSRapport;
unsigned int gsm_aInitSMSRapport;

unsigned int gsm_aInitUDPCfg;
unsigned char gsm_NbrVarUDP;

uInfoSauvegrde gsm_InfoHisto;
unsigned int gsm_aInitCfgSave;

uInfoSauvegrde gsm_InfoHistoRam;
unsigned int gsm_aInitCfgSaveRam;

unsigned int gsm_TailleTrame;
char gsm_TrameSIM32[256];



unsigned char gsm_MAERapport;
unsigned char gsm_CptNbrLineRapport;
unsigned char gsm_NbrVarRapport;
unsigned char gsm_CptNbrVarRapport;

unsigned int gsm_aSMSRapport;
unsigned int gsm_aMsgRapport;

SMSEntrantRapport gsm_DataVarRapport;

unsigned int gsm_pMsgAlarmeToSend;
unsigned char gsm_IndiceTeleAlarme;
unsigned char gsm_NumAlarme;
unsigned int gsm_aSMSAlarme;
unsigned int gsm_aMsgAlarme;
unsigned char gsm_MAEAlarme;
unsigned char gsm_CptAlarme;

unsigned char gsm_IndexTeleRapport;

unsigned char gsm_NbrEssaiEnvoi;

unsigned char gsm_GPRSProblem = 0;

#define GSM_TAILLE_LISTE_CONFIG1		11
const char *gsm_pListeConfig1[GSM_TAILLE_LISTE_CONFIG1] =
{
	"AT+IPR=19200\r",
	"ATZ0\r",
	"ATE0\r",
	//"AT+CGPSPWR=0\r",//0: pas de vielle, 1: veille avec RTS, 2: vielle tempo
 	//"AT+CGPSRST=1\r",//0: pas de vielle, 1: veille avec RTS, 2: vielle tempo
 	"AT+CSCLK=0\r",
 	"AT+CMEE=1\r",
	"AT+CENG=0\r",
	"AT+CSMINS=1\r",
	"AT+CREG=1\r",
	"ATS0=0\r",
	"AT+IFC=2,2\r",
	"AT&W\r",
};

#define GSM_TAILLE_LISTE_CONFIG2		8
const char *gsm_pListeConfig2[GSM_TAILLE_LISTE_CONFIG2] =
{
	"AT+CPMS=\"SM\",\"SM\",\"SM\"\r",
	"AT+CLIP=1\r",
	"AT+CMGF=1\r",
	"AT+CSMP=17,255,0,0\r",
	"AT+CSDH=0\r",
	"AT+CNMI=3,1\r",
	"AT+CSCS=\"8859-1\"\r",
	"AT&W\r",
};

#define GSM_TAILLE_LISTE_CONFIG_GPRS		1
const char *gsm_pListeConfigGPRS[GSM_TAILLE_LISTE_CONFIG_GPRS] =
{
//	"AT+CIPSHUT\r",
	"AT+CIPMUX=1\r",
};

char gsm_QualiteReseau;
short gsm_NumMessage;
char gsm_TelephoneAppel[16];
char gsm_TelephoneUser[16];
char gsm_TelephoneSMS[16];
char gsm_MessageSMS[162];
char gsm_ReponseSMS[162];

#define	GSM_AT_CPIN			"AT+CPIN?\r"
#define	GSM_AT_CPIN_SET		"AT+CPIN=%04ld\r"
#define GSM_AT_CSQ			"AT+CSQ\r"
#define GSM_AT_CMGR			"AT+CMGR=%d\r"
#define GSM_AT_CPMS			"AT+CPMS?\r"
#define GSM_AT_CMGD			"AT+CMGD=%d\r"
#define GSM_AT_CMGS			"AT+CMGS=\"%s\";\r"
#define GSM_ATH0			"ATH0\r"
#define GSM_ATCREG			"AT+CREG?\r"
#define GSM_AT_CFUN         "AT+CFUN=0,1\r"

//GPRS
#define GSM_DEACTIVEGPRS	"AT+CIPSHUT\r"
#define GSM_GPRSETAT		"AT+CGATT?\r"
#define GSM_GPRSATTCH		"AT+CGATT=1\r"
#define GSM_GPRSAPN			"AT+CSTT=\"%s\",\"%s\",\"%s\"\r"
#define GSM_GPRSCONNECT		"AT+CIICR\r"
#define GSM_GPRSADRESSE		"AT+CIFSR\r"
#define GSM_IP_START		"AT+CIPSTART=%d,\"UDP\",\"%s\",%d\r"
#define GSM_IP_SEND			"AT+CIPSEND=%d,%d\r"
#define GSM_UDPCLOSE		"AT+CIPCLOSE=0\r"



#define GSM_DEFAULT_VITESSE		1
const int gsm_ListeVitesse[5]={9600, 19200, 38400, 57600, 115200};
unsigned char gsm_CptVitesse;
unsigned char gsm_CptConfig;

char gsm_Chaine[256];

char *gsm_pCommande;
char *gsm_pParametre;
char gsm_ChaineTemps[162];

unsigned char gsm_MAESbus;
unsigned char gsm_MAEDateHeure;

//GPRS
unsigned char gsm_UDPNumIP;
unsigned short gsm_UDPPort;

//
int gsm_NumSocket;
int gsm_GPRSCnxNum;
int gsm_GPRSDataNbr;
char gsm_GPRSData[1024];

void gsm_Init()
{
	long *pVal;
	long preVal = 0xFFFFFFFF;
	unsigned int IndexConfig;
	unsigned int Taille;

	GSM_TRIS_ALIM = OUTPUT;
	GSM_TRIS_RESET = OUTPUT;
	GSM_TRIS_POWER = OUTPUT;
	GSM_TRIS_ETAT = INPUT;
	GSM_TRIS_SIM_PRESENT = INPUT;


        // init input capture : RI_GSM
        GSM_TRIS_RI = INPUT;
        IC1CON = 0x8002;        // TMR2, int every cap, fall'n edge
        mIC1ClearIntFlag();     // clear the interrupt flag
        mIC1SetIntPriority(3);
        mIC1IntEnable( 1);      // enable the IC1 interrupt


	GSM_ALIM = 0;
	GSM_RESET = 0;
	GSM_POWER = 0;

	gsm_MAE = GSM_INIT;
	gsm_MAEInit = GSM_INIT_PWR;
	gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
	gsm_MAECmd = GSM_CMD_ENVOI;
	gsm_MAESbus = GSM_SBUS_INIT;
	gsm_MAEDateHeure = GSM_DH_INIT;

	gsm_IndexTeleRapport = 0;
	gsm_AddresseSaveRam = 0;
        gsm_AddresseSaveRamFlash = 0;
        gsm_AddresseSaveRamEnvoi = 0;

	gsm_NbrSaveRAM = 0;
        comsbusmaitre_TOReponse[0] = 8;
	comsbusmaitre_bit[0].bSbusLibre = 1;
//	gsmbit.bSMSRapport = 0;
	gsmbit.bConfigSysOK = 0;
	gsmbit.bActiveGSM = 1;



	if(GetlReg(RNV_NBR_CAR_TELE_CMP) == 0xFFFFFFFF)
	{
		SetFlag(FNV_GSM_VERIF_NUM_TELE, 0);
		SetlReg(RNV_NBR_CAR_TELE_CMP, 8);
		SetFlag(FNV_ACTIVE_CODE_PINE, 0);
		SetlReg(RNV_CODE_PINE, 0);
		SetlReg(RNV_VIRIF_SMS, 60);
		SetlReg(RNV_NBR_MAX_SMS_HEURE, 10);
		SetlReg(RNV_NBR_MAX_SMS_JOURS, 100);
		SetlReg(RNV_TEMPO_ENVOI_RAPPORT, 0);
		SetFlag(FNV_GSM_MODE_GPRS, 0);
		SetlReg(RNV_TEMPO_SEND_UDPR, 0);
		SetlReg(RNV_TEMPO2_SEND_UDPR, 0);
		SetlReg(RNV_HEURE_DEBUT_POINT_UDPR, 7);
		SetlReg(RNV_HEURE_FIN_POINT_UDPR, 19);
		SetlReg(RNV_NUM_ADD_IP_UDPR, 0);
		SetlReg(RNV_STATION_CONDITION_UDPR, 0);
		SetlReg(RNV_ADRESSE_CONDITION_UDPR, 0);
		SetlReg(RNV_GSM_DUREE_APRES_N_RESET, GSM_DUREE_APRES_N_RESET);
		SetFlag(FNV_MODE_ENVOI_CONDITIONNEL_UDPR, 0);

		SetlReg(RNV_SEUIL1_ENVOI_UDPH, 360);
		SetlReg(RNV_SEUIL2_ENVOI_UDPH, 600);
		SetlReg(RNV_SEUIL3_ENVOI_UDPH, 960);
		SetlReg(RNV_SEUIL4_ENVOI_UDPH, 1380);

		SetlReg(RNV_SEUIL1_NBR_SAVE_SEND_UDPH, 90);
		SetlReg(RNV_SEUIL2_NBR_SAVE_SEND_UDPH, 90);
		SetlReg(RNV_SEUIL3_NBR_SAVE_SEND_UDPH, 90);
		SetlReg(RNV_SEUIL4_NBR_SAVE_SEND_UDPH, 90);

		SetlReg(RNV_TEMPS_MIN_ENTRE_SAVE_RAM, 3600);
		SetlReg(RNV_TEMPS_MAX_ENTRE_SAVE_RAM, 7200);
		SetlReg(RNV_TEMPS_RETARD_SAVE_RAM, 60);

        SetFlag(FNV_RAZ_SAUVEGARDE_RAM_APRES_ENVOI, 0);
        SetFlag(FNV_INFO_PROBLEM_GPRS_ACTIVE, 1);

        SetlReg(RNV_TAILLE_SAVE_RAM, TAILLE_MEMOIR_HESTORIQUE_H);

        SetlReg(RNV_TEMPS_REVEILLE_GSM_OK, 3);

        SetlReg(RNV_TEMPS_ENTRE_TRAME_UDPP, 15);

        SetlReg(RNV_NBR_REG_MAX_UDP, MAX_NBR_SAVE_A_ENVOYER_UDPH);

		gsmplus_Init();
	}

    if( GetlReg(RNV_NBR_SECTEUR_SAVE_RAM) == 0xFFFFFFFF ||
        GetlReg(RNV_NBR_SECTEUR_SAVE_RAM) > TAILLE_SECTEUR_HISTORIQUE/BYTE_PAGE_SIZE
        )
	{
        SetlReg(RNV_NBR_SECTEUR_SAVE_RAM, TAILLE_SECTEUR_HISTORIQUE/BYTE_PAGE_SIZE);
    }

    pVal = (long *) (SECTEUR_HISTORIQUE + (gsm_AddresseSaveRamFlash * BYTE_PAGE_SIZE));
    for (gsm_AddresseSaveRamFlash = 0; gsm_AddresseSaveRamFlash < GetlReg(RNV_NBR_SECTEUR_SAVE_RAM); gsm_AddresseSaveRamFlash++) {
        if (*pVal == 0xFFFFFFFF || *pVal < preVal) {
            break;
        }
        preVal = *pVal;
        pVal+=0x400;
    }
    if(gsm_AddresseSaveRamFlash >= GetlReg(RNV_NBR_SECTEUR_SAVE_RAM))
    {
        gsm_AddresseSaveRamFlash = 0;
    }

	if((configGSM_Long(0) != 0xFFFFFFFF) && !gsmbit.bConfigSysOK)
	{
		// Init des TimeOut Sbus
		IndexConfig = 2;
		comsbusmaitre_TOReponse[0] = configGSM_Short(IndexConfig);
		if(!comsbusmaitre_TOReponse[0]) comsbusmaitre_TOReponse[0] = 8;
		IndexConfig += 2;

		// Init des N° des téléphone
		gsm_aNumTelephone[0] = IndexConfig;
		IndexConfig += 25;
		gsm_aNumTelephone[1] = IndexConfig;
		IndexConfig += 25;
		gsm_aNumTelephone[2] = IndexConfig;
		IndexConfig += 25;
		gsm_aNumTelephone[3] = IndexConfig;
		IndexConfig += 25;

		gsm_aNomStation = IndexConfig;
		IndexConfig += 25;

		gsm_aAdresseIP[0] = IndexConfig;
		IndexConfig += 50;

		gsm_aAdresseIP[1] = IndexConfig;
		IndexConfig += 50;

		gsm_aAdresseIP[2] = IndexConfig;
		IndexConfig += 50;

		gsm_aAdresseIP[3] = IndexConfig;
		IndexConfig += 50;

		gsm_aTCPPort = IndexConfig;
		IndexConfig += 2;

		gsm_aUDPRPort = IndexConfig;
		IndexConfig += 2;

		gsm_aUDPHPort = IndexConfig;
		IndexConfig += 2;

		gsm_aTOSKT = IndexConfig;
		IndexConfig += 2;

		gsm_aAPN = IndexConfig;
		IndexConfig += 25;

		gsm_aUser = IndexConfig;
		IndexConfig += 25;

		// Mot passe
		gsm_aPW = IndexConfig;
		IndexConfig += 25;
		//////////////////////////////////////////////////
		// Config Homme Mort
		gsm_aHommetMort = IndexConfig;
		gsm_HommetMort.HommetMort = configGSM_Char(IndexConfig);
		IndexConfig++;

		// Config Sonnerie Avant Envoi SMS
		gsm_aSonneAvantEnvoiSMS = IndexConfig;
		gsm_SonneAvantEnvoiSMS.SonneAvantEnvoiSMS =  configGSM_Char(IndexConfig);
		IndexConfig++;

		// Nom ANA1
		gsm_aNomAna = IndexConfig;
		IndexConfig += GSM_NBR_ANA * 17; //2 Ana (nom : 17car)
		gsm_aNomEntree = IndexConfig;
		IndexConfig += GSM_NBR_ENTREE * 17; //4 Entree (nom : 17car)
		gsm_aNomSortie = IndexConfig;
		IndexConfig += GSM_NBR_SORTIE * 17; //2 Sortie (nom : 17car)

		gsm_aInitCfgAlrmeAna = IndexConfig;
		IndexConfig += GSM_NBR_ANA * 18; //2 Ana (nom : 18car)
		gsm_aInitCfgAlrmeEntree = IndexConfig;
		IndexConfig += GSM_NBR_ENTREE * 18; //4 Entree (nom : 18car)

		gsmbit.bConfigSysOK = 1;
	}

	// Initialisation des pointeur sur la configuration
	if((configGSM_Long(4096) != 0xFFFFFFFF) && !gsmbit.bConfigOK)
	{
		//Taille Globale
		IndexConfig = 4098;

		// Refferance text
		//TailleRef = configGSM_Short(IndexConfig);
		//IndexConfig += TailleRef;
		IndexConfig += configGSM_Short(IndexConfig);

		//Entrant
		Taille = configGSM_Short(IndexConfig);
		gsm_NbrSMSEntrant	= configGSM_Char(IndexConfig + 2);
		gsm_aInitSMSEntrant = IndexConfig + 3;

		//Alarme
		IndexConfig += Taille;
		Taille	= configGSM_Short(IndexConfig);
		gsm_NbrAlarme	= configGSM_Char(IndexConfig + 2);
		gsm_aInitEntetAlarme = IndexConfig + 3;
		gsm_EntetAlarme.cEntetSMSAlarme = configGSM_Char(gsm_aInitEntetAlarme);
		gsm_aInitAlarme = IndexConfig + 4;

		//Rappot
		IndexConfig += Taille;
		Taille = configGSM_Short(IndexConfig);
		//gsmbit.bSMSRapport = configGSM_Char(IndexConfig + 2);
		gsm_NbrSMSRapport = configGSM_Char(IndexConfig + 3);
		gsm_aInitSMSRapport = IndexConfig + 4;

		//UDP
		IndexConfig += Taille;
		gsm_aInitUDPCfg = IndexConfig;
		gsm_NbrVarUDP = configGSM_Char(gsm_aInitUDPCfg);
		gsm_aInitUDPCfg++;
		IndexConfig++;
		IndexConfig += (gsm_NbrVarUDP * 4);

		Taille = configGSM_Short(IndexConfig);
		if(Taille!=0xFFFF)
		{
			gsm_InfoHisto.InfoSauvegrde = configGSM_Long(IndexConfig + 2);
			gsm_InfoHisto.TSTVA.EtatCfg = 1;
			gsm_aInitCfgSave = IndexConfig+6;
		}
		IndexConfig += Taille;

		Taille = configGSM_Short(IndexConfig);
		if(Taille!=0xFFFF)
		{
			gsm_InfoHistoRam.InfoSauvegrde = configGSM_Long(IndexConfig + 2);
			gsm_InfoHistoRam.TSTVA.EtatCfg = 1;
			gsm_aInitCfgSaveRam = IndexConfig+6;
            gsm_NbrSaveRAMUDPH = GetlReg(RNV_NBR_REG_MAX_UDP)/(gsm_InfoHistoRam.TSTVA.NbrReg + 1);
            gsm_NbrRegSaveRAMUDPH = gsm_NbrSaveRAMUDPH * (gsm_InfoHistoRam.TSTVA.NbrReg + 1);
            gsm_NbrRegSaveSecUDPH = (1024/(gsm_InfoHistoRam.TSTVA.NbrReg + 1)) * (gsm_InfoHistoRam.TSTVA.NbrReg + 1);
		}
		IndexConfig += Taille;

		gsmbit.bConfigOK = 1;
	}

	SetTimer(TIMER_ENVOI_RAPPORT, 600);
	SetTimer(TIMER_SEND_UDPR, 600);
	SetTimer(TIMER_MIN_ENTRE_SAVE_RAM, 300);

	gsm_MAEUDPR = GSM_UDPR_VERIF;
/*
    INTSetVectorPriority( INT_EXTERNAL_4_VECTOR, INT_PRIORITY_LEVEL_3);
    INTSetVectorSubPriority( INT_EXTERNAL_4_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_INT4);
    INTEnable(INT_INT4, INT_ENABLED);
 */
}

char gsm_IsValideTelephone(char *pTelephone)
{
    int taille = strlen(pTelephone);
	if( (taille >= 8)
        &&
        (taille < 16)
        &&
		(atoi(pTelephone) != 0)
	)
	{
        while(*pTelephone)
        {
            if(*pTelephone != '+' && (*pTelephone < '0' || *pTelephone > '9'))
            {
                return 0;
            }
            pTelephone++;
        }
		return 1;
	}
	return 0;
}

//void strtoupper(register char *s1)
//{
//	while(*s1)
//	{
//		*s1 = toupper(*s1);
//		s1++;
//	}
//}

char *strtrim(register char *s1)
{
	register char *s2;
	s2 = s1 + strlen(s1) - 1;

	while(*s2 && *s2 == ' ')
	{
		*s2 = 0;
		s2--;
	}

	while(*s1 && *s1 == ' ')
	{
		s1++;
	}
	return s1;
}

// Fonction pour comparaison de deux chaine a partire du fin
int strrcmp(register const char * s1, register const char * s2, size_t len)
{
	register const char *	cp1;
	register const char *	cp2;
	register signed char	r;

	cp1 = s1+strlen(s1);
	cp2 = s2+strlen(s2);

	while(!(r = *cp1 - *cp2--) && len-- && cp1--)
		continue;
	return r;
}

char *gsm_NomStation()
{
	if(gsmbit.bConfigSysOK)
	{
		return configGSM_String(gsm_aNomStation);
	}
	else
	{
		return " ";
	}
}

unsigned char gsm_SendCmd(const char *pCmd, char *pReponse, int TO)
{
	switch(gsm_MAECmd)
	{
		case GSM_CMD_ENVOI:
		{
			gsmbit.bCommande = 1;
			uart_Rx_Buffer[0] = 0;
			gsm_TrameSIM32[0] = 0;
			uart_Write((char *)pCmd, strlen(pCmd));
			SetTimer(TIMER_CMD_TO, TO);
			gsm_TailleTrame = 0;
			gsmbit.bReponse = 0;
			gsm_MAECmd = GSM_CMD_ATT_REPONSE;
			break;
		}
		case GSM_CMD_ATT_REPONSE:
		{
			if(gsmbit.bReponse)
			{
				gsm_MAECmd = GSM_CMD_VERIF_REPONSE;
			}

			if(!GetTimer(TIMER_CMD_TO))
			{
				gsm_MAECmd = GSM_CMD_ENVOI;
				return GSM_CMD_TO_ERR;
			}
			break;
		}
		case GSM_CMD_VERIF_REPONSE:
		{
			gsm_MAECmd = GSM_CMD_ENVOI;
			if(strstr(gsm_TrameSIM32, pReponse))
			{
				return GSM_CMD_SUCCES;
			}
			else
			{
				gsmbit.bReponse = 0;
				gsm_MAECmd = GSM_CMD_ATT_REPONSE;
			}
			break;
		}
	}
	return GSM_CMD_ENCOUR;
}

unsigned char gsm_SendData(const char *pCmd, int Taille, char *pReponse,int TO)
{
	switch(gsm_MAECmd)
	{
		case GSM_CMD_ENVOI:
		{
			gsmbit.bCommande = 1;
			uart_Rx_Buffer[0] = 0;
			gsm_TrameSIM32[0] = 0;
			uart_Write((char *)pCmd, Taille);
			SetTimer(TIMER_CMD_TO, TO);
			gsm_TailleTrame = 0;
			gsmbit.bReponse = 0;
			gsm_MAECmd = GSM_CMD_ATT_REPONSE;
			break;
		}
		case GSM_CMD_ATT_REPONSE:
		{
			if(gsmbit.bReponse)
			{
				gsm_MAECmd = GSM_CMD_VERIF_REPONSE;
			}

			if(!GetTimer(TIMER_CMD_TO))
			{
				gsm_MAECmd = GSM_CMD_ENVOI;
				return GSM_CMD_TO_ERR;
			}
			break;
		}
		case GSM_CMD_VERIF_REPONSE:
		{
			gsm_MAECmd = GSM_CMD_ENVOI;
			if(strstr(gsm_TrameSIM32, pReponse))
			{
				return GSM_CMD_SUCCES;
			}
			else
			{
				gsmbit.bReponse = 0;
				gsm_MAECmd = GSM_CMD_ATT_REPONSE;
			}
			break;
		}
	}
	return GSM_CMD_ENCOUR;
}

// Sauvegarde en RAM
//------------------------------------------------------------------------------
// Fonction : Formatage et sauvgarde des données (6 Température)
// Entrée 	: rien
// Sortie 	: rien
//------------------------------------------------------------------------------
int gsm_MAEHistoRam;
unsigned int gsm_aCfgSaveRam;
unsigned char gsm_NbrVarSaveRam;
unsigned char gsm_CptRegHistoRam;
unsigned char gsm_CptVarHistoRam;
FormatVarSave gsm_FormatHistoRam;

#define GSM_HISTO_RAM_INIT 0
#define GSM_HISTO_RAM_CFG 1
#define GSM_HISTO_RAM_FORMAT 2

int gsm_FctFormatHistoRam(long *pDataSave)
{
	long double ldVal;
	ofsld Val;

	//if(GSM_CMD_ACTIVATION_SAVE_RAM && gsm_InfoHistoRam.TSTVA.EtatCfg)
	if(GetFlag(FNV_ACTIVATION_SAUVEGARDE_RAM) && gsm_InfoHistoRam.TSTVA.EtatCfg)
	{


		switch(gsm_MAEHistoRam)
		{
			case GSM_HISTO_RAM_INIT :	*pDataSave = GetlReg(RV_DATE_HEURE);

										gsm_aCfgSaveRam = gsm_aInitCfgSaveRam;
										gsm_NbrVarSaveRam = configGSM_Char(gsm_aCfgSaveRam);
										gsm_aCfgSaveRam++;
										configGSM_Read(gsm_FormatHistoRam.Data, gsm_aCfgSaveRam, sizeof(FormatVarSave));
										gsm_CptRegHistoRam = 0;
										gsm_CptVarHistoRam = 0;
										gsm_MAEHistoRam = GSM_HISTO_RAM_CFG;
										break;

			case GSM_HISTO_RAM_CFG :	if(gsm_CptRegHistoRam < gsm_InfoHistoRam.TSTVA.NbrReg)
										{
											if(gsm_CptVarHistoRam < gsm_NbrVarSaveRam)
											{
												if(/*mainbit.bSbusMaitreRAM || */(gsm_FormatHistoRam.Info.Station == gNumStation || gsm_FormatHistoRam.Info.Station == 0))
												{
													if(!comsbusmaitre_bit[0].bSbusLibre)
													{
														return 0;
													}
													
													comsbusmaitre_bit[0].bSbusLibre = 0;
													comsbusmaitre_bit[0].bSbusEnd = 0;
													comsbusmaitre_bit[0].SbusErr = 0;
													comsbusmaitre_SbusStation[0] = gsm_FormatHistoRam.Info.Station;
													comsbusmaitre_SbusAdresse[0][0].l = gsm_FormatHistoRam.Info.Adresse;
													comsbusmaitre_SbusNbr[0] = 1;
                                                    comsbusmaitre_SbusType[0] = 'R';
													comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
													if(!gsm_FormatHistoRam.Info.Type)
													{
														comsbusmaitre_SbusType[0] = 'F';
														comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
													}
													gsm_MAEHistoRam = GSM_HISTO_RAM_FORMAT;
												}
												else
												{
													gsm_aCfgSaveRam += 17;
													configGSM_Read(gsm_FormatHistoRam.Data, gsm_aCfgSaveRam, sizeof(FormatVarSave));
													gsm_CptVarHistoRam++;
												}
											}
											else
											{
												gsm_CptVarHistoRam = 0;
												gsm_CptRegHistoRam++;
												gsm_NbrVarSaveRam = configGSM_Char(gsm_aCfgSaveRam);
												gsm_aCfgSaveRam++;
												configGSM_Read(gsm_FormatHistoRam.Data, gsm_aCfgSaveRam, sizeof(FormatVarSave));
											}
										}
										else
										{
											gsm_MAEHistoRam = GSM_HISTO_RAM_INIT;

											//mainbit.bSbusMaitreRAM = 0;
//											if(mainbit.bOrdreMAJHistoRAM)
//											{
//												mainbit.bOrdreMAJHistoRAM = 0;
//												//mainbit.bSbusMaitreRAM = 1;
//											}
//											else
//											{
//												mainbit.bMAJHistoOkRAM = 1;
//											}
											return 0;
										}

										break;

			case GSM_HISTO_RAM_FORMAT:	if(comsbusmaitre_bit[0].bSbusEnd)
										{
											comsbusmaitre_bit[0].bSbusLibre = 1;
											if(!comsbusmaitre_bit[0].SbusErr)
											{
												switch(gsm_FormatHistoRam.Info.Type)
												{
													// Flag
													case 0 : 	Val.l = comsbusmaitre_SbusBuffer[0][0].of.octet[0];
																Val.l <<= gsm_FormatHistoRam.Info.Decalage;
																pDataSave[gsm_CptRegHistoRam + 1] &= ~gsm_FormatHistoRam.Info.Masque;
																pDataSave[gsm_CptRegHistoRam + 1] |= Val.l;
																break;

													// registre long
													case 1 :	ldVal = comsbusmaitre_SbusBuffer[0][0].l;
																ldVal *= gsm_FormatHistoRam.Info.Coefficient;
																ldVal += gsm_FormatHistoRam.Info.Offset;
																Val.l = (long)ldVal;
																if(gsm_FormatHistoRam.Info.Masque != 0xFFFFFFFF)
																{
																	Val.l <<= gsm_FormatHistoRam.Info.Decalage;
																	Val.l &= gsm_FormatHistoRam.Info.Masque;
																	pDataSave[gsm_CptRegHistoRam + 1] &= ~gsm_FormatHistoRam.Info.Masque;
																	pDataSave[gsm_CptRegHistoRam + 1] |= Val.l;
																}
																else
																{
																	pDataSave[gsm_CptRegHistoRam + 1] = Val.l;
																}
																break;

													// registre float
													case 2 :	Val.d = comsbusmaitre_SbusBuffer[0][0].d;
																Val.d *= gsm_FormatHistoRam.Info.Coefficient;
																Val.d += gsm_FormatHistoRam.Info.Offset;
																if(gsm_FormatHistoRam.Info.Masque != 0xFFFFFFFF)
																{
																	Val.l = (long)Val.d;
																	Val.l <<= gsm_FormatHistoRam.Info.Decalage;
																	Val.l &= gsm_FormatHistoRam.Info.Masque;
																	pDataSave[gsm_CptRegHistoRam + 1] &= ~gsm_FormatHistoRam.Info.Masque;
																	pDataSave[gsm_CptRegHistoRam + 1] |= Val.l;

																}
																else
																{
																	pDataSave[gsm_CptRegHistoRam + 1] = Val.l;
																}
																break;
												}
											}
											gsm_aCfgSaveRam += 17;
											configGSM_Read(gsm_FormatHistoRam.Data, gsm_aCfgSaveRam, sizeof(FormatVarSave));
											gsm_CptVarHistoRam++;
											gsm_MAEHistoRam = GSM_HISTO_RAM_CFG;
										}
										break;
		}
	}
	else
	{
//		mainbit.bMAJHistoOkRAM = 1;
//		mainbit.bOrdreMAJHistoRAM = 0;
		return 0;
	}
	return 1;
}

//--------------------------------------------------------------------------------------------
// Historique RAM
//--------------------------------------------------------------------------------------------

void gsm_RAZHistoRam()
{
    gsm_AddresseSaveRamEnvoi = 0;
	gsm_AddresseSaveRam = 0;
	SetlReg(RV_INDEX_SAVE_RAM, 0);
    gsmbit.bSaveRamPlein = 0;
	memset(gsm_MemoirRam, 0, TAILLE_MEMOIR_HESTORIQUE_H*4);
}

#define GSM_SAVEH_VIRIF_HEURE_SAVE		0
#define GSM_SAVEH_RETARD				1
#define GSM_SAVEH_SAVE					2

void gsm_SauvegardeRam()
{
	ofsld Val;
	unsigned int TailleSave;
	unsigned int TaillePaquet;
    unsigned int index;

	// Si le sauvegarde est active et la configuration de sauvegarde existe
	if(GetFlag(FNV_ACTIVATION_SAUVEGARDE_RAM) && gsm_InfoHistoRam.TSTVA.EtatCfg && GetFlag(FNV_GSM_MODE_GPRS))
	{
        if(gsmbit.bSaveRamPlein)
        {
            if(gsmbit.bRAZRAMAutorisie)
            {
                gsmbit.bRAZRAMAutorisie = 0;
                gsm_RAZHistoRam();
            }
        }


		switch(gsm_MAESaveH)
		{
			case GSM_SAVEH_VIRIF_HEURE_SAVE:
			{
				if(!GetTimer(TIMER_MIN_ENTRE_SAVE_RAM) && GetlReg(RNV_TEMPS_MIN_ENTRE_SAVE_RAM)) //
				{
					// Si le mode de sauvegarde est par seuil
					if(gsm_InfoHistoRam.TSTVA.ModeSave)
					{
						// Si le type de registre seuil est float
						if(gsm_InfoHistoRam.TSTVA.TypeVariable)
						{
							Val.d = GetfReg(gsm_InfoHistoRam.TSTVA.Adresse);
						}
						// Si le type de registre seuil est entier long
						else
						{
							Val.l = GetlReg(gsm_InfoHistoRam.TSTVA.Adresse);
							Val.d = Val.l;
						}

						// Si le temps max entre sauvegarde est écouler
						if(	!GetTimer(TIMER_MAX_ENTRE_SAVE_RAM) ||
							// Ou
							// si la seuil est on mode passe bas et la variable > a la consigne
							(gsm_InfoHistoRam.TSTVA.TypeSeuil && (Val.d > GetfReg(RNV_CNSG_SEUIL_SAVE_RAM))) ||
							// ou  si la seuil est on mode passe haut et la variable < a la consigne
							(!gsm_InfoHistoRam.TSTVA.TypeSeuil && (Val.d < GetfReg(RNV_CNSG_SEUIL_SAVE_RAM)))
							)
						{
							// On indique qu'il faut fair une sauvegarde
							gsm_MAESaveH = GSM_SAVEH_RETARD;
							// On charge les tempo min et max entre sauvegarde
							SetTimer(TIMER_MIN_ENTRE_SAVE_RAM, GetlReg(RNV_TEMPS_MIN_ENTRE_SAVE_RAM) * 10);
							SetTimer(TIMER_MAX_ENTRE_SAVE_RAM, GetlReg(RNV_TEMPS_MAX_ENTRE_SAVE_RAM) * 10);
							SetTimer(TIMER_RETARD_SAVE_RAM, GetlReg(RNV_TEMPS_RETARD_SAVE_RAM));
							gEtatApp.bSaveRAM = 1;
						}
					}
					else
					{
						gsm_MAESaveH = GSM_SAVEH_RETARD;
						SetTimer(TIMER_MIN_ENTRE_SAVE_RAM, GetlReg(RNV_TEMPS_MIN_ENTRE_SAVE_RAM) * 10);
						SetTimer(TIMER_RETARD_SAVE_RAM, GetlReg(RNV_TEMPS_RETARD_SAVE_RAM));
						gEtatApp.bSaveRAM = 1;
					}
				}
				break;
			}
			case GSM_SAVEH_RETARD:
			{
				if(!GetTimer(TIMER_RETARD_SAVE_RAM)) //
				{
					if(!gsm_FctFormatHistoRam((long *)gsm_MemoirPaquet))
					{
						gsm_MAESaveH = GSM_SAVEH_SAVE;
					}
				}
				break;
			}
			case GSM_SAVEH_SAVE:
			{

				TailleSave = GetlReg(RNV_TAILLE_SAVE_RAM); //En registres
				if(TailleSave > TAILLE_MEMOIR_HESTORIQUE_H)
				{
					TailleSave = TAILLE_MEMOIR_HESTORIQUE_H;
				}

				TaillePaquet = gsm_InfoHistoRam.TSTVA.NbrReg + 1; //En registres

				if(gsm_AddresseSaveRam + TaillePaquet <= TailleSave)
				{
					memcpy(gsm_MemoirRam + gsm_AddresseSaveRam, gsm_MemoirPaquet, TaillePaquet*4);

					if(gsm_AddresseSaveRam + TaillePaquet >= TailleSave)
                    {

                        NVMErasePage((void *)(SECTEUR_HISTORIQUE + (gsm_AddresseSaveRamFlash*BYTE_PAGE_SIZE)));

                        // Program the Page
                        for(index = 0; index < NUM_ROWS_PAGE; index++)
                        {
                               NVMWriteRow((void *)(SECTEUR_HISTORIQUE + (gsm_AddresseSaveRamFlash*BYTE_PAGE_SIZE) + (index * BYTE_ROW_SIZE)), (void *)(gsm_MemoirRam) + (index * BYTE_ROW_SIZE));
                        }

                        gsm_AddresseSaveRamFlash++;
                        if(gsm_AddresseSaveRamFlash >= GetlReg(RNV_NBR_SECTEUR_SAVE_RAM))
                        {
                            gsm_AddresseSaveRamFlash = 0;
                        }

                        gsmbit.bSaveRamPlein = 1;
                        gsmbit.bRAZRAMAutorisie = 0;
                    }
                    gsm_AddresseSaveRam += TaillePaquet; //incrimente add de TAILLE_PACKET_SAVE

                    gsm_NbrSaveRAM++;

				}

				gsm_MAESaveH = GSM_SAVEH_VIRIF_HEURE_SAVE;
				gEtatApp.bSaveRAM = 0;
				break;
			}
		}

        SetlReg(RV_INDEX_SAVE_RAM, gsm_AddresseSaveRam);
		SetlReg(RV_INDEX_SAVE_RAM_SECTEUR, gsm_AddresseSaveRamFlash * (BYTE_PAGE_SIZE/4));

	}
}

int gsm_PlageEnvoi = 0;
int gsm_NbrSecteurUDPH = 0;
int gsm_NumSecteurUDPH = 0;


void gsm_VerifTransfertUDPH()
{
	int AdresseRegNbr;
	int HM;
	if(GetFlag(FNV_MODE_UDPH) && GetFlag(FNV_GSM_MODE_GPRS))
	{
		HM = (clock_GetHour()) * 60 + clock_GetMinute();

		if(HM > GetlReg(RNV_SEUIL1_ENVOI_UDPH) && HM <= GetlReg(RNV_SEUIL2_ENVOI_UDPH))
		{
            if(gsm_PlageEnvoi != 1)
            {
                gsmbit.bEnvoiUDPH = 1;
                gsm_PlageEnvoi = 1;
            }
			AdresseRegNbr = RNV_SEUIL1_NBR_SAVE_SEND_UDPH;
		}
		else if(HM > GetlReg(RNV_SEUIL2_ENVOI_UDPH) && HM <= GetlReg(RNV_SEUIL3_ENVOI_UDPH))
		{
			if(gsm_PlageEnvoi != 2)
            {
                gsmbit.bEnvoiUDPH = 1;
                gsm_PlageEnvoi = 2;
            }
			AdresseRegNbr = RNV_SEUIL2_NBR_SAVE_SEND_UDPH;
		}
		else if(HM > GetlReg(RNV_SEUIL3_ENVOI_UDPH) && HM <= GetlReg(RNV_SEUIL4_ENVOI_UDPH))
		{
			if(gsm_PlageEnvoi != 3)
            {
                gsmbit.bEnvoiUDPH = 1;
                gsm_PlageEnvoi = 3;
            }
			AdresseRegNbr = RNV_SEUIL3_NBR_SAVE_SEND_UDPH;
		}
		else
		{
			if(gsm_PlageEnvoi != 4)
            {
                gsmbit.bEnvoiUDPH = 1;
                gsm_PlageEnvoi = 4;
            }

            if(GetlReg(RNV_SEUIL4_NBR_SAVE_SEND_UDPH) == 0xFFFFFFFF)
			{
				AdresseRegNbr = RNV_NBR_SAVE_SEND_UDPH;
			}
			else
			{
				AdresseRegNbr = RNV_SEUIL4_NBR_SAVE_SEND_UDPH;
			}
		}

		if(
			(	!gsmbit.bSendUDPH
				&&
				!gsmbit.bSendSecUDPH
				&&
				((
				(!GetlReg(RNV_HEURE_DEBUT_POINT_UDPH)) &&
				(!GetlReg(RNV_HEURE_FIN_POINT_UDPH))
				)
				||
				(
				(clock_GetHour() >= GetlReg(RNV_HEURE_DEBUT_POINT_UDPH))
				&&
				(clock_GetHour() < GetlReg(RNV_HEURE_FIN_POINT_UDPH))
				))
			)
		)
		{
            if( (GetlReg(AdresseRegNbr) > 0)
				&&
                gsm_NbrSaveRAM
                &&
                (
                    gsmbit.bEnvoiUDPH
                    ||
                    gsm_NbrSaveRAM >= gsm_NbrSaveRAMUDPH
                    ||
                    gsm_NbrSaveRAM >= GetlReg(AdresseRegNbr)
                    ||
                    gsmbit.bSaveRamPlein
                )
			)
            {
                gsm_NbrSaveRAM = 0;
                gsmbit.bEnvoiUDPH = 0;
                gsmbit.bStopGSM = 0;
                gsmbit.bSendUDPH = 1;
                gsm_NbrEssaiEnvoi = 0;
            }
            else if(GetlReg(RV_NBR_SECTEUR_A_ENVOYE))
            {
                gsm_NbrSecteurUDPH = GetlReg(RV_NBR_SECTEUR_A_ENVOYE);
                SetlReg(RV_NBR_SECTEUR_A_ENVOYE, 0);
                if(gsm_NbrSecteurUDPH > GetlReg(RNV_NBR_SECTEUR_SAVE_RAM) + 1)
                {
                    gsm_NbrSecteurUDPH = GetlReg(RNV_NBR_SECTEUR_SAVE_RAM) + 1;
                }

                if(gsm_NbrSecteurUDPH == 1)
                {
                    gsm_NumSecteurUDPH = -1;
                }
                else
                {
                    gsm_NbrSecteurUDPH--;
                    gsm_NumSecteurUDPH = 0;

                    if(gsm_NbrSecteurUDPH > gsm_AddresseSaveRamFlash)
                    {
                        gsm_NumSecteurUDPH = GetlReg(RNV_NBR_SECTEUR_SAVE_RAM) - (gsm_NbrSecteurUDPH - gsm_AddresseSaveRamFlash);
                    }
                    else
                    {
                        gsm_NumSecteurUDPH = gsm_AddresseSaveRamFlash - gsm_NbrSecteurUDPH;
                    }
                    gsm_NbrSecteurUDPH++;
                }
                gsmbit.bStopGSM = 0;
                gsmbit.bSendSecUDPH = 1;
            }
        }
	}
}

int gsm_IndexEnvoiUDPH;
int gsm_LastIndexEnvoiUDPH;

int gsm_GetDataSecUDPH()
{
    char *pVal;
    int NbrOctetData;

    if(gsm_NbrSecteurUDPH)
    {
        if(gsm_NumSecteurUDPH == -1)
        {
            if(gsm_AddresseSaveRam > gsm_LastIndexEnvoiUDPH)
            {
                gsm_UDPCptOctet = sprintf(gsm_BufferTCPUDP, "%s MC3GP", gsm_NomStation());

                NbrOctetData = gsm_AddresseSaveRam - gsm_LastIndexEnvoiUDPH;

                if(NbrOctetData > gsm_NbrRegSaveRAMUDPH)
                {
                    NbrOctetData = gsm_NbrRegSaveRAMUDPH;
                }

                memcpy(gsm_BufferTCPUDP + gsm_UDPCptOctet, gsm_MemoirRam + gsm_LastIndexEnvoiUDPH, NbrOctetData*4);
                gsm_UDPCptOctet += (NbrOctetData*4);

                gsm_LastIndexEnvoiUDPH += NbrOctetData;

                if(gsm_LastIndexEnvoiUDPH >= gsm_AddresseSaveRam)
                {
                    gsm_NbrSecteurUDPH = 0;
                    gsm_NumSecteurUDPH = 0;
                    gsm_LastIndexEnvoiUDPH = 0;
                }
                return 1;
            }
            else
            {
                gsm_NbrSecteurUDPH = 0;
                gsm_NumSecteurUDPH = 0;
                gsm_LastIndexEnvoiUDPH = 0;
            }
        }
        else
        {
            gsm_UDPCptOctet = sprintf(gsm_BufferTCPUDP, "%s MC3GP", gsm_NomStation());

            NbrOctetData = gsm_NbrRegSaveSecUDPH - gsm_LastIndexEnvoiUDPH;

            if(NbrOctetData > gsm_NbrRegSaveRAMUDPH)
            {
                NbrOctetData = gsm_NbrRegSaveRAMUDPH;
            }

            pVal = (char *) (SECTEUR_HISTORIQUE + (gsm_NumSecteurUDPH * BYTE_PAGE_SIZE));

            memcpy(gsm_BufferTCPUDP + gsm_UDPCptOctet, pVal + (gsm_LastIndexEnvoiUDPH * 4), NbrOctetData*4);

            gsm_UDPCptOctet += (NbrOctetData*4);

            gsm_LastIndexEnvoiUDPH += NbrOctetData;

            if(gsm_LastIndexEnvoiUDPH >= gsm_NbrRegSaveSecUDPH)
            {
                gsm_NumSecteurUDPH++;
                if(gsm_NumSecteurUDPH >= GetlReg(RNV_NBR_SECTEUR_SAVE_RAM))
                {
                    gsm_NumSecteurUDPH = 0;
                }
                gsm_LastIndexEnvoiUDPH = 0;
                gsm_NbrSecteurUDPH--;
                if(gsm_NbrSecteurUDPH == 1)
                {
                    gsm_NumSecteurUDPH = -1;
                }
            }
            return 1;
        }
    }
	return 0;
}

int gsm_GetDataUDPH()
{
    int NbrOctetData;

    if(gsm_AddresseSaveRam - gsm_AddresseSaveRamEnvoi)
    {
        gsm_UDPCptOctet = sprintf(gsm_BufferTCPUDP, "%s MC3GH", gsm_NomStation());
        NbrOctetData = (gsm_AddresseSaveRam - gsm_AddresseSaveRamEnvoi);
        if(NbrOctetData > gsm_NbrRegSaveRAMUDPH)
        {
            NbrOctetData = gsm_NbrRegSaveRAMUDPH;
            gsm_AddresseSaveRamEnvoi = gsm_AddresseSaveRam - gsm_NbrRegSaveRAMUDPH;
        }
        NbrOctetData *= 4;

        memcpy(gsm_BufferTCPUDP + gsm_UDPCptOctet, gsm_MemoirRam + gsm_AddresseSaveRamEnvoi, NbrOctetData);
        gsm_UDPCptOctet += NbrOctetData;
        gsm_AddresseSaveRamEnvoi = gsm_AddresseSaveRam;
        return 1;
    }
	return 0;
}


int gsm_Rapport()
{
	char 		Chaine[32];
	struct tm	*pSTime;
	int 		Cpt;
	int 		Cpt1;

	if(GetFlag(FNV_RAPPORT_ACTIF))//gsmbit.bSMSRapport)
	{
		switch(gsm_MAERapport)
		{
			case GSM_GET_INIT : gsm_aSMSRapport = gsm_aInitSMSRapport;
								gsm_CptNbrLineRapport = 0;
								gsm_ChaineTemps[0] = 0;

			case GSM_GET_LINE : if(gsm_CptNbrLineRapport < gsm_NbrSMSRapport)
								{
									gsm_NbrVarRapport = configGSM_Char(gsm_aSMSRapport);
									gsm_aSMSRapport++;
									configGSM_Read(gsm_DataVarRapport.Data, gsm_aSMSRapport, 18);

									if(gsm_ChaineTemps[0])
									{
										strcat(gsm_ChaineTemps, "\r\n");
									}
									gsm_CptNbrLineRapport++;
									gsm_MAERapport = GSM_GET_VARIABLE;
									gsm_CptNbrVarRapport = 0;
								}
								else
								{
									gsm_MAERapport = GSM_GET_INIT;
									return 1;
								}

			case GSM_GET_VARIABLE :
								if(gsm_CptNbrVarRapport < gsm_NbrVarRapport)
								{
									if(!comsbusmaitre_bit[0].bSbusLibre)
									{
										break;
									}
									comsbusmaitre_bit[0].bSbusLibre = 0;
									comsbusmaitre_bit[0].bSbusEnd = 0;
									comsbusmaitre_bit[0].SbusErr = 0;
									comsbusmaitre_SbusStation[0] = gsm_DataVarRapport.Info.Station;
									comsbusmaitre_SbusAdresse[0][0].l = gsm_DataVarRapport.Info.Adresse;
									comsbusmaitre_SbusNbr[0] = 1;

									gsm_MAERapport = GSM_FORMAT_VARIABLE;
									// Si Flag ou Ref Flag
									if(gsm_DataVarRapport.Info.Type == 0 || gsm_DataVarRapport.Info.Type == 4)
									{
										comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
										comsbusmaitre_SbusType[0] = 'F';
									}
									else
									{
										comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
										comsbusmaitre_SbusType[0] = 'R';
									}
									//gsm_MAEOld = gsm_MAE;
									//gsm_MAE = GSM_ETAT_SBUS;
									gsm_CptNbrVarRapport++;
								}
								else
								{
									gsm_MAERapport = GSM_GET_LINE;
									break;
								}

			case GSM_FORMAT_VARIABLE :
								if(comsbusmaitre_bit[0].bSbusEnd)
								{
									comsbusmaitre_bit[0].bSbusLibre = 1;
									if(comsbusmaitre_bit[0].SbusErr == 2)
									{
										strcpy(Chaine, "000");
									}
									else if(comsbusmaitre_bit[0].SbusErr)
									{
										strcpy(Chaine, "???");
									}
									else
									{
										switch(gsm_DataVarRapport.Info.Type)
										{
											case 0 : 	sprintf(Chaine, "%d", comsbusmaitre_SbusBuffer[0][0].of.octet[0]);
														break;
											case 1 : 	pSTime = localtime(&(comsbusmaitre_SbusBuffer[0][0].l));
														sprintf(Chaine, "%02d:%02d:%02d %02d/%02d/%04d", pSTime->tm_hour, pSTime->tm_min,	pSTime->tm_sec, pSTime->tm_mday, pSTime->tm_mon + 1, pSTime->tm_year + 1900);
													 	break;
											case 2 : 	comsbusmaitre_SbusBuffer[0][0].d *= gsm_DataVarRapport.Info.Coefficient;
														comsbusmaitre_SbusBuffer[0][0].d += gsm_DataVarRapport.Info.Offset;
														sprintf(Chaine, "%.*f", gsm_DataVarRapport.Info.Precision, comsbusmaitre_SbusBuffer[0][0].d);
													 	break;
											case 3 : 	comsbusmaitre_SbusBuffer[0][0].l &= gsm_DataVarRapport.Info.Masque;
														comsbusmaitre_SbusBuffer[0][0].l >>= gsm_DataVarRapport.Info.Decalage;
														comsbusmaitre_SbusBuffer[0][1].d = ((double)comsbusmaitre_SbusBuffer[0][0].l) * gsm_DataVarRapport.Info.Coefficient;
														comsbusmaitre_SbusBuffer[0][1].d += gsm_DataVarRapport.Info.Offset;
														sprintf(Chaine, "%.*f", gsm_DataVarRapport.Info.Precision, comsbusmaitre_SbusBuffer[0][1].d);
														break;
											case 4 : 	Cpt = gsm_DataVarRapport.Info.Masque + 1;
														if(comsbusmaitre_SbusBuffer[0][0].of.octet[0])
														{
															Cpt += strlen(configGSM_String(Cpt)) + 1;
														}
														strcpy(Chaine, configGSM_String(Cpt));
														break;
											case 5 :
														Cpt = gsm_DataVarRapport.Info.Masque;
														if(comsbusmaitre_SbusBuffer[0][0].l < configGSM_Char(Cpt))
														{
															Cpt++;
															for(Cpt1 = 0; Cpt1 < comsbusmaitre_SbusBuffer[0][0].l; Cpt1++)
															{
																Cpt += strlen(configGSM_String(Cpt)) + 1;
															}
															strcpy(Chaine, configGSM_String(Cpt));
														}
														else
														{
															strcpy(Chaine, "???");
														}
														break;
														// Foramt Time
											case 6 : 	sprintf(Chaine, "%ld:%02d", comsbusmaitre_SbusBuffer[0][0].l/60,	(int)(comsbusmaitre_SbusBuffer[0][0].l%60));
													 	break;

											default:	Chaine[0] = 0;
														break;
										}
									}

									gsm_aMsgRapport = gsm_aSMSRapport + 18;
									//Message
									strcpy(gsm_Chaine, configGSM_String(gsm_aMsgRapport));
									//Valeur
									strcat(gsm_Chaine, Chaine);
									//Unité
									gsm_aMsgRapport += strlen(configGSM_String(gsm_aMsgRapport)) + 1;
									strcat(gsm_Chaine, configGSM_String(gsm_aMsgRapport));

									if(strlen(gsm_ChaineTemps) +  strlen(gsm_Chaine) < 160)
									{
										strcat(gsm_ChaineTemps, gsm_Chaine);
										gsm_aSMSRapport = gsm_aMsgRapport + strlen(configGSM_String(gsm_aMsgRapport)) + 1;
										gsm_aMsgRapport = gsm_aSMSRapport + 18;
										configGSM_Read(gsm_DataVarRapport.Data, gsm_aSMSRapport, 18);

										gsm_MAERapport = GSM_GET_VARIABLE;
									}
									else
									{
										gsm_MAERapport = GSM_GET_INIT;
										return 1;
									}
								}
								break;
		}
	}
	else
	{
		gsm_ChaineTemps[0] = 0;
		return 1;
	}
	return 0;
}

unsigned char gsm_MAESMSEntrant;
unsigned char gsm_CptSMSEntrant;

unsigned int gsm_aSMSEntrant;
unsigned int gsm_aMsgEntrant;
unsigned int gsm_aMsgSortantE;
unsigned int gsm_aMsgSortantL;

SMSEntrantRapport gsm_InfoSMSEntrant;

#define GSM_SMSENTRANT_INIT				0
#define GSM_SMSENTRANT_MSG				1
#define GSM_SMSENTRANT_DATA				2
#define GSM_SMSENTRANT_READ_SBUS		3
#define GSM_SMSENTRANT_WRITE_SBUS		4

char gsm_RequeteUtilisateur()
{
	char *pIndex1;
	struct tm	*pSTime;
	unsigned char Cpt;
	unsigned char Cpt1;

	if(gsmbit.bConfigSysOK)
	{
		switch(gsm_MAESMSEntrant)
		{
			case GSM_SMSENTRANT_INIT :
										gsm_aSMSEntrant	= gsm_aInitSMSEntrant;
										gsm_aMsgEntrant		= gsm_aSMSEntrant + 18;
										gsm_aMsgSortantE	= gsm_aMsgEntrant + strlen(configGSM_String(gsm_aMsgEntrant)) + 1;
										gsm_aMsgSortantL	= gsm_aMsgSortantE + strlen(configGSM_String(gsm_aMsgSortantE)) + 1;

										gsm_MAESMSEntrant = GSM_SMSENTRANT_MSG;
										gsm_CptSMSEntrant = 0;

			case GSM_SMSENTRANT_MSG :	if(gsm_CptSMSEntrant < gsm_NbrSMSEntrant) //gsm_pCommande gsm_pParametre
										{
											pIndex1 = configGSM_String(gsm_aMsgEntrant);
											strtoupper(pIndex1);
											if(strcmp(gsm_pCommande, pIndex1) == 0)
											{
												gsm_MAESMSEntrant = GSM_SMSENTRANT_DATA;
												configGSM_Read(gsm_InfoSMSEntrant.Data, gsm_aSMSEntrant, 18);
											}
											else
											{
												gsm_aSMSEntrant		= gsm_aMsgSortantL + strlen(configGSM_String(gsm_aMsgSortantL)) + 1;
												gsm_aMsgEntrant		= gsm_aSMSEntrant + 18;
												gsm_aMsgSortantE	= gsm_aMsgEntrant + strlen(configGSM_String(gsm_aMsgEntrant)) + 1;
												gsm_aMsgSortantL	= gsm_aMsgSortantE + strlen(configGSM_String(gsm_aMsgSortantE)) + 1;
												gsm_CptSMSEntrant++;
												break;
											}
										}
										else
										{
											gsm_MAESMSEntrant = GSM_SMSENTRANT_INIT;
											return 2;
										}


			case GSM_SMSENTRANT_DATA :	if(!comsbusmaitre_bit[0].bSbusLibre)
										{
											break;
										}
										comsbusmaitre_bit[0].bSbusLibre = 0;
										comsbusmaitre_bit[0].bSbusEnd = 0;
										comsbusmaitre_bit[0].SbusErr = 0;
										comsbusmaitre_SbusStation[0] = gsm_InfoSMSEntrant.Info.Station;
										comsbusmaitre_SbusAdresse[0][0].l = gsm_InfoSMSEntrant.Info.Adresse;
										comsbusmaitre_SbusNbr[0] = 1;

										if(gsmbit.bSMSEntrantEcriture) //Ecritue
										{
											//if(strlen(configGSM_String(gsm_aMsgSortantE)))
											{
												if(gsm_InfoSMSEntrant.Info.Type == 3)// Si Registre Long on lie la variable
												{
													gsm_MAESMSEntrant = GSM_SMSENTRANT_READ_SBUS;
													comsbusmaitre_SbusType[0] = 'R';
													comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
												}
												else //Registre Float et Date heure Flag
												{
													gsm_MAESMSEntrant = GSM_SMSENTRANT_WRITE_SBUS;
													switch(gsm_InfoSMSEntrant.Info.Type)
													{
														case 0 : 	comsbusmaitre_SbusBuffer[0][0].of.octet[0] = (char)(atoi(gsm_pParametre)!=0);
																	comsbusmaitre_SbusType[0] = 'F';
																	comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_FLG;
																 	break;
														case 1 : 	//comsbusmaitre_SbusBuffer[0][0] = (char)(atoi(gsm_pParametre)!=0);
																	comsbusmaitre_SbusType[0] = 'R';
																	comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_REG;
																 	break;
														case 2 : 	comsbusmaitre_SbusBuffer[0][0].d  = atof(gsm_pParametre);
																	comsbusmaitre_SbusBuffer[0][0].d -= gsm_InfoSMSEntrant.Info.Offset;
																	comsbusmaitre_SbusBuffer[0][0].d /= gsm_InfoSMSEntrant.Info.Coefficient;
																	comsbusmaitre_SbusType[0] = 'R';
																	comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_REG;
																 	break;
														case 4 :
																	strtoupper(gsm_pParametre);
																	comsbusmaitre_SbusType[0] = 'F';
																	comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_FLG;

																	gsm_InfoSMSEntrant.Info.Masque++;
																	if(!strcmp(gsm_pParametre, configGSM_String(gsm_InfoSMSEntrant.Info.Masque)))
																	{
																		comsbusmaitre_SbusBuffer[0][0].of.octet[0] = 0;
																	}
																	else
																	{
																		gsm_InfoSMSEntrant.Info.Masque += strlen(configGSM_String(gsm_InfoSMSEntrant.Info.Masque)) + 1;
																		if(!strcmp(gsm_pParametre, configGSM_String(gsm_InfoSMSEntrant.Info.Masque)))
																		{
																			comsbusmaitre_SbusBuffer[0][0].of.octet[0] = 1;
																		}
																		else
																		{
																			strcpy(gsm_ChaineTemps, "Erreur reference");
																			comsbusmaitre_bit[0].bSbusLibre = 1;
																			gsm_MAESMSEntrant = GSM_SMSENTRANT_INIT;
																			return 1;
																		}
																	}
																	break;
														case 5 :
																	strtoupper(gsm_pParametre);
																	comsbusmaitre_SbusType[0] = 'R';
																	comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_REG;

																	Cpt = configGSM_Char(gsm_InfoSMSEntrant.Info.Masque);

																	gsm_InfoSMSEntrant.Info.Masque++;
																	for(Cpt1 = 0; Cpt1 < Cpt; Cpt1++)
																	{
																		pIndex1 = configGSM_String(gsm_InfoSMSEntrant.Info.Masque);

																		if(!strcmp(gsm_pParametre, pIndex1))
																		{
																			comsbusmaitre_SbusBuffer[0][0].l = Cpt1;
																			break;
																		}
																		gsm_InfoSMSEntrant.Info.Masque += strlen(pIndex1) + 1;
																	}

																	if(Cpt1 == Cpt)
																	{
																		strcpy(gsm_ChaineTemps, "Erreur reference");
																		comsbusmaitre_bit[0].bSbusLibre = 1;
																		gsm_MAESMSEntrant = GSM_SMSENTRANT_INIT;
																		return 1;
																	}
																	break;
													}
												}
												//gsm_MAEOld = gsm_MAE;
												//gsm_MAE = GSM_ETAT_SBUS;
											}
											//else
											//{
											//	gsm_MAETraitementRq = GSM_TRAITEMENTRQ_REPONSE;
											//	comsbusmaitre_bit[0].bSbusLibre = 1;
											//}
										}
										else
										{
											if(strlen(configGSM_String(gsm_aMsgSortantL)))
											{
												gsm_MAESMSEntrant = GSM_SMSENTRANT_READ_SBUS;
												// Si Flag ou Ref flag
												if(gsm_InfoSMSEntrant.Info.Type == 0 || gsm_InfoSMSEntrant.Info.Type == 4)
												{
													comsbusmaitre_SbusType[0] = 'F';
													comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
												}
												else
												{
													comsbusmaitre_SbusType[0] = 'R';
													comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
												}
												//gsm_MAEOld = gsm_MAE;
												//gsm_MAE = GSM_ETAT_SBUS;
											}
											else
											{
												comsbusmaitre_bit[0].bSbusLibre = 1;
												gsm_MAESMSEntrant = GSM_SMSENTRANT_INIT;
												return 1;
											}
										}

										break;

			case GSM_SMSENTRANT_READ_SBUS :	if(comsbusmaitre_bit[0].bSbusEnd)
											{
												if(gsmbit.bSMSEntrantEcriture) //Ecritue
												{
													if(!comsbusmaitre_bit[0].SbusErr)
													{
														*(((double *)comsbusmaitre_SbusBuffer[0])+2) = atof(gsm_pParametre);
														*(((double *)comsbusmaitre_SbusBuffer[0])+2) -= gsm_InfoSMSEntrant.Info.Offset;
														*(((double *)comsbusmaitre_SbusBuffer[0])+2) /= gsm_InfoSMSEntrant.Info.Coefficient;
														*(((long *)comsbusmaitre_SbusBuffer[0])+1) = (long)*(((double *)comsbusmaitre_SbusBuffer[0])+2);

														*((long *)comsbusmaitre_SbusBuffer[0]) &= ~gsm_InfoSMSEntrant.Info.Masque;
														*(((long *)comsbusmaitre_SbusBuffer[0])+1) <<= gsm_InfoSMSEntrant.Info.Decalage;
														*((long *)comsbusmaitre_SbusBuffer[0]) |= *(((long *)comsbusmaitre_SbusBuffer[0])+1);
														comsbusmaitre_SbusType[0] = 'R';
														comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_REG;
														//gsm_MAEOld = gsm_MAE;
														//gsm_MAE = GSM_ETAT_SBUS;
													}
													comsbusmaitre_bit[0].bSbusLibre = 0;
													comsbusmaitre_bit[0].bSbusEnd = 0;
													comsbusmaitre_bit[0].SbusErr = 0;
													comsbusmaitre_SbusStation[0] = gsm_InfoSMSEntrant.Info.Station;
													comsbusmaitre_SbusAdresse[0][0].l = gsm_InfoSMSEntrant.Info.Adresse;
													comsbusmaitre_SbusNbr[0] = 1;
													gsm_MAESMSEntrant = GSM_SMSENTRANT_WRITE_SBUS;
												}
												else
												{
													sprintf(gsm_ChaineTemps, "%s", configGSM_String(gsm_aMsgSortantL));
													if(comsbusmaitre_bit[0].SbusErr)
													{
														strcat(gsm_ChaineTemps, "???");
													}
													else
													{
														switch(gsm_InfoSMSEntrant.Info.Type)
														{
															case 0 : 	sprintf(gsm_ChaineTemps + strlen(gsm_ChaineTemps), "%d", comsbusmaitre_SbusBuffer[0][0].of.octet[0]);
																	 	break;
															case 1 : 	pSTime = localtime(&(comsbusmaitre_SbusBuffer[0][0].t));
																		sprintf(gsm_ChaineTemps + strlen(gsm_ChaineTemps), "%02d:%02d:%02d %02d/%02d/%04d", pSTime->tm_hour, pSTime->tm_min,	pSTime->tm_sec, pSTime->tm_mday, pSTime->tm_mon + 1, pSTime->tm_year + 1900);
																	 	break;
															case 2 : 	comsbusmaitre_SbusBuffer[0][0].d *= gsm_InfoSMSEntrant.Info.Coefficient;
																		comsbusmaitre_SbusBuffer[0][0].d  += gsm_InfoSMSEntrant.Info.Offset;
																		sprintf(gsm_ChaineTemps + strlen(gsm_ChaineTemps), "%.*f", gsm_InfoSMSEntrant.Info.Precision, comsbusmaitre_SbusBuffer[0][0].d);
																	 	break;
															case 3 :

																		comsbusmaitre_SbusBuffer[0][0].l &= gsm_InfoSMSEntrant.Info.Masque;
																		comsbusmaitre_SbusBuffer[0][0].l >>= gsm_InfoSMSEntrant.Info.Decalage;
																		comsbusmaitre_SbusBuffer[0][1].d = ((double)comsbusmaitre_SbusBuffer[0][0].l) * gsm_InfoSMSEntrant.Info.Coefficient;
																		comsbusmaitre_SbusBuffer[0][1].d += gsm_InfoSMSEntrant.Info.Offset;
																		sprintf(gsm_ChaineTemps + strlen(gsm_ChaineTemps), "%.*f", gsm_InfoSMSEntrant.Info.Precision, comsbusmaitre_SbusBuffer[0][1].d);
																	 	break;

															case 4 : 	gsm_InfoSMSEntrant.Info.Masque++;
																		if(comsbusmaitre_SbusBuffer[0][0].of.octet[0])
																		{
																			gsm_InfoSMSEntrant.Info.Masque += strlen(configGSM_String(gsm_InfoSMSEntrant.Info.Masque)) + 1;
																		}
																		strcat(gsm_ChaineTemps, configGSM_String(gsm_InfoSMSEntrant.Info.Masque));
																		break;

															case 5 : 	Cpt = gsm_InfoSMSEntrant.Info.Masque;
																		if(comsbusmaitre_SbusBuffer[0][0].l < configGSM_Char(gsm_InfoSMSEntrant.Info.Masque))
																		{
																			gsm_InfoSMSEntrant.Info.Masque++;
																			for(Cpt1 = 0; Cpt1 < comsbusmaitre_SbusBuffer[0][0].l; Cpt1++)
																			{
																				gsm_InfoSMSEntrant.Info.Masque += strlen(configGSM_String(gsm_InfoSMSEntrant.Info.Masque)) + 1;
																			}
																			strcat(gsm_ChaineTemps, configGSM_String(gsm_InfoSMSEntrant.Info.Masque));
																		}
																		else
																		{
																			strcat(gsm_ChaineTemps, "???");
																		}
																		break;
															case 6 : 	sprintf(gsm_ChaineTemps, "%ld:%02d", comsbusmaitre_SbusBuffer[0][0].l/60,	(int)(comsbusmaitre_SbusBuffer[0][0].l%60));
																		break;

															default:	gsm_ChaineTemps[0] = 0;
																		break;
														}
													}
													comsbusmaitre_bit[0].bSbusLibre = 1;
													gsm_MAESMSEntrant = GSM_SMSENTRANT_INIT;
													return 1;
												}
											}
											break;

			case GSM_SMSENTRANT_WRITE_SBUS :
											if(comsbusmaitre_bit[0].bSbusEnd)
											{
												comsbusmaitre_bit[0].bSbusLibre = 1;
												gsm_ChaineTemps[0] = 0;
												if(strlen(configGSM_String(gsm_aMsgSortantE)))
												{
													if(comsbusmaitre_bit[0].SbusErr)
													{
														strcpy(gsm_ChaineTemps, "Erreur ecriture");
													}
													else
													{
														strcpy(gsm_ChaineTemps, configGSM_String(gsm_aMsgSortantE));
													}
												}
												gsm_MAESMSEntrant = GSM_SMSENTRANT_INIT;
												return 1;
											}
											break;

		}
	}
	else
	{
		return 2;
	}
	return 0;
}


//----------------------------------------
// extraction de la valeur a ecrire
//----------------------------------------
int gsm_DateHeure()
{
	struct tm	*pSTime;
	char *pIndex1;
	char *pIndex2;
	char Cpt;

	switch(gsm_MAEDateHeure)
	{
		case GSM_DH_INIT :
		{
			if(comsbusmaitre_bit[0].bSbusLibre)
			{
				comsbusmaitre_bit[0].bSbusLibre = 0;
				comsbusmaitre_bit[0].bSbusEnd = 0;
				comsbusmaitre_bit[0].SbusErr = 0;

				comsbusmaitre_SbusStation[0] = atoi(gsm_pCommande + 2);
				comsbusmaitre_SbusType[0] = 'R';
				comsbusmaitre_SbusNbr[0] = 1;
				comsbusmaitre_SbusAdresse[0][0].l = 0;
				comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
				gsm_MAEDateHeure = GSM_DH_REPONSE;
			}
			break;
		}
		case GSM_DH_REPONSE :
		{
			if(comsbusmaitre_bit[0].bSbusEnd)
			{
				comsbusmaitre_bit[0].bSbusLibre = 1;
				if(!comsbusmaitre_bit[0].SbusErr)
				{
					pSTime = localtime(&(comsbusmaitre_SbusBuffer[0][0].l));
					if(gsmbit.bSMSEntrantEcriture)
					{
						pIndex1 = gsm_pParametre;

						for(Cpt = 0; Cpt < 6; Cpt++)
						{
							pIndex2 = strchr(pIndex1, ' ');
							if(pIndex2)
							{
								*pIndex2 = 0;
								pIndex2++;
							}

							switch(Cpt)
							{
								case 0:
								{
									pSTime->tm_hour = atoi(pIndex1); // minute
									break;
								}
								case 1:
								{
									pSTime->tm_min = atoi(pIndex1); // minute
									break;
								}
								case 2:
								{
									pSTime->tm_sec = atoi(pIndex1); // minute
									break;
								}
								case 3:
								{
									pSTime->tm_mday = atoi(pIndex1); // minute
									break;
								}
								case 4:
								{
									pSTime->tm_mon = atoi(pIndex1)-1; // minute
									break;
								}
								case 5:
								{
									pSTime->tm_year = atoi(pIndex1)-1900; // minute
									break;
								}
							}

							if(!pIndex2) break;

							pIndex1 = pIndex2;
						}

						comsbusmaitre_SbusBuffer[0][0].t = mktime(pSTime);

						comsbusmaitre_bit[0].bSbusLibre = 0;
						comsbusmaitre_bit[0].bSbusEnd = 0;
						comsbusmaitre_bit[0].SbusErr = 0;
						comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_REG;
						gsm_MAEDateHeure = GSM_DH_REPONSE_ECRITURE;
						break;

					}
					else
					{
						sprintf(gsm_ChaineTemps, "Date et heure Station %d est %02d:%02d:%02d %02d/%02d/%04d", comsbusmaitre_SbusStation[0], pSTime->tm_hour, pSTime->tm_min, pSTime->tm_sec, pSTime->tm_mday, pSTime->tm_mon + 1, pSTime->tm_year + 1900);
					}
				}
				else
				{
					sprintf(gsm_ChaineTemps, "Erreur lecture Date et Heure Station %d", comsbusmaitre_SbusStation[0]);
				}
				gsm_MAEDateHeure = GSM_DH_INIT;
				return 1;
			}
			break;
		}
		case GSM_DH_REPONSE_ECRITURE :
		{
			if(comsbusmaitre_bit[0].bSbusEnd)
			{
				comsbusmaitre_bit[0].bSbusLibre = 1;
				if(!comsbusmaitre_bit[0].SbusErr)
				{
					sprintf(gsm_ChaineTemps, "Date et heure Station %d sont modifies", comsbusmaitre_SbusStation[0]);
				}
				else
				{
					sprintf(gsm_ChaineTemps, "Erreur modification Date et Heure Station %d", comsbusmaitre_SbusStation[0]);
				}
				return 1;
			}
			break;
		}
	}
	return 0;
}

int gsm_Sbus()
{
	char pTempData[9];
	char Cpt;

	switch(gsm_MAESbus)
	{
		case GSM_SBUS_INIT :
		{
			if(comsbusmaitre_bit[0].bSbusLibre)
			{
				comsbusmaitre_bit[0].bSbusLibre = 0;
				comsbusmaitre_bit[0].bSbusEnd = 0;
				comsbusmaitre_bit[0].SbusErr = 0;

				strncpy(pTempData, gsm_pParametre, 2);
				pTempData[2] = 0;
				comsbusmaitre_SbusStation[0] = xtoi(pTempData);
				gsm_pParametre += 2;

				// Type
				strncpy(pTempData, gsm_pParametre, 2);
				pTempData[2] = 0;
				comsbusmaitre_SbusRq[0] = xtoi(pTempData);
				gsm_pParametre += 2;

				// Nombre
				strncpy(pTempData, gsm_pParametre, 2);
				pTempData[2] = 0;
				comsbusmaitre_SbusNbr[0] = xtoi(pTempData);
				gsm_pParametre += 2;

				// Adresse
				strncpy(pTempData, gsm_pParametre, 4);
				pTempData[4] = 0;
				comsbusmaitre_SbusAdresse[0][0].l = xtoi(pTempData);
				gsm_pParametre += 4;

				switch(comsbusmaitre_SbusRq[0])
				{
					case SLVXSBUS_RQ_READ_REG :
					{
						comsbusmaitre_SbusType[0] = 'R';
						comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
						break;
					}
					case SLVXSBUS_RQ_WRITE_REG :
					{
						// Valeur
						for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[0]; Cpt++)
						{
							strncpy(pTempData, gsm_pParametre, 8);
							pTempData[8] = 0;
							comsbusmaitre_SbusBuffer[0][Cpt].l = xtoi(pTempData);
							gsm_pParametre += 8;
						}
						comsbusmaitre_SbusType[0] = 'R';
						comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_REG;
						break;
					}
					case SLVXSBUS_RQ_READ_FLAG :
					{
						comsbusmaitre_SbusType[0] = 'F';
						comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
						break;
					}
					case SLVXSBUS_RQ_WRITE_FLAG :
					{
						for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[0]; Cpt++)
						{
							*(((char *)comsbusmaitre_SbusBuffer[0]) + Cpt) = gsm_pParametre[Cpt] == '0'?0:1;
						}

						comsbusmaitre_SbusType[0] = 'F';
						comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_WRITE_FLG;
						break;
					}
					case SLVXSBUS_RQ_RESET		:
					case SLVXSBUS_RQ_READ_INPUT	:
					case SLVXSBUS_RQ_READ_OUTPUT	:
					case SLVXSBUS_RQ_READ_TIMER	:
					case SLVXSBUS_RQ_READ_STATUS	:
					case SLVXSBUS_RQ_READ_MEM	:
					case SLVXSBUS_RQ_WRITE_OUTPUT:
					case SLVXSBUS_RQ_WRITE_TIMER	:
					case SLVXSBUS_RQ_WRITE_MEM	:
					case SLVXSBUS_RQ_RUN			:
					case SLVXSBUS_RQ_STOP		:
					default					:   strcpy(gsm_ChaineTemps, "RSBUS REQUETTE ERREUR");
												comsbusmaitre_bit[0].bSbusLibre = 1;
												return 1;
				}

				gsm_MAESbus = GSM_SBUS_REPONSE;
			}
			break;
		}
		case GSM_SBUS_REPONSE :
		{
			if(comsbusmaitre_bit[0].bSbusEnd)
			{
				comsbusmaitre_bit[0].bSbusLibre = 1;
				if(!comsbusmaitre_bit[0].SbusErr)
				{
					strcpy(gsm_ChaineTemps, "RSBUS ");
					switch(comsbusmaitre_SbusRq[0])
					{
						case SLVXSBUS_RQ_READ_REG :
						{
							for(Cpt = 0; Cpt < comsbusmaitre_SbusNbr[0]; Cpt++)
							{
								sprintf(gsm_ChaineTemps + 6 + (Cpt*8), "%08lX", comsbusmaitre_SbusBuffer[0][Cpt].l);
							}
							break;
						}
						case SLVXSBUS_RQ_WRITE_REG :
						{
							strcat(gsm_ChaineTemps, "06");
							break;
						}
						case SLVXSBUS_RQ_READ_FLAG :
						{
							for(Cpt = 0; Cpt < comsbusmaitre_SbusNbr[0]; Cpt++)
							{
								sprintf(gsm_ChaineTemps + 6 + Cpt, "%d", comsbusmaitre_SbusBuffer[0][Cpt/4].c[Cpt%4]);
							}
							break;
						}
						case SLVXSBUS_RQ_WRITE_FLAG :
						{
							strcat(gsm_ChaineTemps, "06");
							break;
						}
						case SLVXSBUS_RQ_RESET		:
						case SLVXSBUS_RQ_READ_INPUT	:
						case SLVXSBUS_RQ_READ_OUTPUT	:
						case SLVXSBUS_RQ_READ_TIMER	:
						case SLVXSBUS_RQ_READ_STATUS	:
						case SLVXSBUS_RQ_READ_MEM	:
						case SLVXSBUS_RQ_WRITE_OUTPUT:
						case SLVXSBUS_RQ_WRITE_TIMER	:
						case SLVXSBUS_RQ_WRITE_MEM	:
						case SLVXSBUS_RQ_RUN			:
						case SLVXSBUS_RQ_STOP		:
						default					:
						{
							strcpy(gsm_ChaineTemps, "RSBUS REQUETTE ERREUR");
							return 1;
						}
					}
				}
				else
				{
					strcpy(gsm_ChaineTemps, "RSBUS ERREUR");
				}
				gsm_MAESbus = GSM_SBUS_INIT;
				return 1;
			}
			break;
		}
	}
	return 0;
}

char gsm_TraitementSMS()
{
	unsigned int aConfig;
	char *pIndex1;
	int Index;
	int Cpt;
	ofsld Valeur;
	struct tm	*pSTime;

	switch(gsm_MAETraittementSMS)
	{
		case GSM_SMS_START:
		{
			*gsm_ReponseSMS = 0;
			gsm_pCommande = strtok(gsm_MessageSMS, "#");
			if(!gsm_pCommande)
			{
				return 1;
			}
			gsm_MAETraittementSMS = GSM_SMS_GET_CMD;
		}
		case GSM_SMS_GET_CMD:
		{
			*gsm_ChaineTemps = 0;

			gsm_pCommande = strtrim(gsm_pCommande);

			if(*gsm_pCommande == 0)
			{
				gsm_MAETraittementSMS = GSM_SMS_REPONSE;
				return 0;
			}

			gsm_pParametre = strchr(gsm_pCommande, ' ');
			if(gsm_pParametre)
			{
				*gsm_pParametre = 0;
				gsm_pParametre++;
                gsm_pParametre = strtrim(gsm_pParametre);
				gsmbit.bSMSEntrantEcriture = 1;
			}
			else
			{
				gsmbit.bSMSEntrantEcriture = 0;
			}
			strtoupper(gsm_pCommande);
			gsm_MAETraittementSMS = GSM_SMS_TRAITTEMENT;
			gsm_MAERequette = GSM_REQUETTE_RESET;
		}
		case GSM_SMS_TRAITTEMENT:
		{
			switch(gsm_MAERequette)
			{
				case GSM_REQUETTE_RESET:
				{
					if(strcmp(gsm_pCommande, "RESET") == 0)
					{
						strcpy(gsm_ChaineTemps, "Reset OK");
						gsmbit.bDemandeReset = 1;
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_DH_COMPILE;
				}
				case GSM_REQUETTE_DH_COMPILE:
				{
					if(strcmp(gsm_pCommande, "DCOMPIL") == 0)
					{
						Valeur.l = GetlReg(RNV_DATE_COMPILE);
						pSTime = localtime(&Valeur.l);
						sprintf(gsm_ChaineTemps, "Date compilation est %02d:%02d:%02d %02d/%02d/%04d", pSTime->tm_hour, pSTime->tm_min, pSTime->tm_sec, pSTime->tm_mday, pSTime->tm_mon + 1, pSTime->tm_year + 1900);
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_VERSION;
				}
				case GSM_REQUETTE_VERSION:
				{
					if(strcmp(gsm_pCommande, "VERSION") == 0)
					{
						sprintf(gsm_ChaineTemps,"Version : %.4f", GetfReg(RNV_APPLICATION_VERSION));
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_QGSM;
				}
				case GSM_REQUETTE_QGSM:
				{
					if(strcmp(gsm_pCommande, "QGSM") == 0)
					{
						sprintf(gsm_ChaineTemps,"Qualite GSM = %d%%", gsm_QualiteReseau);
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_DH;
				}
				case GSM_REQUETTE_DH:
				{
					if(strncmp(gsm_pCommande, "DH", 2) == 0)
					{
						if(!gsm_DateHeure())
						{
							return 0;
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_STA;
				}
                case GSM_REQUETTE_STA:
				{
					if(strcmp(gsm_pCommande, "STA") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							configGSM_Write(gsm_pParametre, gsm_aNomStation, 24);
							strcpy(gsm_ChaineTemps, "Nom de la station est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps,"Nom de la station est : %s", gsm_NomStation());
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_NT;
				}
				case GSM_REQUETTE_NT:
				{
					//gsm_aNumTelephone
					for(Cpt=1; Cpt < 5; Cpt++)
                    {
                        sprintf(gsm_ChaineTemps, "NT%d", Cpt);
                        if(strncmp(gsm_pCommande, gsm_ChaineTemps, 3) == 0)
                        {
                            break;
                        }
                    }
                    gsm_ChaineTemps[0] = 0;

                    if (Cpt < 5 || (strcmp(gsm_pCommande, "NT") == 0)) {
                        // Ecriture de l'adresse IP
                        if (gsmbit.bSMSEntrantEcriture) {
                            if(gsm_IsValideTelephone(gsm_pParametre) || gsm_pParametre[0] == '0')
                            {
                                configGSM_Write(gsm_pParametre, gsm_aNumTelephone[Cpt - 1], 24);
                                pIndex1 = configGSM_String(gsm_aNumTelephone[0]);
                                sprintf(gsm_ChaineTemps, "N1 : %s\r\nN2 : %s\r\nN3 : %s\r\nN4 : %s\r\n", pIndex1, pIndex1+25, pIndex1+50, pIndex1+75);
                            }
                            else
                            {
                                strcpy(gsm_ChaineTemps, "Num Telephone invalide");
                            }
                        } else {
                            pIndex1 = configGSM_String(gsm_aNumTelephone[0]);
                            sprintf(gsm_ChaineTemps, "N1 : %s\r\nN2 : %s\r\nN3 : %s\r\nN4 : %s\r\n", pIndex1, pIndex1+25, pIndex1+50, pIndex1+75);
                        }
                        break;
                    }
					gsm_MAERequette = GSM_REQUETTE_ADMV;
				}
				case GSM_REQUETTE_ADMV:
				{
					if(strcmp(gsm_pCommande, "ADMV") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetFlag(FNV_MODE_VIELLE, atoi(gsm_pParametre)?1:0);
							SetTimer(TIMER_REVEILLE_SLEEP, GetlReg(RNV_TEMPS_REVEILLE_MAX)*600);
							sprintf(gsm_ChaineTemps, "Le mode veille est %s", GetFlag(FNV_MODE_VIELLE)?"active":"desactive");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Le mode veille est %s", GetFlag(FNV_MODE_VIELLE)?"Actif":"Inactif");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_TRV;
				}
				case GSM_REQUETTE_TRV:
				{
					if(strcmp(gsm_pCommande, "TRV") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_TEMPS_REVEILLE_MAX, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "Le temps reveille est modifiee");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Le temps reveille est %ld mn", GetlReg(RNV_TEMPS_REVEILLE_MAX));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_GPRS;
				}
				case GSM_REQUETTE_GPRS:
				{
					if(strcmp(gsm_pCommande, "GPRS") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)//GetFlag(FNV_GSM_MODE_GPRS)
						{
							SetFlag(FNV_GSM_MODE_GPRS, atoi(gsm_pParametre)?1:0);
							SetTimer(TIMER_REVEILLE_SLEEP, GetlReg(RNV_TEMPS_REVEILLE_MAX)*600);
							sprintf(gsm_ChaineTemps, "Le module GPRS est %s", GetFlag(FNV_GSM_MODE_GPRS)?"active":"desactive");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Le module GPRS est %s", GetFlag(FNV_GSM_MODE_GPRS)?"Actif":"Inactif");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_HENV;
				}
				case GSM_REQUETTE_HENV:
				{
                    for(Cpt=1; Cpt < 5; Cpt++)
                    {
                        sprintf(gsm_ChaineTemps, "H%dENV", Cpt);
                        if(strncmp(gsm_pCommande, gsm_ChaineTemps, 5) == 0)
                        {
                            break;
                        }

                    }
                    gsm_ChaineTemps[0] = 0;
                    if(Cpt < 5)
					{
						// Ecriture des 4 N° des téléphones
						if(gsmbit.bSMSEntrantEcriture)
						{
                            SetlReg(RNV_SEUIL1_ENVOI_UDPH + Cpt - 1, atoi(gsm_pParametre));
							sprintf(gsm_ChaineTemps, "Heure envoi UDPH %d modifie", Cpt);
						}
						else
						{
                            sprintf(gsm_ChaineTemps, "Heure envoi UDPH %d est %d", Cpt, GetlReg(RNV_SEUIL1_ENVOI_UDPH + Cpt - 1));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_TENV;
				}
				case GSM_REQUETTE_TENV:
				{
                    for(Cpt=1; Cpt < 5; Cpt++)
                    {
                        sprintf(gsm_ChaineTemps, "T%dENV", Cpt);
                        if(strncmp(gsm_pCommande, gsm_ChaineTemps, 5) == 0)
                        {
                            break;
                        }

                    }
                    gsm_ChaineTemps[0] = 0;
                    if(Cpt < 5)
					{
						// Ecriture des 4 N° des téléphones
						if(gsmbit.bSMSEntrantEcriture)
						{
                            SetlReg(RNV_SEUIL1_NBR_SAVE_SEND_UDPH + Cpt - 1, atoi(gsm_pParametre));
							sprintf(gsm_ChaineTemps, "Taille envoi UDPH %d modifie", Cpt);
						}
						else
						{
                            sprintf(gsm_ChaineTemps, "Taille envoi UDPH %d est %d", Cpt, GetlReg(RNV_SEUIL1_NBR_SAVE_SEND_UDPH + Cpt - 1));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_MSA;
				}
				case GSM_REQUETTE_MSA:
				{
					if(strncmp(gsm_pCommande, "MSA", 3) == 0)
					{
						Index = atoi(gsm_pCommande + 3);

						if(Index > 0 && Index <= gsm_NbrAlarme && Index <= 50)
						{
							aConfig = gsm_aInitAlarme + 6;
							for(Cpt=1; Cpt < Index; Cpt++)
							{
								aConfig += strlen(configGSM_String(aConfig)) + 1;
								aConfig += 6;
							}
							sprintf(gsm_ChaineTemps, "Message Alarme %d est %s", Cpt, configGSM_String(aConfig));
						}
						else
						{
							strcpy(gsm_ChaineTemps, "Erreur numero alarme");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_CTA;
				}
				case GSM_REQUETTE_CTA:
				{
					if(strncmp(gsm_pCommande, "CTA", 3) == 0)
					{
						Index = atoi(gsm_pCommande + 3);

						if(Index > 0 && Index <= gsm_NbrAlarme && Index <= 50)
						{
							aConfig = gsm_aInitAlarme + 5;
							for(Cpt=1; Cpt < Index; Cpt++)
							{
								aConfig += 1;
								aConfig += strlen(configGSM_String(aConfig)) + 1;
								aConfig += 5;
							}
							if(gsmbit.bSMSEntrantEcriture)
							{
								Valeur.c[0] = atoi(gsm_pParametre);
								configGSM_Write(Valeur.c, aConfig, 1);
								sprintf(gsm_ChaineTemps, "La configuration du choix des telephones et du mode de l'alarme %d est modifiee", Cpt);
							}
							else
							{
								sprintf(gsm_ChaineTemps, "La configuration du choix des telephones et du mode de l'alarme %d est %d", Cpt, configGSM_Char(aConfig));
							}
						}
						else
						{
							strcpy(gsm_ChaineTemps, "Erreur numero alarme");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_IA;
				}
				case GSM_REQUETTE_IA:
				{
					if(strncmp(gsm_pCommande, "IA", 2) == 0)
					{
						Index = atoi(gsm_pCommande + 2);

						if(Index > 0 && Index <= gsm_NbrAlarme && Index <= 50)
						{
							aConfig = gsm_aInitAlarme;
							for(Cpt=1; Cpt < Index; Cpt++)
							{
								aConfig += 6;
								aConfig += strlen(configGSM_String(aConfig)) + 1;
							}
							if(gsmbit.bSMSEntrantEcriture)
							{
								Valeur.us[0] = atoi(gsm_pParametre);
								configGSM_Write(Valeur.c, aConfig, 2);
								sprintf(gsm_ChaineTemps, "L'intervalle d'emission de l'alarme %d est modifie", Cpt);
							}
							else
							{
								sprintf(gsm_ChaineTemps, "L'intervalle d'emission de l'alarme %d = %d mn", Cpt, configGSM_Short(aConfig));
							}
						}
						else
						{
							strcpy(gsm_ChaineTemps, "Erreur numero alarme");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_AAX;
				}
				case GSM_REQUETTE_AAX:
				{
					if(strcmp(gsm_pCommande, "AAX") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							gsm_EntetAlarme.cESMSA.ActiveDistant = atoi(gsm_pParametre)?1:0;
							configGSM_Write(&gsm_EntetAlarme.cEntetSMSAlarme, gsm_aInitEntetAlarme, 1);
							sprintf(gsm_ChaineTemps, "Les alarmes externes sont %s", gsm_EntetAlarme.cESMSA.ActiveDistant?"activees":"desactivees");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Les alarmes externes sont %s", gsm_EntetAlarme.cESMSA.ActiveDistant?"actives":"inactives");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_IIA;
				}
				case GSM_REQUETTE_IIA:
				{
					if(strcmp(gsm_pCommande, "IIA") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							gsm_EntetAlarme.cESMSA.IntervalVerife = atoi(gsm_pParametre);
							configGSM_Write(&gsm_EntetAlarme.cEntetSMSAlarme, gsm_aInitEntetAlarme, 1);
							strcpy(gsm_ChaineTemps, "L'intervalle d'interrogation des alarmes est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "L'intervalle d'interrogation des alarmes = %d mn", gsm_EntetAlarme.cESMSA.IntervalVerife);
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_VNT;
				}
				case GSM_REQUETTE_VNT:
				{
					if(strcmp(gsm_pCommande, "VNT") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetFlag(FNV_GSM_VERIF_NUM_TELE, atoi(gsm_pParametre));
							sprintf(gsm_ChaineTemps, "La verification des numeros telephone est %s", GetFlag(FNV_GSM_VERIF_NUM_TELE)?"activee":"desactivee");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "La verification des numeros telephone est %s", GetFlag(FNV_GSM_VERIF_NUM_TELE)?"active":"inactive");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_NSTA;
				}
				case GSM_REQUETTE_NSTA:
				{
					if(strcmp(gsm_pCommande, "NSTA") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_NUM_STATION, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "Numeros de la station est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Numeros de la station est %ld", GetlReg(RNV_NUM_STATION));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_MAXSMSH;
				}
				case GSM_REQUETTE_MAXSMSH:
				{
					if(strcmp(gsm_pCommande, "MAXSMSH") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_NBR_MAX_SMS_HEURE, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "Max SMS heure est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Max SMS heure est %ld", GetlReg(RNV_NBR_MAX_SMS_HEURE));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_MAXSMSJ;
				}
				case GSM_REQUETTE_MAXSMSJ:
				{
					if(strcmp(gsm_pCommande, "MAXSMSJ") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_NBR_MAX_SMS_JOURS, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "Max SMS jours est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Max SMS jours est %ld", GetlReg(RNV_NBR_MAX_SMS_JOURS));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_AAI;
				}
				case GSM_REQUETTE_AAI:
				{
					if(strcmp(gsm_pCommande, "AAI") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							gsm_EntetAlarme.cESMSA.ActiveLocal = atoi(gsm_pParametre)?1:0;
							configGSM_Write(&gsm_EntetAlarme.cEntetSMSAlarme, gsm_aInitEntetAlarme, 1);
							sprintf(gsm_ChaineTemps, "Les alarmes internes sont %s", gsm_EntetAlarme.cESMSA.ActiveLocal?"activees":"desactivees");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "Les alarmes internes sont %s", gsm_EntetAlarme.cESMSA.ActiveLocal?"actives":"inactives");
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_TER;
				}
				case GSM_REQUETTE_TER:
				{
					if(strcmp(gsm_pCommande, "TER") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_TEMPO_ENVOI_RAPPORT, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "La frequence d'envoi rapport est modifiee");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "La frequence d'envoi rapport est %ld mn", GetlReg(RNV_TEMPO_ENVOI_RAPPORT));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_VCU;
				}
				case GSM_REQUETTE_VCU:
				{
					if(strcmp(gsm_pCommande, "VCU") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_VITESSE_UART2, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "La vitesse de communication est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "La vitesse de communication est %ld", GetlReg(RNV_VITESSE_UART2));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_MCU;
				}
				case GSM_REQUETTE_MCU:
				{
					if(strcmp(gsm_pCommande, "MCU") == 0)
					{
						if(gsmbit.bSMSEntrantEcriture)
						{
							SetlReg(RNV_MODE_UART2, atoi(gsm_pParametre));
							strcpy(gsm_ChaineTemps, "La mode de communication est modifie");
						}
						else
						{
							sprintf(gsm_ChaineTemps, "La mode de communication est %ld", GetlReg(RNV_MODE_UART2));
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_ATCP;
				}
                case GSM_REQUETTE_ATCP	 :
                {
//                    if (strcmp(gsm_pCommande, "ATCP") == 0) //
//                    {
//                        if (gsmbit.bSMSEntrantEcriture) {
//                            SetFlag(FNV_MODE_TCP, atoi(gsm_pParametre));
//                            sprintf(gsm_ChaineTemps, "Le mode TCP est %s", GetFlag(FNV_MODE_TCP) ? "active" : "desactive");
//                        } else {
//                            sprintf(gsm_ChaineTemps, "Le mode TCP est %s", GetFlag(FNV_MODE_TCP) ? "Actif" : "Inactif");
//                        }
//                        break;
//                    }
                    gsm_MAERequette = GSM_REQUETTE_AUDPR;
                }

                case GSM_REQUETTE_AUDPR:
                {
                    if (strcmp(gsm_pCommande, "AUDPR") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetFlag(FNV_MODE_UDPR, atoi(gsm_pParametre));
                            sprintf(gsm_ChaineTemps, "Le mode UDP_R est %s", GetFlag(FNV_MODE_UDPR) ? "active" : "desactive");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le mode UDP_R est %s", GetFlag(FNV_MODE_UDPR) ? "Actif" : "Inactif");
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_AUDPH;
                }
                case GSM_REQUETTE_AUDPH:
                {
                    if (strcmp(gsm_pCommande, "AUDPH") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetFlag(FNV_MODE_UDPH, atoi(gsm_pParametre));
                            sprintf(gsm_ChaineTemps, "Le mode UDP_H est %s", GetFlag(FNV_MODE_UDPH) ? "active" : "desactive");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le mode UDP_H est %s", GetFlag(FNV_MODE_UDPH) ? "Actif" : "Inactif");
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_IPTCP;
                }
                case GSM_REQUETTE_IPTCP:
                {
                    if (strcmp(gsm_pCommande, "IPTCP") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_NUM_ADD_IP_TCP, atoi(gsm_pParametre) - 1);
                            strcpy(gsm_ChaineTemps, "Le numero de l'adresse IP TCP est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le TCP utilise l'adresse IP %ld", GetlReg(RNV_NUM_ADD_IP_TCP) + 1);
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_IPUDPR;
                }
                case GSM_REQUETTE_IPUDPR:
                {
                    if (strcmp(gsm_pCommande, "IPUDPR") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_NUM_ADD_IP_UDPR, atoi(gsm_pParametre) - 1);
                            strcpy(gsm_ChaineTemps, "Le numero de l'adresse IP UDP_R est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le UDP_R utilise l'adresse IP %ld", GetlReg(RNV_NUM_ADD_IP_UDPR) + 1);
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_IPUDPH;
                }
                case GSM_REQUETTE_IPUDPH:
                {
                    if (strcmp(gsm_pCommande, "IPUDPH") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_NUM_ADD_IP_UDPH, atoi(gsm_pParametre) - 1);
                            strcpy(gsm_ChaineTemps, "Le numero de l'adresse IP UDP_H est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le UDP_H utilise l'adresse IP %ld", GetlReg(RNV_NUM_ADD_IP_UDPH) + 1);
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_TCPPORT;
                }
                case GSM_REQUETTE_TCPPORT:
                {
                    if (strcmp(gsm_pCommande, "TCPPORT") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            Valeur.us[0] = atoi(gsm_pParametre);
                            configGSM_Write(Valeur.c, gsm_aTCPPort, 2);
                            strcpy(gsm_ChaineTemps, "Le port TCP est modifie");
                        }                            // Ecriture du port TCP
                        else {
                            sprintf(gsm_ChaineTemps, "Le Port TCP est : %d", configGSM_Short(gsm_aTCPPort));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_UDPRPORT;
                }
                case GSM_REQUETTE_UDPRPORT:
                {
                    if (strcmp(gsm_pCommande, "UDPRPORT") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            Valeur.us[0] = atoi(gsm_pParametre);
                            configGSM_Write(Valeur.c, gsm_aUDPRPort, 2);
                            strcpy(gsm_ChaineTemps, "Le port UDP R est modifie");
                        }                            // Ecriture du port UDP
                        else {
                            sprintf(gsm_ChaineTemps, "Le Port UDP R est : %d", configGSM_Short(gsm_aUDPRPort));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_UDPHPORT;
                }
                case GSM_REQUETTE_UDPHPORT:
                {
                    if (strcmp(gsm_pCommande, "UDPHPORT") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            Valeur.us[0] = atoi(gsm_pParametre);
                            configGSM_Write(Valeur.c, gsm_aUDPHPort, 2);
                            strcpy(gsm_ChaineTemps, "Le port UDP H est modifie");
                        }                            // Ecriture du port UDP
                        else {
                            sprintf(gsm_ChaineTemps, "Le Port UDP H est : %d", configGSM_Short(gsm_aUDPHPort));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_FEUDPR;
                }
                case GSM_REQUETTE_FEUDPR:
                {
                    if (strcmp(gsm_pCommande, "FEUDPR") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_TEMPO_SEND_UDPR, atol(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "La frequence d'envoi UDP_R est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "La frequence d'envoi UDP_R est %ld mn", GetlReg(RNV_TEMPO_SEND_UDPR));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_FE2UDPR;
                }
                case GSM_REQUETTE_FE2UDPR:
                {
                    if (strcmp(gsm_pCommande, "FE2UDPR") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_TEMPO2_SEND_UDPR, atol(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "La frequence d'envoi 2 UDP_R est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "La frequence d'envoi 2 UDP_R est %ld mn", GetlReg(RNV_TEMPO2_SEND_UDPR));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_FEUDPH;
                }
                case GSM_REQUETTE_FEUDPH:
                {
                    if (strcmp(gsm_pCommande, "FEUDPH") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_NBR_SAVE_SEND_UDPH, atol(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Le nombre de sauvegarde par envoi UDP_H est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le nombre de sauvegarde par envoi UDP_H est %ld", GetlReg(RNV_NBR_SAVE_SEND_UDPH));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_HDUDPR;
                }
                case GSM_REQUETTE_HDUDPR:
                {
                    if (strcmp(gsm_pCommande, "HDUDPR") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_HEURE_DEBUT_POINT_UDPR, atol(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Heure debut UDP_R est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "Heure debut UDP_R est %ldh", GetlReg(RNV_HEURE_DEBUT_POINT_UDPR));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_HFUDPR;
                }
                case GSM_REQUETTE_HFUDPR:
                {
                    if (strcmp(gsm_pCommande, "HFUDPR") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_HEURE_FIN_POINT_UDPR, atoi(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Heure fin UDP_R est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "Heure fin UDP_R est %ldh", GetlReg(RNV_HEURE_FIN_POINT_UDPR));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_HDUDPH;
                }
                case GSM_REQUETTE_HDUDPH:
                {
                    if (strcmp(gsm_pCommande, "HDUDPH") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_HEURE_DEBUT_POINT_UDPH, atol(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Heure debut UDP_H est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "Heure debut UDP_H est %ldh", GetlReg(RNV_HEURE_DEBUT_POINT_UDPH));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_HFUDPH;
                }
                case GSM_REQUETTE_HFUDPH:
                {
                    if (strcmp(gsm_pCommande, "HFUDPH") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_HEURE_FIN_POINT_UDPH, atol(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Heure fin UDP_H est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "Heure fin UDP_H est %ldh", GetlReg(RNV_HEURE_FIN_POINT_UDPH));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_IP;
                }
                case GSM_REQUETTE_IP:
                {
                    for(Cpt=1; Cpt < 5; Cpt++)
                    {
                        sprintf(gsm_ChaineTemps, "IP%d", Cpt);
                        if(strncmp(gsm_pCommande, gsm_ChaineTemps, 3) == 0)
                        {
                            break;
                        }
                    }
                    gsm_ChaineTemps[0] = 0;

                    if (Cpt < 5 || (strcmp(gsm_pCommande, "IP") == 0)) {
                        // Ecriture de l'adresse IP
                        if (gsmbit.bSMSEntrantEcriture) {
                            configGSM_Write(gsm_pParametre, gsm_aAdresseIP[Cpt - 1], 49);
                            sprintf(gsm_ChaineTemps, "Adresse IP %d est modifiee", Cpt);
                        } else {
                            pIndex1 = configGSM_String(gsm_aAdresseIP[0]);
                            sprintf(gsm_ChaineTemps, "IP1 : %s\r\nIP2 : %s\r\nIP3 : %s\r\nIP4 : %s\r\n", pIndex1, pIndex1+50, pIndex1+100, pIndex1+150);
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_IDOP;
                }
				case GSM_REQUETTE_IDOP:
                {
                    if (strcmp(gsm_pCommande, "IDOP") == 0) //
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            if (gsm_pParametre) {
                                // Extraction du nom du serveur
                                pIndex1 = strchr(gsm_pParametre, ' ');
                                if (pIndex1) {
                                    *pIndex1 = 0;
                                    *pIndex1++;
                                }
                                configGSM_Write(gsm_pParametre, gsm_aAPN, 24);

                                if (pIndex1) {
                                    // Extraction du nom utilisateur
                                    gsm_pParametre = strchr(pIndex1, ' ');
                                    if (pIndex1) {
                                        *gsm_pParametre = 0;
                                        *gsm_pParametre++;
                                    }
                                    configGSM_Write(pIndex1, gsm_aUser, 24);

                                    if (gsm_pParametre) {
                                        // Extraction du mot de passe
                                        configGSM_Write(gsm_pParametre, gsm_aPW, 24);
                                    } else {
                                        *pIndex1 = 0;
                                        configGSM_Write(pIndex1, gsm_aPW, 1);
                                    }
                                } else {
                                    *gsm_pParametre = 0;
                                    configGSM_Write(gsm_pParametre, gsm_aUser, 1);
                                    configGSM_Write(gsm_pParametre, gsm_aPW, 1);
                                }
                                strcpy(gsm_ChaineTemps, "Les identifications GPRS sont modifiees");
                            } else {
                                strcpy(gsm_ChaineTemps, "Erreur identifications GPRS");
                            }
                        } else {
                            pIndex1 = configGSM_String(gsm_aAPN);
                            sprintf(gsm_ChaineTemps, "SVR : %s\r\nUSER : %s\r\nPW : %s", pIndex1, pIndex1 + 25, pIndex1 + 50);
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_ESMPR;
                }
                case GSM_REQUETTE_ESMPR:
                {
//                    if (strcmp(gsm_pCommande, "ESMPR") == 0) {
//                        if (gsmbit.bSMSEntrantEcriture) {
//                            SetFlag(FNV_FTP_SEND_IF_SAVE_RAM_FULL, atoi(gsm_pParametre));
//                            sprintf(gsm_ChaineTemps, "L'envoi de sauvegarde par UDP si la memoire RAM est pleine est %s", GetFlag(FNV_FTP_SEND_IF_SAVE_RAM_FULL) ? "active" : "desactive");
//                        } else {
//                            sprintf(gsm_ChaineTemps, "L'envoi de sauvegarde par UDP si la memoire RAM est pleine est %s", GetFlag(FNV_FTP_SEND_IF_SAVE_RAM_FULL) ? "Actif" : "Inactif");
//                        }
//                        break;
//                    }
                    gsm_MAERequette = GSM_REQUETTE_TMINESR;
                }
                case GSM_REQUETTE_TMINESR:
                {
                    if (strcmp(gsm_pCommande, "TMINESR") == 0) {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_TEMPS_MIN_ENTRE_SAVE_RAM, atoi(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Le temps min entre sauvegardes RAM est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le temps min entre sauvegardes RAM est %ld s", GetlReg(RNV_TEMPS_MIN_ENTRE_SAVE_RAM));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_TMAXESR;
                }
                case GSM_REQUETTE_TMAXESR:
                {
                    if (strcmp(gsm_pCommande, "TMAXESR") == 0) {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_TEMPS_MAX_ENTRE_SAVE_RAM, atoi(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "Le temps max entre sauvegardes RAM est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le temps max entre sauvegardes RAM est %ld s", GetlReg(RNV_TEMPS_MAX_ENTRE_SAVE_RAM));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_TSR;
                }
                case GSM_REQUETTE_TSR:
                {
                    if (strcmp(gsm_pCommande, "TSR") == 0) {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetlReg(RNV_TAILLE_SAVE_RAM, atoi(gsm_pParametre));
                            strcpy(gsm_ChaineTemps, "La taille de la sauvegarde RAM est modifiee");
                        } else {
                            sprintf(gsm_ChaineTemps, "La taille de la sauvegarde RAM est %ld registres", GetlReg(RNV_TAILLE_SAVE_RAM));
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_MSR;
                }
                case GSM_REQUETTE_MSR:
                {
                    if (strcmp(gsm_pCommande, "MSR") == 0) {
                        if (gsmbit.bSMSEntrantEcriture) {
                            gsm_InfoHistoRam.TSTVA.ModeSave = atoi(gsm_pParametre) ? 1 : 0;
                            configGSM_Write((char *) &gsm_InfoHistoRam.InfoSauvegrde, gsm_aInitCfgSaveRam - 4, 4);
                            strcpy(gsm_ChaineTemps, "Le mode de la sauvegarde RAM est modifie");
                        } else {
                            sprintf(gsm_ChaineTemps, "La sauvegarde RAM est en mode %s", gsm_InfoHistoRam.TSTVA.ModeSave ? "seuil" : "cyclique");
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_AR;
                }
                case GSM_REQUETTE_AR:
                {
                    if (strcmp(gsm_pCommande, "AR") == 0)
                    {
                        if (gsmbit.bSMSEntrantEcriture) {
                            SetFlag(FNV_RAPPORT_ACTIF, atoi(gsm_pParametre));
                            sprintf(gsm_ChaineTemps, "Le rapport est %s", GetFlag(FNV_RAPPORT_ACTIF) ? "active" : "desactive");
                        } else {
                            sprintf(gsm_ChaineTemps, "Le rapport est %s", GetFlag(FNV_RAPPORT_ACTIF) ? "Actif" : "Inactif");
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_CTR;
                }
				case GSM_REQUETTE_CTR:
                {
                    if (strcmp(gsm_pCommande, "CTR") == 0) {
                        if (gsmbit.bSMSEntrantEcriture) {
                            Valeur.of.octet[0] = atoi(gsm_pParametre);
                            SetFlag(FNV_ENVOI_RAPPORT_AU_TEL(0), Valeur.of.flags.f0);
                            SetFlag(FNV_ENVOI_RAPPORT_AU_TEL(1), Valeur.of.flags.f1);
                            SetFlag(FNV_ENVOI_RAPPORT_AU_TEL(2), Valeur.of.flags.f2);
                            SetFlag(FNV_ENVOI_RAPPORT_AU_TEL(3), Valeur.of.flags.f3);

                            strcpy(gsm_ChaineTemps, "La configuration du choix des telephones du rapport est modifiee");
                        } else {
                            Valeur.of.flags.f0 = GetFlag(FNV_ENVOI_RAPPORT_AU_TEL(0));
                            Valeur.of.flags.f1 = GetFlag(FNV_ENVOI_RAPPORT_AU_TEL(1));
                            Valeur.of.flags.f2 = GetFlag(FNV_ENVOI_RAPPORT_AU_TEL(2));
                            Valeur.of.flags.f3 = GetFlag(FNV_ENVOI_RAPPORT_AU_TEL(3));

                            sprintf(gsm_ChaineTemps, "La configuration du choix des telephones du rapport est %d", Valeur.of.octet[0]);
                        }
                        break;
                    }
                    gsm_MAERequette = GSM_REQUETTE_RAPPORT;
                }
				case GSM_REQUETTE_RAPPORT:
				{
					if(strcmp(gsm_pCommande, "RAPPORT") == 0)
					{
						if(!gsm_Rapport())
						{
							return 0;
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_SBUS;
				}
                case GSM_REQUETTE_SBUS:
				{
					if(strcmp(gsm_pCommande, "SBUS") == 0)
					{
						if(!gsm_Sbus())
						{
							return 0;
						}
						break;
					}
					gsm_MAERequette = GSM_REQUETTE_ANA_IO;
				}
				case GSM_REQUETTE_ANA_IO:
				{
					if(!gsmplus_Requete(gsm_pCommande, gsm_pParametre, gsm_ChaineTemps))
					{
						break;
					}
                    gsm_ChaineTemps[0] = 0;
					gsm_MAERequette = GSM_REQUETTE_USER;
				}
				case GSM_REQUETTE_USER:
				{
					/////////////////////////////////////
					/////////////////////////////////////
					switch(gsm_RequeteUtilisateur())
					{
						case 0:
						{
							return 0;
						}
						case 1:
						{
							break;
						}
						case 2:
						{
							gsm_MAERequette = GSM_REQUETTE_NOT_EXISTE;
							return 0;
						}
					}
					break;
				}
				case GSM_REQUETTE_NOT_EXISTE:
				{
					gsm_ChaineTemps[0] = 0;
					break;
				}
			}

			if(strlen(gsm_ReponseSMS) + strlen(gsm_ChaineTemps) >= 160)
			{
				gsm_MAETraittementSMS = GSM_SMS_REPONSE;
				return 3;
			}

			gsm_MAETraittementSMS = GSM_SMS_REPONSE;
		}
		case GSM_SMS_REPONSE:
		{
			if(strlen(gsm_ChaineTemps))
			{
				strcat(gsm_ReponseSMS, gsm_ChaineTemps);
				strcat(gsm_ReponseSMS, "#");
			}

			gsm_pCommande = strtok(NULL, "#");
			if(gsm_pCommande)
			{
				gsm_MAETraittementSMS = GSM_SMS_GET_CMD;
			}
			else
			{
				gsm_MAETraittementSMS = GSM_SMS_START;
				if(strlen(gsm_ReponseSMS))
				return 2;
				else
				return 1;
			}
			break;
		}
	}
	return 0;
}





SMSAlarme	gsm_DataAlarme;

long long gsm_AlarmeOk;

int gsm_VerifierAlarme()
{

	// Si il y a une alarme encoure d'envoi alors on quitte la fonction
	if(gsm_pMsgAlarmeToSend || !gsmbit.bConfigOK/* || !GetFlag(FNV_ACTIF_ALARME_GSM)*/)
	{
		return 0;
	}

	switch(gsm_MAEAlarme)
	{
		case GSM_ALARME_INIT :
		{
			gsm_aSMSAlarme = gsm_aInitAlarme;
			gsm_aMsgAlarme = gsm_aSMSAlarme + 6;
			gsm_MAEAlarme = GSM_ALARME_GET;
			gsm_CptAlarme = 0;
			break;
		}
		case GSM_ALARME_GET :
		{
			if(gsm_CptAlarme < gsm_NbrAlarme)
			{
				configGSM_Read(gsm_DataAlarme.Data, gsm_aSMSAlarme, 6);
				if(	GetTimer(TIMER_ALARME_GSM(gsm_CptAlarme)) ||
					(!gsm_EntetAlarme.cESMSA.ActiveLocal && (gsm_DataAlarme.Info.Station == gNumStation || gsm_DataAlarme.Info.Station == 0)) ||
					(	(gsm_DataAlarme.Info.Station != gNumStation && gsm_DataAlarme.Info.Station != 0) &&
						(
							!gsm_EntetAlarme.cESMSA.ActiveDistant ||
							(gsm_EntetAlarme.cESMSA.ActiveDistant && !gsmbit.bConsultAlarmeDis)
						)
					)
				)
				{
					gsm_aSMSAlarme = gsm_aMsgAlarme + strlen(configGSM_String(gsm_aMsgAlarme)) + 1;
					gsm_aMsgAlarme = gsm_aSMSAlarme + 6;
					gsm_CptAlarme++;
				}
				else
				{
					if(!comsbusmaitre_bit[0].bSbusLibre)
					{
						return 0;
					}

					comsbusmaitre_bit[0].bSbusLibre = 0;
					comsbusmaitre_bit[0].bSbusEnd = 0;
					comsbusmaitre_bit[0].SbusErr = 0;
					comsbusmaitre_SbusStation[0] = gsm_DataAlarme.Info.Station;
					comsbusmaitre_SbusAdresse[0][0].l = gsm_DataAlarme.Info.Adresse;
					comsbusmaitre_SbusNbr[0] = 1;
					comsbusmaitre_SbusType[0] = 'F';
					comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
					gsm_MAEAlarme = GSM_ALARME_DATA;
					//gsm_MAEOld = gsm_MAE;
					//gsm_MAE = GSM_ETAT_SBUS;
					// Dernière trame
				}
			}
			else
			{
				gsmbit.bConsultAlarmeDis = 0;
				if(!GetTimer(TIMER_VERIFY_ALARME_GSM))
				{
					SetTimer(TIMER_VERIFY_ALARME_GSM, gsm_EntetAlarme.cESMSA.IntervalVerife * 600);
					gsmbit.bConsultAlarmeDis = 1;
				}
				gsm_MAEAlarme = GSM_ALARME_INIT;
				return 0;
			}
			break;
		}
		case GSM_ALARME_DATA :
		{
			if(comsbusmaitre_bit[0].bSbusEnd)
			{
				comsbusmaitre_bit[0].bSbusLibre = 1;
				if(!comsbusmaitre_bit[0].SbusErr) //Alarme Actif
				{
					gsmbit.bAlarmeCfg = comsbusmaitre_SbusBuffer[0][0].of.octet[0]!=0;
					gsm_MAEAlarme = GSM_ALARME_SET;
				}
				else
				{
					gsm_aSMSAlarme = gsm_aMsgAlarme + strlen(configGSM_String(gsm_aMsgAlarme)) + 1;
					gsm_aMsgAlarme = gsm_aSMSAlarme + 6;

					gsm_CptAlarme++;
					gsm_MAEAlarme = GSM_ALARME_GET;
					return 0;
				}
			}
			break;
		}
		case GSM_ALARME_SET :
		{
			if(gsm_DataAlarme.Info.AlarmeParFronf)
			{
				if((gsm_AlarmeOk>>gsm_CptAlarme) & ((long long)gsmbit.bAlarmeCfg))
				{
					gsm_aSMSAlarme = gsm_aMsgAlarme + strlen(configGSM_String(gsm_aMsgAlarme)) + 1;
					gsm_aMsgAlarme = gsm_aSMSAlarme + 6;
					gsm_CptAlarme++;
					gsm_MAEAlarme = GSM_ALARME_GET;
					return 0;
				}
			}

			if(gsmbit.bAlarmeCfg)
			{
				gsm_AlarmeOk |= ((long long)0x01<<gsm_CptAlarme);
				gsm_MAEAlarme = GSM_ALARME_SEND_TO_TELE1;
			//	gsmbit.bWakeUpFromGSM = 1;
			}
			else
			{
				gsm_AlarmeOk &= ~((long long)0x01<<gsm_CptAlarme);

				gsm_aSMSAlarme = gsm_aMsgAlarme + strlen(configGSM_String(gsm_aMsgAlarme)) + 1;
				gsm_aMsgAlarme = gsm_aSMSAlarme + 6;
				gsm_CptAlarme++;
				gsm_MAEAlarme = GSM_ALARME_GET;
				return 0;
			}

			break;
		}
		case GSM_ALARME_SEND_TO_TELE1 :
		{
			gsmbit.bStopGSM = 0;

			if(gsm_DataAlarme.Info.AlarmeToTele1)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsm_aMsgAlarme;

				gsm_NumAlarme = gsm_CptAlarme+1;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;

				//gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 0;
			}
			gsm_MAEAlarme = GSM_ALARME_SEND_TO_TELE2;

			break;
		}
		case GSM_ALARME_SEND_TO_TELE2 : 	// Pointe sur l'alarme
		{
			if(gsm_DataAlarme.Info.AlarmeToTele2)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsm_aMsgAlarme;

				gsm_NumAlarme = gsm_CptAlarme+1;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;

				//gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 1;
			}
			gsm_MAEAlarme = GSM_ALARME_SEND_TO_TELE3;
			break;
		}
		case GSM_ALARME_SEND_TO_TELE3 : 	// Pointe sur l'alarme
		{
			if(gsm_DataAlarme.Info.AlarmeToTele3)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsm_aMsgAlarme;

				gsm_NumAlarme = gsm_CptAlarme+1;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;

				//gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 2;
			}
			gsm_MAEAlarme = GSM_ALARME_SEND_TO_TELE4;
			break;
		}
		case GSM_ALARME_SEND_TO_TELE4 : 	// Pointe sur l'alarme
		{
			if(gsm_DataAlarme.Info.AlarmeToTele4)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsm_aMsgAlarme;

				gsm_NumAlarme = gsm_CptAlarme+1;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;

				//gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 3;
			}

			if(gsm_DataAlarme.Info.IntervalleEmission)
			SetTimer(TIMER_ALARME_GSM(gsm_CptAlarme), gsm_DataAlarme.Info.IntervalleEmission * 600);
			else
			SetTimer(TIMER_ALARME_GSM(gsm_CptAlarme), 600);

			gsm_aSMSAlarme = gsm_aMsgAlarme + strlen(configGSM_String(gsm_aMsgAlarme)) + 1;
			gsm_aMsgAlarme = gsm_aSMSAlarme + 6;

			gsm_CptAlarme++;
			gsm_MAEAlarme = GSM_ALARME_GET;
			return 0;
		}
	}
	return 1;
}

unsigned char gsm_IndiceTeleUser;
char *gsm_pMsgUserToSend;

char gsm_UserSMS(char *pMsgUserToSend, char IndiceTeleUser)
{
	if(!gsmbit.bUserSMS)
	{
		gsmbit.bUserSMS = 1;
		gsm_IndiceTeleUser = IndiceTeleUser;
		gsm_pMsgUserToSend = pMsgUserToSend;
		gsmbit.bStopGSM = 0;
		return 1;
	}
	return 0;
}

char gsm_UserSMSLibre()
{
	return !gsmbit.bUserSMS;
}

void gsm_VerifierRapport()
{
	if(!GetTimer(TIMER_ENVOI_RAPPORT) && GetlReg(RNV_TEMPO_ENVOI_RAPPORT))
	{
		SetTimer(TIMER_ENVOI_RAPPORT, GetlReg(RNV_TEMPO_ENVOI_RAPPORT)*600);
		SetFlag(FV_ENVOI_RAPPORT, 1);
		gsmbit.bStopGSM = 0;
	}
}

char gsm_GetDataUDPR()
{
	switch(gsm_MAEUDP)
	{
		case GSM_UDP_INIT	:	gsm_aUDPCfg = gsm_aInitUDPCfg;
								gsm_CptVarUDP = 0;
								gsm_UDPCptBit = 0;
								gsm_BufferTCPUDP[0] = 0;
								gsm_MAEUDP = GSM_UDP_GET_VAR;
								gsm_UDPCptOctet = sprintf(gsm_BufferTCPUDP, "%s MC3GR", gsm_NomStation());
								break;

		case GSM_UDP_GET_VAR :	if(!comsbusmaitre_bit[0].bSbusLibre)
								{
									break;
								}

								if(gsm_CptVarUDP < gsm_NbrVarUDP)
								{
									configGSM_Read(gsm_DataVarUDP.Data, gsm_aUDPCfg, sizeof(uVarUDP));

									comsbusmaitre_bit[0].bSbusLibre = 0;
									comsbusmaitre_bit[0].bSbusEnd = 0;
									comsbusmaitre_bit[0].SbusErr = 0;
									comsbusmaitre_SbusStation[0] = gsm_DataVarUDP.Info.Station;
									comsbusmaitre_SbusAdresse[0][0].l = gsm_DataVarUDP.Info.Adresse;
									comsbusmaitre_SbusNbr[0] = 1;
									gsm_MAEUDP = GSM_INSERT_VARIABLE;
									// Si Flag ou Ref Flag
									if(gsm_DataVarUDP.Info.Type)
									{
										comsbusmaitre_SbusType[0] = 'F';
										comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
									}
									else
									{
										comsbusmaitre_SbusType[0] = 'R';
										comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_REG;
									}
								}
								else
								{
									if(gsm_UDPCptBit)
									{
										gsm_UDPCptOctet++;
									}
									gsm_MAEUDP = GSM_UDP_INIT;
									return 1;
								}
								break;
		case GSM_INSERT_VARIABLE :
								if(comsbusmaitre_bit[0].bSbusEnd)
								{
									comsbusmaitre_bit[0].bSbusLibre = 1;
									if(comsbusmaitre_bit[0].SbusErr)
									{
										comsbusmaitre_SbusBuffer[0][0].l = 0;
									}

									if(gsm_DataVarUDP.Info.Type) // Flag
									{
										if(comsbusmaitre_SbusBuffer[0][0].of.octet[0])
										{
											gsm_BufferTCPUDP[gsm_UDPCptOctet] |= (0x01 << gsm_UDPCptBit);
										}
										else
										{
											gsm_BufferTCPUDP[gsm_UDPCptOctet] &= ~(0x01 << gsm_UDPCptBit);
										}
									}
									else // Registre
									{
										gsm_BufferTCPUDP[gsm_UDPCptOctet + 0] = comsbusmaitre_SbusBuffer[0][0].of.octet[0];
										gsm_BufferTCPUDP[gsm_UDPCptOctet + 1] = comsbusmaitre_SbusBuffer[0][0].of.octet[1];
										gsm_BufferTCPUDP[gsm_UDPCptOctet + 2] = comsbusmaitre_SbusBuffer[0][0].of.octet[2];
										gsm_BufferTCPUDP[gsm_UDPCptOctet + 3] = comsbusmaitre_SbusBuffer[0][0].of.octet[3];
									}

									if(gsm_DataVarUDP.Info.Type) // Flag
									{

										gsm_UDPCptBit++;
										if(gsm_UDPCptBit >= 8)
										{
											gsm_UDPCptBit = 0;
											gsm_UDPCptOctet++;
											gsm_BufferTCPUDP[gsm_UDPCptOctet] = 0;
										}
									}
									else // Registre
									{

										gsm_UDPCptBit = 0;
										gsm_UDPCptOctet += 4;
										gsm_BufferTCPUDP[gsm_UDPCptOctet] = 0;
									}


									if(gsm_UDPCptOctet >= 1024)
									{
										gsm_UDPCptOctet = 1024;
										gsm_MAEUDP = GSM_UDP_INIT;
										return 1;
									}

									//bonjour
									gsm_CptVarUDP++;
									gsm_aUDPCfg += sizeof(uVarUDP);
									gsm_MAEUDP = GSM_UDP_GET_VAR;
								}
								break;
	}

	return 0;
}

void gsm_VerifTransfertUDPR()
{
	switch(gsm_MAEUDPR)
	{
		case GSM_UDPR_VERIF:
		{
			if(	GetFlag(FNV_GSM_MODE_GPRS)
				&&
				GetFlag(FNV_MODE_UDPR)
				&&
				GetlReg(RNV_TEMPO_SEND_UDPR)
			)
			{
				if(
					!GetTimer(TIMER_SEND_UDPR)
					&&
					(
					(
					(!GetlReg(RNV_HEURE_DEBUT_POINT_UDPR)) &&
					(!GetlReg(RNV_HEURE_FIN_POINT_UDPR))
					)
					||
					(
					(clock_GetHour() >= GetlReg(RNV_HEURE_DEBUT_POINT_UDPR))
					&&
					(clock_GetHour() < GetlReg(RNV_HEURE_FIN_POINT_UDPR))
					)
					)
				)
				{
					gsmbit.bStopGSM = 0;
					gsmbit.bSendUDPR = 1;
					gsm_MAEUDPR = GSM_UDPR_RECHARGE;
				}
				else
				{
                    if(GetFlag(FNV_MODE_ENVOI_CONDITIONNEL_UDPR))
                    {
                        if(!GetTimer(TIMER_FRQ_RAFF_FLAG_CONDITION))
                        {
                            SetTimer(TIMER_FRQ_RAFF_FLAG_CONDITION, 600);
                            gsm_MAEUDPR = GSM_UDPR_TEST_MODE;
                        }
                    }
                    else
                    {
                        gsmbit.bFreqEnvoi1 = 1;
                        if(GetTimer(TIMER_SEND_UDPR) > GetlReg(RNV_TEMPO_SEND_UDPR)*600)
                        {
                            gsm_MAEUDPR = GSM_UDPR_RECHARGE;
                        }
                    }
				}
			}
			break;
		}
		case GSM_UDPR_TEST_MODE:
		{	
            if(!comsbusmaitre_bit[0].bSbusLibre)
            {
                break;
            }

            comsbusmaitre_bit[0].bSbusLibre = 0;
            comsbusmaitre_bit[0].bSbusEnd = 0;
            comsbusmaitre_bit[0].SbusErr = 0;
            comsbusmaitre_SbusStation[0] = GetlReg(RNV_STATION_CONDITION_UDPR);
            comsbusmaitre_SbusAdresse[0][0].l = GetlReg(RNV_ADRESSE_CONDITION_UDPR);
            comsbusmaitre_SbusNbr[0] = 1;
            comsbusmaitre_SbusType[0] = 'F';
            comsbusmaitre_MAESbusMaitre[0] = SBUS_MAITRE_READ_FLG;
            gsm_MAECondEnvoiUDPR = GSM_COND_ENVOI_UDPR_REPONSE;

            gsm_MAEUDPR = GSM_UDPR_ATT_REPONSE;
			
			break;
		}
		case GSM_UDPR_ATT_REPONSE:
		{
			if(comsbusmaitre_bit[0].bSbusEnd)
			{
				comsbusmaitre_bit[0].bSbusLibre = 1;

				if(!comsbusmaitre_bit[0].SbusErr)
				{
					if(comsbusmaitre_SbusBuffer[0][0].of.octet[0])
					{
						gsmbit.bFreqEnvoi1 = 1;
					}
					else
					{
						gsmbit.bFreqEnvoi1 = 0;
					}
				}

				gsm_MAEUDPR = GSM_UDPR_VERIF;
				if(gsmbit.bFreqEnvoi1)
				{
					if(GetTimer(TIMER_SEND_UDPR) > GetlReg(RNV_TEMPO_SEND_UDPR)*600)
					{
						gsm_MAEUDPR = GSM_UDPR_RECHARGE;
					}
				}
				else
				{
					if(GetTimer(TIMER_SEND_UDPR) > GetlReg(RNV_TEMPO2_SEND_UDPR)*600)
					{
						gsm_MAEUDPR = GSM_UDPR_RECHARGE;
					}
				}
			}
			break;
		}
		case GSM_UDPR_RECHARGE:
		{
			if(gsmbit.bFreqEnvoi1)
			{
				SetTimer(TIMER_SEND_UDPR, GetlReg(RNV_TEMPO_SEND_UDPR)*600);
			}
			else
			{
				SetTimer(TIMER_SEND_UDPR, GetlReg(RNV_TEMPO2_SEND_UDPR)*600);
			}
			gsm_MAEUDPR = GSM_UDPR_VERIF;
			break;
		}

	}
}

void gsm_DesactiveGSM()
{
	if(gsmbit.bActiveGSM)
	{
		gsm_MAE = GSM_INIT;
		gsm_MAEInit = GSM_INIT_PWR;
		gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
		gsm_MAEInitCfg = GSM_INIT_CFG_VITESSE;
		gsm_NombreReset = 0;

		INTEnable(INT_INT4, INT_DISABLED);

		uart_Stop();
//        gps_Stop();

		_TRISF8 = 1;
		_TRISF2 = 1;

		GSM_ALIM = 0;
		GSM_RESET = 0;
		GSM_POWER = 0;

		SetlReg(RV_NOMBRE_MESSAGES_STOCKABLES, 0);
		SetlReg(RV_QUALITE_SIGNAL_GSM, 0);
		SetTimer(TIMER_GSM_TEMPO, 0);
		gsmbit.bActiveGSM = 0;
	}
}

void gsm_Stop()
{
	gsmbit.bStopGSM = 1;
	gsm_DesactiveGSM();
}

char gsm_Gestion()
{
	int Cpt;
	int Val;
	char *pIndex1;
	char *pIndex2;

	gsmplus_Gestion();
	gsm_VerifierAlarme();
	gsm_VerifierRapport();

	gsm_SauvegardeRam();
	gsm_VerifTransfertUDPR();
	gsm_VerifTransfertUDPH();


	uart_Gestion();


	SetlReg(RV_ETAT1_GSM, gsm_MAE);
	SetlReg(RV_ETAT2_GSM, gsm_MAEInit);
	SetlReg(RV_ETAT3_GSM, gsm_MAEInitCfg);

	//Reset bloquage envoi sms
	if(GetlReg(RV_CPT_SMS_ENVOYER_JOURS) < GetlReg(RNV_NBR_MAX_SMS_JOURS))
	{
		if(!GetTimer(TIMER_RESET_NBR_SMS_HEURE))
		{
			SetTimer(TIMER_RESET_NBR_SMS_HEURE, 36000);
			SetlReg(RV_CPT_SMS_ENVOYER_HEURE, 0);
			gsmbit.bSMSBloquer = 0;
		}
	}

	if(!GetTimer(TIMER_RESET_NBR_SMS_JOUR))
	{
		SetTimer(TIMER_RESET_NBR_SMS_JOUR, 864000);
		SetlReg(RV_CPT_SMS_ENVOYER_HEURE, 0);
		SetlReg(RV_CPT_SMS_ENVOYER_JOURS, 0);
		gsmbit.bSMSBloquer = 0;
	}

	if(!GetFlag(FNV_ACTIVE_GSM) || (/*!GSM_SIM_PRESENT && */!GetFlag(FNV_GESTION_SIM_PRESENT)) || gsmbit.bStopGSM)
	{
		gsm_DesactiveGSM();
        if(!GetFlag(FNV_ACTIVE_GSM) || (/*!GSM_SIM_PRESENT && */!GetFlag(FNV_GESTION_SIM_PRESENT)))
        {
            gsmbit.bRAZRAMAutorisie = 1;
        }
        return 0;
	}

	gsmbit.bActiveGSM = 1;

	switch(gsm_MAE)
	{
		case GSM_INIT:
		{
			switch(gsm_MAEInit)
			{
				case GSM_INIT_PWR:
				{
					switch(gsm_MAEInitPwr)
					{
						case GSM_INIT_PWR_OFF:
						{
							gsm_QualiteReseau = 0;
							SetlReg(RV_QUALITE_SIGNAL_GSM, gsm_QualiteReseau);

							if(!GetTimer(TIMER_GSM_TEMPO))
							{
								gsmbit.b3Reset = 0;
								SetTimer(TIMER_GSM_TEMPO, GSM_DUREE_OFF);
								uart_Stop();
								GSM_ALIM = 0;
								GSM_RESET = 0;
								GSM_POWER = 0;
								gsm_NombreReset++;
								if(gsm_NombreReset > GSM_NBR_RESET)
								{
									gsm_NombreReset = 0;
									gsmbit.b3Reset = 1;
									gsm_MAE = GSM_INIT;
									gsm_MAEInit = GSM_INIT_PWR;
									gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
									SetTimer(TIMER_GSM_TEMPO, GetlReg(RNV_GSM_DUREE_APRES_N_RESET)*600);
                                    gsmbit.bRAZRAMAutorisie = 1;
									return 0;
								}
								gsm_MAEInitPwr = GSM_INIT_PWR_ALIM;
								gsm_NombreErreur = 0;
							}
							else
							{
								if(gsmbit.b3Reset)
								{
									GSM_ALIM = 0;
                                    gsmbit.bRAZRAMAutorisie = 1;
									return 0;
								}
							}
							break;
						}
						case GSM_INIT_PWR_ALIM:
						{
							if(!GetTimer(TIMER_GSM_TEMPO))
							{
								GSM_ALIM = 1;
								SetTimer(TIMER_GSM_TEMPO, GSM_DUREE_ON);
								gsm_MAEInitPwr = GSM_INIT_PWR_PWRKEY_UP;
							}
							break;
						}
						case GSM_INIT_PWR_PWRKEY_UP:
						{
							if(!GetTimer(TIMER_GSM_TEMPO))
							{
								GSM_POWER = 1;
								SetTimer(TIMER_GSM_TEMPO, GSM_DUREE_PWRKEY_UP);
								gsm_MAEInitPwr = GSM_INIT_PWR_PWRKEY_DOWN;
							}
							break;
						}
						case GSM_INIT_PWR_PWRKEY_DOWN:
						{
							if(!GetTimer(TIMER_GSM_TEMPO))
							{
								GSM_POWER = 0;
								SetTimer(TIMER_GSM_TEMPO, GSM_DUREE_PRE_ATCMD);
								gsm_MAEInitPwr = GSM_INIT_PWR_PRE_ATCMD;
							}
							break;
						}
						case GSM_INIT_PWR_PRE_ATCMD:
						{
							if(!GetTimer(TIMER_GSM_TEMPO)) // si Etat == 1
							{
								if(GSM_ETAT) // si Etat == 1
								{
									uart_Init(gsm_ListeVitesse[GSM_DEFAULT_VITESSE]);
									gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
									gsm_MAEInit = GSM_INIT_CONFIG;
									gsm_MAEInitCfg = GSM_INIT_CFG_VITESSE;
									gsm_CptVitesse = 0;
									gsm_NombreErreur = 0;
									break;
								}

								SetTimer(TIMER_GSM_TEMPO, GSM_DUREE_ON);
								gsm_MAEInitPwr = GSM_INIT_PWR_PWRKEY_UP;

								gsm_NombreErreur++;
								if(gsm_NombreErreur > 3)
								{
									gsm_MAE = GSM_INIT;
									gsm_MAEInit = GSM_INIT_PWR;
									gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
								}
							}
							break;
						}
					}
					break;
				}
				case GSM_INIT_CONFIG:
				{
					switch(gsm_MAEInitCfg)
					{
						case GSM_INIT_CFG_VITESSE:
						{
							if(!GetTimer(TIMER_GSM_TEMPO))
							{
								switch(gsm_SendCmd(gsm_pListeConfig1[0], "OK", GSM_TIMEOUT_CMD_AT)) // Initialisation de la vitesse a 19200
								{
									case GSM_CMD_SUCCES:
									{
										uart_Init(gsm_ListeVitesse[GSM_DEFAULT_VITESSE]);
//                                        gps_Init();
										gsm_CptConfig = 1;
										gsm_NombreErreur = 0;
										gsm_MAEInitCfg = GSM_INIT_CFG1_CMD;
										break;
									}
									case GSM_CMD_TO_ERR:
									case GSM_CMD_REPONSE_ERR:
									{
										SetTimer(TIMER_GSM_TEMPO, 10);
										if(gsm_CptVitesse >= 5)
										{
											gsm_MAE = GSM_INIT;
											gsm_MAEInit = GSM_INIT_PWR;
											gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
										}
										else
										{
											uart_Init(gsm_ListeVitesse[gsm_CptVitesse]);
											gsm_CptVitesse++;
										}
										break;
									}
								}
							}
							break;
						}
						case GSM_INIT_CFG1_CMD:
						{
							switch(gsm_SendCmd(gsm_pListeConfig1[gsm_CptConfig], "OK", GSM_TIMEOUT_CMD_AT))
							{
								case GSM_CMD_SUCCES:
								{
									gsm_CptConfig++;
									if(gsm_CptConfig >= GSM_TAILLE_LISTE_CONFIG1)
									{
										gsm_NombreErreur = 0;
										gsm_MAEInitCfg = GSM_INIT_VERIF_PIN;
									}
									break;
								}
								case GSM_CMD_TO_ERR:
								case GSM_CMD_REPONSE_ERR:
								{
									gsm_NombreErreur++;
									if(gsm_NombreErreur > 3)
									{
										gsm_MAE = GSM_INIT;
										gsm_MAEInit = GSM_INIT_PWR;
										gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
									}
									break;
								}
							}
							break;
						}
						case GSM_INIT_VERIF_PIN:
						{
							//if(GetFlag(FNV_ACTIVE_CODE_PINE))
							switch(gsm_SendCmd(GSM_AT_CPIN, "OK", GSM_TIMEOUT_CMD_AT))
							{
								case GSM_CMD_SUCCES:
								{
									pIndex1 = strstr(gsm_TrameSIM32, "+CPIN: ");
									if(pIndex1)
									{
										if(strstr(pIndex1, "READY"))
										{
											gsm_MAEInitCfg = GSM_INIT_ATT_RESEAU;
											SetTimer(TIMER_GSM_TEMPO, GSM_TIME_LECTURE_ATCREG);
										}
										else if(strstr(pIndex1, "SIM PIN"))
										{
											if(GetFlag(FNV_ACTIVE_CODE_PINE) && GetlReg(RNV_CODE_PINE) >= 0 && GetlReg(RNV_CODE_PINE) < 10000)
											{
												sprintf(gsm_Chaine, GSM_AT_CPIN_SET, GetlReg(RNV_CODE_PINE));
												gsm_MAEInitCfg = GSM_INIT_ENTRE_PIN;
											}
											else
											{
												gsm_MAE = GSM_SIM_PIN_REQUIS;
											}
										}
										else
										{
											gsm_MAE = GSM_SIM_PIN_REQUIS;
										}
									}
									break;
								}
								case GSM_CMD_TO_ERR:
								case GSM_CMD_REPONSE_ERR:
								{
									gsm_NombreErreur++;
									if(gsm_NombreErreur > 1)
									{
										gsm_MAE = GSM_INIT;
										gsm_MAEInit = GSM_INIT_PWR;
										gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
									}
									break;
								}
							}
							break;
						}
						case GSM_INIT_ENTRE_PIN:
						{
							switch(gsm_SendCmd(gsm_Chaine, "OK", GSM_TIMEOUT_CMD_AT))
							{
								case GSM_CMD_SUCCES:
								{
									gsm_MAEInitCfg = GSM_INIT_ATT_RESEAU;
									SetTimer(TIMER_GSM_TEMPO, GSM_TIME_LECTURE_ATCREG);
									break;
								}
								case GSM_CMD_TO_ERR:
								case GSM_CMD_REPONSE_ERR:
								{
									gsm_MAE = GSM_SIM_PIN_REQUIS;
									break;
								}
							}
							break;
						}
						case GSM_INIT_ATT_RESEAU:
						{
							if(!GetTimer(TIMER_GSM_TEMPO))
							{
								switch(gsm_SendCmd(GSM_ATCREG, "OK", GSM_TIMEOUT_CMD_AT))
								{
									case GSM_CMD_SUCCES:
									{
										pIndex1 = strstr(gsm_TrameSIM32, "+CREG");
										if(pIndex1)
										{
											pIndex1 += 9;
											if(*pIndex1 == '1' || *pIndex1 == '5')
											{
												gsm_CptConfig = 0;
												gsm_MAEInitCfg = GSM_INIT_CFG2_CMD;
											}
											else
											{
												SetTimer(TIMER_GSM_TEMPO, 100 + (gsm_NombreErreur * 500));
												gsm_NombreErreur++;
												if(gsm_NombreErreur > 2)
												{
													gsm_MAE = GSM_CMD_CFUN;
												}
											}
										}
										break;
									}
									case GSM_CMD_TO_ERR:
									case GSM_CMD_REPONSE_ERR:
									{
										SetTimer(TIMER_GSM_TEMPO, GSM_TIME_LECTURE_ATCREG);
										gsm_NombreErreur++;
										if(gsm_NombreErreur > 10)
										{
											gsm_MAE = GSM_CMD_CFUN;
										}
										break;
									}
								}
							}
							break;
						}
						case GSM_INIT_CFG2_CMD:
						{
							switch(gsm_SendCmd(gsm_pListeConfig2[gsm_CptConfig], "OK", GSM_TIMEOUT_CMD_AT))
							{
								case GSM_CMD_SUCCES:
								{
									if(!gsm_CptConfig) //"AT+CPMS
									{
										//+CPMS: 1,40,1,40,1,40
										pIndex1 = strstr(gsm_TrameSIM32, "+CPMS: ");
										if(pIndex1)
										{
											pIndex1 = strchr(pIndex1, ',');
                                            if(pIndex1)
                                            {
												pIndex1++;
                                                pIndex2 = strchr(pIndex1, ',');
                                                if(pIndex2)
                                                {
                                                    *pIndex2 = 0;
                                                    Val = atoi(pIndex1);
                                                    SetlReg(RV_NOMBRE_MESSAGES_STOCKABLES, Val);
                                                }
                                            }
										}
									}

									gsm_CptConfig++;
									if(gsm_CptConfig >= GSM_TAILLE_LISTE_CONFIG2)
									{
										if(gsmbit.bConfigSysOK && GetFlag(FNV_GSM_MODE_GPRS) && GSM_TAILLE_LISTE_CONFIG_GPRS)
										{
											gsm_NombreErreur = 0;
											gsm_CptConfig = 0;
											strcpy(gsm_Chaine, gsm_pListeConfigGPRS[gsm_CptConfig]);
											gsm_MAEInitCfg = GSM_INIT_GPRS_CMD;
										}
										else
										{
											gsm_CptConfig = 0;
											gsm_NombreErreur = 0;
											SetTimer(TIMER_REVEILLE_SLEEP, GetlReg(RNV_TEMPS_REVEILLE_GSM_OK)*600);
											gsm_MAE = GSM_ORDONNANCEUR;
										}
									}
									break;
								}
								case GSM_CMD_TO_ERR:
								case GSM_CMD_REPONSE_ERR:
								{
									gsm_NombreErreur++;
									if(gsm_NombreErreur > 3)
									{
										gsm_MAE = GSM_CMD_CFUN;
									}
									break;
								}
							}
							break;
						}
						case GSM_INIT_GPRS_CMD:
						{
							switch(gsm_SendCmd(gsm_Chaine, "OK", GSM_TIMEOUT_CMD_AT))
							{
								case GSM_CMD_SUCCES:
								{
									gsm_CptConfig++;
									if(gsm_CptConfig >= GSM_TAILLE_LISTE_CONFIG_GPRS)
									{
										gsm_CptConfig = 0;
										gsm_NombreErreur = 0;
										SetTimer(TIMER_REVEILLE_SLEEP, GetlReg(RNV_TEMPS_REVEILLE_GSM_OK)*600);
										gsm_MAE = GSM_ORDONNANCEUR;
										break;
									}
									strcpy(gsm_Chaine, gsm_pListeConfigGPRS[gsm_CptConfig]);
									break;
								}
								case GSM_CMD_TO_ERR:
								case GSM_CMD_REPONSE_ERR:
								{
									gsm_NombreErreur++;
									if(gsm_NombreErreur > 3)
									{
										gsm_MAE = GSM_CMD_CFUN;
									}
									break;
								}
							}
							break;
						}
					}

					break;
				}

			}
			break;
		}
		case GSM_SIM_PIN_REQUIS:
		{
            gsmbit.bRAZRAMAutorisie = 1;
			return 0;
		}
		case GSM_ORDONNANCEUR:
		{
            if(gUart1Mode == UART_MODE_PAS || gUart2Mode == UART_MODE_PAS)
            {
                return 1;
            }
			if(!GetTimer(TIMER_CONSULT_SMS_GSM) && GetlReg(RNV_VIRIF_SMS))
			{
				SetTimer(TIMER_CONSULT_SMS_GSM, GetlReg(RNV_VIRIF_SMS)*10);
				gsm_MAE = GSM_QUALITE_RESEAUX;
			}
			else if(gsmbit.bAppel)
			{
				gsmbit.bAppel = 0;
				gsm_MAE = GSM_RACCROCHER_APPEL;
			}
			else if(gsmbit.bUserSMS)
			{
				gsmbit.bUserSMS = 0;

				if(gsmbit.bConfigOK)
				{
					pIndex1 = configGSM_String(gsm_aNumTelephone[gsm_IndiceTeleUser]);
					if(gsm_IsValideTelephone(pIndex1))
					{
						gsmbit.bFinTraitementSMS = 1;

						sprintf(gsm_Chaine, GSM_AT_CMGS, pIndex1);

						sprintf(gsm_ReponseSMS, "%s\x1A", gsm_pMsgUserToSend);

						gsm_MAE = GSM_ENVOI1_SMS;
					}
					gsm_pMsgUserToSend = 0;
				}
			}
			else if(GetFlag(FV_ENVOI_SMS_TEST))
			{
				SetFlag(FV_ENVOI_SMS_TEST, 0);
				if(gsmbit.bConfigOK)
				{
					strcpy(gsm_TelephoneUser, configGSM_String(gsm_aNumTelephone[0]));
					gsm_MAE = GSM_USER_ENVOI_RAPPORT;
				}
				gsm_GPRSProblem = 0;
			}
			else if(gsmbit.bAlarme)
			{
				gsmbit.bAlarme = 0;

				if(gsmbit.bConfigOK)
				{
					pIndex1 = configGSM_String(gsm_aNumTelephone[gsm_IndiceTeleAlarme]);
					if(gsm_IsValideTelephone(pIndex1))
					{
						gsmbit.bFinTraitementSMS = 1;

						sprintf(gsm_Chaine, GSM_AT_CMGS, pIndex1);

						if(gsm_NumAlarme)
						{
							sprintf(gsm_ReponseSMS, "A%d:", gsm_NumAlarme);
							gsm_NumAlarme = 0;
						}
						strncat(gsm_ReponseSMS, configGSM_String(gsm_pMsgAlarmeToSend), 80);
						strcat(gsm_ReponseSMS, "\x1A");


						gsm_MAE = GSM_ENVOI1_SMS;
					}
					gsm_pMsgAlarmeToSend = 0;
				}
			}
			else if(GetFlag(FV_ENVOI_RAPPORT))
			{
				if(gsmbit.bConfigOK)
				{
					if(GetFlag(FNV_ENVOI_RAPPORT_AU_TEL(gsm_IndexTeleRapport)))
					{
						strcpy(gsm_TelephoneUser, configGSM_String(gsm_aNumTelephone[gsm_IndexTeleRapport]));
						gsm_MAE = GSM_USER_ENVOI_RAPPORT;
					}

					gsm_IndexTeleRapport++;
					if(gsm_IndexTeleRapport > 3)
					{
						gsm_IndexTeleRapport = 0;
						SetFlag(FV_ENVOI_RAPPORT, 0);
					}
				}
				else
				{
					SetFlag(FV_ENVOI_RAPPORT, 0);
				}
			}
			else if(gsmbit.bTrameGPRS)
			{
				gsmbit.bTrameGPRS = 0;

				gsm_MAE = GSM_UDP_REPONSE_TRAME1;
				gsm_NumSocket = gsm_GPRSCnxNum;

				switch(gsm_NumSocket)
				{
					case GSM_UDPR_SOCKET:
					{
						gsm_UDPNumIP = GetlReg(RNV_NUM_ADD_IP_UDPR);
						gsm_UDPPort = configGSM_Short(gsm_aUDPRPort);
						break;
					}
					case GSM_UDPH_SOCKET:
					{
						gsm_UDPNumIP = GetlReg(RNV_NUM_ADD_IP_UDPH);
						gsm_UDPPort = configGSM_Short(gsm_aUDPHPort);
						break;
					}
					case GSM_TCP_SOCKET:
					{
						break;
					}
				}
			}
			else if(GetFlag(FNV_GSM_MODE_GPRS) && gsmbit.bConfigOK)
			{
				if(gsmbit.bSendUDPR)
				{
					gsmbit.bSendUDPR = 0;
                    gsmbit.bEnvoiSave = 0;
					gsm_UDPNumIP = GetlReg(RNV_NUM_ADD_IP_UDPR);
					gsm_UDPPort = configGSM_Short(gsm_aUDPRPort);
					gsm_NumSocket = GSM_UDPR_SOCKET;
					gsm_MAE = GSM_UDP_DATA;
				}
				if(gsmbit.bSendUDPH)
				{
                    if(gsm_NbrEssaiEnvoi == 0)
                    {
                        gsmbit.bEnvoiSave = 0;
                        gsm_UDPNumIP = GetlReg(RNV_NUM_ADD_IP_UDPH);
                        gsm_UDPPort = configGSM_Short(gsm_aUDPHPort);
                        gsm_NumSocket = GSM_UDPH_SOCKET;
                        gsm_MAE = GSM_UDP_DATA;
                    }
                    else
                    {
                        sprintf(gsm_Chaine, GSM_IP_SEND, gsm_NumSocket, gsm_UDPCptOctet);
                        gsm_MAE = GSM_UDP_ENVOI1;
                        if(gsm_NbrEssaiEnvoi >= 2)
                        {
                            gsmbit.bSendUDPH = 0;
                        }
                    }
                    gsm_NbrEssaiEnvoi++;
				}
				else
				if(gsmbit.bSendSecUDPH)
				{
                    gsmbit.bSendSecUDPH = 0;
					gsmbit.bEnvoiSave = 1;
					gsm_UDPNumIP = GetlReg(RNV_NUM_ADD_IP_UDPH);
					gsm_UDPPort = configGSM_Short(gsm_aUDPHPort);
					gsm_NumSocket = GSM_UDPH_SOCKET;
					gsm_MAE = GSM_UDP_DATA;
				}
				else
				{
                    gsmbit.bRAZRAMAutorisie = 1;
					return 0;
				}
			}
			else
			{
                gsmbit.bRAZRAMAutorisie = 1;
				return 0;
			}
			break;
		}
		case GSM_RACCROCHER_APPEL:
		{
			switch(gsm_SendCmd(GSM_ATH0, "OK", GSM_TIMEOUT_CMD_AT))
			{
				case GSM_CMD_SUCCES:
				{
					if(gsmbit.bConfigOK)
					{
						gsm_MAE = GSM_ENVOI_RAPPORT;
					}
					else
					{
						gsm_MAE = GSM_ORDONNANCEUR;
					}
					gsm_NombreErreur = 0;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_INIT;
						gsm_MAEInit = GSM_INIT_PWR;
						gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
					}
					break;
				}
			}
			break;
		}
		case GSM_ENVOI_RAPPORT:
		{
			if(gsm_Rapport())
			{
				if(strlen(gsm_ChaineTemps) && gsm_IsValideTelephone(gsm_TelephoneAppel))
				{
					gsmbit.bFinTraitementSMS = 1;

					sprintf(gsm_Chaine, GSM_AT_CMGS, gsm_TelephoneAppel);

					strcat(gsm_ReponseSMS, gsm_ChaineTemps);
					strcat(gsm_ReponseSMS, "\x1A");

					gsm_MAE = GSM_ENVOI1_SMS;
				}
				else
				{
					gsm_MAE = GSM_ORDONNANCEUR;
				}
			}
			break;
		}
		case GSM_USER_ENVOI_RAPPORT:
		{
			if(gsm_Rapport())
			{
				if(strlen(gsm_ChaineTemps) && gsm_IsValideTelephone(gsm_TelephoneUser))
				{
					gsmbit.bFinTraitementSMS = 1;

					sprintf(gsm_Chaine, GSM_AT_CMGS, gsm_TelephoneUser);

					strcat(gsm_ReponseSMS, gsm_ChaineTemps);
					strcat(gsm_ReponseSMS, "\x1A");

					gsm_MAE = GSM_ENVOI1_SMS;
				}
				else
				{
					gsm_MAE = GSM_ORDONNANCEUR;
				}
			}
			break;
		}
		case GSM_CMD_CFUN:
		{
			switch(gsm_SendCmd(GSM_AT_CFUN, "OK", GSM_TIMEOUT_CMD_AT))
			{
				case GSM_CMD_SUCCES:
				{
					gsm_MAE = GSM_INIT;
                    gsm_MAEInit = GSM_INIT_PWR;
                    gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
                    SetTimer(TIMER_GSM_TEMPO, 50);
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_INIT;
						gsm_MAEInit = GSM_INIT_PWR;
						gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
                        SetTimer(TIMER_GSM_TEMPO, 50);
					}
					break;
				}
			}
			break;
		}
		case GSM_QUALITE_RESEAUX:
		{
			switch(gsm_SendCmd(GSM_AT_CSQ, "+CSQ:", GSM_TIMEOUT_CMD_AT))
			{
				case GSM_CMD_SUCCES:
				{
					pIndex1 = strstr(gsm_TrameSIM32, "+CSQ: ");
					if(pIndex1)
					{
						pIndex1 = strchr(pIndex1, ' ');
						if(pIndex1)
                        {
                            pIndex2 = strchr(pIndex1, ',');
                            if(pIndex2)
                            {
                                *pIndex2 = 0;
                                Val = atoi(pIndex1);
                                if(Val > 31) Val = 0;

                                gsm_QualiteReseau = Val * 3.2258;

                                SetlReg(RV_QUALITE_SIGNAL_GSM, gsm_QualiteReseau);
                            }
                        }
					}
					gsm_NombreErreur = 0;

					gsm_NumMessage = 0;
					gsm_MAE = GSM_TEST_MESSAGES_EXISTE;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
        case GSM_TEST_MESSAGES_EXISTE:
        {
            switch(gsm_SendCmd(GSM_AT_CPMS, "OK", GSM_TIMEOUT_CMD_AT))
            {
                case GSM_CMD_SUCCES:
                {
                    pIndex1 = strstr(gsm_TrameSIM32, "+CPMS: ");
                    if(pIndex1)
                    {
                        pIndex1 = strchr(gsm_TrameSIM32, ',');
                        if(pIndex1)
                        {
                            pIndex1++;
                            if(*pIndex1 != '0')
                            {
                                gsm_MAE = GSM_MESSAGES_STOCKABLES;
                                break;
                            }
                        }
                    }
                    gsm_MAE = GSM_ORDONNANCEUR;
                    break;
                }
                case GSM_CMD_TO_ERR:
                case GSM_CMD_REPONSE_ERR:
                {
                    gsm_NombreErreur++;
                    if(gsm_NombreErreur > 3)
                    {
                        gsm_MAE = GSM_CMD_CFUN;
                    }
                    break;
                }
            }
            break;
        }
		case GSM_MESSAGES_STOCKABLES:
		{
			gsm_NumMessage++;
			if(gsm_NumMessage <= GetlReg(RV_NOMBRE_MESSAGES_STOCKABLES))
			{
				sprintf(gsm_Chaine, GSM_AT_CMGR, gsm_NumMessage);
				gsm_MAE = GSM_LECTURE_SMS;
			}
			else
			{
				gsm_MAE = GSM_ORDONNANCEUR;
			}
			break;
		}
		case GSM_LECTURE_SMS:
		{
			switch(gsm_SendCmd(gsm_Chaine, "OK", GSM_TIMEOUT_CMD_ATCMGR))
			{
				case GSM_CMD_SUCCES:
				{


                    pIndex1 = strstr(gsm_TrameSIM32, "+CMGR: ");
                    if(pIndex1)
                    {
                        pIndex1 = strstr(gsm_TrameSIM32, "REC UNREAD");
                        if(pIndex1)
                        {
                            /*
                            +CMGR: "REC READ","+21698948883","","11/11/18,09:58:22+04"
                            Salem

                            OK
                            */
                            gsmbit.bSMSOk = 0;
                            pIndex1 = strchr(pIndex1, ',');
                            if(pIndex1)
                            {
                                pIndex1 += 2;
                                pIndex2 = strchr(pIndex1, '\"');
                                if(pIndex2)
                                {
                                    *pIndex2 = 0;
                                    pIndex2++;

                                    if( GetFlag(FNV_GSM_VERIF_NUM_TELE) &&
                                        (
                                        strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[0]), GetlReg(RNV_NBR_CAR_TELE_CMP))&&
                                        strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[1]), GetlReg(RNV_NBR_CAR_TELE_CMP))&&
                                        strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[2]), GetlReg(RNV_NBR_CAR_TELE_CMP))&&
                                        strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[3]), GetlReg(RNV_NBR_CAR_TELE_CMP))
                                        )
                                    )
                                    {
                                        gsm_MAE = GSM_MESSAGES_STOCKABLES;
                                        break;
                                    }
                                    strcpy(gsm_TelephoneSMS, pIndex1);

                                    pIndex1 = strstr(pIndex2, "\r\n");
                                    if(pIndex1)
                                    {
                                        pIndex1 += 2;
                                        pIndex2 = strstr(pIndex1, "\r\n");
                                        if(pIndex2)
                                        {
                                            *pIndex2 = 0;

                                            strcpy(gsm_MessageSMS, pIndex1);

                                            if(gsm_IsValideTelephone(gsm_TelephoneSMS))
                                            {
                                                gsmbit.bSMSOk = 1;
                                            }
                                        }
                                    }
                                }
                            }
                            sprintf(gsm_Chaine, GSM_AT_CMGD, gsm_NumMessage);
                            SetTimer(TIMER_GSM_TEMPO, 20);
                            gsm_MAE = GSM_EFFACER_SMS;
                        //	gsm_MAE = GSM_TRAITTEMENT_SMS;
                        }
                        else
                        {
                            sprintf(gsm_Chaine, GSM_AT_CMGD, gsm_NumMessage);
                            SetTimer(TIMER_GSM_TEMPO, 20);
                            gsm_MAE = GSM_EFFACER_SMS;
                            gsmbit.bSMSOk = 0;
                        }
                    }
                    else
                    {
                        gsm_MAE = GSM_MESSAGES_STOCKABLES;
                    }
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					pIndex1 = strstr(gsm_TrameSIM32, "REC READ");
                    pIndex2 = strstr(gsm_TrameSIM32, "REC UNREAD");
                    if(pIndex1 || pIndex2)
                    {
                        sprintf(gsm_Chaine, GSM_AT_CMGD, gsm_NumMessage);
						SetTimer(TIMER_GSM_TEMPO, 20);
						gsm_MAE = GSM_EFFACER_SMS;
                        gsmbit.bSMSOk = 0;
                    }
					else
					{
                        pIndex1 = strstr(gsm_TrameSIM32, "OK");
                        if(pIndex1)
                        {
                            gsm_MAE = GSM_MESSAGES_STOCKABLES;
                        }
                        else
                        {
                            gsm_NombreErreur++;
                            if(gsm_NombreErreur > 3)
                            {
                                gsm_MAE = GSM_CMD_CFUN;
                            }
                        }
                    }
					break;
				}
			}
			break;
		}
		case GSM_EFFACER_SMS:
		{
			if(!GetTimer(TIMER_GSM_TEMPO))
			{
				switch(gsm_SendCmd(gsm_Chaine, "OK", GSM_TIMEOUT_CMD_AT))
				{
					case GSM_CMD_SUCCES:
					{
						if(gsmbit.bSMSOk)
						{
							gsm_MAE = GSM_TRAITTEMENT_SMS;
						}
						else
						{
							gsm_MAE = GSM_MESSAGES_STOCKABLES;
						}
						break;
					}
					case GSM_CMD_TO_ERR:
					case GSM_CMD_REPONSE_ERR:
					{
						gsm_NombreErreur++;
						if(gsm_NombreErreur > 3)
						{
							gsm_MAE = GSM_CMD_CFUN;
						}
						break;
					}
				}
			}
			break;
		}
		case GSM_TRAITTEMENT_SMS:
		{
			gsmbit.bFinTraitementSMS = 0;
			switch(gsm_TraitementSMS())
			{
				case 1:
				{
					gsm_MAE = GSM_MESSAGES_STOCKABLES;
					break;
				}
				case 2:
				{
					gsmbit.bFinTraitementSMS = 1;
				}
				case 3:
				{
					if(strlen(gsm_ReponseSMS) && gsm_IsValideTelephone(gsm_TelephoneSMS))
					{
						sprintf(gsm_Chaine, GSM_AT_CMGS, gsm_TelephoneSMS);
						strcat(gsm_ReponseSMS, "\x1A");
						gsm_MAE = GSM_ENVOI1_SMS;
					}
					else
					{
						gsm_MAE = GSM_ORDONNANCEUR;
					}
					break;
				}
			}
			break;
		}
		case GSM_ENVOI1_SMS:
		{
			if(!gsmbit.bSMSBloquer)//
			{
				switch(gsm_SendCmd(gsm_Chaine, ">", GSM_TIMEOUT_CMD_AT))
				{
					case GSM_CMD_SUCCES:
					{
						gsm_MAE = GSM_ENVOI2_SMS;
						break;
					}
					case GSM_CMD_TO_ERR:
					case GSM_CMD_REPONSE_ERR:
					{
						gsm_NombreErreur++;
						if(gsm_NombreErreur > 3)
						{
							gsm_MAE = GSM_CMD_CFUN;
						}
						break;
					}
				}
			}
			else
			{
				*gsm_ReponseSMS = 0;
				if(gsmbit.bFinTraitementSMS)
				{
					if(gsm_NumMessage <= GetlReg(RV_NOMBRE_MESSAGES_STOCKABLES))
					{
						gsm_MAE = GSM_MESSAGES_STOCKABLES;
					}
					else
					{
						gsm_MAE = GSM_ORDONNANCEUR;
					}
				}
				else
				{
					gsm_MAE = GSM_TRAITTEMENT_SMS;
				}

				if(gsmbit.bDemandeReset)
				{
					mainbit.bReset = 1;
				}
			}
			break;
		}
		case GSM_ENVOI2_SMS:
		{
			switch(gsm_SendCmd(gsm_ReponseSMS, "OK", 100))
			{
				case GSM_CMD_SUCCES:
				{
					*gsm_ReponseSMS = 0;
					if(gsmbit.bFinTraitementSMS)
					{
						if(gsm_NumMessage <= GetlReg(RV_NOMBRE_MESSAGES_STOCKABLES))
						{
							gsm_MAE = GSM_MESSAGES_STOCKABLES;
						}
						else
						{
							gsm_MAE = GSM_ORDONNANCEUR;
						}
					}
					else
					{
						gsm_MAE = GSM_TRAITTEMENT_SMS;
					}
					gsm_NombreErreur = 0;

					if(gsmbit.bDemandeReset)
					{
						mainbit.bReset = 1;
					}

					SetlReg(RV_CPT_SMS_ENVOYER_HEURE, GetlReg(RV_CPT_SMS_ENVOYER_HEURE) + 1);
					SetlReg(RV_CPT_SMS_ENVOYER_JOURS, GetlReg(RV_CPT_SMS_ENVOYER_JOURS) + 1);

					if(	GetlReg(RV_CPT_SMS_ENVOYER_HEURE) >= GetlReg(RNV_NBR_MAX_SMS_HEURE) ||
						GetlReg(RV_CPT_SMS_ENVOYER_JOURS) >= GetlReg(RNV_NBR_MAX_SMS_JOURS)
					)
					{
						gsmbit.bSMSBloquer = 1;
					}
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_MAE = GSM_ENVOI1_SMS;
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						*gsm_ReponseSMS = 0;
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_UDP_REPONSE_TRAME1:
		{
			if(!comsbusmaitre_bit[0].bSbusLibre)
			{
				break;
			}
			comsbusmaitre_bit[0].bSbusLibre = 0;
			comsbusmaitre_bit[0].bSbusEnd = 0;
			comsbusmaitre_bit[0].SbusErr = 0;

			comsbusmaitre_SbusStation[0] = gsm_GPRSData[0];
			comsbusmaitre_SbusNbr[0] = gsm_GPRSDataNbr;
			memcpy(comsbusmaitre_SbusBuffer[0], gsm_GPRSData, gsm_GPRSDataNbr);

			comsbusmaitre_MAESbusMaitre[0] = SBUS_TRAME;
			gsm_MAE = GSM_UDP_REPONSE_TRAME2;
			break;
		}
		case GSM_UDP_REPONSE_TRAME2:
		{
			if(comsbusmaitre_bit[0].bSbusEnd)
			{
				comsbusmaitre_bit[0].bSbusLibre = 1;
				gsm_UDPCptOctet = sprintf(gsm_BufferTCPUDP, "%s MC3GS", gsm_NomStation());
				if(comsbusmaitre_bit[0].SbusErr)
				{
					strcat(gsm_BufferTCPUDP, "Erreur");
					gsm_UDPCptOctet += 6;
				}
				else
				{
					memcpy(gsm_BufferTCPUDP + gsm_UDPCptOctet, comsbusmaitre_SbusBuffer[0], comsbusmaitre_SbusNbr[0]);
					gsm_UDPCptOctet += comsbusmaitre_SbusNbr[0];
				}

				sprintf(gsm_Chaine, GSM_IP_SEND, gsm_NumSocket, gsm_UDPCptOctet);
				gsm_MAE = GSM_UDP_ENVOI1;
			}
			break;
		}
		case GSM_UDP_DATA:
		{
			switch(gsm_NumSocket)
			{
				case GSM_UDPR_SOCKET:
				{
					if(gsm_GetDataUDPR())
					{
						sprintf(gsm_Chaine, GSM_IP_SEND, gsm_NumSocket, gsm_UDPCptOctet);
						gsm_MAE = GSM_UDP_ENVOI1;
					}
					break;
				}
				case GSM_UDPH_SOCKET:
				{
                    if(gsmbit.bEnvoiSave)
                    {
                        if(gsm_GetDataSecUDPH())
                        {
                            sprintf(gsm_Chaine, GSM_IP_SEND, gsm_NumSocket, gsm_UDPCptOctet);
                            gsm_MAE = GSM_UDP_ENVOI1;
                        }
                        else
                        {
                            gsm_MAE = GSM_ORDONNANCEUR;
                        }
                    }
                    else
                    {
                        if(gsm_GetDataUDPH())
                        {
                            sprintf(gsm_Chaine, GSM_IP_SEND, gsm_NumSocket, gsm_UDPCptOctet);
                            gsm_MAE = GSM_UDP_ENVOI1;
                        }
                        else
                        {
                            gsmbit.bRAZRAMAutorisie = 1;
                            gsm_MAE = GSM_ORDONNANCEUR;
                        }
                    }
					break;
				}
			}
            break;
		}
		case GSM_DEACTIVE_GPRS:
		{
			switch(gsm_SendCmd(GSM_DEACTIVEGPRS, "SHUT OK", 50))
			{
				case GSM_CMD_SUCCES:
				{
					gsm_MAE = GSM_GPRS_ATTCH;

					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}

		case GSM_GPRS_ETAT:
		{
			switch(gsm_SendCmd(GSM_GPRSETAT, "OK", GSM_TIMEOUT_CMD_AT))
			{
				case GSM_CMD_SUCCES:
				{
					if(strstr(gsm_TrameSIM32, "ERROR"))
					{
						gsm_MAE = GSM_GPRS_ATTCH;
					}
					else
					{
						pIndex1 = configGSM_String(gsm_aAPN);
						sprintf(gsm_Chaine, GSM_GPRSAPN, pIndex1, pIndex1+25, pIndex1+50);
						gsm_MAE = GSM_GPRS_APN;
					}
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}

		case GSM_GPRS_ATTCH:
		{
			switch(gsm_SendCmd(GSM_GPRSATTCH, "OK", GSM_TIMEOUT_CMD_GPRSATTCH))
			{
				case GSM_CMD_SUCCES:
				{
					if(strstr(gsm_TrameSIM32, "ERROR"))
					{
						gsm_NombreErreur++;
						if(gsm_NombreErreur > 3)
						{
							gsm_GPRSProblem = 1;
							gsm_MAE = GSM_CMD_CFUN;
						}
					}
					else
					{
						gsm_NombreErreur = 0;
						pIndex1 = configGSM_String(gsm_aAPN);
						sprintf(gsm_Chaine, GSM_GPRSAPN, pIndex1, pIndex1+25, pIndex1+50);
						gsm_MAE = GSM_GPRS_APN;
					}
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_GPRS_APN:
		{
			switch(gsm_SendCmd(gsm_Chaine, "OK", GSM_TIMEOUT_CMD_AT))
			{
				case GSM_CMD_SUCCES:
				{
					gsm_MAE = GSM_GPRS_CONNECT;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_GPRSProblem = 2;
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_GPRS_CONNECT:
		{
			switch(gsm_SendCmd(GSM_GPRSCONNECT, "OK", GSM_TIMEOUT_CMD_GPRS_CNX))
			{
				case GSM_CMD_SUCCES:
				{
					gsm_MAE = GSM_GPRS_ADRESSE;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_GPRSProblem = 3;
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_GPRS_ADRESSE:
		{
			switch(gsm_SendCmd(GSM_GPRSADRESSE, "\r\n", GSM_TIMEOUT_CMD_AT))
			{
				case GSM_CMD_SUCCES:
				{
					sprintf(gsm_Chaine, GSM_IP_START, gsm_NumSocket, configGSM_String(gsm_aAdresseIP[gsm_UDPNumIP]), gsm_UDPPort);
					gsm_MAE = GSM_UDP_START;

					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_GPRSProblem = 4;
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_UDP_START:
		{
			switch(gsm_SendCmd(gsm_Chaine, "CONNECT OK", GSM_TIMEOUT_CMD_UDP_CNX))
			{
				case GSM_CMD_SUCCES:
				{
					switch(gsm_NumSocket)
					{
						case GSM_UDPH_SOCKET:
						case GSM_UDPR_SOCKET:
						{
							sprintf(gsm_Chaine, GSM_IP_SEND, gsm_NumSocket, gsm_UDPCptOctet);
							gsm_MAE = GSM_UDP_ENVOI1;
							break;
						}
						case GSM_TCP_SOCKET:
						{
							break;
						}
					}
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_GPRSProblem = 5;
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_UDP_ENVOI1:
		{
			switch(gsm_SendCmd(gsm_Chaine, "> ", 50))
			{
				case GSM_CMD_SUCCES:
				{
					gsm_MAE = GSM_UDP_ENVOI2;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_MAE = GSM_DEACTIVE_GPRS;
                    gsm_NombreErreur++;
                    if(gsm_NombreErreur > 3)
                    {
                        gsm_MAE = GSM_CMD_CFUN;
                    }
					break;
				}
			}
			break;
		}
		case GSM_UDP_ENVOI2:
		{
			switch(gsm_SendData(gsm_BufferTCPUDP, gsm_UDPCptOctet, "SEND OK", 50))
			{
				case GSM_CMD_SUCCES:
				{
					switch(gsm_NumSocket)
					{
						case GSM_UDPR_SOCKET:
						{
                            SetTimer(TIMER_GSM_TEMPO, 50);
							break;
						}
						case GSM_UDPH_SOCKET:
						{
                            SetTimer(TIMER_GSM_TEMPO, GetlReg(RNV_TEMPS_ENTRE_TRAME_UDPP));

                            if(!gsmbit.bEnvoiSave)
                            {
                                if(GetFlag(FNV_RAZ_SAUVEGARDE_RAM_APRES_ENVOI))
                                {
                                    gsm_RAZHistoRam();
                                }
                                else
                                {
                                    gsmbit.bRAZRAMAutorisie = 1;
                                }
                                SetTimer(TIMER_GSM_TEMPO, 50);
                            }
							break;
						}
					}

    				gsm_MAE = GSM_UDP_TEMPO;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_MAE = GSM_UDP_ENVOI1;
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
		case GSM_UDP_TEMPO:
		{
			if(!GetTimer(TIMER_GSM_TEMPO))
			{
                if(gsmbit.bEnvoiSave)
                {
                    gsm_MAE = GSM_UDP_DATA;
                }
                else
                {
                    gsm_MAE = GSM_ORDONNANCEUR;
                    gsmbit.bSendUDPH = 0;
                }
			}
			break;
		}
		case GSM_UDP_CLOSE:
		{
			switch(gsm_SendCmd(GSM_UDPCLOSE, "OK", 50))
			{
				case GSM_CMD_SUCCES:
				{
					gsm_MAE = GSM_ORDONNANCEUR;
					break;
				}
				case GSM_CMD_TO_ERR:
				case GSM_CMD_REPONSE_ERR:
				{
					gsm_NombreErreur++;
					if(gsm_NombreErreur > 3)
					{
						gsm_MAE = GSM_CMD_CFUN;
					}
					break;
				}
			}
			break;
		}
	}
	return 1;
}


void gsm_Trame(char *pTrame, int Taille)
{
	char Cpt;
	char *pIndex1;
	char *pIndex2;
	int Val;

	pTrame[Taille] = 0;

	pIndex1 = strstr(pTrame, "+CSMINS");
	if(pIndex1)
	{
		pIndex1 = strchr(pIndex1, ',');
        if(pIndex1)
        {
			pIndex1++;
            pIndex2 = strchr(pIndex1, '\n');
            if(pIndex2)
            {
                *pIndex2 = 0;
                Val = atoi(pIndex1);
                switch(Val)
                {
                    case 0:
                    {
                        gsm_MAE = GSM_INIT;
                        gsm_MAEInit = GSM_INIT_PWR;
                        gsm_MAEInitPwr = GSM_INIT_PWR_OFF;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
	}
	else
	{
        pIndex1 = strcmp(pTrame, "\r\n+CREG: 5\r\n");
        pIndex2 = strcmp(pTrame, "\r\n+CREG: 1\r\n");
        if(pIndex1 == 0 || pIndex2 == 0)
        {
            SetTimer(TIMER_GSM_TEMPO, 0);
        }
        else
        {
            //+CLIP: "96832880",129,"",,"",0

            pIndex1 = strstr(pTrame, "+CLIP");
            if(pIndex1)
            {
                SetTimer(TIMER_GSM_TEMPO, 0);
                pIndex1 = strchr(pIndex1, '\"');
                if(pIndex1)
                {
                    pIndex1++;
                    pIndex2 = strchr(pIndex1, '\"');
                    if(pIndex2)
                    {
                        *pIndex2 = 0;

                        if( GetFlag(FNV_GSM_VERIF_NUM_TELE) &&
                            (
                            strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[0]), GetlReg(RNV_NBR_CAR_TELE_CMP))&&
                            strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[1]), GetlReg(RNV_NBR_CAR_TELE_CMP))&&
                            strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[2]), GetlReg(RNV_NBR_CAR_TELE_CMP))&&
                            strrcmp(pIndex1, configGSM_String(gsm_aNumTelephone[3]), GetlReg(RNV_NBR_CAR_TELE_CMP))
                            )
                        )
                        {
                            return;
                        }

                        strcpy(gsm_TelephoneAppel, pIndex1);

                        gsmbit.bAppel = 1;
                    }
                }
            }//+CMTI: "SM",1
            else
            {
                pIndex1 = strstr(pTrame, "+CMTI");
                if(pIndex1)
                {
                    SetTimer(TIMER_GSM_TEMPO, 0);
                    SetTimer(TIMER_CONSULT_SMS_GSM, 0);
                }
                else
                {
                    pIndex1 = strstr(pTrame, "+RECEIVE");//+RECEIVE,0,11:\r\nXXXXX
                    if(pIndex1)
                    {
                        SetTimer(TIMER_GSM_TEMPO, 0);
                        if(!gsmbit.bEnvoiSave)
                        {
                            pIndex1 = strchr(pIndex1, ',');
                            if(pIndex1)
                            {
                                pIndex1++;
                                pIndex2 = strchr(pIndex1, ',');
                                if(pIndex2)
                                {
                                    *pIndex2 = 0;
                                    gsm_GPRSCnxNum = atoi(pIndex1);

                                    pIndex1 = pIndex2 + 1;
                                    pIndex2 = strchr(pIndex1, ':');
                                    if(pIndex2)
                                    {
                                        *pIndex2 = 0;
                                        gsm_GPRSDataNbr = atoi(pIndex1);

                                        pIndex1 = pIndex2 + 3;//\r\n
                                        for(Cpt=0; Cpt<gsm_GPRSDataNbr; Cpt++)
                                        {
                                            gsm_GPRSData[Cpt] = pIndex1[Cpt];
                                        }

                                        gsmbit.bTrameGPRS = 1;

                                        SetTimer(TIMER_GSM_TEMPO, 0);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        strcat(gsm_TrameSIM32, pTrame);
                        gsm_TailleTrame += Taille;
                        gsm_TrameSIM32[gsm_TailleTrame] = 0;
                        gsmbit.bReponse = 1;
                    }
                }
            }
        }
	}
}


//void __ISR(_EXTERNAL_4_VECTOR , ipl3) INT4InterruptHandler(void)
//{
//	mINT4ClearIntFlag();
//	SetTimer(TIMER_GSM_VEILLE, GetlReg(RNV_TEMPS_GSM_VEILLE) * 10);
//	if(GetTimer(TIMER_GSM_TEMPO) > 300)
//	{
//		SetTimer(TIMER_GSM_TEMPO, 0);
//	}
//}

void __ISR( _INPUT_CAPTURE_1_VECTOR, ipl3) IC1Interrupt( void)
{
    int temp;

    SetTimer(TIMER_GSM_VEILLE, GetlReg(RNV_TEMPS_GSM_VEILLE) * 10);
    if(GetTimer(TIMER_GSM_TEMPO) > 300)
    {
	SetTimer(TIMER_GSM_TEMPO, 0);
    }

    // this should clean the interrupt flag
    temp = IC1BUF;

    mIC1ClearIntFlag();
}