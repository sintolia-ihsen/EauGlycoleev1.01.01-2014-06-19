/* 
 * File:   comsbusmaitre.h
 * Author: Faouzi
 *
 * Created on 9 mars 2013, 11:37
 */

#ifndef COMSBUSMAITRE_H
#define	COMSBUSMAITRE_H

#include "variables.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define SBUS_MAITRE_NBR_TENTATIVE_ERR   3

#define SBUS_MAITRE_LIBRE				0
#define SBUS_MAITRE_READ_REG			1
#define SBUS_MAITRE_READ_FLG			2
#define SBUS_MAITRE_WRITE_REG			3
#define SBUS_MAITRE_WRITE_FLG			4
#define SBUS_MAITRE_READ_MEM			5
#define SBUS_MAITRE_WRITE_MEM			6
#define SBUS_MAITRE_READ_MULTI_REG		7
#define SBUS_MAITRE_WRITE_MULTI_REG		8
#define SBUS_MAITRE_READ_MULTI_FLG		9
#define SBUS_MAITRE_WRITE_MULTI_FLG		10
#define SBUS_MAITRE_READ_MEM_EX			11
#define SBUS_MAITRE_WRITE_MEM_EX		12
#define SBUS_TRAME						13

#define SBUS_STATION_EXTERNE			1

    // Uart maitre externe
//#define gUartMode		gUart2Mode
//#define UART_MODE_MTR	UART2_MODE_MTR


typedef struct
{
	unsigned bSbusLibre:1;
	unsigned bSbusEnd:1;
	unsigned SbusErr:3;
}bitComSbusMaitre;

extern bitComSbusMaitre    comsbusmaitre_bit[3];
extern unsigned char comsbusmaitre_MAESbusMaitre[3];
extern unsigned char comsbusmaitre_SbusStation[3];
extern unsigned char comsbusmaitre_SbusNbr[3];
extern unsigned char comsbusmaitre_SbusType[3];
extern unsigned char comsbusmaitre_SbusRq[3];
extern unsigned short comsbusmaitre_TOReponse[3];
extern unsigned char comsbusmaitre_SbusErrCom[3];

extern ofsld comsbusmaitre_SbusAdresse[3][64];
extern ofsld comsbusmaitre_SbusBuffer[3][512];

void comsbusmaitre_Init(void);
int comsbusmaitre_Gestion(void);

#ifdef	__cplusplus
}
#endif

#endif	/* COMSBUSMAITRE_H */

