#if !defined(TIMERS_H)
#define TIMERS_H

#define TMR_USE_TIMER2
#define TMR_USE_TIMER4
//#define TMR_USE_TIMER5
//#define TMR_USE_TIMER6
//#define TMR_USE_TIMER8

#define TMR_TICK_TIMER			3.2
#define TMR_DUREE_1_SECONDE		10
#define TMR_DUREE_1_MINUTE		600
#define TMR_DUREE_1_HEURE		36000
#define TMR_DUREE_1_JOUR		864000

#ifdef TMR_USE_TIMER2
void timer2_Init(void);
void timer2_Stop(void);
void timer2_Start(void);
#endif

#ifdef TMR_USE_TIMER4
void timer4_Init(void);
unsigned short timer4_GetTimer(int Index);
void timer4_SetTimer(int Index, unsigned short Val);

#define TIMER4_NBR_TV	16

#define TV_UART1_RECEPTION	0
#define TV_UART1_ENVOI		1
#define TV_UART1_TO_ENVOI   2
#define TV_UART2_RECEPTION	3
#define TV_UART2_ENVOI		4
#define TV_UART2_TO_ENVOI   5
#define TV_UART3_RECEPTION	6
#define TV_UART3_ENVOI		7
#define TV_UART3_TO_ENVOI   8
#define TV_UART4_RECEPTION	9
#define TV_UART4_ENVOI		10
#define TV_UART4_TO_ENVOI   11
#define TV_UART5_RECEPTION	12
#define TV_UART5_ENVOI		13
#define TV_UART5_TO_ENVOI   14

#endif


#ifdef TMR_USE_TIMER5
void timer5_Init(void);
void timer5_Stop(void);
void timer5_Start(void);
#endif

#endif
