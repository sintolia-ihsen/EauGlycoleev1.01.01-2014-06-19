#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "ads7846.h"
#include "terminal.h"
#include "graphics.h"
#include "s1d13700.h"
#include "font.h"
#include "image.h"
#include "variables.h"

bittrm trmbit;
char terminal_Chaine[512];

void terminal_Init()
{
    trmbit.bNewEcran = 1;
	graphics_Init();
	ads7846_Init();

    if(GetlReg(RNV_RETROECLAIRAGE) == 0xFFFFFFFF)
	{
		SetlReg(RNV_RETROECLAIRAGE, 4000);
		SetlReg(RNV_DUREE_RETROECLAIRAGE, 120);
		SetlReg(RNV_DUREE_REVEILLE_AFFICHEUR, 0);
	}
    SetTimer(TIMER_RETROECLAIRAGE, GetlReg(RNV_DUREE_RETROECLAIRAGE)*10);
}

void strtoupper(register char *s1)
{
	while(*s1)
	{
		if(*s1 == 'é' || *s1 == 'è') *s1 = 'E';
		*s1 = toupper(*s1);
		s1++;
	}
}

char IsAllFF(long *pBuffer, int Taille)
{
	while(Taille)
	{
		if(*pBuffer != 0xFFFFFFFF)
		return 0;
		pBuffer++;
		Taille--;
	}
	return 1;
}

/********************************************/
GOL_MSG Msg;
int terminal_Bouton(GOL_MSG msg, int x1, int y1, int x2, int y2)
{
	trmbit.bEcranPress = 0;
	if(msg.param1 == (short)0xFFFF && msg.param2 == (short)0xFFFF)
	{
		trmbit.bClick = 1;
		trmbit.bNewPress = 1;
	}
	else
	{
		if(msg.param1 > x1 && msg.param1 < x2 && msg.param2 > y1 && msg.param2 < y2)
		{
			if(trmbit.bNewPress)
			{
				trmbit.bEcranPress = 1;
			}

			if(trmbit.bClick)
			{
				trmbit.bClick = 0;
				trmbit.bReff = 1;
				return 1;
			}
		}
	}
	return 0;
}

void terminal_Gestion()
{
    if(!GetTimer(TIMER_VEILLE_AFFICHEUR) && GetlReg(RNV_DUREE_REVEILLE_AFFICHEUR))
    {
        trmbit.bSleepAfficheur = 1;
    }

	if(trmbit.bSleepAfficheur || !GetFlag(FNV_SLEEP_AFF))
	{
		if(!trmbit.bStop)
		{
			trmbit.bStop = 1;
			s1d13700_SendCMD(S1D13700_CMD_SLEEP);
			SetlReg(RV_DUTY_CYCLE_PWM3, 0);
			s1d13700_OFF();
		}
		return;
	}
	else
	{
		if(trmbit.bStop)
		{
			s1d13700_Init();
			trmbit.bStop = 0;
			trmbit.bReff = 1;
		}
	}

	/************************/

	ads7846_Gestion();

	ads7846_GetMsg(&Msg);


	if(!GetTimer(TIMER_RETROECLAIRAGE) && GetlReg(RNV_DUREE_RETROECLAIRAGE))
	{
		s1d13700_SetBackLight(0);
	}
	else
	{
		s1d13700_SetBackLight(1);
	}

    if(trmbit.bNewEcran)
    {
        trmbit.bNewEcran = 0;
        trmbit.bReff = 1;
    }

    if(!GetTimer(TIMER_TEMPS_REFF) || (Msg.param1 != (short)0xFFFF) || (Msg.param2 != (short)0xFFFF))
    {
        SetTimer(TIMER_TEMPS_REFF, 5);
        trmbit.bReff=1;
    }

    if(trmbit.bReff)
    {
        graphics_ClearDevice();

        s1d13700_SetColor(S1D13700_BLACK);

        graphics_SetFont((void *) &Courier_New_2);
        
        sprintf(terminal_Chaine, "Noyau Version : %.4f", GetfReg(RNV_NOYEAU_VERSION));
        
        graphics_OutTextXY(15, 110, terminal_Chaine);
    }

    if(trmbit.bReff)
	{
		trmbit.bReff = 0;
		s1d13700_Reffrache();
	}

}
