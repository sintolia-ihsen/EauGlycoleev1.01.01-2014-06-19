#include "main.h"
#include "historique.h"
#include "variables.h"
#include "mtrxsbus.h"


bithistorique historique_bit[3];
long	historique_DataSave[3][20];
int	historique_AddresseSave[3];
int	historique_CptHisto = 0;



void historique_Write(char *pData, int Addresse, int Taille)
{
	char RowData[BYTE_ROW_SIZE];	

	unsigned int index;
	unsigned int offset;
	unsigned int RowStartAddr;
	
	Addresse += SECTEUR_HISTORIQUE;
	
	RowStartAddr = (unsigned int)Addresse & (~(BYTE_ROW_SIZE - 1));
	offset =  Addresse - RowStartAddr;
	
	while(Taille)
	{
		if(!offset && !(RowStartAddr & (BYTE_PAGE_SIZE - 1)))
		{
			NVMErasePage((void *)RowStartAddr);
		}
		
		memcpy((unsigned int *)(RowData), (void *)(RowStartAddr), BYTE_ROW_SIZE);
		
		for(index = offset; index < BYTE_ROW_SIZE && Taille; index++)
		{
			RowData[index] = *pData;
			pData++;
			Taille--;
		}
		NVMWriteRow((void *)RowStartAddr, (void *)(RowData));
		RowStartAddr += BYTE_ROW_SIZE;
		offset = 0;
	}	
}

void historique_Read(unsigned int *pData, int Addresse, int Nbr)
{
	Addresse += SECTEUR_HISTORIQUE;
	
	memcpy(pData, (void *)(Addresse), Nbr);
}

void historique_RAZ()
{
	unsigned long Base;

	for(Base = SECTEUR_HISTORIQUE; Base < (SECTEUR_HISTORIQUE + TAILLE_SECTEUR_HISTORIQUE); Base+=0x1000)
	{
		NVMErasePage((void *)Base);
	}
}