//////////////////////////////////////////////////////////////
// Date de création : 20/02/2007							//
// Auteur			: Faouzi								//
// Description		: Fonctions pour gérer l'UART-DSPIC		//
//////////////////////////////////////////////////////////////

#include <plib.h>
#include "main.h"
#include "uart.h"
#include "timers.h"
#include "variables.h"
//#include "slvxsbus.h"
//#include "mtrxsbus.h"


// Table de pré-calcul CRC-166 CCITT en ROM (Table-Driven)
const unsigned short CrcTable[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

//----------------------------------------------------------------------
// Fonction : Effectue le calcul d'un CRC16 à la volée (8 bits / 8 bits)
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
unsigned short Calcul_CRC16(char *pBuffer, unsigned short Nbr)
{
	unsigned short i = 0;
	unsigned short Temp = 0x0000;
	unsigned short TempCRC = 0x0000;
	for(i = 0; i < Nbr; i++)
	{
		Temp = (TempCRC >> 8) ^ (pBuffer[i]&0xFF);
		TempCRC = (TempCRC << 8) ^ CrcTable[Temp];
	}
	return TempCRC;
}

bituart uartbit;

#if UART1_ACTIF == 1

char uart1_Rx_Buffer[UART1_BUFFER_SIZE]={0};
unsigned int uart1_RX_gCompteur = 0;

//----------------------------------------------
// Fonction : Initialisation Port série
// Entrée 	: pointeur sur une structure CfgUart
//----------------------------------------------
void uart1_Init(unsigned long Vitesse)
{
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, GetPeripheralClock(), Vitesse);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
	
	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);

	#ifdef UART1_MODE_422
	UART1_TRIS_ENABLE_TX = 0;
	UART1_OUTPUT_ENABLE_TX = 1;
	#endif
	
	timer4_Init();
	
	switch(gUart1Mode)
	{
		case UART_MODE_SLV :
		{
			slvxsbus_Init();
			break;
		}
	}

	mtrxsbus_Init(0, uart1_Write);
}

void uart1_Stop()
{
	UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTEnable(UART1, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART1), INT_DISABLED);
}

//----------------------------------------------------------------------
// Fonction : ecriture buffeur sur le UART1
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void uart1_Write(char* data, int Nbr)
{
	
	#ifdef UART1_MODE_422
	UART1_OUTPUT_ENABLE_TX = 0;
	timer4_SetTimer(TV_UART1_ENVOI, 2);
	while(timer4_GetTimer(TV_UART1_ENVOI));
	#endif
	uartbit.bUart1Envoi = 1;
    while(Nbr)
    {
        while(U1STAbits.TRMT == 0);
        U1TXREG = *data;
        data++;
        Nbr --;
    }
	uartbit.bUart1Envoi = 0;
    #ifdef UART1_MODE_422
	timer4_SetTimer(TV_UART1_ENVOI, 2);
	while(timer4_GetTimer(TV_UART1_ENVOI));
	UART1_OUTPUT_ENABLE_TX = 1;
	#endif
}

void uart1_Gestion()
{
	int Temp;
	if(!timer4_GetTimer(TV_UART1_RECEPTION))
	{
		if(uart1_RX_gCompteur)
		{
			switch(gUart1Mode)
			{
				case UART_MODE_SLV :
				{
					slvxsbus_Gestion(uart1_Rx_Buffer, uart1_RX_gCompteur, 0, uart1_Write);
					break;
				}
				case UART_MODE_MTR :
				{
					mtrxsbus_Reponse(0, uart1_Rx_Buffer, uart1_RX_gCompteur);
					break;
				}
			}
			

			uart1_RX_gCompteur = 0;
		}
	}
	
	if(UARTGetLineStatus(UART1)&UART_OVERRUN_ERROR)
	{
		U1STAbits.OERR = 0;
		Temp = UARTGetDataByte(UART1);
	}
	
	if(UARTGetLineStatus(UART1)&UART_FRAMING_ERROR)
	{
		U1STAbits.FERR = 0;
	}
}

// UART 1 interrupt handler
// it is set at priority level 1
void __ISR(_UART_1_VECTOR, ipl2) IntUART1Handler(void)
{
	int Temp;
	// Is this an RX interrupt?
 	if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))
	{
		// Echo what we just received.		
		Temp = UARTGetDataByte(UART1);
			
		if(!uartbit.bUart1Envoi)
		{	
			if(uart1_RX_gCompteur >= UART1_BUFFER_SIZE )
			{
				uart1_RX_gCompteur = 0;
			}
			
			// lecture des caracteres recu
			uart1_Rx_Buffer[uart1_RX_gCompteur] = Temp;
			
			uart1_RX_gCompteur++;
			
			// charger le temps de silence
			timer4_SetTimer(TV_UART1_RECEPTION, 2);
		}
			
		// Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART1));
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART1));
	}
}

#endif //UART1_ACTIF == 1

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
#if UART2_ACTIF == 1

char uart2_Rx_Buffer[UART2_BUFFER_SIZE]={0};
unsigned int uart2_RX_gCompteur = 0;


//----------------------------------------------
// Fonction : Initialisation Port série
// Entrée 	: pointeur sur une structure CfgUart
//----------------------------------------------
void uart2_Init(unsigned long Vitesse)
{
    UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, GetPeripheralClock(), Vitesse);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
	
	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_0);
	
	#ifdef UART2_MODE_422
	UART2_TRIS_ENABLE_TX = 0;
	UART2_OUTPUT_ENABLE_TX = 1;
	#endif
	
	timer4_Init();
	
	switch(gUart2Mode)
	{
		case UART_MODE_SLV :
		{
			slvxsbus_Init();
			break;
		}

	}
	mtrxsbus_Init(1, uart2_Write);
}

void uart2_Stop()
{
	UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTEnable(UART2, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART2), INT_DISABLED);
}

//----------------------------------------------------------------------
// Fonction : ecriture buffeur sur le UART2
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void uart2_Write(char* data, int Nbr)
{
    #ifdef UART2_MODE_422
	UART2_OUTPUT_ENABLE_TX = 0;
	timer4_SetTimer(TV_UART2_ENVOI, 2);
	while(timer4_GetTimer(TV_UART2_ENVOI));
	#endif
    uartbit.bUart2Envoi = 1;
    while(Nbr)
    {
        while(U2STAbits.TRMT == 0);
        U2TXREG = *data;
        data++;
        Nbr --;
    }
	uartbit.bUart2Envoi = 0;
    #ifdef UART2_MODE_422
	timer4_SetTimer(TV_UART2_ENVOI, 2);
	while(timer4_GetTimer(TV_UART2_ENVOI));
	UART2_OUTPUT_ENABLE_TX = 1;
	#endif
}

void uart2_Gestion()
{
	int Temp;
	if(!timer4_GetTimer(TV_UART2_RECEPTION))
	{
		if(uart2_RX_gCompteur)
		{
			switch(gUart2Mode)
			{
				case UART_MODE_SLV :
				{
					slvxsbus_Gestion(uart2_Rx_Buffer, uart2_RX_gCompteur, 0, uart2_Write);
					break;
				}
				case UART_MODE_MTR :
				{
					mtrxsbus_Reponse(1, uart2_Rx_Buffer, uart2_RX_gCompteur);
					break;
				}
				case UART_MODE_GSM :
				{
					gsm_Trame(uart2_Rx_Buffer, uart2_RX_gCompteur);
					break;
				}

			}
			uart2_RX_gCompteur = 0;
		}
	}
	
	if(UARTGetLineStatus(UART2)&UART_OVERRUN_ERROR)
	{
		U2STAbits.OERR = 0;
		Temp = UARTGetDataByte(UART2);
	}
	
	if(UARTGetLineStatus(UART2)&UART_FRAMING_ERROR)
	{
		U2STAbits.FERR = 0;
	}

}
// UART 1 interrupt handler
// it is set at priority level 1
void __ISR(_UART_2_VECTOR, ipl2) IntUart2Handler(void)
{
	int Temp;
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART2)))
	{
		// Echo what we just received.		
		Temp = UARTGetDataByte(UART2);
			
		if(!uartbit.bUart2Envoi)
		{	
			if( uart2_RX_gCompteur >= UART2_BUFFER_SIZE )
			{
				uart2_RX_gCompteur = 0;
			}
			
			// lecture des caracteres recu
			uart2_Rx_Buffer[uart2_RX_gCompteur] = Temp;
			
			uart2_RX_gCompteur++;
			
			// charger le temps de silence
			timer4_SetTimer(TV_UART2_RECEPTION, 4);
		}

			
		// Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART2));
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART2)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART2));
	}
}

#endif //UART2_ACTIF == 2


//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
#if UART3_ACTIF == 1

char uart3_Rx_Buffer[UART3_BUFFER_SIZE]={0};
unsigned int uart3_RX_gCompteur = 0;


//----------------------------------------------
// Fonction : Initialisation Port série
// Entrée 	: pointeur sur une structure CfgUart
//----------------------------------------------
void uart3_Init(unsigned long Vitesse)
{
#ifdef UART_USE_RTS_CTS
    UARTConfigure(UART3, UART_ENABLE_PINS_CTS_RTS);
#else
    UARTConfigure(UART3, UART_ENABLE_PINS_TX_RX_ONLY);
#endif
    UARTSetFifoMode(UART3, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART3, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART3, GetPeripheralClock(), Vitesse);
    UARTEnable(UART3, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART3 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART3), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART3), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART3), INT_SUB_PRIORITY_LEVEL_0);

	#ifdef UART3_MODE_422
	UART3_TRIS_ENABLE_TX = 0;
	UART3_OUTPUT_ENABLE_TX = 1;
	#endif

	timer4_Init();
}

void uart3_Stop()
{
	UARTConfigure(UART3, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTEnable(UART3, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART3), INT_DISABLED);
}

//----------------------------------------------------------------------
// Fonction : ecriture buffeur sur le UART3
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void uart3_Write(char* data, int Nbr)
{
    #ifdef UART3_MODE_422
	UART3_OUTPUT_ENABLE_TX = 1;
	timer4_SetTimer(TV_UART3_ENVOI, 2);
	while(timer4_GetTimer(TV_UART3_ENVOI));
	#endif
    uartbit.bUart3Envoi = 1;
    while(Nbr)
    {
        timer4_SetTimer(TV_UART3_TO_ENVOI, 2);
        while(U3STAbits.TRMT == 0 && timer4_GetTimer(TV_UART3_TO_ENVOI));
        U3TXREG = *data;
        if(gUart2Mode == UART_MODE_MCH || gUart2Mode == UART_MODE_PAS )
        {
            U2TXREG = *data;
        }
//        if(gUart6Mode == UART_MODE_MCH || gUart6Mode == UART_MODE_PAS )
//        {
//            U6TXREG = *data;
//        }

        data++;
        Nbr --;
    }
	uartbit.bUart3Envoi = 0;
    #ifdef UART3_MODE_422
	timer4_SetTimer(TV_UART3_ENVOI, 2);
	while(timer4_GetTimer(TV_UART3_ENVOI));
	UART3_OUTPUT_ENABLE_TX = 0;
	#endif
 }

void uart3_Gestion()
{
	int Temp;
	if(!timer4_GetTimer(TV_UART3_RECEPTION))
	{
		if(uart3_RX_gCompteur)
		{
			gsm_Trame(uart3_Rx_Buffer, uart3_RX_gCompteur);
			uart3_RX_gCompteur = 0;
		}
	}

	if(UARTGetLineStatus(UART3)&UART_OVERRUN_ERROR)
	{
		U3STAbits.OERR = 0;
		Temp = UARTGetDataByte(UART3);
	}
	if(UARTGetLineStatus(UART3)&UART_FRAMING_ERROR)
	{
		U3STAbits.FERR = 0;
	}
}
// UART 1 interrupt handler
// it is set at priority level 1
void __ISR(_UART_3_VECTOR, ipl2) IntUart3Handler(void)
{
	int Temp;
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART3)))
	{
		// Echo what we just received.
		Temp = UARTGetDataByte(UART3);
        if(gUart2Mode == UART_MODE_MCH || gUart2Mode == UART_MODE_PAS )
        {
            U2TXREG = Temp;
        }
//		if(gUart6Mode == UART_MODE_MCH || gUart6Mode == UART_MODE_PAS )
//        {
//            U6TXREG = Temp;
//        }
		if(!uartbit.bUart3Envoi)
		{
			if( uart3_RX_gCompteur >= UART3_BUFFER_SIZE )
			{
				uart3_RX_gCompteur = 0;
			}

			// lecture des caracteres recu
			uart3_Rx_Buffer[uart3_RX_gCompteur] = Temp;

			uart3_RX_gCompteur++;

			// charger le temps de silence
			timer4_SetTimer(TV_UART3_RECEPTION, 2);
		}

		// Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART3));
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART3)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART3));
	}
}

#endif //UART3_ACTIF == 1

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
#if UART4_ACTIF == 1

char uart4_Rx_Buffer[UART4_BUFFER_SIZE]={0};
unsigned int uart4_RX_gCompteur = 0;


//----------------------------------------------
// Fonction : Initialisation Port série
// Entrée 	: pointeur sur une structure CfgUart
//----------------------------------------------
void uart4_Init(unsigned long Vitesse)
{
    UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART4, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART4, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART4, GetPeripheralClock(), Vitesse);
    UARTEnable(UART4, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART4 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART4), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART4), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART4), INT_SUB_PRIORITY_LEVEL_0);

	#ifdef UART4_MODE_422
	UART4_TRIS_ENABLE_TX = 0;
	UART4_OUTPUT_ENABLE_TX = 1;
	#endif

	timer4_Init();
}

void uart4_Stop()
{
	UARTConfigure(UART4, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTEnable(UART4, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART4), INT_DISABLED);
}

//----------------------------------------------------------------------
// Fonction : ecriture buffeur sur le UART4
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void uart4_Write(char* data, int Nbr)
{
    #ifdef UART4_MODE_422
	UART4_OUTPUT_ENABLE_TX = 1;
	timer4_SetTimer(TV_UART4_ENVOI, 2);
	while(timer4_GetTimer(TV_UART4_ENVOI));
	#endif
    uartbit.bUart4Envoi = 1;
    while(Nbr)
    {
        timer4_SetTimer(TV_UART4_TO_ENVOI, 2);
        while(U4STAbits.TRMT == 0);// && timer4_GetTimer(TV_UART4_TO_ENVOI));
        U4TXREG = *data;
        //U2TXREG = *data;
        data++;
        Nbr --;
    }
	uartbit.bUart4Envoi = 0;
    #ifdef UART4_MODE_422
	timer4_SetTimer(TV_UART4_ENVOI, 2);
	while(timer4_GetTimer(TV_UART4_ENVOI));
	UART4_OUTPUT_ENABLE_TX = 0;
	#endif
 }

void uart4_Gestion()
{
	int Temp;
	if(!timer4_GetTimer(TV_UART4_RECEPTION))
	{
		if(uart4_RX_gCompteur)
		{
			slvxsbus_Gestion(uart4_Rx_Buffer, uart4_RX_gCompteur, 0, uart4_Write);

			uart4_RX_gCompteur = 0;
		}
	}

	if(UARTGetLineStatus(UART4)&UART_OVERRUN_ERROR)
	{
		U4STAbits.OERR = 0;
		Temp = UARTGetDataByte(UART4);
	}
	if(UARTGetLineStatus(UART4)&UART_FRAMING_ERROR)
	{
		U4STAbits.FERR = 0;
	}
}
// UART 1 interrupt handler
// it is set at priority level 1
void __ISR(_UART_4_VECTOR, ipl2) IntUart4Handler(void)
{
	int Temp;
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART4)))
	{
		// Echo what we just received.
		Temp = UARTGetDataByte(UART4);
		//U2TXREG = Temp;
		if(!uartbit.bUart4Envoi)
		{
			if( uart4_RX_gCompteur >= UART4_BUFFER_SIZE )
			{
				uart4_RX_gCompteur = 0;
			}

			// lecture des caracteres recu
			uart4_Rx_Buffer[uart4_RX_gCompteur] = Temp;

			uart4_RX_gCompteur++;

			// charger le temps de silence
			timer4_SetTimer(TV_UART4_RECEPTION, 2);
		}

		// Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART4));
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART4)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART4));
	}
}

#endif //UART4_ACTIF == 1

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
#if UART5_ACTIF == 1

char uart5_Rx_Buffer[UART5_BUFFER_SIZE]={0};
unsigned int uart5_RX_gCompteur = 0;


//----------------------------------------------
// Fonction : Initialisation Port série
// Entrée 	: pointeur sur une structure CfgUart
//----------------------------------------------
void uart5_Init(unsigned long Vitesse)
{
    UARTConfigure(UART5, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART5, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART5, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART5, GetPeripheralClock(), Vitesse);
    UARTEnable(UART5, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART5 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART5), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART5), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART5), INT_SUB_PRIORITY_LEVEL_0);

	#ifdef UART5_MODE_422
	UART5_TRIS_ENABLE_TX = 0;
	UART5_OUTPUT_ENABLE_TX = 1;
	#endif

	timer4_Init();
	
	mtrxsbus_Init(2, uart5_Write);
}

void uart5_Stop()
{
	UARTConfigure(UART5, UART_ENABLE_PINS_TX_RX_ONLY);
	UARTEnable(UART5, UART_DISABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

	// Configure UART1 RX Interrupt
	INTEnable(INT_SOURCE_UART_RX(UART5), INT_DISABLED);
}

//----------------------------------------------------------------------
// Fonction : ecriture buffeur sur le UART5
// Entrée 	: Rien
// Sortie 	: Rien
//----------------------------------------------------------------------
void uart5_Write(char* data, int Nbr)
{
    #ifdef UART5_MODE_422
	UART5_OUTPUT_ENABLE_TX = 1;
	timer4_SetTimer(TV_UART5_ENVOI, 2);
	while(timer4_GetTimer(TV_UART5_ENVOI));
	#endif
    uartbit.bUart5Envoi = 1;
    while(Nbr)
    {
        timer4_SetTimer(TV_UART5_TO_ENVOI, 2);
        while(U5STAbits.TRMT == 0);// && timer4_GetTimer(TV_UART5_TO_ENVOI));
        U5TXREG = *data;
        data++;
        Nbr --;
    }
	uartbit.bUart5Envoi = 0;
    #ifdef UART5_MODE_422
	timer4_SetTimer(TV_UART5_ENVOI, 2);
	while(timer4_GetTimer(TV_UART5_ENVOI));
	UART5_OUTPUT_ENABLE_TX = 0;
	#endif
 }

void uart5_Gestion()
{
	int Temp;
	if(!timer4_GetTimer(TV_UART5_RECEPTION))
	{
		if(uart5_RX_gCompteur)
		{
			mtrxsbus_Reponse(2, uart5_Rx_Buffer, uart5_RX_gCompteur);
			uart5_RX_gCompteur = 0;
		}
	}

	if(UARTGetLineStatus(UART5)&UART_OVERRUN_ERROR)
	{
		U5STAbits.OERR = 0;
		Temp = UARTGetDataByte(UART5);
	}
	if(UARTGetLineStatus(UART5)&UART_FRAMING_ERROR)
	{
		U5STAbits.FERR = 0;
	}
}
// UART 1 interrupt handler
// it is set at priority level 1
void __ISR(_UART_5_VECTOR, ipl2) IntUart5Handler(void)
{
	int Temp;
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART5)))
	{
		// Echo what we just received.
		Temp = UARTGetDataByte(UART5);

		if(!uartbit.bUart5Envoi)
		{
			if( uart5_RX_gCompteur >= UART5_BUFFER_SIZE )
			{
				uart5_RX_gCompteur = 0;
			}

			// lecture des caracteres recu
			uart5_Rx_Buffer[uart5_RX_gCompteur] = Temp;

			uart5_RX_gCompteur++;

			// charger le temps de silence
			timer4_SetTimer(TV_UART5_RECEPTION, 2);
		}

		// Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART5));
	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART5)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART5));
	}
}

#endif //UART5_ACTIF == 1

