
#include "variables.h"

//#define MODEM_GSM_SIMUL

#ifdef MODEM_GSM_SIMUL
	#warning code pour simulation gsm
#endif

#define GSM_TRIS_ALIM	 				_TRISD13
#define GSM_TRIS_RESET	 				_TRISC1
#define GSM_TRIS_POWER	 				_TRISC2
#define GSM_TRIS_ETAT	 				_TRISC3
#define GSM_TRIS_SIM_PRESENT	 		mainbit.bUnused
#define GSM_TRIS_RI				 		_TRISD8

#define GSM_ALIM	 					_LATD13
#define GSM_RESET	 					_LATC1
#define GSM_POWER						_LATC2
#define GSM_ETAT						_RC3
//#define GSM_SIM_PRESENT					(!_RD13)
#define GSM_RI					 		_RD8


#define GSM_UDPR_SOCKET					0
#define GSM_UDPH_SOCKET					1
#define GSM_TCP_SOCKET					2
					
#define GSM_NBR_ANA						24
#define GSM_NBR_ENTREE					12
#define GSM_NBR_ENTREE_ECHANT			4
#define GSM_NBR_SORTIE					12

#define GSM_CMD_SUCCES					0
#define GSM_CMD_ENCOUR					1
#define GSM_CMD_TO_ERR					2
#define GSM_CMD_REPONSE_ERR				3

#define GSM_CMD_ENVOI					0
#define GSM_CMD_ATT_REPONSE				1
#define GSM_CMD_VERIF_REPONSE			2

#define GSM_SBUS_INIT					0
#define GSM_SBUS_REPONSE				1

#define GSM_DH_INIT						0
#define GSM_DH_REPONSE					1
#define GSM_DH_REPONSE_ECRITURE			2


#define GSM_UDP_INIT					0
#define GSM_UDP_GET_VAR					1
#define GSM_INSERT_VARIABLE				2

#define GSM_INIT						0
#define GSM_SIM_PIN_REQUIS				1
#define GSM_ORDONNANCEUR				2
#define GSM_RACCROCHER_APPEL			3
#define GSM_ENVOI_RAPPORT				4
#define GSM_QUALITE_RESEAUX				5
#define GSM_TEST_MESSAGES_EXISTE        6
#define GSM_MESSAGES_STOCKABLES			7
#define GSM_LECTURE_SMS					8
#define GSM_EFFACER_SMS					9
#define GSM_TRAITTEMENT_SMS				10
#define GSM_ENVOI1_SMS					11
#define GSM_ENVOI2_SMS					12
#define GSM_USER_ENVOI_RAPPORT			13
#define GSM_SIM_PRESENCE_ETAT			14
#define GSM_UDP_REPONSE_TRAME1			15
#define GSM_UDP_REPONSE_TRAME2			16
#define GSM_UDP_DATA					17
#define GSM_DEACTIVE_GPRS				18
#define GSM_GPRS_ETAT					19
#define GSM_GPRS_ATTCH					20
#define GSM_GPRS_APN					21
#define GSM_GPRS_CONNECT				22
#define GSM_GPRS_ADRESSE				23
#define GSM_UDP_START					24
#define GSM_UDP_ENVOI1					25
#define GSM_UDP_ENVOI2					26
#define GSM_UDP_TEMPO					27
#define GSM_UDP_CLOSE					28
#define GSM_CMD_CFUN					29


#define GSM_INIT_PWR					0
#define GSM_INIT_CONFIG					1
#define GSM_INIT_CONFIG2				2

#define GSM_INIT_PWR_OFF				0
#define GSM_INIT_PWR_ALIM				1
#define GSM_INIT_PWR_PWRKEY_UP			2
#define GSM_INIT_PWR_PWRKEY_DOWN		3
#define GSM_INIT_PWR_PRE_ATCMD			4

#define GSM_INIT_CFG_VITESSE			0
#define GSM_INIT_VERIF_PIN				1
#define GSM_INIT_ENTRE_PIN				2
#define GSM_INIT_CFG1_CMD				3
#define GSM_INIT_ATT_RESEAU				4
#define GSM_INIT_CFG2_CMD				5
#define GSM_INIT_GPRS_CMD				6
#define GSM_INIT_GPRS_ADRESSE			7

#define GSM_SMS_START					0
#define GSM_SMS_GET_CMD					1
#define GSM_SMS_TRAITTEMENT				2
#define GSM_SMS_REPONSE					3


#define GSM_DUREE_APRES_N_RESET			5
#define GSM_DUREE_OFF					1
#define GSM_DUREE_ON					1
#define GSM_DUREE_PWRKEY_UP				15
#define GSM_DUREE_PRE_ATCMD				30
#define GSM_TIMEOUT_CMD_AT				50
#define GSM_TIMEOUT_CMD_GPRS_CNX		100
#define GSM_TIMEOUT_CMD_UDP_CNX			100
#define GSM_TIMEOUT_CMD_ATCMGR			50
#define GSM_TIMEOUT_CMD_GPRSATTCH       600
#define GSM_TIME_LECTURE_ATCREG			20
#define GSM_NBR_RESET					6


#define GSM_GET_INIT		0
#define GSM_GET_LINE		1
#define GSM_GET_VARIABLE	2
#define GSM_FORMAT_VARIABLE	3

#define	GSM_REQUETTE_RESET			0 
#define	GSM_REQUETTE_VERSION        1 
#define	GSM_REQUETTE_QGSM           2 
#define	GSM_REQUETTE_DH             3 
#define	GSM_REQUETTE_STA            4 
#define	GSM_REQUETTE_GPRS           5 
#define	GSM_REQUETTE_ADMV           6 
#define	GSM_REQUETTE_TRV			7 
#define	GSM_REQUETTE_NT             8
#define	GSM_REQUETTE_HENV           9
#define	GSM_REQUETTE_TENV           10
#define	GSM_REQUETTE_MSA            11
#define	GSM_REQUETTE_CTA            12
#define	GSM_REQUETTE_IA             13
#define	GSM_REQUETTE_AAX            14
#define	GSM_REQUETTE_IIA            15
#define	GSM_REQUETTE_VNT            16
#define	GSM_REQUETTE_NSTA           17
#define	GSM_REQUETTE_MAXSMSH        18
#define	GSM_REQUETTE_MAXSMSJ        19
#define	GSM_REQUETTE_MCU        	20
#define	GSM_REQUETTE_VCU        	21
#define	GSM_REQUETTE_TER        	22
#define	GSM_REQUETTE_AAI        	23
#define	GSM_REQUETTE_DH_COMPILE     24
#define	GSM_REQUETTE_ATCP           25
#define	GSM_REQUETTE_AUDPR          26
#define	GSM_REQUETTE_AUDPH          27
#define	GSM_REQUETTE_IPTCP          28
#define	GSM_REQUETTE_IPUDPR         29
#define	GSM_REQUETTE_IPUDPH         30
#define	GSM_REQUETTE_TCPPORT        31
#define	GSM_REQUETTE_UDPRPORT       32
#define	GSM_REQUETTE_UDPHPORT       33
#define	GSM_REQUETTE_FEUDPR         34
#define	GSM_REQUETTE_FE2UDPR        35
#define	GSM_REQUETTE_FEUDPH         36
#define	GSM_REQUETTE_HDUDPR         37
#define	GSM_REQUETTE_HFUDPR         38
#define	GSM_REQUETTE_HDUDPH         39
#define	GSM_REQUETTE_HFUDPH         40
#define	GSM_REQUETTE_IP             41
#define	GSM_REQUETTE_IDOP           42
#define	GSM_REQUETTE_ESMPR          43
#define	GSM_REQUETTE_TMINESR        44
#define	GSM_REQUETTE_TMAXESR        45
#define	GSM_REQUETTE_TSR            46
#define	GSM_REQUETTE_MSR            47
#define	GSM_REQUETTE_AR             48
#define	GSM_REQUETTE_CTR            49
#define	GSM_REQUETTE_RAPPORT        50
#define	GSM_REQUETTE_SBUS           51
#define	GSM_REQUETTE_ANA_IO         52
#define	GSM_REQUETTE_USER           53
#define	GSM_REQUETTE_NOT_EXISTE     54

#define	GSM_REQUETEANAIO_ANA		0 
#define	GSM_REQUETEANAIO_ENTREE     1 
#define	GSM_REQUETEANAIO_SORTIE     2 

#define GSM_ALARME_INIT				0
#define GSM_ALARME_GET				1
#define GSM_ALARME_DATA				2
#define GSM_ALARME_SET				3
#define GSM_ALARME_SEND_TO_TELE1	4
#define GSM_ALARME_SEND_TO_TELE2	5
#define GSM_ALARME_SEND_TO_TELE3	6
#define GSM_ALARME_SEND_TO_TELE4	7


#define GSM_COND_ENVOI_UDPR_TRAME			0
#define GSM_COND_ENVOI_UDPR_REPONSE			1

#define GSM_UDPR_VERIF						0
#define GSM_UDPR_TEST_MODE					1
#define GSM_UDPR_ATT_REPONSE				2
#define GSM_UDPR_RECHARGE					3

#define GSM_CME_PHONE_FAILURE										0  
#define GSM_CME_NO_CONNECTION_TO_PHONE                              1  
#define GSM_CME_PHONE_ADAPTOR_LINK_RESERVED                         2  
#define GSM_CME_OPERATION_NOT_ALLOWED                               3  
#define GSM_CME_OPERATION_NOT_SUPPORTED                             4  
#define GSM_CME_PH_SIM_PIN_REQUIRED                                 5  
#define GSM_CME_PH_FSIM_PIN_REQUIRED                                6  
#define GSM_CME_PH_FSIM_PUK_REQUIRED                                7  
#define GSM_CME_SIM_NOT_INSERTED                                    10 
#define GSM_CME_SIM_PIN_REQUIRED                                    11 
#define GSM_CME_SIM_PUK_REQUIRED                                    12 
#define GSM_CME_SIM_FAILURE                                         13 
#define GSM_CME_SIM_BUSY                                            14 
#define GSM_CME_SIM_WRONG                                           15 
#define GSM_CME_INCORRECT_PASSWORD                                  16 
#define GSM_CME_SIM_PIN2_REQUIRED                                   17 
#define GSM_CME_SIM_PUK2_REQUIRED                                   18 
#define GSM_CME_MEMORY_FULL                                         20 
#define GSM_CME_INVALID_INDEX                                       21 
#define GSM_CME_NOT_FOUND                                           22 
#define GSM_CME_MEMORY_FAILURE                                      23 
#define GSM_CME_TEXT_STRING_TOO_LONG                                24 
#define GSM_CME_INVALID_CHARACTERS_IN_TEXT_STRING                   25 
#define GSM_CME_DIAL_STRING_TOO_LONG                                26 
#define GSM_CME_INVALID_CHARACTERS_IN_DIAL_STRING                   27 
#define GSM_CME_NO_NETWORK_SERVICE                                  30 
#define GSM_CME_NETWORK_TIMEOUT                                     31 
#define GSM_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY            32 
#define GSM_CME_NETWORK_PERSONALIZATION_PIN_REQUIRED                40 
#define GSM_CME_NETWORK_PERSONALIZATION_PUK_REQUIRED                41 
#define GSM_CME_NETWORK_SUBSET_PERSONALIZATION_PIN_REQUIRED         42 
#define GSM_CME_NETWORK_SUBSET_PERSONALIZATION_PUK_REQUIRED         43 
#define GSM_CME_SERVICE_PROVIDER_PERSONALIZATION_PIN_REQUIRED       44 
#define GSM_CME_SERVICE_PROVIDER_PERSONALIZATION_PUK_REQUIRED       45 
#define GSM_CME_CORPORATE_PERSONALIZATION_PIN_REQUIRED              46 
#define GSM_CME_CORPORATE_PERSONALIZATION_PUK_REQUIRED              47 
#define GSM_CME_RESOURCE_LIMITATION                                 99 
#define GSM_CME_UNKNOWN                                             100
#define GSM_CME_ILLEGAL_MS                                          103
#define GSM_CME_ILLEGAL_ME                                          106
#define GSM_CME_GPRS_SERVICES_NOT_ALLOWED                           107
#define GSM_CME_PLMN_NOT_ALLOWED                                    111
#define GSM_CME_LOCATION_AREA_NOT_ALLOWED                           112
#define GSM_CME_ROAMING_NOT_ALLOWED_IN_THIS_LOCATION_AREA           113
#define GSM_CME_SERVICE_OPTION_NOT_SUPPORTED                        132
#define GSM_CME_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED             133
#define GSM_CME_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER             134
#define GSM_CME_UNSPECIFIED_GPRS_ERROR                              148
#define GSM_CME_PDP_AUTHENTICATION_FAILURE                          149
#define GSM_CME_INVALID_MOBILE_CLASS                                150

#define GSM_CMS_ME_FAILURE									300 
#define GSM_CMS_SMS_ME_RESERVED                             301 
#define GSM_CMS_OPERATION_NOT_ALLOWED                       302 
#define GSM_CMS_OPERATION_NOT_SUPPORTED                     303 
#define GSM_CMS_INVALID_PDU_MODE                            304 
#define GSM_CMS_INVALID_TEXT_MODE                           305 
#define GSM_CMS_SIM_NOT_INSERTED                            310 
#define GSM_CMS_SIM_PIN_NECESSARY                           311 
#define GSM_CMS_PH_SIM_PIN_NECESSARY                        312 
#define GSM_CMS_SIM_FAILURE                                 313 
#define GSM_CMS_SIM_BUSY                                    314 
#define GSM_CMS_SIM_WRONG                                   315 
#define GSM_CMS_SIM_PUK_REQUIRED                            316 
#define GSM_CMS_SIM_PIN2_REQUIRED                           317 
#define GSM_CMS_SIM_PUK2_REQUIRED                           318 
#define GSM_CMS_MEMORY_FAILURE                              320 
#define GSM_CMS_INVALID_MEMORY_INDEX                        321 
#define GSM_CMS_MEMORY_FULL                                 322 
#define GSM_CMS_INVALID_PARAMETER                           323 
#define GSM_CMS_INVALID_INPUT_FORMAT                        324 
#define GSM_CMS_SMSC_ADDRESS_UNKNOWN                        330 
#define GSM_CMS_NO_NETWORK                                  331 
#define GSM_CMS_NETWORK_TIMEOUT                             332 
#define GSM_CMS_NO_CNMA_ACK                                 340 
#define GSM_CMS_UNKNOWN                                     500 
#define GSM_CMS_SIM_NOT_READY                               512 
#define GSM_CMS_UNREAD_RECORDS_ON_SIM                       513 
#define GSM_CMS_CB_ERROR_UNKNOWN                            514 
#define GSM_CMS_PS_BUSY                                     515 
#define GSM_CMS_SM_NOT_READY                                517 
#define GSM_CMS_INVALID_NON_HEX_CHARS_INPDU                 528 
#define GSM_CMS_INCORRECT_PDU_LENGTH                        529 
#define GSM_CMS_INVALID_MTI                                 530 
#define GSM_CMS_INVALID_NON_HEX_CHARS_IN_ADDRESS            531 
#define GSM_CMS_INVALID_ADDRESS_NO_DIGITS_READ              532 
#define GSM_CMS_INCORRECT_PDU_LENGTH_UDL                    533 
#define GSM_CMS_INCORRECT_SCA_LENGTH                        534 
#define GSM_CMS_INVALID_FIRST_OCTET                         536 
#define GSM_CMS_INVALID_COMMAND_TYPE                        537 
#define GSM_CMS_SRR_BIT_NOT_SET                             538 
#define GSM_CMS_SRR_BIT_SET                                 539 
#define GSM_CMS_INVALID_USER_DATA_HEADER_IE                 540 
#define GSM_CMS_CRSM_MISSING_PARAMETER                      753 
#define GSM_CMS_CRSM_INVALID_COMMAND                        754 
#define GSM_CMS_CRSM_INVALID_FILE_ID                        755 
#define GSM_CMS_CRSM_MISSING_P_PARAMETER                    756 
#define GSM_CMS_CRSM_INVALID_P_PARAMETER                    757 
#define GSM_CMS_CRSM_MISSING_COMMAND_DATA                   758 
#define GSM_CMS_CRSM_INVALID_CHARACTERS_IN_COMMAND_DATA     759 
#define GSM_CMS_INVALID_INPUT_VALUE                         765 
#define GSM_CMS_UNSUPPORTED_MODE                            766 
#define GSM_CMS_OPERATION_FAILED                            767 
#define GSM_CMS_MUX_ALREADY_RUNNING                         768 
#define GSM_CMS_UNABLE_TO_GET_CONTROL                       769 
#define GSM_CMS_SIM_NETWORK_REJECT                          770 
#define GSM_CMS_CALL_SETUP_IN_PROGRESS                      771 
#define GSM_CMS_SIM_POWERED_DOWN                            772 
#define GSM_CMS_SIM_FILE_NOT_PRESENT                        773 

typedef union 
{
	struct sAlarmePeriphe
	{
		unsigned	AlarmeToTele1:1;
		unsigned	AlarmeToTele2:1;
		unsigned	AlarmeToTele3:1;
		unsigned	AlarmeToTele4:1;
		unsigned	AlarmeParFronf:1;
		unsigned	Reserved:3;	
	}Info;
	char Data;
}uAlarmePeriphe;

typedef union
{
	struct sHommetMort
	{
		unsigned	Heure:7;
		unsigned	bActive:1;
	}iHommetMort;
	char HommetMort;
}uHommetMort;

typedef union 
{
	struct sSonneAvantEnvoiSMS
	{
		unsigned	Duree:6;
		unsigned	SAA:1;
		unsigned	SAMS:1;
	}iSonneAvantEnvoiSMS;
	char SonneAvantEnvoiSMS;
}uSonneAvantEnvoiSMS;

typedef union 
{
	struct
	{
		long Masque;
		double Coefficient;
		double Offset;
		short Adresse;
		char Station;
		char Type;
		char Decalage;
		char Precision;
	}Info;
	char Data[18];
}SMSEntrantRapport;

typedef union 
{
	struct sEntetSMSAlarme
	{
		unsigned	IntervalVerife:6;
		unsigned	ActiveLocal:1;
		unsigned	ActiveDistant:1;
	}cESMSA;
	char cEntetSMSAlarme;
}EntetSMSAlarme;

typedef union 
{
	struct
	{
		short IntervalleEmission;	
		short Adresse;
		char Station;
		unsigned	AlarmeToTele1:1;
		unsigned	AlarmeToTele2:1;
		unsigned	AlarmeToTele3:1;
		unsigned	AlarmeToTele4:1;
		unsigned	AlarmeParFronf:1;
		unsigned	Reserved:3;
	}Info;
	char Data[6];
}SMSAlarme;

typedef union 
{
	struct sInfoSauvegrde
	{
		short Adresse;
		char NbrReg;
		unsigned TypeVariable:1;
		unsigned TypeSeuil:1;
		unsigned ModeSave:1;
		unsigned EtatCfg:1;
		unsigned Reserved:4;
	}TSTVA;
	long InfoSauvegrde;
}uInfoSauvegrde;

typedef union 
{
	struct sVarUDP
	{
		char Type;
		char Station;
		unsigned short Adresse;
	}Info;
	char Data[4];
}uVarUDP;

typedef union
{
	struct 
	{
		short Adresse;
		char Station;
		char Type;
		long Masque;
		double Coefficient;
		double Offset;
		char Decalage;
	}Info;
	char Data[17];
}FormatVarSave;

typedef struct
{
	unsigned bDemandeReset:1;
	unsigned bSMSBloquer:1;
	unsigned bAppel:1;
	unsigned bSaveRamPlein:1;
	unsigned bConfigSysOK:1;
	unsigned bReponse:1;
	unsigned bConfigOK:1;
	unsigned bReseauOK:1;
	unsigned bCommande:1;
	unsigned bSMSOk:1;
	unsigned bFinTraitementSMS:1;
	unsigned bSMSEntrantEcriture:1;
	unsigned bAlarmeCfg:1;
	unsigned bAlarme:1;
	unsigned bConsultAlarmeDis:1;
	unsigned bActiveGSM:1;
	unsigned bUserSMS:1;
	unsigned bStopGSM:1;
	unsigned bSendUDPR:1;
	unsigned bSendUDPH:1;
	unsigned b3Reset:1;
	unsigned bTrameGPRS:1;
	unsigned bConcat:1;
	unsigned bFreqEnvoi1:1;
	unsigned bEnvoiUDPH:1;
	unsigned bSendSecUDPH:1;
    unsigned bEnvoiSave:1;
	unsigned bRAZRAMAutorisie:1;
}bitgsm;

// Uart GSM
#ifdef MODEM_GSM_SIMUL
#define uart_Init			uart2_Init
#define uart_Gestion		uart2_Gestion
#define uart_Stop			uart2_Stop
#define uart_Rx_Buffer		uart2_Rx_Buffer
#define uart_Write			uart2_Write
#else
#define uart_Init			uart3_Init
#define uart_Gestion		uart3_Gestion
#define uart_Stop			uart3_Stop
#define uart_Rx_Buffer		uart3_Rx_Buffer
#define uart_Write			uart3_Write
#endif

extern bitgsm gsmbit;
extern unsigned char gsm_SbusStation;
extern unsigned char gsm_SbusNbr;
extern unsigned char gsm_SbusType;
extern unsigned char gsm_MAESbusMaitreTemp;
extern ofsld gsm_SbusAdresse[];
extern ofsld gsm_SbusBuffer[];

extern unsigned int gsm_aInitAlarme;
extern unsigned char gsm_NbrAlarme;

extern unsigned int gsm_pMsgAlarmeToSend;
extern unsigned int gsm_aInitCfgAlrmeAna;
extern unsigned int gsm_aInitCfgAlrmeEntree;
extern unsigned char gsm_NumAlarme;
extern unsigned char gsm_IndiceTeleAlarme;
extern unsigned int gsm_aNomAna;
extern unsigned int gsm_aNomEntree;
extern unsigned int gsm_aNomSortie;
extern unsigned int gsm_aNumTelephone[4];

void gsm_Init(void);
char gsm_Gestion(void);
void gsm_Stop(void);
void gsm_Trame(char *pTrame, int Taille);
char gsm_UserSMSLibre(void);
char gsm_UserSMS(char *pMsgUserToSend, char IndiceTeleUser);
char *gsm_NomStation(void);
