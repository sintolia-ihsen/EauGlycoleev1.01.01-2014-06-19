//////////////////////////////////////////////////////////////
// Date de création : 08 Février 2007						//
// Auteur			: Faouzi									//
// Description		: Fonctions pour gérer l'UART			//
//////////////////////////////////////////////////////////////
#if !defined(UART_H)
#define UART_H


#define FCY 80000000L
//#define FCY 20000000L
//#define FCY 4000000L
#define UART1_ACTIF		1
#define UART2_ACTIF		1
#define UART3_ACTIF		1
#define UART4_ACTIF		1
#define UART5_ACTIF		1

#define OUTPUT		0
#define INPUT		1


typedef struct
{
	unsigned bUart1Envoi :1;
	unsigned bUart2Envoi :1;
	unsigned bUart3Envoi :1;
	unsigned bUart4Envoi :1;
	unsigned bUart5Envoi :1;
}bituart;
	
///////////////////////////////////////////////////////
//	**	**	   **		*******		**********	  ****
//	**	**	  ****		********	**********	    **
//	**	**	 **	 **		**    **		**	   	    **
//	******	********	********		**			**    	 
//	******  **	  **	**	  **		**        *****
///////////////////////////////////////////////////////
#if UART1_ACTIF == 1

//extern char Uart1_Tx_Buffer[];
extern char uart1_Rx_Buffer[];
extern unsigned int uart1_RX_gCompteur;

#define UART1_BUFFER_SIZE			1024

#define UART1_MODE_422
#ifdef UART1_MODE_422
#define UART1_TRIS_ENABLE_TX		_TRISC13
#define UART1_OUTPUT_ENABLE_TX		_LATC13
#endif
	
// Prototypes des fonctions
void uart1_Init(unsigned long Vitesse);
void uart1_Stop();
void uart1_Gestion(void);
void uart1_Write(char* data, int Nbr);

#endif //UART1_ACTIF == 1
///////////////////////////////////////////////////////
//	**	**	   **		*******		**********	*******
//	**	**	  ****		********	**********	*    **
//	**	**	 **	 **		**    **		**	   	   *
//	******	********	********		**		**    	 
//	******  **	  **	**	  **		**      *******
///////////////////////////////////////////////////////
#if UART2_ACTIF == 1
//extern char Uart2_Tx_Buffer[];
extern char uart2_Rx_Buffer[];
extern unsigned int uart2_RX_gCompteur;

#define UART2_BUFFER_SIZE			1024

#define UART2_MODE_422
#ifdef UART2_MODE_422
#define UART2_TRIS_ENABLE_TX		_TRISB2
#define UART2_OUTPUT_ENABLE_TX		_LATB2
#endif



// Prototypes des fonctions
void uart2_Init(unsigned long Vitesse);
void uart2_Stop();
void uart2_Gestion(void);
void uart2_Write(char* data, int Nbr);

#endif //UART2_ACTIF == 2


///////////////////////////////////////////////////////
//	**	**	   **		*******		**********	*******
//	**	**	  ****		********	**********	*    **
//	**	**	 **	 **		**    **		**	   	   ****
//	******	********	********		**		*    **
//	******  **	  **	**	  **		**      *******
///////////////////////////////////////////////////////
#if UART3_ACTIF == 1
//extern char Uart3_Tx_Buffer[];
extern char uart3_Rx_Buffer[];
extern unsigned int uart3_RX_gCompteur;

#define UART_USE_RTS_CTS
//#define UART_USE_MOUCHARD
#define UART3_BUFFER_SIZE			1024

#ifndef UART_USE_RTS_CTS
	#warning CTS RTS non utilisie
#endif
#ifdef UART_USE_MOUCHARD
	#warning Mouchard GSM utilisie
#endif

//#define UART3_MODE_422
#ifdef UART3_MODE_422
#define UART3_TRIS_ENABLE_TX		_TRISF1
#define UART3_OUTPUT_ENABLE_TX		_LATF1
#endif

// Prototypes des fonctions
void uart3_Init(unsigned long Vitesse);
void uart3_Stop();
void uart3_Gestion(void);
void uart3_Write(char* data, int Nbr);

#endif //UART3_ACTIF == 1



///////////////////////////////////////////////////////
//	**	**	   **		*******		**********    *
//	**	**	  ****		********	**********   *
//	**	**	 **	 **		**    **		**	   	*  *
//	******	********	********		**	   *******
//	******  **	  **	**	  **		**         *
///////////////////////////////////////////////////////
#if UART4_ACTIF == 1

extern char uart4_Rx_Buffer[];
extern unsigned int uart4_RX_gCompteur;


#define UART4_BUFFER_SIZE			1024

//#define UART4_MODE_422
#ifdef UART4_MODE_422
#define UART4_TRIS_ENABLE_TX		_TRISF1
#define UART4_OUTPUT_ENABLE_TX		_LATF1
#endif

// Prototypes des fonctions
void uart4_Init(unsigned long Vitesse);
void uart4_Stop();
void uart4_Gestion(void);
void uart4_Write(char* data, int Nbr);

#endif //UART4_ACTIF == 1

///////////////////////////////////////////////////////
//	**	**	   **		*******		**********   *****
//	**	**	  ****		********	**********   *
//	**	**	 **	 **		**    **		**       *****
//	******	********	********		**	         *
//	******  **	  **	**	  **		**       *****
///////////////////////////////////////////////////////
#if UART5_ACTIF == 1

extern char uart5_Rx_Buffer[];
extern unsigned int uart5_RX_gCompteur;


#define UART5_BUFFER_SIZE			1024

//#define UART5_MODE_422
#ifdef UART5_MODE_422
#define UART5_TRIS_ENABLE_TX		_TRISF1
#define UART5_OUTPUT_ENABLE_TX		_LATF1
#endif

// Prototypes des fonctions
void uart5_Init(unsigned long Vitesse);
void uart5_Stop();
void uart5_Gestion(void);
void uart5_Write(char* data, int Nbr);

#endif //UART5_ACTIF == 1

unsigned short Calcul_CRC16(char *pBuffer, unsigned short Nbr);

#endif // !defined(UART_H)
