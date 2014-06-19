#include <plib.h>
#include "config.h"
#include "main.h"
//#include "rtspApi.h"

char config_Mem[4096];
//--------------------------------------------------------------------------------------------
// Fonction : config_ReadIsr
// Utilité : écrire dans la zonne de configuration appartire du main
//--------------------------------------------------------------------------------------------
void config_Write(char *pBuffer, long Base, int Nbr, long IdConfig)
{
	unsigned int pageStartAddr;
	unsigned int offset, numLeftInPage;
	unsigned int index;
	
	Base +=  IdConfig;//SECTEUR_CONFIG;
	
	// 2. Calculate Page Start address
    pageStartAddr = (unsigned int)Base & (~(BYTE_PAGE_SIZE - 1));
	
	
	offset = (unsigned int)Base &   (BYTE_PAGE_SIZE - 1);
	
	while(Nbr)
    {
	    
		memcpy((unsigned int *)(config_Mem), (void *)(pageStartAddr), BYTE_PAGE_SIZE);
	    
		NVMErasePage((void *)pageStartAddr);
		
		numLeftInPage = BYTE_PAGE_SIZE - offset;
		
		if(Nbr <= numLeftInPage)
		{
			memcpy((unsigned int *)(config_Mem + offset), (void *)(pBuffer), Nbr);
		
			Nbr = 0;
		}
		else
		{
			memcpy((unsigned int *)(config_Mem + offset), (void *)(pBuffer), numLeftInPage);
			
			Nbr -= numLeftInPage;
			pBuffer += numLeftInPage;
		}
		
		// Program the Page
	    for(index = 0; index < NUM_ROWS_PAGE; index++)
	    {
	        NVMWriteRow((void *)(pageStartAddr), (void *)config_Mem + (index * BYTE_ROW_SIZE));
		    pageStartAddr +=  BYTE_ROW_SIZE;
	    }
	    offset = 0;  
	}
}

//--------------------------------------------------------------------------------------------
// Fonction : config_Read
// Utilité : lire un buffer des données appartire du main
//--------------------------------------------------------------------------------------------
void config_Read(char *pBuffer, long Base, int Nbr, long IdConfig)
{
	Base +=  IdConfig;//SECTEUR_CONFIG;

	memcpy((void *)(pBuffer), (void *)(Base), Nbr);
}

//--------------------------------------------------------------------------------------------
// Fonction : config_String
// Utilité : lire une chaine de carractére appartire du main
//--------------------------------------------------------------------------------------------
char *config_String(long Base, long IdConfig)
{
	config_Read(config_Mem, Base, 256, IdConfig);
	return config_Mem;
}

//--------------------------------------------------------------------------------------------
// Fonction : config_Char
// Utilité : lire un variable char appartire du main
//--------------------------------------------------------------------------------------------
char config_Char(long Base, long IdConfig)
{
	config_Read(config_Mem, Base, 1, IdConfig);
	return *config_Mem;
}

//--------------------------------------------------------------------------------------------
// Fonction : config_Short
// Utilité : lire un variable Short appartire du main
//--------------------------------------------------------------------------------------------
short config_Short(long Base, long IdConfig)
{
	config_Read(config_Mem, Base, 2, IdConfig);
	return *((short *)config_Mem);
}

//--------------------------------------------------------------------------------------------
// Fonction : config_Long
// Utilité : lire un variable Long appartire du main
//--------------------------------------------------------------------------------------------
long config_Long(long Base, long IdConfig)
{
	config_Read(config_Mem, Base, 4, IdConfig);
	return *((long *)config_Mem);
}

//--------------------------------------------------------------------------------------------
// Fonction : config_Double
// Utilité : lire un variable Double appartire du main
//--------------------------------------------------------------------------------------------
double config_Double(long Base, long IdConfig)
{
	config_Read(config_Mem, Base, 4, IdConfig);
	return *((double *)config_Mem);
}
