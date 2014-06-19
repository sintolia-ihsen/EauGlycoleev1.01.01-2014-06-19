//////////////////////////////////////////////////////////////////////////
// Date de création : 14 Février 2007									//
// Auteur			: Faouzi											//
// Description		: gérer le protocole XSBus Esclave d'un PIC			//
//////////////////////////////////////////////////////////////////////////
#include <plib.h>
#include "slvxsbus.h"
#include "variables.h"
#include "main.h"
#include "timers.h"
#include "s1d13700.h"
#include "config.h"
#include "historique.h"
//#include "histograph.h"

// Configuration
//extern bool bConfigModified;
octetflags sbusoctetflags;
bitslv	slvbit;


// Variables globales de fonctionnement XSBus
long Uart1_AdresseDebut;
long Uart1_AdresseFin;
char slvxsbus_Buffer[1024];
ofsld slv_AdresseBuf[64];

//extern unsigned char __attribute__((far)) s1d13700_MemImage[9600];

//----------------------------------------------------------------------
// Fonction : Initialisation XSBus
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void slvxsbus_Init()
{
	slvbit.bDemandeReset = 0;
	slvbit.bDataFlash = 0;
}

#ifdef SLVXSBUS_RQ_READ_REG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de registres
	// Entrée 	: Pointeur sur les données de la trame
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadReg(char *pBuffer)
	{
		unsigned int Nbr = 0;
		ofsld Base;
		ofsld ValTemp;
		unsigned int i = 0;

		// Lecture AdBase et Nbr
		Nbr = (pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1);

		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Vérification dépassement capacité
#if (USE_FLASH)
		//Si les variable est dans la flash
		if(Base.us[0] >= BASE_NVREGS1 && Base.us[0] + Nbr <= BASE_NVREGS1 + NB_NVREGS_TOTAL1)
		{
			Base.us[0] -= BASE_NVREGS1;
			for(i = 0; i < Nbr; i++)
			{
				if(!Sec_ReadReg(0, Base.us[0] + i, &ValTemp.l))
				{
					return 0;
				}
				pBuffer[(i*4) + 0] = ValTemp.of.octet[3];
				pBuffer[(i*4) + 1] = ValTemp.of.octet[2];
				pBuffer[(i*4) + 2] = ValTemp.of.octet[1];
				pBuffer[(i*4) + 3] = ValTemp.of.octet[0];
			}
		}
		else
		if(Base.us[0] >= BASE_NVREGS2 && Base.us[0] + Nbr <= BASE_NVREGS2 + NB_NVREGS_TOTAL2)
		{
			Base.us[0] -= BASE_NVREGS2;
			for(i = 0; i < Nbr; i++)
			{
				if(!Sec_ReadReg(1, Base.us[0] + i, &ValTemp.l))
				{
					return 0;
				}
				pBuffer[(i*4) + 0] = ValTemp.of.octet[3];
				pBuffer[(i*4) + 1] = ValTemp.of.octet[2];
				pBuffer[(i*4) + 2] = ValTemp.of.octet[1];
				pBuffer[(i*4) + 3] = ValTemp.of.octet[0];
			}
		}
		else
		if(Base.us[0] >= BASE_NVREGS3 && Base.us[0] + Nbr <= BASE_NVREGS3 + NB_NVREGS_TOTAL3)
		{
			Base.us[0] -= BASE_NVREGS3;
			for(i = 0; i < Nbr; i++)
			{
				if(!Sec_ReadReg(2, Base.us[0] + i, &ValTemp.l))
				{
					return 0;
				}
				pBuffer[(i*4) + 0] = ValTemp.of.octet[3];
				pBuffer[(i*4) + 1] = ValTemp.of.octet[2];
				pBuffer[(i*4) + 2] = ValTemp.of.octet[1];
				pBuffer[(i*4) + 3] = ValTemp.of.octet[0];
			}
		}
		else
		if(Base.us[0] >= BASE_NVREGS4 && Base.us[0] + Nbr <= BASE_NVREGS4 + NB_NVREGS_TOTAL4)
		{
			Base.us[0] -= BASE_NVREGS4;
			for(i = 0; i < Nbr; i++)
			{
				if(!Sec_ReadReg(3, Base.us[0] + i, &ValTemp.l))
				{
					return 0;
				}
				pBuffer[(i*4) + 0] = ValTemp.of.octet[3];
				pBuffer[(i*4) + 1] = ValTemp.of.octet[2];
				pBuffer[(i*4) + 2] = ValTemp.of.octet[1];
				pBuffer[(i*4) + 3] = ValTemp.of.octet[0];
			}
		}
		else
#endif
		if(Base.us[0] >= BASE_HISTORIQUE && Base.us[0] + Nbr <= BASE_HISTORIQUE + NB_REG_HISTORIQUE_TOTAL)
		{
			Base.us[0] -= BASE_HISTORIQUE;
			for(i = 0; i < Nbr; i++)
			{
				historique_Read((int *)ValTemp.c, Base.us[0] + i, 4);
				pBuffer[(i*4) + 0] = ValTemp.c[3];
				pBuffer[(i*4) + 1] = ValTemp.c[2];
				pBuffer[(i*4) + 2] = ValTemp.c[1];
				pBuffer[(i*4) + 3] = ValTemp.c[0];
			}
		}
		else
		if(Nbr <= SLVXSBUS_NB_REGS_MAX && Nbr+Base.us[0] <= SLVXSBUS_NB_REGS_TOTAL)
		{
			// Préparation de la trame de réponse
			for(i = 0; i < Nbr; i++)
			{
				ValTemp.l = Registres[Base.us[0] + i];
				pBuffer[(i*4) + 0] = ValTemp.of.octet[3];
				pBuffer[(i*4) + 1] = ValTemp.of.octet[2];
				pBuffer[(i*4) + 2] = ValTemp.of.octet[1];
				pBuffer[(i*4) + 3] = ValTemp.of.octet[0];
			}
		}
		else
		{
			return 0;
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_MULTI_REG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de registres
	// Entrée 	: Pointeur sur les données de la trame
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadMultiReg(char *pBuffer)
	{
		unsigned int Nbr = 0;
		unsigned int Cpt = 0;
		ofsld Base;
		ofsld ValTemp;

		// Lecture AdBase et Nbr
		Nbr = (pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1);

		if(Nbr > SLVXSBUS_NB_MULTI_REGS_MAX) return 0;

		// Copier des adresse pour ne pas etre écraser
		for(Cpt = 0; Cpt < (Nbr*4); Cpt++)
		{
			((char *)slv_AdresseBuf)[Cpt] = pBuffer[SLVXSBUS_OFFSET_DATA + Cpt];
		}

		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
			Base.of.octet[3] = slv_AdresseBuf[Cpt].of.octet[0];
			Base.of.octet[2] = slv_AdresseBuf[Cpt].of.octet[1];
			Base.of.octet[1] = slv_AdresseBuf[Cpt].of.octet[2];
			Base.of.octet[0] = slv_AdresseBuf[Cpt].of.octet[3];

			// Vérification dépassement capacité
		#if (USE_FLASH)
			//Si les variable est dans la flash
			if(Base.l >= BASE_NVREGS1 && Base.l <= BASE_NVREGS1 + NB_NVREGS_TOTAL1)
			{
				// Préparation de la trame de réponse
				if(!Sec_ReadReg(0, Base.l-BASE_NVREGS1, &ValTemp.l))
				{
					Base.l = -1;
					ValTemp.l = -1;
				}
			}
			else
			if(Base.l >= BASE_NVREGS2 && Base.l <= BASE_NVREGS2 + NB_NVREGS_TOTAL2)
			{
				// Préparation de la trame de réponse
				if(!Sec_ReadReg(1, Base.l-BASE_NVREGS2, &ValTemp.l))
				{
					Base.l = -1;
					ValTemp.l = -1;
				}
			}
			else
			if(Base.l >= BASE_NVREGS3 && Base.l <= BASE_NVREGS3 + NB_NVREGS_TOTAL3)
			{
				// Préparation de la trame de réponse
				if(!Sec_ReadReg(2, Base.l-BASE_NVREGS3, &ValTemp.l))
				{
					Base.l = -1;
					ValTemp.l = -1;
				}
			}
			else
			if(Base.l >= BASE_NVREGS4 && Base.l <= BASE_NVREGS4 + NB_NVREGS_TOTAL4)
			{
				// Préparation de la trame de réponse
				if(!Sec_ReadReg(3, Base.l-BASE_NVREGS4, &ValTemp.l))
				{
					Base.l = -1;
					ValTemp.l = -1;
				}
			}
			else
			#endif

			if(Base.l <= SLVXSBUS_NB_REGS_TOTAL)
			{
				// Préparation de la trame de réponse
				ValTemp.l = Registres[Base.l];
			}
			else
			{
				Base.l = -1;
				ValTemp.l = -1;
				///////
			}

			// Adresse
			pBuffer[(Cpt * 8) + 0] = Base.of.octet[3];
			pBuffer[(Cpt * 8) + 1] = Base.of.octet[2];
			pBuffer[(Cpt * 8) + 2] = Base.of.octet[1];
			pBuffer[(Cpt * 8) + 3] = Base.of.octet[0];

			// Valeur
			pBuffer[(Cpt * 8) + 4] = ValTemp.of.octet[3];
			pBuffer[(Cpt * 8) + 5] = ValTemp.of.octet[2];
			pBuffer[(Cpt * 8) + 6] = ValTemp.of.octet[1];
			pBuffer[(Cpt * 8) + 7] = ValTemp.of.octet[0];
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_TIMER_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de timers
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadTimer(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld ValTemp;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr
		Nbr = (pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1);

		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Vérification dépassement capacité
		if(Nbr > SLVXSBUS_NB_TIMERS_MAX || Nbr+Base.us[0] > SLVXSBUS_NB_TIMERS_TOTAL)
		{
			return 0;
		}

		// Préparation de la trame de réponse
		for(i = 0; i < Nbr; i++)
		{
			ValTemp.l = GetTimerIsr(Base.us[0] + i);

			pBuffer[(i * 4) + 0] = ValTemp.of.octet[3];
			pBuffer[(i * 4) + 1] = ValTemp.of.octet[2];
			pBuffer[(i * 4) + 2] = ValTemp.of.octet[1];
			pBuffer[(i * 4) + 3] = ValTemp.of.octet[0];
		}
		return 1;
	}
#endif
#ifdef SLVXSBUS_RQ_READ_MULTI_TIMER_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de registres
	// Entrée 	: Pointeur sur les données de la trame
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadMultiTimer(char *pBuffer)
	{
		unsigned int Nbr = 0;
		unsigned int Cpt = 0;
		ofsld Base;
		ofsld ValTemp;

		// Lecture AdBase et Nbr
		Nbr = (pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1);

		if(Nbr > SLVXSBUS_NB_MULTI_TIMERS_MAX) return 0;

		// Copier des adresse pour ne pas etre écraser
		for(Cpt = 0; Cpt < (Nbr*4); Cpt++)
		{
			((char *)slv_AdresseBuf)[Cpt] = pBuffer[SLVXSBUS_OFFSET_DATA + Cpt];
		}

		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
			Base.of.octet[3] = slv_AdresseBuf[Cpt].of.octet[0];
			Base.of.octet[2] = slv_AdresseBuf[Cpt].of.octet[1];
			Base.of.octet[1] = slv_AdresseBuf[Cpt].of.octet[2];
			Base.of.octet[0] = slv_AdresseBuf[Cpt].of.octet[3];


			if(Base.l <= SLVXSBUS_NB_TIMERS_TOTAL)
			{
				// Préparation de la trame de réponse
				ValTemp.l = Timers[Base.l];
			}
			else
			{
				Base.l = -1;
				ValTemp.l = -1;
				///////
			}

			// Adresse
			pBuffer[(Cpt * 8) + 0] = Base.of.octet[3];
			pBuffer[(Cpt * 8) + 1] = Base.of.octet[2];
			pBuffer[(Cpt * 8) + 2] = Base.of.octet[1];
			pBuffer[(Cpt * 8) + 3] = Base.of.octet[0];

			// Valeur
			pBuffer[(Cpt * 8) + 4] = ValTemp.of.octet[3];
			pBuffer[(Cpt * 8) + 5] = ValTemp.of.octet[2];
			pBuffer[(Cpt * 8) + 6] = ValTemp.of.octet[1];
			pBuffer[(Cpt * 8) + 7] = ValTemp.of.octet[0];
		}
		return 1;
	}
#endif


#ifdef SLVXSBUS_RQ_READ_FLAG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de flags
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadFlag(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr
		Nbr =  pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1;
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Préparation de la trame de réponse
		for(i = 0; i < 16; i++) pBuffer[i] = 0;

		for(i = 0; i < Nbr; i++)
		{
            sbusoctetflags.flags.f8 = GetFlag(Base.us[0] + i);
            
			switch(i%8)
			{
				case 0 : sbusoctetflags.flags.f0 = sbusoctetflags.flags.f8; break;
				case 1 : sbusoctetflags.flags.f1 = sbusoctetflags.flags.f8; break;
				case 2 : sbusoctetflags.flags.f2 = sbusoctetflags.flags.f8; break;
				case 3 : sbusoctetflags.flags.f3 = sbusoctetflags.flags.f8; break;
				case 4 : sbusoctetflags.flags.f4 = sbusoctetflags.flags.f8; break;
				case 5 : sbusoctetflags.flags.f5 = sbusoctetflags.flags.f8; break;
				case 6 : sbusoctetflags.flags.f6 = sbusoctetflags.flags.f8; break;
				case 7 : sbusoctetflags.flags.f7 = sbusoctetflags.flags.f8; 
                         pBuffer[i/8] = sbusoctetflags.octet[0];
                         break;
			}
		}
        if(i%8)
        {
            pBuffer[i/8] = sbusoctetflags.octet[0];
        }
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_MULTI_FLAG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de flags
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadMultiFlag(char *pBuffer)
	{
		ofsld Base;
		ofsld ValTemp;
		unsigned int Nbr;
		unsigned int Cpt;


		// Lecture AdBase et Nbr
		Nbr =  pBuffer[SLVXSBUS_OFFSET_TAILLE]+1;

		if(Nbr > SLVXSBUS_NB_MULTI_FLAGS_MAX) return 0;

		// Copier des adresse pour ne pas etre écraser
		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
            slv_AdresseBuf[Cpt].of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + (Cpt*4) + 0];
			slv_AdresseBuf[Cpt].of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + (Cpt*4) + 1];
			slv_AdresseBuf[Cpt].of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + (Cpt*4) + 2];
			slv_AdresseBuf[Cpt].of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + (Cpt*4) + 3];
		}

        for(Cpt = 0; Cpt < 640; Cpt++) pBuffer[Cpt] = 0;

		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
            // Adresse
			pBuffer[(Cpt * 5) + 0] = slv_AdresseBuf[Cpt].of.octet[3];
			pBuffer[(Cpt * 5) + 1] = slv_AdresseBuf[Cpt].of.octet[2];
			pBuffer[(Cpt * 5) + 2] = slv_AdresseBuf[Cpt].of.octet[1];
			pBuffer[(Cpt * 5) + 3] = slv_AdresseBuf[Cpt].of.octet[0];

			// Valeur
			pBuffer[(Cpt * 5) + 4] = GetFlag(slv_AdresseBuf[Cpt].l);
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_INPUT_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture d'entrées
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadInput(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr
		Nbr = (pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1);
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Vérification dépassement capacité
		if(Nbr > SLVXSBUS_NB_INPUT_MAX || (Nbr + Base.us[0]) > SLVXSBUS_NB_INPUT_TOTAL)
		{
			return 0;
		}

		// Préparation de la trame de réponse
		pBuffer[0] = 0x00;
		pBuffer[1] = 0x00;
		for(i = 0; i < Nbr; i++)
		{
			if(slvxsbus_ReadInput(i + Base.us[0]))
			{
				pBuffer[0] += 0x01 << i;
			}
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_OUTPUT_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Préparation réponse lecture de sorties
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadOutput(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld Base = 0;
		unsigned char i = 0;

		// Lecture AdBase et Nbr
		Nbr = (pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1);
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Vérification dépassement capacité
		if(Nbr > SLVXSBUS_NB_OUTPUT_MAX || Nbr + Base.us[0] > SLVXSBUS_NB_OUTPUT_TOTAL)
		{
			return 0;
		}

		// Préparation de la trame de réponse
		pBuffer[0] = 0x00;
		pBuffer[1] = 0x00;
		for(i = 0; i < Nbr; i++)
		{
			if(slvxsbus_ReadOutput(i + Base.us[0]))
			{
				pBuffer[0] += 0x01 << i;
			}
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_MEM_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement Requête Lecture Mémoire
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon erreur
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadMem(char *pBuffer)
	{
		char *pMem;
		int Nbr = 0;
		ofsld Base;
		int i = 0;

		// Lecture Nbr
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1;

		if(Nbr > SLVXSBUS_NB_MEM_MAX) return 0;
		// Lecture adresse de base
		// Limité à 24 bits pour un PIC (16 Mo d'adressage)
		// lParamTraiteMem = pBuffer[SLVXSBUS_OFFSET_DATA + 1] * 0x10000 + pBuffer[SLVXSBUS_OFFSET_DATA + 2] * 0x100 + pBuffer[SLVXSBUS_OFFSET_DATA + 3];
		// On modifie l'ordre des octets pour le convertir en donnée 32 bits (bug compilateur)
		Base.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 2];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 3];

		// Préparation de la trame de réponse
		pBuffer[0] = 0x00;
		pBuffer[1] = 0x00;

		#if (USE_FLASH)
		if(Base.ul >= 0xFFE00000 && Base.ul + Nbr <= (0xFFE00000 + TAILLE_SECTEUR_CONFIG_GSM))// ROM (flash)
		{
			Base.ul -=0xFFE00000;
			configGSM_Read(pBuffer, Base.ul, Nbr);

			return 1;
		}
        if(Base.ul >= 0xFFF00000 && Base.ul + Nbr <= (0xFFF00000 + TAILLE_SECTEUR_CONFIG_AFF))// ROM (flash)
		{
			Base.ul -=0xFFF00000;
			configAFF_Read(pBuffer, Base.ul, Nbr);

			return 1;
		}
		#endif
		if(Base.ul >= 0x80000000 && Base.ul + Nbr < 0x80004800)// RAM
		{
			Base.ul -=0x80000000;
			pMem = (char *)(int)Base.ul;
			for(i = 0; i < Nbr; i++)
			{
				pBuffer[i] = *(pMem + i);
			}
			return 1;
		}
		return 0;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_MEM_EX_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement Requête Lecture Mémoire
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon erreur
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadMemEx(char *pBuffer)
	{
		char *pMem;
		int Nbr = 0;
		ofsld Base;
		int i = 0;

		// Lecture Nbr
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_TAILLE + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_TAILLE + 1];
		Nbr = Base.s[0]+1;

		if(Nbr > SLVXSBUS_NB_MEM_MAX) return 0;
		// Lecture adresse de base
		// Limité à 24 bits pour un PIC (16 Mo d'adressage)
		// lParamTraiteMem = pBuffer[SLVXSBUS_OFFSET_DATA + 1] * 0x10000 + pBuffer[SLVXSBUS_OFFSET_DATA + 2] * 0x100 + pBuffer[SLVXSBUS_OFFSET_DATA + 3];
		// On modifie l'ordre des octets pour le convertir en donnée 32 bits (bug compilateur)
		Base.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 0];
		Base.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 1];
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 2];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 3];

		// Préparation de la trame de réponse
		pBuffer[0] = 0x00;
		pBuffer[1] = 0x00;

		#if (USE_FLASH)
		if(Base.ul >= 0xFFE00000 && Base.ul + Nbr <= (0xFFE00000 + TAILLE_SECTEUR_CONFIG_GSM))// ROM (flash)
		{
			Base.ul -=0xFFE00000;
			configGSM_Read(pBuffer, Base.ul, Nbr);

			return 1;
		}
		if(Base.ul >= 0xFFF00000 && Base.ul + Nbr <= (0xFFF00000 + TAILLE_SECTEUR_CONFIG_AFF))// ROM (flash)
		{
			Base.ul -=0xFFF00000;
			configAFF_Read(pBuffer, Base.ul, Nbr);

			return 1;
		}
		#endif
		if(Base.ul >= 0x80000000 && Base.ul + Nbr < 0x80004800)// RAM
		{
			Base.ul -=0x80000000;
			pMem = (char *)(int)Base.ul;
			for(i = 0; i < Nbr; i++)
			{
				pBuffer[i] = *(pMem + i);
			}
			return 1;
		}
		if(Base.ul >= 0x70000000 && Base.ul + Nbr <= 0x70002580)// RAM
		{
			Base.ul -=0x70000000;

			for(i = 0; i < Nbr; i++)
			{
				pBuffer[i] = s1d13700_MemImage[Base.ul + i];
			}
			return 1;
		}
		return 0;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_REG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de registres
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteReg(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld ValTemp;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr Registres
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE];
		Nbr -= 1;
		Nbr /= 4;

		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];
	//	pBufferReg = (long *)(pBuffer + SLVXSBUS_OFFSET_DATA + 2);
		if(Nbr > SLVXSBUS_NB_REGS_MAX)
		{
			return 0;
		}
		
		// Vérification dépassement capacité
#if (USE_FLASH)
		if(Base.us[0] >= BASE_NVREGS1 && Base.us[0] + Nbr <= BASE_NVREGS1 + NB_NVREGS_TOTAL1)
		{
			Base.us[0] -= BASE_NVREGS1;
			//ecriture frome maine
			for(i = 0; i < Nbr; i++)
			{
				ValTemp.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 2 + (i*4)];
				ValTemp.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i*4)];
				ValTemp.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 4 + (i*4)];
				ValTemp.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 5 + (i*4)];

				if(!Sec_WriteReg(0, Base.us[0] + i, ValTemp.l))
				{
					return 0;
				}
			}
		}
		else
		if(Base.us[0] >= BASE_NVREGS2 && Base.us[0] + Nbr <= BASE_NVREGS2 + NB_NVREGS_TOTAL2)
		{
			Base.us[0] -= BASE_NVREGS2;
			//ecriture frome maine
			for(i = 0; i < Nbr; i++)
			{
				ValTemp.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 2 + (i*4)];
				ValTemp.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i*4)];
				ValTemp.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 4 + (i*4)];
				ValTemp.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 5 + (i*4)];

				if(!Sec_WriteReg(1, Base.us[0] + i, ValTemp.l))
				{
					return 0;
				}
			}
		}
		else
		if(Base.us[0] >= BASE_NVREGS3 && Base.us[0] + Nbr <= BASE_NVREGS3 + NB_NVREGS_TOTAL3)
		{
			Base.us[0] -= BASE_NVREGS3;
			//ecriture frome maine
			for(i = 0; i < Nbr; i++)
			{
				ValTemp.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 2 + (i*4)];
				ValTemp.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i*4)];
				ValTemp.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 4 + (i*4)];
				ValTemp.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 5 + (i*4)];

				if(!Sec_WriteReg(2, Base.us[0] + i, ValTemp.l))
				{
					return 0;
				}
			}
		}
		else
		if(Base.us[0] >= BASE_NVREGS4 && Base.us[0] + Nbr <= BASE_NVREGS4 + NB_NVREGS_TOTAL4)
		{
			Base.us[0] -= BASE_NVREGS4;
			//ecriture frome maine
			for(i = 0; i < Nbr; i++)
			{
				ValTemp.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 2 + (i*4)];
				ValTemp.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i*4)];
				ValTemp.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 4 + (i*4)];
				ValTemp.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 5 + (i*4)];

				if(!Sec_WriteReg(3, Base.us[0] + i, ValTemp.l))
				{
					return 0;
				}
			}
		}
		else

#endif
		if((Nbr + Base.us[0]) <= SLVXSBUS_NB_REGS_TOTAL)
		{
			// Ecriture des Registres
			for(i = 0; i < Nbr; i++)
			{
				ValTemp.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 2 + (i*4)];
				ValTemp.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i*4)];
				ValTemp.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 4 + (i*4)];
				ValTemp.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 5 + (i*4)];

				Registres[Base.us[0] + i] = ValTemp.l;
			}

		}
		else
		{
			return 0;
		}
		return 1;
	}
#endif


#ifdef SLVXSBUS_RQ_WRITE_MULTI_REG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de registres
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteMultiReg(char *pBuffer)
	{
		unsigned int Cpt;
		unsigned char Nbr;
		ofsld ValTemp;
		ofsld Base;

		// Lecture AdBase et Nbr Registres
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE];
		Nbr -= 1;
		Nbr /= 4;

		if(Nbr > SLVXSBUS_NB_MULTI_REGS_MAX) return 0;

		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
			Base.of.octet[3] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 0];
			Base.of.octet[2] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 1];
			Base.of.octet[1] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 2];
			Base.of.octet[0] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 3];

			ValTemp.of.octet[3] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 4];
			ValTemp.of.octet[2] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 5];
			ValTemp.of.octet[1] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 6];
			ValTemp.of.octet[0] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 7];

			// Vérification dépassement capacité
	#if (USE_FLASH)
			if(Base.l >= BASE_NVREGS1 && Base.l <= BASE_NVREGS1 + NB_NVREGS_TOTAL1)
			{
				Base.l -= BASE_NVREGS1;
				//ecriture frome maine
				Sec_WriteReg(0, Base.l, ValTemp.l);
			}
			else
			if(Base.l >= BASE_NVREGS2 && Base.l <= BASE_NVREGS2 + NB_NVREGS_TOTAL2)
			{
				Base.l -= BASE_NVREGS2;
				//ecriture frome maine
				Sec_WriteReg(1, Base.l, ValTemp.l);
			}
			else
			if(Base.l >= BASE_NVREGS3 && Base.l <= BASE_NVREGS3 + NB_NVREGS_TOTAL3)
			{
				Base.l -= BASE_NVREGS3;
				//ecriture frome maine
				Sec_WriteReg(2, Base.l, ValTemp.l);
			}
			else
			if(Base.l >= BASE_NVREGS4 && Base.l <= BASE_NVREGS4 + NB_NVREGS_TOTAL4)
			{
				Base.l -= BASE_NVREGS4;
				//ecriture frome maine
				Sec_WriteReg(3, Base.l, ValTemp.l);
			}
			else

	#endif
			if(Base.l <= SLVXSBUS_NB_REGS_TOTAL)
			{
				Registres[Base.l] = ValTemp.l;
			}
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_TIMER_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de Timer
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteTimer(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld ValTemp;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr Timer
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE];
		Nbr -=  1;
		Nbr /= 4;
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Vérification dépassement de capacité
		if(Nbr > SLVXSBUS_NB_TIMERS_MAX || (Nbr + Base.us[0]) > SLVXSBUS_NB_TIMERS_TOTAL)
		{
			return 0;
		}

		// Ecriture des Registres
		for(i = 0; i < Nbr; i++)
		{
			ValTemp.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 2 + (i*4)];
			ValTemp.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i*4)];
			ValTemp.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 4 + (i*4)];
			ValTemp.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 5 + (i*4)];

			SetTimerIsr(Base.us[0] + i, ValTemp.l);
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_MULTI_TIMER_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de Timer
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteMultiTimer(char *pBuffer)
	{
		unsigned int Nbr = 0;
		ofsld ValTemp;
		ofsld Base;
		unsigned int Cpt = 0;

		// Lecture AdBase et Nbr Timer
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE];
		Nbr -=  1;
		Nbr /= 4;

		if(Nbr > SLVXSBUS_NB_MULTI_TIMERS_MAX) return 0;

		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
			Base.of.octet[3] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 0];
			Base.of.octet[2] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 1];
			Base.of.octet[1] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 2];
			Base.of.octet[0] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 3];

			ValTemp.of.octet[3] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 4];
			ValTemp.of.octet[2] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 5];
			ValTemp.of.octet[1] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 6];
			ValTemp.of.octet[0] = pBuffer[(Cpt * 8) + SLVXSBUS_OFFSET_DATA + 7];

			// Vérification dépassement de capacité
			if(Base.l < SLVXSBUS_NB_TIMERS_TOTAL)
			{
				SetTimerIsr(Base.l, ValTemp.l);
			}
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_FLASH_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de Timer
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadFlash(char *pBuffer, long *pAdresseDebut, long *pAdresseFin)
	{
		unsigned long Nbr = 0;
		unsigned char i = 0;


		// Vérification dépassement de capacité
		if(	*pAdresseDebut >= *pAdresseFin || *pAdresseFin > 0x20000)
		{
			return 0;
		}

		Nbr = *pAdresseFin - *pAdresseDebut;
		if(Nbr > 128)
		{
			Nbr = 128;
		}

		pBuffer[0]	= 0xFE;
		pBuffer[1]	= 0xFE;
		pBuffer[2]	= 0xFE;
		pBuffer[3]	= 0xFE;
		*((long *)(pBuffer + 4)) = *pAdresseDebut;
		// Ecriture des Registres
		for(i = 0; i < Nbr; i++)
		{
			pBuffer[8+i] = *((const char *)(*pAdresseDebut));
			(*pAdresseDebut)++;
		}
		return Nbr+10;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_FLAG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de flags
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteFlag(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr Registres
		Nbr = pBuffer[SLVXSBUS_OFFSET_DATA + 2] + 1;
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];


		for(i = 0; i < Nbr; i++)
		{
            switch(i%8)
			{
				case 0 : sbusoctetflags.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i/8)];
						 slvbit.bTempbitsbus = sbusoctetflags.flags.f0; break;
				case 1 : slvbit.bTempbitsbus = sbusoctetflags.flags.f1; break;
				case 2 : slvbit.bTempbitsbus = sbusoctetflags.flags.f2; break;
				case 3 : slvbit.bTempbitsbus = sbusoctetflags.flags.f3; break;
				case 4 : slvbit.bTempbitsbus = sbusoctetflags.flags.f4; break;
				case 5 : slvbit.bTempbitsbus = sbusoctetflags.flags.f5; break;
				case 6 : slvbit.bTempbitsbus = sbusoctetflags.flags.f6; break;
				case 7 : slvbit.bTempbitsbus = sbusoctetflags.flags.f7; break;
			}

            SetFlag(Base.us[0] + i, slvbit.bTempbitsbus);
		}
		
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_MULTI_FLAG_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de flags
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteMultiFlag(char *pBuffer)
	{
		ofsld Base;
		ofsld ValTemp1;
		ofsld ValTemp2;
		unsigned int Nbr;
		unsigned int Cpt;

		// Lecture AdBase et Nbr Registres
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE] + 1;

		if(Nbr > SLVXSBUS_NB_MULTI_FLAGS_MAX) return 0;

		for(Cpt = 0; Cpt < Nbr; Cpt++)
		{
			Base.of.octet[3] = pBuffer[(Cpt * 5) + SLVXSBUS_OFFSET_DATA + 0];
			Base.of.octet[2] = pBuffer[(Cpt * 5) + SLVXSBUS_OFFSET_DATA + 1];
			Base.of.octet[1] = pBuffer[(Cpt * 5) + SLVXSBUS_OFFSET_DATA + 2];
			Base.of.octet[0] = pBuffer[(Cpt * 5) + SLVXSBUS_OFFSET_DATA + 3];

			ValTemp1.of.octet[0] = pBuffer[(Cpt * 5) + SLVXSBUS_OFFSET_DATA + 4];

            SetFlag(Base.l, ValTemp1.of.octet[0]);
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_OUTPUT_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement écriture de sorties
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteOutput(char *pBuffer)
	{
		unsigned char Nbr = 0;
		ofsld Base;
		unsigned char i = 0;

		// Lecture AdBase et Nbr Registres
		Nbr = pBuffer[SLVXSBUS_OFFSET_DATA + 2] + 1;
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];

		// Vérification dépassement capacité
		if(Nbr > SLVXSBUS_NB_OUTPUT_MAX || Nbr + Base.us[0] > SLVXSBUS_NB_OUTPUT_TOTAL)
		{
			return 0;
		}

		// Ecriture des Sorties
		for(i = 0; i < Nbr; i++)
		{
			if(pBuffer[SLVXSBUS_OFFSET_DATA + 3 + (i / 8)] & (0x01 << i))
			{
				slvxsbus_WriteOutput(i + Base.us[0], 1);
			}
			else
			{
				slvxsbus_WriteOutput(i + Base.us[0], 0);
			}
		}
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_MEM_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement Requête Ecriture Mémoire
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon erreur
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteMem(char *pBuffer)
	{
		char *pMem;
		int i = 0;
		int Nbr = 0;
		ofsld Base;

		// Lecture Nbr
		Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE]  - 3;

		if(Nbr > SLVXSBUS_NB_MEM_MAX) return 0;
		// Lecture adresse de base
		// Limité à 24 bits pour un PIC (16 Mo d'Adressage)
		// lParamTraiteMem = pBuffer[SLVXSBUS_OFFSET_DATA + 1] * 0x10000 + pBuffer[SLVXSBUS_OFFSET_DATA + 2] * 0x100 + pBuffer[SLVXSBUS_OFFSET_DATA + 3];
		// On modifie l'ordre des octets pour le convertir en donnée 32 bits (bug compilateur)
		Base.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA + 0];
		Base.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA + 1];
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA + 2];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA + 3];


		#if (USE_FLASH)

		if(Base.ul >= 0xFFE00000 && Base.ul + Nbr <= (0xFFE00000 + TAILLE_SECTEUR_CONFIG_GSM))// ROM (flash)
		{
			Base.ul -=0xFFE00000;
			configGSM_Write(pBuffer + 7, Base.ul, Nbr);

			return 1;
		}

		if(Base.ul >= 0xFFF00000 && Base.ul + Nbr <= (0xFFF00000 + TAILLE_SECTEUR_CONFIG_AFF))// ROM (flash)
		{
			Base.ul -=0xFFF00000;
			configAFF_Write(pBuffer + 7, Base.ul, Nbr);

			return 1;
		}

		#endif
		if(Base.ul >= 0x80000000 && Base.ul + Nbr <= 0x80004800)// RAM
		{
			Base.ul -=0x80000000;
			pMem = (char *)(int)Base.ul;
			for(i = 0; i < Nbr; i++)
			{
				*(pMem + i) = pBuffer[i + 7];
			}
			return 1;
		}
		return 0;
	}
#endif

#ifdef SLVXSBUS_RQ_WRITE_MEM_EX_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement Requête Ecriture Mémoire
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon erreur
	//----------------------------------------------------------------------
	int slvxsbus_TraiteWriteMemEx(char *pBuffer)
	{
		char *pMem;
		int i = 0;
		int Nbr = 0;
		ofsld Base;

		// Lecture Nbr
		// Nbr = pBuffer[SLVXSBUS_OFFSET_TAILLE]  - 3;
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_TAILLE + 0];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_TAILLE + 1];
		Nbr = Base.s[0] - 4;

		if(Nbr > SLVXSBUS_NB_MEM_MAX) return 0;
		// Lecture adresse de base
		// Limité à 24 bits pour un PIC (16 Mo d'Adressage)
		// lParamTraiteMem = pBuffer[SLVXSBUS_OFFSET_DATA + 1] * 0x10000 + pBuffer[SLVXSBUS_OFFSET_DATA + 2] * 0x100 + pBuffer[SLVXSBUS_OFFSET_DATA + 3];
		// On modifie l'ordre des octets pour le convertir en donnée 32 bits (bug compilateur)
		Base.of.octet[3] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 0];
		Base.of.octet[2] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 1];
		Base.of.octet[1] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 2];
		Base.of.octet[0] = pBuffer[SLVXSBUS_OFFSET_DATA_MEM_EX + 3];


		#if (USE_FLASH)

		#endif
		if(Base.ul >= 0xFFE00000 && Base.ul + Nbr <= (0xFFE00000 + TAILLE_SECTEUR_CONFIG_GSM))// ROM (flash)
		{
			Base.ul -=0xFFE00000;
			configGSM_Write(pBuffer + 8, Base.ul, Nbr);

			return 1;
		}

		if(Base.ul >= 0xFFF00000 && Base.ul + Nbr <= (0xFFF00000 + TAILLE_SECTEUR_CONFIG_AFF))// ROM (flash)
		{
			Base.ul -=0xFFF00000;
			configAFF_Write(pBuffer + 8, Base.ul, Nbr);

			return 1;
		}

		if(Base.ul >= 0x80000000 && Base.ul + Nbr <= 0x80004800)// RAM
		{
			Base.ul -=0x80000000;
			pMem = (char *)(int)Base.ul;
			for(i = 0; i < Nbr; i++)
			{
				*(pMem + i) = pBuffer[i+8];
			}
			return 1;
		}

//        if(Base.ul >= 0x90000000 && Base.ul + Nbr <= 0x900000C8)// TABLE Histo max 50 reg : 200 octet
//		{
//			Base.ul -=0x90000000;
//			pMem = (char *)(terminal_HistoDataTable + (int)Base.ul);
//			for(i = 0; i < Nbr; i++)
//			{
//				*(pMem + i) = pBuffer[i+8];
//			}
//			return 1;
//		}

//		if(Base.ul >= 0x95000000 && Base.ul + Nbr <= 0x95001770)// TABLE Graph max 1500 reg : 6000 octet
//		{
//			Base.ul -=0x95000000;
//			pMem = (char *)(terminal_HistoDataGraph + (int)Base.ul);
//			for(i = 0; i < Nbr; i++)
//			{
//				*(pMem + i) = pBuffer[i+8];
//			}
//			return 1;
//		}

		return 0;
	}
#endif

#ifdef SLVXSBUS_RQ_RUN_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement du Redémarrage à froid
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteRun(void)
	{
		gSysEtat = RUN;
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_STOP_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement du Redémarrage à froid
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteStop(void)
	{
		gSysEtat = STOP;
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_RESET_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement du Redémarrage à froid
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReset(void)
	{
		slvbit.bDemandeReset = 1;
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_READ_STATUS_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement du Redémarrage à froid
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteReadEtat(void)
	{
		return gSysEtat;
	}
#endif

#ifdef SLVXSBUS_RQ_FLASH_USER_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement FLASHAGE USER (effacement secteurs [1 - 127])
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteFlashUser(void)
	{
		return 1;
	}
#endif

#ifdef SLVXSBUS_RQ_FLASH_CORE_ACTIF
	//----------------------------------------------------------------------
	// Fonction : Traitement FLASHAGE CORE (effacement complet de la FLASH)
	// Entrée 	: Rien
	// Sortie 	: 1 si OK sinon 0
	//----------------------------------------------------------------------
	int slvxsbus_TraiteFlashCore(void)
	{
		return 1;
	}
#endif


//-------------------------------------------
// Fonction : Interruption en réception UART
// Entrée 	: Rien
// Sortie 	: Rien
//-------------------------------------------
void slvxsbus_Gestion(char *pBuffer, unsigned short Taille, unsigned char Destination, void (*fonc)())
{
	int NbCar;
	unsigned char Station;
	unsigned char Requette;
	ofsld TempCRC;

	if(Taille < 1024)
	{
		for(NbCar = 0; NbCar < Taille; NbCar++)
		{
			slvxsbus_Buffer[NbCar] = pBuffer[NbCar];
		}

		if(	Taille >= 5 &&
			slvxsbus_Buffer[0] == 'S' &&
			slvxsbus_Buffer[1] == 'Y' &&
			slvxsbus_Buffer[2] == 'N' &&
			slvxsbus_Buffer[3] == 'C'
		)
		{
			slvbit.bSync = 1;
			Station = slvxsbus_Buffer[4];
		}
		else
		{
			slvbit.bSync = 0;
			Station = slvxsbus_Buffer[SLVXSBUS_OFFSET_NUMSTATION];
		}



		if(Station == SLVXSBUS_NUM_STATION || Station == SLVXSBUS_NUM_STATION_UNIVERSELLE)
		{
			if(slvbit.bSync)
			{
				slvxsbus_Buffer[0] = 'O';
				slvxsbus_Buffer[1] = 'K';
				NbCar = 2;
			}
			else
			{

				TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, Taille-2);

				Requette = slvxsbus_Buffer[SLVXSBUS_OFFSET_RQ];


				if(TempCRC.of.octet[1] != (unsigned char)slvxsbus_Buffer[Taille - 2] || TempCRC.of.octet[0]  != (unsigned char)slvxsbus_Buffer[Taille-1])
				{
					slvxsbus_Buffer[0] = 0x15;
					NbCar = 1;
				}
				else
				{
					// Test du code requête
					switch(Requette)
					{
						#ifdef SLVXSBUS_RQ_READ_REG_ACTIF
							case SLVXSBUS_RQ_READ_REG 	:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] + 1) * 4 + 2; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadReg(slvxsbus_Buffer))	// Traitement
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_MULTI_REG_ACTIF
							case SLVXSBUS_RQ_READ_MULTI_REG 	:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] + 1) * 8 + 2; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadMultiReg(slvxsbus_Buffer))	// Traitement
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
							#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_TIMER_ACTIF
							case SLVXSBUS_RQ_READ_TIMER	:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] + 1) * 4 + 2; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadTimer(slvxsbus_Buffer))	// Traitement
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_MULTI_TIMER_ACTIF
							case SLVXSBUS_RQ_READ_MULTI_TIMER	:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] + 1) * 8 + 2; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadMultiTimer(slvxsbus_Buffer))	// Traitement
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_FLAG_ACTIF
							case SLVXSBUS_RQ_READ_FLAG	:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] / 8) + 3; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadFlag(slvxsbus_Buffer))
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_MULTI_FLAG_ACTIF
							case SLVXSBUS_RQ_READ_MULTI_FLAG	:
							{
								NbCar = ((slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] + 1) * 5) + 2; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadMultiFlag(slvxsbus_Buffer))
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_INPUT_ACTIF
							case SLVXSBUS_RQ_READ_INPUT	:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] / 8) + 3; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadInput(slvxsbus_Buffer))
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_OUTPUT_ACTIF
							case SLVXSBUS_RQ_READ_OUTPUT:
							{
								NbCar = (slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] / 8) + 3; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadOutput(slvxsbus_Buffer))
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_MEM_EX_ACTIF
							case SLVXSBUS_RQ_READ_MEM_EX	:
							{
								TempCRC.of.octet[0] = slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE + 1];
								TempCRC.of.octet[1] = slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE + 0];
								NbCar = TempCRC.s[0] + 3;
								
								if(slvxsbus_TraiteReadMemEx(slvxsbus_Buffer))
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_MEM_EX_ACTIF
							case SLVXSBUS_RQ_WRITE_MEM_EX	:
							{
								if(slvxsbus_TraiteWriteMemEx(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_READ_MEM_ACTIF
							case SLVXSBUS_RQ_READ_MEM	:
							{
								NbCar = slvxsbus_Buffer[SLVXSBUS_OFFSET_TAILLE] + 3; 	// Indication Taille de la réponse

								if(slvxsbus_TraiteReadMem(slvxsbus_Buffer))
								{
									TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
									slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
									slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_MEM_ACTIF
							case SLVXSBUS_RQ_WRITE_MEM	:
							{
								if(slvxsbus_TraiteWriteMem(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_FLAG_ACTIF
							case SLVXSBUS_RQ_WRITE_FLAG	:
							{
								if(slvxsbus_TraiteWriteFlag(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_MULTI_FLAG_ACTIF
							case SLVXSBUS_RQ_WRITE_MULTI_FLAG	:
							{
								NbCar = 1;									// Compteur = 1 caractère envoyé

								if(slvxsbus_TraiteWriteMultiFlag(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_OUTPUT_ACTIF
							case SLVXSBUS_RQ_WRITE_OUTPUT:
							{
								if(slvxsbus_TraiteWriteOutput(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_REG_ACTIF
							case SLVXSBUS_RQ_WRITE_REG	:
							{
								if(slvxsbus_TraiteWriteReg(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_MULTI_REG_ACTIF
							case SLVXSBUS_RQ_WRITE_MULTI_REG	:
							{
								if(slvxsbus_TraiteWriteMultiReg(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_TIMER_ACTIF
							case SLVXSBUS_RQ_WRITE_TIMER:
							{
								if(slvxsbus_TraiteWriteTimer(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_WRITE_MULTI_TIMER_ACTIF
							case SLVXSBUS_RQ_WRITE_MULTI_TIMER:
							{
								if(slvxsbus_TraiteWriteMultiTimer(slvxsbus_Buffer))
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
						#ifdef SLVXSBUS_RQ_RESET_ACTIF
							case SLVXSBUS_RQ_RESET :
							{
								if(slvxsbus_TraiteReset())
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						#ifdef SLVXSBUS_RQ_RUN_ACTIF
							case SLVXSBUS_RQ_RUN :
							{
								if(slvxsbus_TraiteRun())
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						#ifdef SLVXSBUS_RQ_STOP_ACTIF
							case SLVXSBUS_RQ_STOP :
							{
								if(slvxsbus_TraiteStop())
								{
									slvxsbus_Buffer[0] = 0x06;
									NbCar = 1;
								}
								else
								{
									slvxsbus_Buffer[0] = 0x15;
									NbCar = 1;
								}
								break;
							}
						#endif
						#ifdef SLVXSBUS_RQ_READ_STATUS_ACTIF
							case SLVXSBUS_RQ_READ_STATUS :
							{
								NbCar = 3;
								slvxsbus_Buffer[0] = slvxsbus_TraiteReadEtat();

								TempCRC.us[0] = Calcul_CRC16(slvxsbus_Buffer, NbCar-2);
								slvxsbus_Buffer[NbCar-2] = TempCRC.of.octet[1];
								slvxsbus_Buffer[NbCar-1] = TempCRC.of.octet[0];
								break;
							}
						#endif
						//__________________________________________________________________________________________________________________
							default :
							{
								slvxsbus_Buffer[0] = 0x15;
								NbCar = 1;
								break;
							}
					}
				}
			}
			fonc(slvxsbus_Buffer, NbCar);
			if(slvbit.bDemandeReset)
			{
				slvbit.bDemandeReset = 0;
				mainbit.bReset = 1;
            }
		}
	}
}
