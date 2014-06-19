
#if !defined(TERMINAL_H)
#define TERMINAL_H

#include "ads7846.h"


typedef struct
{
	unsigned bReff:1;
	unsigned bRes:1;
	unsigned bFirst:1;
	unsigned bEtatPress:1;
	unsigned bNewPress:1;
	unsigned bClick:1;
	unsigned bEcranPress:1;
	unsigned bNewEcran:1;
	unsigned bDefaut:1;	
	unsigned bSaveFull:1;	
	unsigned bSave:1;	
	unsigned bPompesAux:1;	
	unsigned bEvtFirstClik:1;	
	unsigned bEtatUSB:1;	
	unsigned bConfigOK:1;	
	unsigned bGraphe:1;	
	unsigned bStop:1;	
	unsigned bSleepAfficheur:1;	
	unsigned bQuitter:1;	
	unsigned bMDP:1;	
	unsigned bFavorisPlein:1;	
	unsigned bAlarteFavorisPlein:1;	
	unsigned bLegende:1;
	unsigned bCligne:1;
	unsigned bWiteSbus:1;
	unsigned bWiteCfg:1;
}bittrm;


extern bittrm trmbit;


void terminal_Init(void);
void terminal_Gestion(void);


#endif //TERMINAL_H
