
#define MTRXSBUS_SUCCESS			0
#define MTRXSBUS_ERROR_TIMEOUT		1
#define MTRXSBUS_ERROR_REPONSE		2
#define MTRXSBUS_TX_EN_COURS		3
#define MTRXSBUS_RX_EN_COURS		4

#define MTRXSBUS_TAILLE_BUFFER		258

typedef struct
{
	unsigned bSynchro:1;
	unsigned bEncoure:1;
	unsigned bTxEncoure:1;
	unsigned bReponseOk:1;
}mtrxsbusbit;

typedef struct
{
	unsigned int NbrCarRx;
	unsigned char Buffer[MTRXSBUS_TAILLE_BUFFER];
	void (*Sendfonc)();
}mtrVar;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/****************************	UART 1	***************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
void mtrxsbus_Init(char Uart, void (*fonc)());
void mtrxsbus_Reponse(char Uart, char *pBuffer, unsigned int Taille);

int mtrxsbus_ReadRTC(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, long *pReponse, unsigned short TimeOut);
int mtrxsbus_WriteRTC(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, long *pData, unsigned short TimeOut);
int mtrxsbus_ReadMultiRTC(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, long *pReponse, unsigned short TimeOut);
int mtrxsbus_WriteMultiRTC(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, long *pData, unsigned short TimeOut);
int mtrxsbus_ReadIOF(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, char *pData, unsigned short TimeOut);
int mtrxsbus_WriteOF(char Uart, char Type, unsigned char Cible, unsigned short AdbaseCible, unsigned char Nbr, char *pData, unsigned short TimeOut);
int mtrxsbus_ReadMultiIOF(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, char *pData, unsigned short TimeOut);
int mtrxsbus_WriteMultiOF(char Uart, char Type, unsigned char Cible, unsigned long *pAdresse, unsigned char Nbr, char *pData, unsigned short TimeOut);
int mtrxsbus_ReadMemEx(char Uart, unsigned char Cible, unsigned long AdbaseCible, unsigned int Nbr, char *pReponse, unsigned short TimeOut);
int mtrxsbus_WriteMemEx(char Uart, unsigned char Cible, unsigned long AdbaseCible, unsigned int Nbr, char *pData, unsigned short TimeOut);
