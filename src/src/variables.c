//////////////////////////////////////////////////////////////
// Date de création : 14 Février 2007						//
// Auteur			: Faouzi								//
// Description		: 										//
//////////////////////////////////////////////////////////////

#include "variables.h"
#include "main.h"
//#include "secteur.h"
//Si on utilise des variables (registre et flag) dans la flash


volatile unsigned long	Timers[NB_TIMERS_TOTAL];
volatile unsigned long	Compteurs[NB_COMPTEURS_TOTAL];
volatile long 			Registres[NB_REGS_TOTAL];
volatile char			Flags[NB_FLAGS_TOTAL/8];

ofsld					uLongFloat;
unsigned long			regflag_i;
octetflags				OctetToFlags;
variablebit 			bitvariable;
octetflags				EtatCompteurs;
//----------------------------------------------------------------------
// Fonction : ecriture de registres
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
int SetTimer(unsigned short Num,unsigned long  Val)
{
	if(Num < NB_TIMERS_TOTAL)
	{
		Timers[Num] = Val;
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------
// Fonction : ecriture de registres
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
unsigned long GetTimer(unsigned short Num)
{
	if(Num < NB_TIMERS_TOTAL)
	{
		return Timers[Num];
	}
	return 0;
}

//----------------------------------------------------------------------
// Fonction : ecriture de registres
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
int SetTimerIsr(unsigned short Num,unsigned long  Val)
{
	if(Num < NB_TIMERS_TOTAL)
	{
		Timers[Num] = Val;
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------
// Fonction : ecriture de registres
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
unsigned long GetTimerIsr(unsigned short Num)
{
	if(Num < NB_TIMERS_TOTAL)
	{
		return Timers[Num];
	}
	return 0;
}

//----------------------------------------------------------------------
// Fonction : ecriture de registres
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
int SetfReg(unsigned short Num,double Val)
{
	uLongFloat.d = Val;
	return SetlReg(Num, uLongFloat.l);
}

//----------------------------------------------------------------------
// Fonction : ecriture de registres RAM, ROM
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
int SetlReg(unsigned short Num,long Val)
{
#if (USE_FLASH)
	if(Num >= BASE_NVREGS1 && Num < BASE_NVREGS1 + NB_NVREGS_TOTAL1)
	{
		Num -= BASE_NVREGS1;
		Sec_WriteReg(0, Num, Val);
		return 1;
	}
	else
	if(Num >= BASE_NVREGS2 && Num < BASE_NVREGS2 + NB_NVREGS_TOTAL2)
	{
		Num -= BASE_NVREGS2;
		Sec_WriteReg(1, Num, Val);
		return 1;
	}
	else
	if(Num >= BASE_NVREGS3 && Num < BASE_NVREGS3 + NB_NVREGS_TOTAL3)
	{
		Num -= BASE_NVREGS3;
		Sec_WriteReg(2, Num, Val);
		return 1;
	}
	else
	if(Num >= BASE_NVREGS4 && Num < BASE_NVREGS4 + NB_NVREGS_TOTAL4)
	{
		Num -= BASE_NVREGS4;
		Sec_WriteReg(3, Num, Val);
		return 1;
	}
	else
#endif
	if(Num < NB_REGS_TOTAL)
	{
		Registres[Num] = Val;
		return 1;
	}
	return 0;
}
//----------------------------------------------------------------------
// Fonction : lecture de registres
// Entrée 	: Num : numéro registre
// Sortie 	: valeur
//----------------------------------------------------------------------
double GetfReg(unsigned short Num)
{
	uLongFloat.l = GetlReg(Num);
	return uLongFloat.d;
}

//----------------------------------------------------------------------
// Fonction : lecture de registres RAM, ROM
// Entrée 	: Num : numéro registre
// Sortie 	: valeur de registre
//----------------------------------------------------------------------
long GetlReg(unsigned short Num)
{
	long Val;//=0;
#if (USE_FLASH)
	if(Num >= BASE_NVREGS1 && Num < BASE_NVREGS1 + NB_NVREGS_TOTAL1)
	{
		Num -= BASE_NVREGS1;
		Sec_ReadReg(0, Num, &Val);
	}
	else
	if(Num >= BASE_NVREGS2 && Num < BASE_NVREGS2 + NB_NVREGS_TOTAL2)
	{
		Num -= BASE_NVREGS2;
		Sec_ReadReg(1, Num, &Val);
	}
	else
	if(Num >= BASE_NVREGS3 && Num < BASE_NVREGS3 + NB_NVREGS_TOTAL3)
	{
		Num -= BASE_NVREGS3;
		Sec_ReadReg(2, Num, &Val);
	}
	else
	if(Num >= BASE_NVREGS4 && Num < BASE_NVREGS4 + NB_NVREGS_TOTAL4)
	{
		Num -= BASE_NVREGS4;
		Sec_ReadReg(3, Num, &Val);
	}
	else
#endif
	if(Num < NB_REGS_TOTAL)
	{
		Val = Registres[Num];
	}
	return Val;
}

//----------------------------------------------------------------------
// Fonction : ecriture de flag
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
int GetOctet(unsigned short Num)
{
#if (USE_FLASH)
	if(Num >= BASE_NVFLAGS1 && Num < BASE_NVFLAGS1 + NB_NVFLAGS_TOTAL1)
	{
		Num -= BASE_NVFLAGS1;
		Sec_ReadOctet(0, Num/8, &OctetToFlags.octet[0]);
		return 1;
	}
	else
	if(Num >= BASE_NVFLAGS2 && Num < BASE_NVFLAGS2 + NB_NVFLAGS_TOTAL2)
	{
		Num -= BASE_NVFLAGS2;
		Sec_ReadOctet(1, Num/8, &OctetToFlags.octet[0]);
		return 1;
	}
	else
	if(Num >= BASE_NVFLAGS3 && Num < BASE_NVFLAGS3 + NB_NVFLAGS_TOTAL3)
	{
		Num -= BASE_NVFLAGS3;
		Sec_ReadOctet(2, Num/8, &OctetToFlags.octet[0]);
		return 1;
	}
	else
	if(Num >= BASE_NVFLAGS4 && Num < BASE_NVFLAGS4 + NB_NVFLAGS_TOTAL4)
	{
		Num -= BASE_NVFLAGS4;
		Sec_ReadOctet(3, Num/8, &OctetToFlags.octet[0]);
		return 1;
	}
	else
#endif
	if(Num < NB_FLAGS_TOTAL)
	{
		OctetToFlags.octet[0] = Flags[(Num/8)];
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------
// Fonction : ecriture de flag
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
void SetOctet(unsigned short Num)
{
	char Val = OctetToFlags.octet[0];
#if (USE_FLASH)
	if(Num >= BASE_NVFLAGS1 && Num < BASE_NVFLAGS1 + NB_NVFLAGS_TOTAL1)
	{
		Num -= BASE_NVFLAGS1;
		Sec_WriteOctet(0, Num/8, Val);
	}
	else
	if(Num >= BASE_NVFLAGS2 && Num < BASE_NVFLAGS2 + NB_NVFLAGS_TOTAL2)
	{
		Num -= BASE_NVFLAGS2;
		Sec_WriteOctet(1, Num/8, Val);
	}
	else
	if(Num >= BASE_NVFLAGS3 && Num < BASE_NVFLAGS3 + NB_NVFLAGS_TOTAL3)
	{
		Num -= BASE_NVFLAGS3;
		Sec_WriteOctet(2, Num/8, Val);
	}
	else
	if(Num >= BASE_NVFLAGS4 && Num < BASE_NVFLAGS4 + NB_NVFLAGS_TOTAL4)
	{
		Num -= BASE_NVFLAGS4;
		Sec_WriteOctet(3, Num/8, Val);
	}
	else
#endif
	if(Num < NB_FLAGS_TOTAL)
	{
		Flags[(Num/8)] = Val;
	}
}

//----------------------------------------------------------------------
// Fonction : ecriture de flag
// Entrée 	: Num : numéro registre, Val : valeur
// Sortie 	: 1 : c'est si bon, 0 : sinon
//----------------------------------------------------------------------
int SetFlag(unsigned short Num,char Val)
{
	if(GetOctet(Num))
	{
		bitvariable.bTempbit = Val != 0;
		switch(Num%8)
		{
			case 0 : OctetToFlags.flags.f0 = bitvariable.bTempbit; break;
			case 1 : OctetToFlags.flags.f1 = bitvariable.bTempbit; break;
			case 2 : OctetToFlags.flags.f2 = bitvariable.bTempbit; break;
			case 3 : OctetToFlags.flags.f3 = bitvariable.bTempbit; break;
			case 4 : OctetToFlags.flags.f4 = bitvariable.bTempbit; break;
			case 5 : OctetToFlags.flags.f5 = bitvariable.bTempbit; break;
			case 6 : OctetToFlags.flags.f6 = bitvariable.bTempbit; break;
			case 7 : OctetToFlags.flags.f7 = bitvariable.bTempbit; break;
		}
		SetOctet(Num);
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------------
// Fonction : lecture de registres
// Entrée 	: Num : numéro registre
// Sortie 	: valeur du flag
//----------------------------------------------------------------------
int GetFlag(unsigned short Num)
{
	if(GetOctet(Num))
	{
		switch(Num%8)
		{
			case 0 : if(OctetToFlags.flags.f0) return 1; break;
			case 1 : if(OctetToFlags.flags.f1) return 1; break;
			case 2 : if(OctetToFlags.flags.f2) return 1; break;
			case 3 : if(OctetToFlags.flags.f3) return 1; break;
			case 4 : if(OctetToFlags.flags.f4) return 1; break;
			case 5 : if(OctetToFlags.flags.f5) return 1; break;
			case 6 : if(OctetToFlags.flags.f6) return 1; break;
			case 7 : if(OctetToFlags.flags.f7) return 1; break;
		}
	}
	return 0;
}
