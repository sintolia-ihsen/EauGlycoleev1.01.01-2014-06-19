#include "comsbusmaitre.h"
#include "main.h"
#include "variables.h"
#include "mtrxsbus.h"
#include "config.h"

unsigned char comsbusmaitre_Uart;
unsigned char comsbusmaitre_UartMode[3];
bitComSbusMaitre    comsbusmaitre_bit[3];
unsigned char comsbusmaitre_MAESbusMaitre[3];
unsigned char comsbusmaitre_SbusStation[3];
unsigned char comsbusmaitre_SbusNbr[3];
unsigned char comsbusmaitre_SbusType[3];
unsigned char comsbusmaitre_SbusRq[3];
unsigned short comsbusmaitre_TOReponse[3];
unsigned char comsbusmaitre_SbusErrCom[3];

ofsld comsbusmaitre_SbusAdresse[3][64];
ofsld comsbusmaitre_SbusBuffer[3][512];

void comsbusmaitre_Init()
{
    comsbusmaitre_UartMode[0] = gUart1Mode;
    comsbusmaitre_UartMode[1] = gUart2Mode;
    comsbusmaitre_UartMode[2] = UART_MODE_MTR;
}

void comsbusmaitre_ReponseSbusTrame(const char* data, int Nbr)
{
	memcpy(comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], data, Nbr);
	comsbusmaitre_SbusNbr[comsbusmaitre_Uart] = Nbr;
    comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
}

int comsbusmaitre_Gestion()
{
	int Cpt;

	comsbusmaitre_Uart++;
    if(comsbusmaitre_Uart >= 3)
    {
        comsbusmaitre_Uart = 0;
    }

    switch(comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart])
	{
		case SBUS_MAITRE_LIBRE:
		{
			return 0;
		}
		case SBUS_MAITRE_READ_REG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
					comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
					comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
					return 0;
				}

				mainbit.bMaitreLibre = 0;
				switch(	mtrxsbus_ReadRTC(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (long *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusType[comsbusmaitre_Uart] == 'R')
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][Cpt].l = GetlReg(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l+Cpt);
					}
				}
				else
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][Cpt].l = GetTimer(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l+Cpt);
					}
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_READ_MULTI_REG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(	mtrxsbus_ReadMultiRTC(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], (unsigned long *)comsbusmaitre_SbusAdresse[comsbusmaitre_Uart], comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (long *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
                        comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusType[comsbusmaitre_Uart] == 'R')
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][(Cpt*2) + 0].l = comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l;
						comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][(Cpt*2) + 1].l = GetlReg(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l);
					}
				}
				else
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][(Cpt*2) + 0].l = comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l;
						comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][(Cpt*2) + 1].l = GetTimer(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l);
					}
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_READ_FLG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(mtrxsbus_ReadIOF(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
				{
					*(((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart]) + Cpt) = GetFlag(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l+Cpt);
				}

				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_READ_MULTI_FLG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(mtrxsbus_ReadMultiIOF(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], (unsigned long *)comsbusmaitre_SbusAdresse[comsbusmaitre_Uart], comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
				{
					*((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart] + (Cpt*5) + 0) = comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].of.octet[3];
					*((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart] + (Cpt*5) + 1) = comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].of.octet[2];
					*((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart] + (Cpt*5) + 2) = comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].of.octet[1];
					*((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart] + (Cpt*5) + 3) = comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].of.octet[0];

					*((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart] + (Cpt*5) + 4) = GetFlag(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l);
				}

				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_WRITE_REG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(mtrxsbus_WriteRTC(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (long *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusType[comsbusmaitre_Uart] == 'R')
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						SetlReg(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l+Cpt, comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][Cpt].l);
					}
				}
				else
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						SetTimer(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l+Cpt, comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][Cpt].l);
					}
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}

		case SBUS_MAITRE_WRITE_MULTI_REG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(mtrxsbus_WriteMultiRTC(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], (unsigned long *)comsbusmaitre_SbusAdresse[comsbusmaitre_Uart], comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (long *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusType[comsbusmaitre_Uart] == 'R')
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						SetlReg(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l, comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][Cpt].l);
					}
				}
				else
				{
					for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
					{
						SetTimer(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l, comsbusmaitre_SbusBuffer[comsbusmaitre_Uart][Cpt].l);
					}
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}

		case SBUS_MAITRE_WRITE_FLG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(mtrxsbus_WriteOF(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
				{
					SetFlag(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l+Cpt, *(((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart]) + Cpt));
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_WRITE_MULTI_FLG	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 1)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(mtrxsbus_WriteMultiOF(comsbusmaitre_Uart, comsbusmaitre_SbusType[comsbusmaitre_Uart], comsbusmaitre_SbusStation[comsbusmaitre_Uart], (unsigned long *)comsbusmaitre_SbusAdresse[comsbusmaitre_Uart], comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				for(Cpt = 0; Cpt<comsbusmaitre_SbusNbr[comsbusmaitre_Uart]; Cpt++)
				{
					SetFlag(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][Cpt].l, *(((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart]) + Cpt));
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_READ_MEM	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 2)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(	mtrxsbus_ReadMem(comsbusmaitre_Uart, comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l >= 0xFFF00000 && comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l + comsbusmaitre_SbusNbr[comsbusmaitre_Uart] <= 0xFFF05400)// ROM (flash)
				{
					comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l -=0xFFF00000;
					configGSM_Read((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart]);
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				}
				else
				{
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_WRITE_MEM	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 2)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(	mtrxsbus_WriteMem(comsbusmaitre_Uart, comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l >= 0xFFF00000 && comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l + comsbusmaitre_SbusNbr[comsbusmaitre_Uart] <= 0xFFF05400)// ROM (flash)
				{
					comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l -=0xFFF00000;
					configGSM_Write((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart]);
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				}
				else
				{
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_READ_MEM_EX	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 2)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(	mtrxsbus_ReadMemEx(comsbusmaitre_Uart, comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l >= 0xFFF00000 && comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l + comsbusmaitre_SbusNbr[comsbusmaitre_Uart] <= 0xFFF05400)// ROM (flash)
				{
					comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l -=0xFFF00000;
					configGSM_Read((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart]);
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				}
				else
				{
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_MAITRE_WRITE_MEM_EX	:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 2)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;
				switch(	mtrxsbus_WriteMemEx(comsbusmaitre_Uart, comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{
						mainbit.bMaitreLibre = 1;
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
				if(comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l >= 0xFFF00000 && comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l + comsbusmaitre_SbusNbr[comsbusmaitre_Uart] < 0xFFF05400)// ROM (flash)
				{
					comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l -=0xFFF00000;
					configGSM_Write((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart]);
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				}
				else
				{
					comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
				}
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
		case SBUS_TRAME:
		{
			if(comsbusmaitre_SbusStation[comsbusmaitre_Uart] != gNumStation && comsbusmaitre_SbusStation[comsbusmaitre_Uart] != 0)
			{
			#if(SBUS_STATION_EXTERNE == 2)
				if(comsbusmaitre_UartMode[comsbusmaitre_Uart] != UART_MODE_MTR)
				{
					if(!GetTimer(TIMER_TO_BASCULE_MODE))
					{
						comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 2;
						comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
						comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
						return 0;
					}
					return 1;
				}

				mainbit.bMaitreLibre = 0;

				mainbit.bMaitreLibre = 1;
				comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 0;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
				/*switch(	mtrxsbus_WriteMemEx(comsbusmaitre_Uart, comsbusmaitre_SbusStation[comsbusmaitre_Uart], comsbusmaitre_SbusAdresse[comsbusmaitre_Uart][0].l, comsbusmaitre_SbusNbr[comsbusmaitre_Uart], (char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_TOReponse[comsbusmaitre_Uart]))
				{
					case MTRXSBUS_SUCCESS :
					{

					}

					case MTRXSBUS_ERROR_REPONSE :
					case MTRXSBUS_ERROR_TIMEOUT :
					{
						comsbusmaitre_SbusErrCom[comsbusmaitre_Uart]++;
						if(comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] >= SBUS_MAITRE_NBR_TENTATIVE_ERR)
						{
							mainbit.bMaitreLibre = 1;
							comsbusmaitre_SbusErrCom[comsbusmaitre_Uart] = 0;
							comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
							comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
							comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
							return 0;
						}
					}
				}*/
			#else
				comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 3;
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			#endif
			}
			else
			{
                comsbusmaitre_bit[comsbusmaitre_Uart].SbusErr = 1;
				slvxsbus_Gestion((char *)comsbusmaitre_SbusBuffer[comsbusmaitre_Uart], comsbusmaitre_SbusNbr[comsbusmaitre_Uart], 0, comsbusmaitre_ReponseSbusTrame);
				comsbusmaitre_bit[comsbusmaitre_Uart].bSbusEnd = 1;
				comsbusmaitre_MAESbusMaitre[comsbusmaitre_Uart] = SBUS_MAITRE_LIBRE;
				return 0;
			}
			break;
		}
	}

    return 1;
}


