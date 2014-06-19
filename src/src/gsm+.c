#include "plib.h"
#include "gsm+.h"
#include "gsm.h"
#include "main.h"
#include "config.h"

unsigned char gsmplus_MAEAlarmePeriphe;
unsigned int gsmplus_aMsgAlarmePeriphe;
uAlarmePeriphe gsmplus_CfgAlrmePeriphe;

unsigned short gsmplus_AlarmePeripheAnaEtat;
unsigned short gsmplus_AlarmePeripheEntreeEtat;

unsigned int gsmplus_aCfgAlrmePeriphe;

const char *gsmplus_pCmdIOAna[GSMPLUS_NBR_CMD_IOANA] = {"COEF", "OFFSET", "FMIN", "FMAX", "TIF", "TF", "SE", "CNSG", "TH", "TB", "HYS", "TFA", "MA", "TMH", "NMH", "VA", "BS", "BO", "AK", "AA", "NOM", "MSGA","TAVA","TAVD","P","TON","MODE","TRA"};

void gsmplus_Init()
{
	int Cpt;
		
	for(Cpt = 0; Cpt < GSM_NBR_ANA; Cpt++)
	{
		SetlReg(RNV_TEMPS_ENTRE_ALARME_ANA(Cpt), 1);
		SetFlag(FNV_ACTIVE_ALARME_GSM_AN(Cpt), 0);
	}
	
	for(Cpt = 0; Cpt < GSM_NBR_ENTREE; Cpt++)
	{
		SetlReg(RNV_TEMPS_ENTRE_ALARME_ENTREE(Cpt), 1);
		SetFlag(FNV_ACTIVE_ALARME_GSM_E(Cpt), 0);
	//	SetFlag(FNV_TYPE_ENTREE_ALARME_GSM(Cpt), 0);
	}
}

int gsmplus_AlarmePeriphe()
{
	int Cpt;
	
	for(Cpt = 0; Cpt < GSM_NBR_ANA; Cpt++)
	{
		if(GetFlag(FNV_ACTIVE_ALARME_GSM_AN(Cpt)))
		{
			SetFlag(FV_ALARME_GSM_AN(Cpt), GetFlag(FV_ALARME_AN(Cpt)));
		}
		else
		{
			SetFlag(FV_ALARME_GSM_AN(Cpt), 0);
		}			
	}		

	for(Cpt = 0; Cpt < GSM_NBR_ENTREE; Cpt++)
	{
		if(GetFlag(FNV_ACTIVE_ALARME_GSM_E(Cpt)))
		{
			if((Cpt < GSM_NBR_ENTREE_ECHANT) && GetFlag(FNV_TYPE_ENTREE_ALARME_GSM(Cpt)))
			{
				SetFlag(FV_ALARME_GSM_E(Cpt), GetFlag(FV_ALARME_E(Cpt)));
			}
			else
			{
				SetFlag(FV_ALARME_GSM_E(Cpt), GetFlag(FV_E(Cpt)));
			}	
		}
		else
		{
			SetFlag(FV_ALARME_GSM_E(Cpt), 0);
		}
	}
}

void gsmplus_VerifierAlarmePeriphe()
{
	int Cpt;
	// Si il y a une alarme encoure d'envoi alors on quitte la fonction
	if(gsm_pMsgAlarmeToSend || !gsmbit.bConfigOK)
	{
		return;
	}
	
	switch(gsmplus_MAEAlarmePeriphe)
	{
		case GSMPLUS_ALARME_PERIPHE_ETAT :
		{
			//Alarme Analogique
			gsmplus_aCfgAlrmePeriphe = gsm_aInitCfgAlrmeAna;
			gsmplus_aMsgAlarmePeriphe = gsmplus_aCfgAlrmePeriphe + 1;
			for(Cpt = 0; Cpt < GSM_NBR_ANA; Cpt++)
			{
				if(!GetTimer(TIMER_ALARME_ANA(Cpt)))
				{
					if(GetFlag(FV_ALARME_GSM_AN(Cpt)))
					{
						gsmplus_CfgAlrmePeriphe.Data = configGSM_Char(gsmplus_aCfgAlrmePeriphe);
																		
						if(	!gsmplus_CfgAlrmePeriphe.Info.AlarmeParFronf ||
							gsmplus_CfgAlrmePeriphe.Info.AlarmeParFronf && ((gsmplus_AlarmePeripheAnaEtat & (0x01<<Cpt)) == 0)
						)
						{
							gsmplus_AlarmePeripheAnaEtat |= (0x01<<Cpt);
							gsmplus_MAEAlarmePeriphe = GSMPLUS_ALARME_PERIPHE_TO_TEL1;
	//						gsmbit.bWakeUpFromGSM = 1;
							SetTimer(TIMER_ALARME_ANA(Cpt), GetlReg(RNV_TEMPS_ENTRE_ALARME_ANA(Cpt)) * 600);
							return;
						}
						
					}
					else
					{
						gsmplus_AlarmePeripheAnaEtat &= ~(0x01<<Cpt);
					}	
				}
				gsmplus_aCfgAlrmePeriphe += 18;
				gsmplus_aMsgAlarmePeriphe = gsmplus_aCfgAlrmePeriphe + 1;
			}
			
			//Alarme Analogique
			gsmplus_aCfgAlrmePeriphe = gsm_aInitCfgAlrmeEntree;
			gsmplus_aMsgAlarmePeriphe = gsmplus_aCfgAlrmePeriphe + 1;
			for(Cpt = 0; Cpt < GSM_NBR_ENTREE; Cpt++)
			{
				if(!GetTimer(TIMER_ALARME_ENTREE(Cpt)))
				{
					if(GetFlag(FV_ALARME_GSM_E(Cpt)))
					{
						gsmplus_CfgAlrmePeriphe.Data = configGSM_Char(gsmplus_aCfgAlrmePeriphe);
						
						if(	!gsmplus_CfgAlrmePeriphe.Info.AlarmeParFronf ||
							gsmplus_CfgAlrmePeriphe.Info.AlarmeParFronf && ((gsmplus_AlarmePeripheEntreeEtat & (0x01<<Cpt)) == 0)
						)
						{
							gsmplus_AlarmePeripheEntreeEtat |= (0x01<<Cpt);
							gsmplus_MAEAlarmePeriphe = GSMPLUS_ALARME_PERIPHE_TO_TEL1;
	//						gsmbit.bWakeUpFromGSM = 1;
							SetTimer(TIMER_ALARME_ENTREE(Cpt), GetlReg(RNV_TEMPS_ENTRE_ALARME_ENTREE(Cpt)) * 600);
							return;
						}
					}
					else
					{
						gsmplus_AlarmePeripheEntreeEtat &= ~(0x01<<Cpt);
					}	
				}
				gsmplus_aCfgAlrmePeriphe += 18;
				gsmplus_aMsgAlarmePeriphe = gsmplus_aCfgAlrmePeriphe + 1;
			}
			break;
		}
		case GSMPLUS_ALARME_PERIPHE_TO_TEL1 :
		{
			gsmbit.bStopGSM = 0;
//			if(gsmplus_MAE != GSMPLUS_ETAT_INIT)
			{
				if(gsmplus_CfgAlrmePeriphe.Info.AlarmeToTele1)
				{
					// Pointe sur l'alarme
					gsm_pMsgAlarmeToSend = gsmplus_aMsgAlarmePeriphe;

					// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
					gsmbit.bAlarme = 1;
					
					gsm_NumAlarme = 0;

//					gsmbit.bPatteRi = 1;

					gsm_IndiceTeleAlarme = 0;
				}
				gsmplus_MAEAlarmePeriphe = GSMPLUS_ALARME_PERIPHE_TO_TEL2;
			}
			break;
		}					
		case GSMPLUS_ALARME_PERIPHE_TO_TEL2 : 	// Pointe sur l'alarme
		{
			if(gsmplus_CfgAlrmePeriphe.Info.AlarmeToTele2)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsmplus_aMsgAlarmePeriphe;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;
				
				gsm_NumAlarme = 0;

//				gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 1;
			}
			gsmplus_MAEAlarmePeriphe = GSMPLUS_ALARME_PERIPHE_TO_TEL3;
			break;
		}					
		case GSMPLUS_ALARME_PERIPHE_TO_TEL3 : 	
		{
			// Pointe sur l'alarme
			if(gsmplus_CfgAlrmePeriphe.Info.AlarmeToTele3)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsmplus_aMsgAlarmePeriphe;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;
				
				gsm_NumAlarme = 0;

//				gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 2;
			}
			gsmplus_MAEAlarmePeriphe = GSMPLUS_ALARME_PERIPHE_TO_TEL4;
			break;
		}					
		case GSMPLUS_ALARME_PERIPHE_TO_TEL4 : 	
		{
			// Pointe sur l'alarme
			if(gsmplus_CfgAlrmePeriphe.Info.AlarmeToTele4)
			{
				// Pointe sur l'alarme
				gsm_pMsgAlarmeToSend = gsmplus_aMsgAlarmePeriphe;

				// On Indique a l'ordonnanceur qu'il y a un alarme a envoyer
				gsmbit.bAlarme = 1;
				
				gsm_NumAlarme = 0;

//				gsmbit.bPatteRi = 1;

				gsm_IndiceTeleAlarme = 3;
			}
			
			gsmplus_MAEAlarmePeriphe = GSMPLUS_ALARME_PERIPHE_ETAT;
			break;
		}					
	}
}

int gsmplus_Gestion()
{
	gsmplus_AlarmePeriphe();
	gsmplus_VerifierAlarmePeriphe();
}

char gsmplus_RequeteAnaIO(char *pCommande, char *pParametre, char *pChaineTemps)
{
	int Cpt;
	int CptEntree;
	unsigned int aNomEntreeSortie;
	unsigned int aCfgMsgAlarmePeriphe;
	char *pDataTemp;
	char *pCmd = NULL;
	char *pNom = NULL;
	char *pPara = NULL;
	
	if(pParametre)
	{
		for(Cpt = 0; Cpt < GSMPLUS_NBR_CMD_IOANA; Cpt++)
		{
			if(strcmp(pCommande, gsmplus_pCmdIOAna[Cpt]) == 0)
			{
				pCmd = pCommande;
				pNom = pParametre;
				
				pPara = strchr(pNom, ' ');
				if(pPara)
				{
					*pPara = 0;
					pPara++;
					gsmbit.bSMSEntrantEcriture = 1;
				}
				else
				{
					gsmbit.bSMSEntrantEcriture = 0;
				}
				
				strtoupper(pNom);
				
				aNomEntreeSortie = gsm_aNomAna;
				aCfgMsgAlarmePeriphe = gsm_aInitCfgAlrmeAna;
				for(CptEntree = 0; CptEntree < GSM_NBR_ANA; CptEntree++)
				{
					sprintf(pChaineTemps, "ANA%d", CptEntree+1);
			
					pDataTemp = configGSM_String(aNomEntreeSortie);
					strtoupper(pDataTemp);
					
					if((*pNom && strcmp(pNom, pChaineTemps) == 0) || (*pDataTemp && strcmp(pNom, pDataTemp) == 0))
					{
						switch(Cpt)
						{
							case GSMPLUS_IOANA_COEF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_COEFF_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Coefficient Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Coefficient Entrée %s est %.6f", pNom, GetfReg(RNV_COEFF_AN(CptEntree)));
								}		
								break;
							}
							case GSMPLUS_IOANA_OFFSET:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_OFFSET_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Offset Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Offset Entrée %s est %.6f", pNom, GetfReg(RNV_OFFSET_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_FMIN:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_FILTRE_MIN_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Filtre Min Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Filtre Min Entrée %s est %.6f", pNom, GetfReg(RNV_FILTRE_MIN_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_FMAX:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_FILTRE_MAX_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Filtre Max Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Filtre Max Entrée %s est %.6f", pNom, GetfReg(RNV_FILTRE_MAX_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TIF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMPS_INSERTION_FIFO_AN(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps Insertion Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps Insertion Entrée %s est %d", pNom, GetlReg(RNV_TEMPS_INSERTION_FIFO_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TAILLE_FIFO_AN(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Taille FIFO Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Taille FIFO Entrée %s est %d", pNom, GetlReg(RNV_TAILLE_FIFO_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_SE:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_SEUIL_ECARTYPE_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Seuil Ecartype Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Seuil Ecartype Entrée %s est %.6f", pNom, GetfReg(RNV_SEUIL_ECARTYPE_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_CNSG:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_CONSIGNE_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Consigne Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Consigne Entrée %s est %.6f", pNom, GetfReg(RNV_CONSIGNE_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TH:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_TOLERANCE_HAUT_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Tolerance Haut Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Tolerance Haut Entrée %s est %.6f", pNom, GetfReg(RNV_TOLERANCE_HAUT_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TB:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_TOLERANCE_BASSE_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Tolerance Basse Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Tolerance Basse Entrée %s est %.6f", pNom, GetfReg(RNV_TOLERANCE_BASSE_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_HYS:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_HYSTERESIS_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Hysteresis Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Hysteresis Entrée %s est %.6f", pNom, GetfReg(RNV_HYSTERESIS_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TFA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMPS_FILTRE_ALARME_AN(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps Filtre Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps Filtre Alarme Entrée %s est %d", pNom, GetlReg(RNV_TEMPS_FILTRE_ALARME_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_MA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_MODE_ALARME_AN(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Mode Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Mode Alarme Entrée %s est %d", pNom, GetlReg(RNV_MODE_ALARME_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TMH:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMPS_MOY_HEURE_AN(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps Moyenne Heure Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps Moyenne Heure %s est %d", pNom, GetlReg(RNV_TEMPS_MOY_HEURE_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_NMH:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_NBR_MOY_HEURE_AN(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Nombre Moyenne Heure Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Nombre Moyenne Heure %s est %d", pNom, GetlReg(RNV_NBR_MOY_HEURE_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_VA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_VALEUR_ABERRANTE_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Valeur Aberrante Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Valeur Aberrante Entrée %s est %.6f", pNom, GetfReg(RNV_VALEUR_ABERRANTE_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_BS:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_KALMAN_Q_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Bruit Systeme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Bruit Systeme Entrée %s est %.6f", pNom, GetfReg(RNV_KALMAN_Q_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_BO:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_KALMAN_R_AN(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Bruit Observation Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Bruit Observation Entrée %s est %.6f", pNom, GetfReg(RNV_KALMAN_R_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_AK:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetFlag(FNV_ACTIVER_KALMAN_AN(CptEntree), atoi(pPara)!=0);
									sprintf(pChaineTemps, "Kalman Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Kalman Entrée %s est %d", pNom, GetFlag(FNV_ACTIVER_KALMAN_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_AA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetFlag(FNV_ACTIVE_ALARME_GSM_AN(CptEntree), atoi(pPara)!=0);
									sprintf(pChaineTemps, "Activation Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Activation Alarme Entrée %s = %d", pNom, GetFlag(FNV_ACTIVE_ALARME_GSM_AN(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TRA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									Cpt = atoi(pPara);
									configGSM_Write((char*)&Cpt, aCfgMsgAlarmePeriphe, 1);
									sprintf(pChaineTemps, "Telephone recevant alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Telephone recevant alarme Entrée %s est %d", pNom, configGSM_Char(aCfgMsgAlarmePeriphe));
								}
								break;
							}
							case GSMPLUS_IOANA_NOM:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									configGSM_Write(pPara, aNomEntreeSortie, 16);
									sprintf(pChaineTemps, "Nom Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Nom Entrée %s est %s", pNom, configGSM_String(aNomEntreeSortie));
								}
								break;
							}
							case GSMPLUS_IOANA_MSGA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									configGSM_Write(pPara, aCfgMsgAlarmePeriphe+1, 16);
									sprintf(pChaineTemps, "Message Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Message Alarme Entrée %s est %s", pNom, configGSM_String(aCfgMsgAlarmePeriphe));
								}
								break;
							}
							default:
							{
								*pChaineTemps = 0;
							}
						}
						return 0;
					}
					aNomEntreeSortie += 17;
					aCfgMsgAlarmePeriphe += 18;
				}
				
				aNomEntreeSortie = gsm_aNomEntree;
				aCfgMsgAlarmePeriphe = gsm_aInitCfgAlrmeEntree;
				for(CptEntree = 0; CptEntree < GSM_NBR_ENTREE; CptEntree++)
				{
					sprintf(pChaineTemps, "E%d", CptEntree+1);
			
					pDataTemp = configGSM_String(aNomEntreeSortie);
					strtoupper(pDataTemp);
					
					if((CptEntree >= GSM_NBR_ENTREE_ECHANT) && (Cpt < GSMPLUS_IOANA_AA)) continue;
					
					if((*pNom && strcmp(pNom, pChaineTemps) == 0) || (*pDataTemp && strcmp(pNom, pDataTemp) == 0))
					{  
						switch(Cpt)
						{
							case GSMPLUS_IOANA_COEF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_COEFF_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Coefficient Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Coefficient Entrée %s est %.6f", pNom, GetfReg(RNV_COEFF_E(CptEntree)));
								}		
								break;
							}
							case GSMPLUS_IOANA_OFFSET:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_OFFSET_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Offset Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Offset Entrée %s est %.6f", pNom, GetfReg(RNV_OFFSET_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_FMIN:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_FILTRE_MIN_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Filtre Min Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Filtre Min Entrée %s est %.6f", pNom, GetfReg(RNV_FILTRE_MIN_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_FMAX:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_FILTRE_MAX_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Filtre Max Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Filtre Max Entrée %s est %.6f", pNom, GetfReg(RNV_FILTRE_MAX_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TIF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMPS_INSERTION_FIFO_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps Insertion Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps Insertion Entrée %s est %d", pNom, GetlReg(RNV_TEMPS_INSERTION_FIFO_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TAILLE_FIFO_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Taille FIFO Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Taille FIFO Entrée %s est %d", pNom, GetlReg(RNV_TAILLE_FIFO_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_SE:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_SEUIL_ECARTYPE_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Seuil Ecartype Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Seuil Ecartype Entrée %s est %.6f", pNom, GetfReg(RNV_SEUIL_ECARTYPE_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_CNSG:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_CONSIGNE_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Consigne Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Consigne Entrée %s est %.6f", pNom, GetfReg(RNV_CONSIGNE_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TH:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_TOLERANCE_HAUT_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Tolerance Haut Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Tolerance Haut Entrée %s est %.6f", pNom, GetfReg(RNV_TOLERANCE_HAUT_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TB:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_TOLERANCE_BASSE_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Tolerance Basse Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Tolerance Basse Entrée %s est %.6f", pNom, GetfReg(RNV_TOLERANCE_BASSE_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_HYS:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_HYSTERESIS_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Hysteresis Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Hysteresis Entrée %s est %.6f", pNom, GetfReg(RNV_HYSTERESIS_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TFA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMPS_FILTRE_ALARME_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps Filtre Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps Filtre Alarme Entrée %s est %d", pNom, GetlReg(RNV_TEMPS_FILTRE_ALARME_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_MA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_MODE_ALARME_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Mode Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Mode Alarme Entrée %s est %d", pNom, GetlReg(RNV_MODE_ALARME_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TMH:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMPS_MOY_HEURE_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps Moyenne Heure Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps Moyenne Heure %s est %d", pNom, GetlReg(RNV_TEMPS_MOY_HEURE_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_NMH:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_NBR_MOY_HEURE_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Nombre Moyenne Heure Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Nombre Moyenne Heure %s est %d", pNom, GetlReg(RNV_NBR_MOY_HEURE_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_VA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_VALEUR_ABERRANTE_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Valeur Aberrante Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Valeur Aberrante Entrée %s est %.6f", pNom, GetfReg(RNV_VALEUR_ABERRANTE_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_BS:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_KALMAN_Q_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Bruit Systeme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Bruit Systeme Entrée %s est %.6f", pNom, GetfReg(RNV_KALMAN_Q_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_BO:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetfReg(RNV_KALMAN_R_E(CptEntree), atof(pPara));
									sprintf(pChaineTemps, "Bruit Observation Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Bruit Observation Entrée %s est %.6f", pNom, GetfReg(RNV_KALMAN_R_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_AK:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetFlag(FNV_ACTIVER_KALMAN_E(CptEntree), atoi(pPara)!=0);
									sprintf(pChaineTemps, "Kalman Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Kalman Entrée %s est %d", pNom, GetFlag(FNV_ACTIVER_KALMAN_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_AA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetFlag(FNV_ACTIVE_ALARME_GSM_E(CptEntree), atoi(pPara)!=0);
									sprintf(pChaineTemps, "ActivationAlarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Activation Alarme Entrée %s = %d", pNom, GetFlag(FNV_ACTIVE_ALARME_GSM_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TAVA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMP_AV_ACTIVATION_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps avant activation Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps avant activation Entrée %s est %d", pNom, GetlReg(RNV_TEMP_AV_ACTIVATION_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TAVD:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMP_AV_DESACTIVATION_E(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps avant desactivation Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps avant desactivation Entrée %s est %d", pNom, GetlReg(RNV_TEMP_AV_DESACTIVATION_E(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TRA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									Cpt = atoi(pPara);
									configGSM_Write((char*)&Cpt, aCfgMsgAlarmePeriphe, 1);
									sprintf(pChaineTemps, "Telephone recevant alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Telephone recevant alarme Entrée %s est %d", pNom, configGSM_Char(aCfgMsgAlarmePeriphe));
								}
								break;
							}
							case GSMPLUS_IOANA_NOM:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									configGSM_Write(pPara, aNomEntreeSortie, 16);
									sprintf(pChaineTemps, "Nom Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Nom Entrée %s est %s", pNom, configGSM_String(aNomEntreeSortie));
								}
								break;
							}
							case GSMPLUS_IOANA_MSGA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									configGSM_Write(pPara, aCfgMsgAlarmePeriphe + 1, 16);
									sprintf(pChaineTemps, "Message Alarme Entrée %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Message Alarme Entrée %s est %s", pNom, configGSM_String(aCfgMsgAlarmePeriphe + 1));
								}
								break;
							}
							default:
							{
								*pChaineTemps = 0;
							}
						}
						return 0;
					}
					aNomEntreeSortie += 17;
					aCfgMsgAlarmePeriphe += 18;
				}
				
				aNomEntreeSortie = gsm_aNomSortie;
				for(CptEntree = 0; CptEntree < GSM_NBR_SORTIE; CptEntree++)
				{
					sprintf(pChaineTemps, "S%d", CptEntree+1);
			
					pDataTemp = configGSM_String(aNomEntreeSortie);
					strtoupper(pDataTemp);
					
					if((*pNom && strcmp(pNom, pChaineTemps) == 0) || (*pDataTemp && strcmp(pNom, pDataTemp) == 0))
					{
						switch(Cpt)
						{
							
							case GSMPLUS_IOANA_TAVA:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMP_AV_ACTIVATION_S(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps avant activation Sortie %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps avant activation Sortie %s est %ld", pNom, GetlReg(RNV_TEMP_AV_ACTIVATION_S(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TF:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMP_ACTIVATION_S(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps fonctionnement Sortie %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps fonctionnement Sortie %s est %ld", pNom, GetlReg(RNV_TEMP_ACTIVATION_S(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_P:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMP_AV_ACTIVATION_S(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Periode Sortie %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Periode Sortie %s est %ld", pNom, GetlReg(RNV_TEMP_AV_ACTIVATION_S(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_TON:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetlReg(RNV_TEMP_ACTIVATION_S(CptEntree), atoi(pPara));
									sprintf(pChaineTemps, "Temps en 1 Sortie %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Temps en 1 Sortie %s est %ld", pNom, GetlReg(RNV_TEMP_ACTIVATION_S(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_MODE:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									SetFlag(FNV_MODE_SORTIE(CptEntree), atoi(pPara)!=0);
									sprintf(pChaineTemps, "Mode Sortie %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Mode Sortie %s est %d", pNom, GetFlag(FNV_MODE_SORTIE(CptEntree)));
								}
								break;
							}
							case GSMPLUS_IOANA_NOM:
							{
								if(gsmbit.bSMSEntrantEcriture)
								{
									configGSM_Write(pPara, aNomEntreeSortie, 16);
									sprintf(pChaineTemps, "Nom Sortie %s est modifiee", pNom);
								}
								else
								{
									sprintf(pChaineTemps, "Nom Sortie %s est %s", pNom, configGSM_String(aNomEntreeSortie));
								}
								break;
							}
							default:
							{
								*pChaineTemps = 0;
							}
						}
						return 0;
					}
					aNomEntreeSortie += 17;
				}
				*pChaineTemps = 0;
				return 0;
			}	
		}
	}
	
	
	aNomEntreeSortie = gsm_aNomAna;	
	for(Cpt = 0; Cpt < GSM_NBR_ANA; Cpt++)
	{
		sprintf(pChaineTemps, "ANA%d", Cpt+1);
		
		pDataTemp = configGSM_String(aNomEntreeSortie);
		strtoupper(pDataTemp);
		
		if(strcmp(pCommande, pChaineTemps) == 0 || (*pDataTemp && strcmp(pCommande, pDataTemp) == 0))
		{
			sprintf(pChaineTemps, "%s = %.2f", pCommande, GetfReg(RV_MOY_ANA_AN(Cpt)));
			return 0;
		}
		aNomEntreeSortie += 17;
	}
		
	aNomEntreeSortie = gsm_aNomEntree;	
	for(Cpt = 0; Cpt < GSM_NBR_ENTREE; Cpt++)
	{
		sprintf(pChaineTemps, "E%d", Cpt+1);
		
		pDataTemp = configGSM_String(aNomEntreeSortie);
		strtoupper(pDataTemp);
		
		if(strcmp(pCommande, pChaineTemps) == 0 || (*pDataTemp && strcmp(pCommande, pDataTemp) == 0))
		{
			sprintf(pChaineTemps, "%s = %d", pCommande, GetFlag(FV_E(Cpt)));
			return 0;
		}
		aNomEntreeSortie += 17;
	}
	
	aNomEntreeSortie = gsm_aNomSortie;	
	for(Cpt = 0; Cpt < GSM_NBR_SORTIE; Cpt++)
	{
		sprintf(pChaineTemps, "S%d", Cpt+1);
		
		pDataTemp = configGSM_String(aNomEntreeSortie);
		strtoupper(pDataTemp);
		
		if(strcmp(pCommande, pChaineTemps) == 0 || (*pDataTemp && strcmp(pCommande, pDataTemp) == 0))
		{
			if(gsmbit.bSMSEntrantEcriture)
			{
				SetFlag(FV_S(Cpt), atoi(pParametre) != 0);
				sprintf(pChaineTemps, "%s est modifie", pCommande);
			}
			else
			{
				sprintf(pChaineTemps, "%s = %d", pCommande, GetFlag(FV_S(Cpt)));
			}					
			return 0;
		}
		aNomEntreeSortie += 17;
	}
	return 1;	
}


char gsmplus_Requete(char *pCommande, char *pParametre, char *pChaineTemps)
{
	return gsmplus_RequeteAnaIO(pCommande, pParametre, pChaineTemps);
}
