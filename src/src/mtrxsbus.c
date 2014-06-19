//#include <p33Fxxxx.h>
#include <string.h>
#include "mtrxsbus.h"
#include "main.h"
#include "uart.h"
#include "timers.h"
#include "variables.h"

mtrxsbusbit bitmtrxsbusuart[3];
mtrVar mtrxsbusuart[3];

void mtrxsbus_Init(char Uart, void (*fonc)())
{
	mtrxsbusuart[Uart].NbrCarRx = 0;
	
	bitmtrxsbusuart[Uart].bSynchro = 1;
	bitmtrxsbusuart[Uart].bEncoure = 0;
	bitmtrxsbusuart[Uart].bTxEncoure = 0;
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	
	mtrxsbusuart[Uart].Sendfonc = fonc;
}

int mtrxsbus_Synchronize(char Uart, unsigned char Cible, unsigned short TimeOut)
{
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			if(	mtrxsbusuart[Uart].NbrCarRx == 2 &&
				mtrxsbusuart[Uart].Buffer[0] == 'O' &&
				mtrxsbusuart[Uart].Buffer[1] == 'K'
				)
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;
				return MTRXSBUS_RX_EN_COURS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;
	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]='S';
	mtrxsbusuart[Uart].Buffer[1]='Y';
	mtrxsbusuart[Uart].Buffer[2]='N';
	mtrxsbusuart[Uart].Buffer[3]='C';
	mtrxsbusuart[Uart].Buffer[4]=Cible;
	
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, 5);
	
	return MTRXSBUS_TX_EN_COURS;
}


int mtrxsbus_ReadRTC(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, long *pReponse, unsigned short TimeOut)
{
	ofsld TempVal;
	unsigned int i;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			
			if(mtrxsbusuart[Uart].NbrCarRx == (Nbr * 4) + 2)
			{
				TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, Nbr*4);
				
				if(	mtrxsbusuart[Uart].Buffer[(Nbr*4)+0] == TempVal.of.octet[1] &&
					mtrxsbusuart[Uart].Buffer[(Nbr*4)+1] == TempVal.of.octet[0])
				{
					bitmtrxsbusuart[Uart].bSynchro = 1;
					
					for(i = 0; i < Nbr; i++)
					{
						TempVal.of.octet[3] = mtrxsbusuart[Uart].Buffer[(i * 4) + 0];
						TempVal.of.octet[2] = mtrxsbusuart[Uart].Buffer[(i * 4) + 1];
						TempVal.of.octet[1] = mtrxsbusuart[Uart].Buffer[(i * 4) + 2];
						TempVal.of.octet[0] = mtrxsbusuart[Uart].Buffer[(i * 4) + 3];
						pReponse[i] = TempVal.l;
					}
					return MTRXSBUS_SUCCESS;
				}
				else
				{
					return MTRXSBUS_ERROR_REPONSE;
				}
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;

	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
	
		case 'T':	
		case 't':	Type=0x07;	// Code de commande écriture Timer
					break;
	
		case 'C':	
		case 'c':	Type=0x00;	// Code de commande écriture Compteur
					break;
		case 'R':	
		case 'r':	
		default :	Type=0x06;	// Code de commande écriture Registre
					break;
	}
	mtrxsbusuart[Uart].Buffer[1]=Type;
	mtrxsbusuart[Uart].Buffer[2]=(Nbr-1);
	TempVal.s[0] = AdbaseCible;
	mtrxsbusuart[Uart].Buffer[3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[4] = TempVal.of.octet[0];
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, 5);
	
	mtrxsbusuart[Uart].Buffer[5] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[6] = TempVal.of.octet[0];
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, 7);
	
	return MTRXSBUS_TX_EN_COURS;
}


int mtrxsbus_WriteRTC(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, long *pData, unsigned short TimeOut)
{
	unsigned int i;
	ofsld TempVal;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			
			if(mtrxsbusuart[Uart].NbrCarRx == 1 && mtrxsbusuart[Uart].Buffer[0] == 0x06)
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;			
				return MTRXSBUS_SUCCESS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;

	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
	
		case 'T':	
		case 't':	Type=0x0F;	// Code de commande écriture Timer
					break;
	
		case 'C':	
		case 'c':	Type=0x0A;	// Code de commande écriture Compteur
					break;
		case 'R':	
		case 'r':	
		default :	Type=0x0E;	// Code de commande écriture Registre
					break;
	}
	
	mtrxsbusuart[Uart].Buffer[1] = Type;
	mtrxsbusuart[Uart].Buffer[2] = (Nbr*4)+1;

	TempVal.s[0] = AdbaseCible;
	mtrxsbusuart[Uart].Buffer[3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[4] = TempVal.of.octet[0];

	for(i=0;i<Nbr;i++)
	{
		TempVal.l = pData[i];
		mtrxsbusuart[Uart].Buffer[(i * 4) + 5] = TempVal.of.octet[3];
		mtrxsbusuart[Uart].Buffer[(i * 4) + 6] = TempVal.of.octet[2];
		mtrxsbusuart[Uart].Buffer[(i * 4) + 7] = TempVal.of.octet[1];
		mtrxsbusuart[Uart].Buffer[(i * 4) + 8] = TempVal.of.octet[0];
	}
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, ((Nbr*4)+5));
	
	mtrxsbusuart[Uart].Buffer[((Nbr*4)+5) + 0] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[((Nbr*4)+5) + 1] = TempVal.of.octet[0];
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, (Nbr*4)+7);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_ReadMultiRTC(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, long *pReponse, unsigned short TimeOut)
{
	ofsld TempVal;
	unsigned int i;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			
			if(mtrxsbusuart[Uart].NbrCarRx == (Nbr * 8) + 2)
			{
				TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, Nbr*8);
			
				if(	mtrxsbusuart[Uart].Buffer[(Nbr*8)+0] == TempVal.of.octet[1] &&
					mtrxsbusuart[Uart].Buffer[(Nbr*8)+1] == TempVal.of.octet[0])
				{
					bitmtrxsbusuart[Uart].bSynchro = 1;
					
					for(i = 0; i < Nbr; i++)
					{
						TempVal.of.octet[3] = mtrxsbusuart[Uart].Buffer[(i * 8) + 0];
						TempVal.of.octet[2] = mtrxsbusuart[Uart].Buffer[(i * 8) + 1];
						TempVal.of.octet[1] = mtrxsbusuart[Uart].Buffer[(i * 8) + 2];
						TempVal.of.octet[0] = mtrxsbusuart[Uart].Buffer[(i * 8) + 3];
						pReponse[(i * 2) + 0] = TempVal.l;
						TempVal.of.octet[3] = mtrxsbusuart[Uart].Buffer[(i * 8) + 4];
						TempVal.of.octet[2] = mtrxsbusuart[Uart].Buffer[(i * 8) + 5];
						TempVal.of.octet[1] = mtrxsbusuart[Uart].Buffer[(i * 8) + 6];
						TempVal.of.octet[0] = mtrxsbusuart[Uart].Buffer[(i * 8) + 7];
						pReponse[(i * 2) + 1] = TempVal.l;
					}
					return MTRXSBUS_SUCCESS;
				}
				else
				{
					return MTRXSBUS_ERROR_REPONSE;
				}
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;

	}
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
	
		case 'T':	
		case 't':	Type=0x15;	// Code de commande écriture Timer
					break;
	
		case 'R':	
		case 'r':	
		default :	Type=0x14;	// Code de commande écriture Registre
					break;
	}
	mtrxsbusuart[Uart].Buffer[1]=Type;
	mtrxsbusuart[Uart].Buffer[2]=(Nbr-1);
	
	for(i=0;i<Nbr;i++)
	{
		TempVal.l = pAdresse[i];
		mtrxsbusuart[Uart].Buffer[(i*4) + 3] = TempVal.of.octet[3];
		mtrxsbusuart[Uart].Buffer[(i*4) + 4] = TempVal.of.octet[2];
		mtrxsbusuart[Uart].Buffer[(i*4) + 5] = TempVal.of.octet[1];
		mtrxsbusuart[Uart].Buffer[(i*4) + 6] = TempVal.of.octet[0];
	}
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, (Nbr*4) + 3);
	
	mtrxsbusuart[Uart].Buffer[(Nbr*4) + 3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[(Nbr*4) + 4] = TempVal.of.octet[0];
	

	
	
	
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, (Nbr*4) + 5);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_WriteMultiRTC(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, long *pData, unsigned short TimeOut)
{
	unsigned int i;
	ofsld TempVal;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			
			if(mtrxsbusuart[Uart].NbrCarRx == 1 && mtrxsbusuart[Uart].Buffer[0] == 0x06)
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;			
				return MTRXSBUS_SUCCESS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;

	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
	
		case 'T':	
		case 't':	Type=0x18;	// Code de commande écriture Timer
					break;
	
		case 'R':	
		case 'r':	
		default :	Type=0x17;	// Code de commande écriture Registre
					break;
	}
	
	mtrxsbusuart[Uart].Buffer[1] = Type;
	mtrxsbusuart[Uart].Buffer[2] = (Nbr*4)+1;

	for(i=0;i<Nbr;i++)
	{
		TempVal.s[0] = pAdresse[i];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 3] = TempVal.of.octet[3];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 4] = TempVal.of.octet[2];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 5] = TempVal.of.octet[1];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 6] = TempVal.of.octet[0];
		TempVal.l = pData[i];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 7] = TempVal.of.octet[3];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 8] = TempVal.of.octet[2];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 9] = TempVal.of.octet[1];
		mtrxsbusuart[Uart].Buffer[(i * 8) + 10] = TempVal.of.octet[0];
	}
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, ((Nbr*8)+3));
	
	mtrxsbusuart[Uart].Buffer[((Nbr*8)+3) + 0] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[((Nbr*8)+3) + 1] = TempVal.of.octet[0];

	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, (Nbr*8)+5);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_ReadIOF(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, char *pData, unsigned short TimeOut)
{
	
	unsigned int i;
	ofsld TempVal;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;

			if(mtrxsbusuart[Uart].NbrCarRx == ((Nbr-1)/8)+ 1 + 2)
			{
				TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, mtrxsbusuart[Uart].NbrCarRx - 2);
			
				if(	mtrxsbusuart[Uart].Buffer[mtrxsbusuart[Uart].NbrCarRx - 2] == TempVal.of.octet[1] &&
					mtrxsbusuart[Uart].Buffer[mtrxsbusuart[Uart].NbrCarRx - 1] == TempVal.of.octet[0])
				{
					bitmtrxsbusuart[Uart].bSynchro = 1;
					
					TempVal.of.octet[0] = mtrxsbusuart[Uart].Buffer[0];
					for(i = 0; i < Nbr; i++)
					{
						switch(i%8)
						{
							case 0 : TempVal.of.octet[0] = mtrxsbusuart[Uart].Buffer[i/8];
									 pData[i] = TempVal.of.flags.f0; break;
							case 1 : pData[i] = TempVal.of.flags.f1; break;
							case 2 : pData[i] = TempVal.of.flags.f2; break;
							case 3 : pData[i] = TempVal.of.flags.f3; break;
							case 4 : pData[i] = TempVal.of.flags.f4; break;
							case 5 : pData[i] = TempVal.of.flags.f5; break;
							case 6 : pData[i] = TempVal.of.flags.f6; break;
							case 7 : pData[i] = TempVal.of.flags.f7; break;
						}
					}
					return MTRXSBUS_SUCCESS;
				}
				else
				{
					return MTRXSBUS_ERROR_REPONSE;
				}
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;
	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
		case 'O':	
		case 'o':	Type=0x05;	// Code de commande écriture Compteur
					break;

		case 'I':	
		case 'i':	Type=0x03;	// Code de commande écriture Registre
					break;

		case 'F':	
		case 'f':	
		default :	Type=0x02;	// Code de commande écriture Timer
					break;
	}
	
	mtrxsbusuart[Uart].Buffer[1] = Type;
	mtrxsbusuart[Uart].Buffer[2] = Nbr-1;

	TempVal.s[0] = AdbaseCible;
	mtrxsbusuart[Uart].Buffer[3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[4] = TempVal.of.octet[0];

	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, 5);
	
	mtrxsbusuart[Uart].Buffer[5] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[6] = TempVal.of.octet[0];
	
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, 7);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_ReadMultiIOF(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, char *pData, unsigned short TimeOut)
{
	
	unsigned int i;
	ofsld TempVal;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;

			if(mtrxsbusuart[Uart].NbrCarRx == (Nbr*5) + 2)
			{
				TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, mtrxsbusuart[Uart].NbrCarRx - 2);
			
				if(	mtrxsbusuart[Uart].Buffer[mtrxsbusuart[Uart].NbrCarRx - 2] == TempVal.of.octet[1] &&
					mtrxsbusuart[Uart].Buffer[mtrxsbusuart[Uart].NbrCarRx - 1] == TempVal.of.octet[0])
				{
					bitmtrxsbusuart[Uart].bSynchro = 1;
					
					for(i = 0; i < Nbr; i++)
					{
						pData[(i * 5) + 3] = mtrxsbusuart[Uart].Buffer[(i * 5) + 0];
						pData[(i * 5) + 2] = mtrxsbusuart[Uart].Buffer[(i * 5) + 1];
						pData[(i * 5) + 1] = mtrxsbusuart[Uart].Buffer[(i * 5) + 2];
						pData[(i * 5) + 0] = mtrxsbusuart[Uart].Buffer[(i * 5) + 3];
						
						pData[(i * 5) + 4] = mtrxsbusuart[Uart].Buffer[(i * 5) + 4];
					}
					return MTRXSBUS_SUCCESS;
				}
				else
				{
					return MTRXSBUS_ERROR_REPONSE;
				}
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;
	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
		case 'F':	
		case 'f':	
		default :	Type=0x16;	// Code de commande écriture Timer
					break;
	}
	
	mtrxsbusuart[Uart].Buffer[1] = Type;
	mtrxsbusuart[Uart].Buffer[2] = Nbr-1;

	for(i=0;i<Nbr;i++)
	{
		TempVal.l = pAdresse[i];
		mtrxsbusuart[Uart].Buffer[(i*4) + 3] = TempVal.of.octet[3];
		mtrxsbusuart[Uart].Buffer[(i*4) + 4] = TempVal.of.octet[2];
		mtrxsbusuart[Uart].Buffer[(i*4) + 5] = TempVal.of.octet[1];
		mtrxsbusuart[Uart].Buffer[(i*4) + 6] = TempVal.of.octet[0];
	}

	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, (Nbr*4) + 3);
	
	mtrxsbusuart[Uart].Buffer[(Nbr*4) + 3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[(Nbr*4) + 4] = TempVal.of.octet[0];
	
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, (Nbr*4) + 5);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_WriteOF(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, char *pData, unsigned short TimeOut)
{
	unsigned int i;
	ofsld TempVal;

	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;

			if(mtrxsbusuart[Uart].NbrCarRx == 1 && mtrxsbusuart[Uart].Buffer[0] == 0x06)
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;			
				return MTRXSBUS_SUCCESS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;
	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
		case 'O':	
		case 'o':	Type=0x0D;	// Code de commande écriture Compteur
					break;

		case 'F':	
		case 'f':	
		default :	Type=0x0B;	// Code de commande écriture Registre
					break;
	}
	
	mtrxsbusuart[Uart].Buffer[1] = Type;
	mtrxsbusuart[Uart].Buffer[2] = (((Nbr-1)/8)+3);

	TempVal.s[0] = AdbaseCible;
	mtrxsbusuart[Uart].Buffer[3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[4] = TempVal.of.octet[0];

	mtrxsbusuart[Uart].Buffer[5] = Nbr-1;

	// *** Remplissage des octets à partir de la valeur des Flags *** 
	for (i = 0; i < Nbr; i++) 
	{
		if(pData[i] != 0)
		{
			mtrxsbusuart[Uart].Buffer[6 + (i/8)] |= (char)(0x01 << (i%8));
		}
		else
		{
			mtrxsbusuart[Uart].Buffer[6 + (i/8)] &= (char)(~(0x01 << (i%8)));
		}
	}
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, (((Nbr-1)/8)+7));
	
	mtrxsbusuart[Uart].Buffer[(((Nbr-1)/8)+7) + 0] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[(((Nbr-1)/8)+7) + 1] = TempVal.of.octet[0];
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, ((Nbr-1)/8)+9);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_WriteMultiOF(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, char *pData, unsigned short TimeOut)
{
	unsigned int i;
	ofsld TempVal;

	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;

			if(mtrxsbusuart[Uart].NbrCarRx == 1 && mtrxsbusuart[Uart].Buffer[0] == 0x06)
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;			
				return MTRXSBUS_SUCCESS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;
			
			
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;
	}
	
	
	
	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;
	
	switch(Type)
	{
		case 'O':	
		case 'o':	Type=0x0D;	// Code de commande écriture Compteur
					break;

		case 'F':	
		case 'f':	
		default :	Type=0x0B;	// Code de commande écriture Registre
					break;
	}
	
	mtrxsbusuart[Uart].Buffer[1] = Type;
	mtrxsbusuart[Uart].Buffer[2] = (Nbr-1);

	for(i=0;i<Nbr;i++)
	{
		TempVal.s[0] = pAdresse[i];
		mtrxsbusuart[Uart].Buffer[(i * 5) + 3] = TempVal.of.octet[3];
		mtrxsbusuart[Uart].Buffer[(i * 5) + 4] = TempVal.of.octet[2];
		mtrxsbusuart[Uart].Buffer[(i * 5) + 5] = TempVal.of.octet[1];
		mtrxsbusuart[Uart].Buffer[(i * 5) + 6] = TempVal.of.octet[0];

		mtrxsbusuart[Uart].Buffer[(i * 5) + 7] = pData[i];
	}
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, (Nbr * 5) + 3);
	
	mtrxsbusuart[Uart].Buffer[(Nbr * 5) + 3] = TempVal.of.octet[1];
	mtrxsbusuart[Uart].Buffer[(Nbr * 5) + 4] = TempVal.of.octet[0];
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, (Nbr * 5) + 2);
	
	return MTRXSBUS_TX_EN_COURS;
}

int mtrxsbus_ReadMemEx(char Uart, unsigned char Cible, unsigned long AdbaseCible, unsigned int Nbr, char *pReponse, unsigned short TimeOut)
{
	ofsld TempVal;
	int i;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			
			TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, Nbr);
			
			if(	mtrxsbusuart[Uart].Buffer[Nbr+0] == TempVal.c[1] &&
				mtrxsbusuart[Uart].Buffer[Nbr+1] == TempVal.c[0])
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;
				
				for(i = 0; i < Nbr; i++)
				{
					pReponse[i] = mtrxsbusuart[Uart].Buffer[i];
				}
				return MTRXSBUS_SUCCESS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;		
			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;

	}

	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0]=Cible;	
	mtrxsbusuart[Uart].Buffer[1]=0x12;
	TempVal.l = (Nbr-1);
	mtrxsbusuart[Uart].Buffer[2]=TempVal.c[1];
	mtrxsbusuart[Uart].Buffer[3]=TempVal.c[0];
	TempVal.l = AdbaseCible;
	mtrxsbusuart[Uart].Buffer[4] = TempVal.c[3];
	mtrxsbusuart[Uart].Buffer[5] = TempVal.c[2];
	mtrxsbusuart[Uart].Buffer[6] = TempVal.c[1];
	mtrxsbusuart[Uart].Buffer[7] = TempVal.c[0];
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, 8);
	
	mtrxsbusuart[Uart].Buffer[8] = TempVal.c[1];
	mtrxsbusuart[Uart].Buffer[9] = TempVal.c[0];
	

	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, 10);

	return MTRXSBUS_TX_EN_COURS;
}


int mtrxsbus_WriteMemEx(char Uart, unsigned char Cible, unsigned long AdbaseCible, unsigned int Nbr, char *pData, unsigned short TimeOut)
{
	int i;
	ofsld TempVal;
		
	if(!bitmtrxsbusuart[Uart].bSynchro)
	{
		return mtrxsbus_Synchronize(Uart, Cible, TimeOut);
	}
	
	if(bitmtrxsbusuart[Uart].bEncoure)
	{
		if(bitmtrxsbusuart[Uart].bReponseOk)
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			
			if(mtrxsbusuart[Uart].Buffer[0] == 0x06)
			{
				bitmtrxsbusuart[Uart].bSynchro = 1;			
				return MTRXSBUS_SUCCESS;
			}
			else
			{
				return MTRXSBUS_ERROR_REPONSE;
			}
		}
	
		if(!GetTimer(TIMER_SBUS_MTR_TO(Uart)))
		{
			bitmtrxsbusuart[Uart].bEncoure = 0;
			bitmtrxsbusuart[Uart].bSynchro = 0;

			return MTRXSBUS_ERROR_TIMEOUT;
		}
		if(bitmtrxsbusuart[Uart].bTxEncoure)
		{
			return MTRXSBUS_TX_EN_COURS;
		}
		return MTRXSBUS_RX_EN_COURS;

	}
	

	//Creation de la trame à envoyer
	mtrxsbusuart[Uart].Buffer[0] = Cible;
	mtrxsbusuart[Uart].Buffer[1] = 0x13;
	TempVal.l = Nbr+4;
	mtrxsbusuart[Uart].Buffer[2] = TempVal.c[1];
	mtrxsbusuart[Uart].Buffer[3] = TempVal.c[0];

	TempVal.l = AdbaseCible;
	mtrxsbusuart[Uart].Buffer[4] = TempVal.c[3];
	mtrxsbusuart[Uart].Buffer[5] = TempVal.c[2];
	mtrxsbusuart[Uart].Buffer[6] = TempVal.c[1];
	mtrxsbusuart[Uart].Buffer[7] = TempVal.c[0];

	for(i=0;i<Nbr;i++)
	{
		mtrxsbusuart[Uart].Buffer[i + 8] = pData[i];
	}
	
	//Ajoute le CRC à la fin de la trame
	TempVal.s[0] = Calcul_CRC16(mtrxsbusuart[Uart].Buffer, Nbr+8);
	
	mtrxsbusuart[Uart].Buffer[Nbr + 8 + 0] = TempVal.c[1];
	mtrxsbusuart[Uart].Buffer[Nbr + 8 + 1] = TempVal.c[0];
	
	SetTimer(TIMER_SBUS_MTR_TO(Uart), TimeOut);
	bitmtrxsbusuart[Uart].bReponseOk = 0;
	bitmtrxsbusuart[Uart].bEncoure = 1;
	
	mtrxsbusuart[Uart].Sendfonc(mtrxsbusuart[Uart].Buffer, Nbr+10);
	
	return MTRXSBUS_TX_EN_COURS;
}

void mtrxsbus_Reponse(char Uart, char *pBuffer, unsigned int Taille)
{
	mtrxsbusuart[Uart].NbrCarRx = Taille;
	if(Taille <= MTRXSBUS_TAILLE_BUFFER)
	{
		memcpy(mtrxsbusuart[Uart].Buffer, pBuffer, Taille);
		bitmtrxsbusuart[Uart].bReponseOk = 1;
	}
}
