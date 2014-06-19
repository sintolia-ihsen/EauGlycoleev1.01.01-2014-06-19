#include <plib.h>
#include "secteur.h"
#include "main.h"
#include "variables.h"

ofsld			Sec_Image[SECTEUR_NBR_SECTEUR][1024];
short			Sec_NbrReg[SECTEUR_NBR_SECTEUR]={SECTEUR_NBR_REGISTRE_SEC1, SECTEUR_NBR_REGISTRE_SEC2};
char			*Sec_pImageFlag[SECTEUR_NBR_SECTEUR];
char			Sec_DemandeEcriture[SECTEUR_NBR_SECTEUR];
SecteurParam	*Sec_pParam;

void Sec_lecture(int sec)
{
	memcpy((unsigned int *)(Sec_Image[sec]), (void *)(SECTEUR_ADRESSE_BASE + (sec*BYTE_PAGE_SIZE)), BYTE_PAGE_SIZE);
}

void Sec_Init(SecteurParam	*pParam)
{
	int Cpt;
	Sec_pParam = pParam;
	
	for(Cpt=0; Cpt<SECTEUR_NBR_SECTEUR; Cpt++)
	{
		Sec_DemandeEcriture[Cpt] = 0;
		Sec_lecture(Cpt);		
		
		Sec_pImageFlag[Cpt] = (char *)(&Sec_Image[Cpt][Sec_NbrReg[Cpt]]);
	}
	
	for(Cpt=0; Cpt<SECTEUR_NBR_SECTEUR; Cpt++)
	{
		if(GetlReg(RNV_TO_ECRITURE_FLASH(Cpt)) == 0xFFFFFFFF)
		{
			SetlReg(RNV_TO_ECRITURE_FLASH(Cpt), 1);
		}
	}
}

void Sec_Ecriture(int sec)
{
	unsigned int index;
	
	NVMErasePage((void *)(SECTEUR_ADRESSE_BASE + (sec*BYTE_PAGE_SIZE)));
	
    // Program the Page
    for(index = 0; index < NUM_ROWS_PAGE; index++)
    {
        NVMWriteRow((void *)(SECTEUR_ADRESSE_BASE + (sec*BYTE_PAGE_SIZE) + (index * BYTE_ROW_SIZE)), (void *)(Sec_Image) + ((sec*BYTE_PAGE_SIZE) + (index * BYTE_ROW_SIZE)));
    }
    SetlReg(RV_CPT_ECRITURE_FLASH(sec), GetlReg(RV_CPT_ECRITURE_FLASH(sec)) + 1);
}

int Sec_ReadReg(int sec, int num, long *pVal)
{
	if(sec < SECTEUR_NBR_SECTEUR && num < SECTEUR_NBR_REG_PAR_SEC)
	{
		*pVal = Sec_Image[sec][num].l;
		return 1;
	}
	return 0;
}

int Sec_WriteReg(int sec, int num, long Val)
{
	if(sec < SECTEUR_NBR_SECTEUR && num < SECTEUR_NBR_REG_PAR_SEC)
	{
		Sec_Image[sec][num].l = Val;
		Sec_DemandeEcriture[sec] = 1;
		SetTimer(TIMERS_TO_ECRITURE_FLASH(sec), GetlReg(RNV_TO_ECRITURE_FLASH(sec))*10);
		return 1;
	}
	return 0;
}

int Sec_ReadOctet(int sec, int num, char *pVal)
{
	if(sec < SECTEUR_NBR_SECTEUR && SECTEUR_NBR_FLAG_SEC(sec))
	{
		*pVal = Sec_pImageFlag[sec][num];
		return 1;
	}
	return 0;
}

int Sec_WriteOctet(int sec, int num, int Val)
{
	if(sec < SECTEUR_NBR_SECTEUR && SECTEUR_NBR_FLAG_SEC(sec))
	{
		Sec_pImageFlag[sec][num] = Val;
		Sec_DemandeEcriture[sec] = 1;
		SetTimer(TIMERS_TO_ECRITURE_FLASH(sec), GetlReg(RNV_TO_ECRITURE_FLASH(sec))*10);
		return 1;
	}
	return 0;
}

void Sec_Gestion()
{
	int Cpt;
	for(Cpt=0; Cpt<SECTEUR_NBR_SECTEUR; Cpt++)
	{
		if(!GetTimer(TIMERS_TO_ECRITURE_FLASH(Cpt)) && Sec_DemandeEcriture[Cpt])
		{
			Sec_DemandeEcriture[Cpt] = 0;
			Sec_Ecriture(Cpt);
		}
	}
}

void Sec_Save()
{
	int Cpt;
	for(Cpt=0; Cpt<SECTEUR_NBR_SECTEUR; Cpt++)
	{
		if(Sec_DemandeEcriture[Cpt])
		{
			Sec_DemandeEcriture[Cpt] = 0;
			Sec_Ecriture(Cpt);
		}
	}
}
