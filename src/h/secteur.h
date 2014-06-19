#if !defined(SECTEUR_H)
#define SECTEUR_H

#define SECTEUR_NBR_SECTEUR			4
#define SECTEUR_ADRESSE_BASE		SECTEUR_CONSIGNE

#define SECTEUR_NBR_REG_PAR_SEC		1024

#define SECTEUR_NBR_REGISTRE_SEC1	NB_NVREGS_TOTAL1
#define SECTEUR_NBR_REGISTRE_SEC2	NB_NVREGS_TOTAL2

#define SECTEUR_NBR_FLAG_SEC(num)	(SECTEUR_NBR_REG_PAR_SEC - Sec_NbrReg[num])


typedef struct
{
	int AdrBaseRV;
	int AdrBaseRNV;
	int AdrBaseFV;
	int AdrBaseFNV;
	int AdrBaseTO;
}SecteurParam;

#define RV_CPT_ECRITURE_FLASH(num)		(Sec_pParam->AdrBaseRV + (num))

#define RNV_TO_ECRITURE_FLASH(num)		(Sec_pParam->AdrBaseRNV + (num))

#define TIMERS_TO_ECRITURE_FLASH(num)	(Sec_pParam->AdrBaseTO + (num))

void Sec_Init(SecteurParam	*pParam);
void Sec_Gestion(void);


int Sec_ReadReg(int sec, int num, long *pVal);
int Sec_WriteReg(int sec, int num, long Val);
int Sec_ReadOctet(int sec, int num, char *pVal);
int Sec_WriteOctet(int sec, int num, int Val);
void Sec_Save(void);

#endif //SECTEUR_H