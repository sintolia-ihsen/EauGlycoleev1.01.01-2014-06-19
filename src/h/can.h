
#if !defined(CAN_H)
#define CAN_H

#define OUTPUT								0
#define INPUT								1


#define CAN_NBR_ENTREE_ANA					24
#define CAN_MUX_PRESENT						1

#if(CAN_MUX_PRESENT)
#define CAN_NBR_ENTREE_PAR_MUX				8

#define CAN_TRIS_MUX_ADD_0					_TRISG12
#define CAN_TRIS_MUX_ADD_1					_TRISG13
#define CAN_TRIS_MUX_ADD_2					_TRISG14

#define CAN_MUX_ADD_0						_LATG12
#define CAN_MUX_ADD_1						_LATG13
#define CAN_MUX_ADD_2						_LATG14

#define Mux_SetNum(Val)					{ 												\
											can_OCtet2flags.octet[0] = (Val);			\
											CAN_MUX_ADD_0 = can_OCtet2flags.flags.f0;	\
											CAN_MUX_ADD_1 = can_OCtet2flags.flags.f1;	\
											CAN_MUX_ADD_2 = can_OCtet2flags.flags.f2;	\
										}
#endif


typedef struct
{
	unsigned b1Convert:1;
	unsigned bScan:2;
}bitcan;

void can_Init(void);
void can_Gestion(void);
char can_Valide(void);
void can_Stop(void);

#endif //#define CAN_H
