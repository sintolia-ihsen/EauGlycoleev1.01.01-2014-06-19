
typedef struct
{
	unsigned bOrdreMAJHisto:1;
	unsigned bMAJHistoOk:1;
	unsigned bSave:1;
	/*
	unsigned bConfigSaveOk:1;
	unsigned bMAJMinMax:1;
	unsigned bMinui:1;
	unsigned bDirectonOk:1;
	*/
}bithistorique;

#define BASE_HISTORIQUE				60000
#define NB_REG_HISTORIQUE_TOTAL		TAILLE_SECTEUR_HISTORIQUE/4

void historique_Write(char *pData, int Addresse, int Taille);
void historique_Read(unsigned int *pData, int Addresse, int Nbr);
void historique_RAZ(void);
