//////////////////////////////////////////////////////////////////////////////////
// Date de création : 20 Décembre 2002											//
// Auteur			: FBY														//
// Description		: Fonctions pour gérer le protocole XSBus Esclave d'un PIC18//
//////////////////////////////////////////////////////////////////////////////////
#if !defined(SLVXSBUS_H)
#define SLVXSBUS_H

#include "uart.h"

#define UART1_SLV_XSBUS_ACTIF		MODE_SLV_UART1_ACTIF
#define UART2_SLV_XSBUS_ACTIF		MODE_SLV_UART2_ACTIF

// Définitions diverses //
#define SLVXSBUS_BANK_RAM					
#define SLVXSBUS_CRC16_BUFFER_SIZE			4
#define SLVXSBUS_NUM_STATION				gNumStation
#define SLVXSBUS_NUM_STATION_UNIVERSELLE	0xFF
#define SLVXSBUS_NB_REGS_MAX				112
#define SLVXSBUS_NB_MULTI_REGS_MAX			64
#define SLVXSBUS_NB_REGS_TOTAL				NB_REGS_TOTAL
#define SLVXSBUS_NB_TIMERS_MAX				112
#define SLVXSBUS_NB_MULTI_TIMERS_MAX		64
#define SLVXSBUS_NB_TIMERS_TOTAL			NB_TIMERS_TOTAL
#define SLVXSBUS_NB_FLAGS_MAX				128
#define SLVXSBUS_NB_MULTI_FLAGS_MAX			128
#define SLVXSBUS_NB_FLAGS_TOTAL				NB_FLAGS_TOTAL
#define SLVXSBUS_NB_INPUT_MAX				0
#define SLVXSBUS_NB_INPUT_TOTAL				0
#define SLVXSBUS_NB_OUTPUT_MAX				2
#define SLVXSBUS_NB_OUTPUT_TOTAL			2
#define SLVXSBUS_NB_MEM_MAX					512
#define SLVXSBUS_NB_MEM_TOTAL				0x7FFFF

#define SLVXSBUS_MODE_DATA					1 // peut être assigné directement à une entrée

// Activation des requêtes de la librairie (mettre en commentaire pour désactiver)
#define	SLVXSBUS_RQ_READ_STATUS_ACTIF
#define	SLVXSBUS_RQ_READ_FLAG_ACTIF
#define	SLVXSBUS_RQ_READ_MULTI_FLAG_ACTIF
#define	SLVXSBUS_RQ_WRITE_FLAG_ACTIF
#define	SLVXSBUS_RQ_WRITE_MULTI_FLAG_ACTIF
//#define	SLVXSBUS_RQ_READ_INPUT_ACTIF
//#define	SLVXSBUS_RQ_READ_OUTPUT_ACTIF
//#define	SLVXSBUS_RQ_WRITE_OUTPUT_ACTIF
#define	SLVXSBUS_RQ_READ_REG_ACTIF
#define	SLVXSBUS_RQ_READ_MULTI_REG_ACTIF
#define	SLVXSBUS_RQ_WRITE_REG_ACTIF
#define	SLVXSBUS_RQ_WRITE_MULTI_REG_ACTIF
#define	SLVXSBUS_RQ_READ_TIMER_ACTIF
#define	SLVXSBUS_RQ_READ_MULTI_TIMER_ACTIF
#define	SLVXSBUS_RQ_WRITE_TIMER_ACTIF
#define	SLVXSBUS_RQ_WRITE_MULTI_TIMER_ACTIF
#define	SLVXSBUS_RQ_READ_MEM_ACTIF	// expérimental
#define	SLVXSBUS_RQ_WRITE_MEM_ACTIF	// expérimental
#define	SLVXSBUS_RQ_READ_MEM_EX_ACTIF	// expérimental
#define	SLVXSBUS_RQ_WRITE_MEM_EX_ACTIF	// expérimental
#define	SLVXSBUS_RQ_RESET_ACTIF
#define SLVXSBUS_RQ_RUN_ACTIF
#define SLVXSBUS_RQ_STOP_ACTIF
//#define	SLVXSBUS_RQ_FLASH_USER_ACTIF
//#define	SLVXSBUS_RQ_FLASH_CORE_ACTIF
//#define SLVXSBUS_RQ_READ_FLASH_ACTIF

#define	STOP 		'S'		// Mode stop (pas de multitâche ni de timers auto-dec)
#define RUN  		'R'		// Mode RUN (normal)
#define	FLASH_USER	'U'		// Mode de flashage de USER (attente écriture en FLASH par SBus puis reset)
#define	FLASH_NOYAU	'N'		// Impossible dans le noyau (uniquement dans le booteur/flasheur
#define	ERREUR		'E'		// En Erreur (si une erreur grâve arrive)
	
#define slvxsbus_ReadInput(Num) 			0
#define slvxsbus_ReadOutput(Num) 			0
//#include "main.h" // pour les définitions des sorties
#define slvxsbus_WriteOutput(Num,Val)		0

// Requêtes S-Bus et XS-Bus
#define	SLVXSBUS_RQ_READ_FLAG				0x02
#define	SLVXSBUS_RQ_READ_INPUT				0x03
#define	SLVXSBUS_RQ_READ_OUTPUT				0x05
#define	SLVXSBUS_RQ_READ_REG				0x06
#define	SLVXSBUS_RQ_READ_TIMER				0x07
#define	SLVXSBUS_RQ_READ_MEM				0x08
#define	SLVXSBUS_RQ_READ_MEM_EX				0x12
#define	SLVXSBUS_RQ_WRITE_FLAG				0x0B
#define	SLVXSBUS_RQ_WRITE_OUTPUT			0x0D
#define	SLVXSBUS_RQ_WRITE_REG				0x0E
#define	SLVXSBUS_RQ_WRITE_TIMER				0x0F
#define	SLVXSBUS_RQ_WRITE_MEM				0x10
#define	SLVXSBUS_RQ_WRITE_MEM_EX			0x13
#define SLVXSBUS_RQ_READ_STATUS				0x1B
#define SLVXSBUS_RQ_RUN						0x2F
#define SLVXSBUS_RQ_RESET					0x39
#define SLVXSBUS_RQ_STOP					0x43
#define SLVXSBUS_RQ_FLASH_USER				0x44
#define SLVXSBUS_RQ_FLASH_CORE				0x46
#define SLVXSBUS_RQ_READ_FLASH				0x47
#define SLVXSBUS_RQ_READ_RAM				0x48
#define SLVXSBUS_RQ_READ_EEPROM				0x49

// Ajouter 05/05/2010
#define SLVXSBUS_RQ_READ_MULTI_REG   		0x14
#define SLVXSBUS_RQ_READ_MULTI_TIMER  		0x15
#define SLVXSBUS_RQ_READ_MULTI_FLAG   		0x16
#define SLVXSBUS_RQ_WRITE_MULTI_REG   		0x17
#define SLVXSBUS_RQ_WRITE_MULTI_TIMER  		0x18
#define SLVXSBUS_RQ_WRITE_MULTI_FLAG  		0x19

// Offsets des trames XSBus reçues
#define SLVXSBUS_OFFSET_NUMSTATION			0x00
#define SLVXSBUS_OFFSET_RQ					0x01
#define SLVXSBUS_OFFSET_TAILLE				0x02
#define SLVXSBUS_OFFSET_DATA				0x03
#define SLVXSBUS_OFFSET_DATA_MEM_EX			0x04

// Machine à états XSBus
#define SLVXSBUS_MAE_RAZ					0x00
#define	SLVXSBUS_MAE_BREAK					0x01
#define	SLVXSBUS_MAE_NUMSTATION				0x02
#define	SLVXSBUS_MAE_RQ						0x03
#define SLVXSBUS_MAE_TAILLE					0x05
#define SLVXSBUS_MAE_DATA					0x06
#define SLVXSBUS_MAE_CRC_HIGH				0x07
#define SLVXSBUS_MAE_CRC_LOW				0x08
#define SLVXSBUS_MAE_WAIT_FIN_TRAITE		0x09
#define SLVXSBUS_MAE_FIN					0x10
#define SLVXSBUS_MAE_WAIT_TX				0x20
#define SLVXSBUS_MAE_TX						0x40

// Machine à états Mode DATA
#define SLVXSBUS_MAE_DATA_RAZ				0x00
#define SLVXSBUS_MAE_DATA_S					0x01
#define SLVXSBUS_MAE_DATA_Y					0x02
#define SLVXSBUS_MAE_DATA_N					0x03
#define SLVXSBUS_MAE_DATA_C					0x04
#define SLVXSBUS_MAE_DATA_SYNC_RECEIVED		0x06
#define SLVXSBUS_MAE_DATA_O					0x07
#define SLVXSBUS_MAE_DATA_K					0x08

// Caractères de synchronisation Mode DATA
#define SLVXSBUS_CAR_1ST_MODE_DATA			'S'
#define SLVXSBUS_CAR_2ND_MODE_DATA			'Y'
#define SLVXSBUS_CAR_3RD_MODE_DATA			'N'
#define SLVXSBUS_CAR_4TH_MODE_DATA			'C'


typedef struct
{
	unsigned bDemandeReset:1;
	unsigned bTempbitsbus:1;
	unsigned bDataFlash:1;
	unsigned bSync:1;
}bitslv;


void slvxsbus_Gestion(char *pBuffer, unsigned short Taille, unsigned char Destination, void (*fonc)());
void slvxsbus_Init(void);
#endif //XSBUS_H
