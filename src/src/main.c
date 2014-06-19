#include <plib.h>
#include "main.h"
#include "secteur.h"
#include "timers.h"
#include "variables.h"
#include "terminal.h"
#include "anatype.h"
#include "ads7846.h"
#include "echantillonnage.h"
#include "clock.h"
#include "pwm.h"
#include "can.h"
#include "tor.h"
#include "s1d13700.h"
#include "exception.h"
#include "eauglycolee.h"
#include "secteur.h"
#include "gsm.h"
#include "comsbusmaitre.h"

// *****************************************************************************
// *****************************************************************************
// Configuration Bits
// *****************************************************************************
// *****************************************************************************
#pragma config UPLLEN   = OFF            // USB PLL Enabled
#pragma config UPLLIDIV = DIV_5         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_5         // PLL Input Divider
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSECME        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
//	#pragma config POSCMOD  = OFF            // Primary Oscillator
#pragma config IESO     = ON           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
//	#pragma config FNOSC    = FRCPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = ON            // Background Debugger Enable




SecteurParam SecteurpParam;

bitmain	mainbit;
EtatApp	gEtatApp;

unsigned char gUart1Mode;
unsigned char gUart2Mode;
unsigned char gNumStation;
unsigned char gSysEtat;
unsigned int gSystemClock = (80000000ul);

double FifoAn[24][10];
AnaType1Param Ana[24];

double FifoETor[4][10];
EchantParam ETor[4];


unsigned int GetSystemClock()
{
	return gSystemClock;
}

void init()
{
	int i;
	
	// Configure the device for maximum performance but do not change the PBDIV
	// Given the options, this function will change the flash wait states, RAM
	// wait state and enable prefetch cache but will not change the PBDIV.
	// The PBDIV value is already set via the pragma FPBDIV option above..
	SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	
	DDPCONbits.JTAGEN = 0;
	
	
	for(i=0; i<0xFFFF; i++);
	
	mainbit.bMaitreLibre = 1;
    comsbusmaitre_bit[2].bSbusLibre = 1;

	EN_MIC52_5V = 1;
	TRIS_EN_MIC52_5V = 0;

    EN_MIC29_3_3V = 1;
    TRIS_EN_MIC29_3_3V = 0;
	
	AD1PCFG=0xFFFF;
	
	SecteurpParam.AdrBaseRV = RV_CPT_ECRITURE_FLASH_S1;
	SecteurpParam.AdrBaseRNV = RNV_TO_ECRITURE_FLASH_S1;
	//SecteurpParam.AdrBaseFV = ;
	//SecteurpParam.AdrBaseFNV = ;
	SecteurpParam.AdrBaseTO = TIMER_TO_ECRITURE_FLASH_S1;
	
	Sec_Init(&SecteurpParam);
	
	if(!GetFlag(FNV_FRQ_80MHZ))
	{
		gSystemClock = (20000000ul);
		OSCConfig(OSC_POSC_PLL,OSC_PLL_MULT_20,OSC_PLL_POST_4, 0x01000000);
		SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	}
	
	tor_init();
	
	can_Init();
	
	clock_init();
	
	pwm_init();
	
	for(i=0; i<CAN_NBR_ENTREE_ANA; i++)
	{
		Ana[i].AdrRegPoint = RV_AN(i);
		Ana[i].AdrBaseRV = RV_ANA_DIRECTE_AN(i);
		Ana[i].AdrBaseRNV = RNV_COEFF_AN(i);
		Ana[i].AdrBaseFV = FV_PREMIER_VALEUR_AN(i);
		Ana[i].AdrBaseFNV = FNV_ACTIVER_KALMAN_AN(i);
		Ana[i].AdrBaseT = TIMER_INSERTION_FIFO_AN(i);
		Ana[i].TailleFifoMax = 10;
		Ana[i].pFifo = FifoAn[i];
						
		AnaType1_Init(&Ana[i]);
	}

	for(i=0; i < TOR_NBR_ENTREE_INT; i++)
	{
		ETor[i].AdrRegPoint = RV_ECHANT_POINT_E(i);
		ETor[i].AdrBaseRV = RV_DIRECTE_E(i);
		ETor[i].AdrBaseRNV = RNV_TEMPS_INSERTION_FIFO_E(i);
		ETor[i].AdrBaseFV = FV_PREMIER_VALEUR_E(i);
		ETor[i].AdrBaseFNV = FNV_ACTIVER_KALMAN_E(i);
		ETor[i].AdrBaseT = TIMER_INSERTION_FIFO_E(i);
		ETor[i].TailleFifoMax = 10;
		ETor[i].pFifo = FifoETor[i];
						
		Echant_Init(&ETor[i]);
	}
	

	if(GetlReg(RNV_NUM_STATION) == -1)
	{
		SetlReg(RNV_NUM_STATION, 1);		
		SetlReg(RNV_MODE_UART1, UART_MODE_SLV);
		SetlReg(RNV_VITESSE_UART1, 19200);
#ifdef MODEM_GSM_SIMUL
        SetlReg(RNV_MODE_UART2, UART_MODE_GSM);
#else
        SetlReg(RNV_MODE_UART2, UART_MODE_SLV);
#endif		
		SetlReg(RNV_VITESSE_UART2, 19200);
		SetfReg(RNV_APPLICATION_VERSION, APPLICATION_VERSION);
		SetfReg(RNV_NOYEAU_VERSION, NOYEAU_VERSION);

		SetFlag(FNV_ALL_MUX1_TYPE_LINEAIRE, 0);
		SetFlag(FNV_ALL_MUX2_TYPE_LINEAIRE, 0);
		for(i=16; i<25; i++)
		{
			SetFlag(FNV_CAPTEUR_LINEAIRE_AN(i), 1);
		}
	} 
	
	terminal_Init();
   
	timer2_Init();

    gSysEtat = 'R';
	gNumStation = GetlReg(RNV_NUM_STATION);
	gUart1Mode = GetlReg(RNV_MODE_UART1);
	gUart2Mode = GetlReg(RNV_MODE_UART2);
	uart1_Init(GetlReg(RNV_VITESSE_UART1));
	uart2_Init(GetlReg(RNV_VITESSE_UART2));
	
	uart4_Init(115200);
	uart5_Init(115200);
	
    gsm_Init();

    InitRelais();

    InitEauGlycolee();

	comsbusmaitre_Init();


	// configure for multi-vectored mode
    INTEnableSystemMultiVectoredInt();
    
    if(!RCONbits.SWR)
    {
		_excep_code = 0;
		_excep_addr = 0;
		_cpt_reset = 0;
	}
}

#define PREPARE_TRAME_IO_SBUS       0
#define REPONSE_TRAME_IO_SBUS       1
#define PREPARE_TRAME_INFO_SBUS     2
#define REPONSE_TRAME_INFO_SBUS     3
int ReadDataMicro2MAE = PREPARE_TRAME_IO_SBUS;

void ReadDataMicro2()
{
	int Cpt;
    switch(ReadDataMicro2MAE)
    {
        case PREPARE_TRAME_IO_SBUS:
        {
	    if(!GetTimer(TIMER_TEMPS_READ_DATA_PIC2))
            {
	            if(!comsbusmaitre_bit[2].bSbusLibre)
	            {
	                return;
	            }
	            
	            comsbusmaitre_bit[2].bSbusLibre = 0;
	            comsbusmaitre_bit[2].bSbusEnd = 0;
	            comsbusmaitre_bit[2].SbusErr = 0;
	            comsbusmaitre_TOReponse[2] = 8;
	            comsbusmaitre_SbusStation[2] = 255;
	            
	            comsbusmaitre_SbusAdresse[2][0].l = 0;
	            
	            comsbusmaitre_SbusNbr[2] = 20;
	            comsbusmaitre_SbusType[2] = 'F';
	            comsbusmaitre_MAESbusMaitre[2] = SBUS_MAITRE_READ_FLG;
	
	            ReadDataMicro2MAE = REPONSE_TRAME_IO_SBUS;
            }
            break;
        }
        case REPONSE_TRAME_IO_SBUS:
        {
            if(comsbusmaitre_bit[2].bSbusEnd)
            {
                comsbusmaitre_bit[2].bSbusLibre = 1;

                if(comsbusmaitre_bit[2].SbusErr)
                {

                }
                else
                {
	                for(Cpt = 0; Cpt < 10; Cpt++)
                    {
	                    SetFlag(FV_E13 + Cpt, ((char *)comsbusmaitre_SbusBuffer[2])[Cpt + 0]);
	                }
	                
	                for(Cpt = 0; Cpt < 10; Cpt++)
                    {
	                    SetFlag(FV_S13 + Cpt, ((char *)comsbusmaitre_SbusBuffer[2])[Cpt + 10]);
	                }
                }
  
                ReadDataMicro2MAE = PREPARE_TRAME_INFO_SBUS;
            }
            break;
        }
        //#define RV_DATE_COMPILE_GSM                     565
        //#define RV_VERSION_GSM                          566
        case PREPARE_TRAME_INFO_SBUS:
        {
	    if(!GetTimer(TIMER_TEMPS_READ_DATA_PIC2))
            {
	            if(!comsbusmaitre_bit[2].bSbusLibre)
	            {
	                return;
	            }

	            comsbusmaitre_bit[2].bSbusLibre = 0;
	            comsbusmaitre_bit[2].bSbusEnd = 0;
	            comsbusmaitre_bit[2].SbusErr = 0;
	            comsbusmaitre_TOReponse[2] = 8;
	            comsbusmaitre_SbusStation[2] = 255;

	            comsbusmaitre_SbusAdresse[2][0].l = 20004;
                comsbusmaitre_SbusAdresse[2][1].l = 20008;

	            comsbusmaitre_SbusNbr[2] = 2;
	            comsbusmaitre_SbusType[2] = 'R';
	            comsbusmaitre_MAESbusMaitre[2] = SBUS_MAITRE_READ_MULTI_REG;

	            ReadDataMicro2MAE = REPONSE_TRAME_INFO_SBUS;
            }
            break;
        }
        case REPONSE_TRAME_INFO_SBUS:
        {
            if(comsbusmaitre_bit[2].bSbusEnd)
            {
                comsbusmaitre_bit[2].bSbusLibre = 1;

                if(comsbusmaitre_bit[2].SbusErr)
                {

                }
                else
                {
                    SetlReg(RV_VERSION_GSM, comsbusmaitre_SbusBuffer[2][1].l);
                    SetlReg(RV_DATE_COMPILE_GSM, comsbusmaitre_SbusBuffer[2][3].l);
                }
                SetTimer(TIMER_TEMPS_READ_DATA_PIC2, 50);
                ReadDataMicro2MAE = PREPARE_TRAME_IO_SBUS;
            }
            break;
        }
    }
}

int main()
{
	int i;
	
	init();

	SetlReg(RV_CODE_EXCEPTION_ERR, _excep_code);
	SetlReg(RV_ADR_EXCEPTION_ERR, _excep_addr);
	SetlReg(RV_CPT_RESET_EXCEPTION, _cpt_reset);

	if(_cpt_reset > 5)
	{
		gSysEtat = 'E';
		pwm_Arret();
	}
	SetTimer(TIMER_MAX_SANS_EXCEPTION, 10*60*15); //15mn
    SetTimer(TIMER_VEILLE_AFFICHEUR, GetlReg(RNV_DUREE_REVEILLE_AFFICHEUR)*10);
	SetTimer(TIMER_MISE_SOUS_TENSION, 300);

    while(1)
	{
        if(!GetTimer(TIMER_MISE_SOUS_TENSION))
        {
            SetFlag(FV_MISE_SOUS_TENSION, 1);
        }

		// Demande reset
		if(GetFlag(FV_RESET))
		{
			mainbit.bReset = 1;
		}
		

		if(mainbit.bReset)
		{
			mainbit.bReset = 0;
			Sec_Save();
			SoftReset();
			while(1);
		}
		
		uart1_Gestion();
		uart2_Gestion();
		uart4_Gestion();
		uart5_Gestion();
		
		if(gSysEtat == 'S')
		{
			terminal_Gestion();
		}
		else if(gSysEtat == 'E')
		{
			if(!GetTimer(TIMER_LED9))
			{
				SetTimer(TIMER_LED9, 1);
				//LED9 = !LED9;
			}
		}	
		else if(gSysEtat == 'R')
		{
			if(!GetTimer(TIMER_MAX_SANS_EXCEPTION))
			{
				_cpt_reset = 0;
				SetlReg(RV_CPT_RESET_EXCEPTION, _cpt_reset);
			}
			
			ReadDataMicro2();

            uart1_Gestion();
			uart2_Gestion();
			
			Sec_Gestion();
			
			clock_Gestion();
			
			can_Gestion();
			
			tor_gestion();
			
			terminal_Gestion();
		
			uart1_Gestion();
			uart2_Gestion();
			
			if(can_Valide())
			{
				for(i=0; i<CAN_NBR_ENTREE_ANA; i++)
				{
					AnaType1_Gestion(&Ana[i]);
				}
			}

			for(i=0; i < TOR_NBR_ENTREE_INT; i++)
			{
				Echant_Gestion(&ETor[i]);
			}

			pwm_Gestion();

            uart1_Gestion();
			uart2_Gestion();

            gsm_Gestion();

            comsbusmaitre_Gestion();

            // Applicatif Irrigation
           	if(GetFlag(FNV_ACTIVE_EGL))
			{
				GestionEauGlycolee();
			}
            else
            {
                GestionRelais();
            }
        }
	}

	return 1;
}


