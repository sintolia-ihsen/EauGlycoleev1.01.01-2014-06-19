#if !defined(TOR_H)
#define TOR_H

#define TOR_TOTAL_POINT(num) 					(RV_TOTAL_POINT_E1 + ((num)*4))
#define TOR_ECHANT_POINT(num) 					(RV_ECHANT_POINT_E1 + ((num)*4))
#define TOR_TEMPS_N_IMP_POINT(num) 				(RV_TEMPS_N_IMP_POINT_E1 + ((num)*4))
#define TOR_CPT_NBR_IMP(num) 					(RV_CPT_NBR_IMP_E1 + ((num)*4))

#define TOR_TEMPS_ECHANT(num) 					(RNV_TEMPS_ECHANT_E1 + ((num)*4))
#define TOR_NBR_IMP(num) 						(RNV_NBR_IMP_E1 + ((num)*4))
#define TOR_TEMPS_MIN_ENTRE_IMP(num) 			(RNV_TEMPS_MIN_ENTRE_IMP_E1 + ((num)*4))
#define TOR_TEMPS_MAX_ENTRE_IMP(num) 			(RNV_TEMPS_MAX_ENTRE_IMP_E1 + ((num)*4))

#define TOR_CPT_TEMPS_MIN_ENTRE_IMP(num)	(0 + ((num)*2))
#define TOR_CPT_TEMPS_ENTRE_N_IMP(num)		(1 + ((num)*2))

#define TOR_TIMER_TEMPS_MAX_TOR(Num)		TIMER_TEMPS_MAX_E(Num)

#define TOR_OUTPUT							0
#define TOR_INPUT							1

#define TOR_AV_ACTIVATION_SORTIE			0
#define TOR_TEMPO_AV_ACTIVATION_SORTIE		1
#define TOR_TEMPO_ACTIVATION_SORTIE			2

#define TOR_SORTIE_EN_ETAT_ON				0
#define TOR_SORTIE_EN_ETAT_OFF				1

#define TOR_NBR_SORTIE						12
#define TOR_NBR_ENTREE						12
#define TOR_NBR_ENTREE_INT					4

#define TOR_ENTREE01						0 
#define TOR_ENTREE02						1 
#define TOR_ENTREE03						2 
#define TOR_ENTREE04						3 
#define TOR_ENTREE05						4 
#define TOR_ENTREE06						5 
#define TOR_ENTREE07						6 
#define TOR_ENTREE08						7 
#define TOR_ENTREE09						8 
#define TOR_ENTREE10						9 
#define TOR_ENTREE11						10
#define TOR_ENTREE12						11

                                  
#define TOR_SORTIE01						0 
#define TOR_SORTIE02						1 
#define TOR_SORTIE03						2 
#define TOR_SORTIE04						3 
#define TOR_SORTIE05						4 
#define TOR_SORTIE06						5 
#define TOR_SORTIE07						6 
#define TOR_SORTIE08						7 
#define TOR_SORTIE09						8 
#define TOR_SORTIE10						9 
#define TOR_SORTIE11						10
#define TOR_SORTIE12						11



#define TOR_ADR_FLAG_IMG_E(num)			FV_E(num)
#define TOR_ADR_FLAG_IMG_S(num)			FV_S(num)

//Entrées
#define TOR_TRIS_E01			_TRISE8
#define TOR_TRIS_E02			_TRISE9
#define TOR_TRIS_E03			_TRISA14
#define TOR_TRIS_E04			_TRISA15
#define TOR_TRIS_E05			_TRISB3
#define TOR_TRIS_E06			_TRISB4
#define TOR_TRIS_E07			_TRISB5
#define TOR_TRIS_E08			_TRISB15
#define TOR_TRIS_E09			_TRISA10
#define TOR_TRIS_E10			_TRISD5
#define TOR_TRIS_E11			_TRISG2
#define TOR_TRIS_E12			_TRISG3


#define TOR_E01					_RE8 
#define TOR_E02					_RE9 
#define TOR_E03					_RA14
#define TOR_E04					_RA15
#define TOR_E05					_RB3
#define TOR_E06					_RB4
#define TOR_E07					_RB5
#define TOR_E08					_RB15
#define TOR_E09					_RA10
#define TOR_E10					_RD5
#define TOR_E11					_RG2
#define TOR_E12					_RG3

//Entrées                         
#define TOR_TRIS_S01			_TRISD6
#define TOR_TRIS_S02			_TRISG0
#define TOR_TRIS_S03			_TRISB9
#define TOR_TRIS_S04			_TRISB10
#define TOR_TRIS_S05			_TRISB11
#define TOR_TRIS_S06			_TRISB12
#define TOR_TRIS_S07			_TRISB13
#define TOR_TRIS_S08			_TRISB14
#define TOR_TRIS_S09			_TRISC14
#define TOR_TRIS_S10			_TRISF0
#define TOR_TRIS_S11			_TRISF1
#define TOR_TRIS_S12			_TRISG1

#define TOR_S01					_LATD6
#define TOR_S02					_LATG0
#define TOR_S03					_LATB9
#define TOR_S04					_LATB10
#define TOR_S05					_LATB11
#define TOR_S06					_LATB12
#define TOR_S07					_LATB13
#define TOR_S08					_LATB14
#define TOR_S09					_LATC14
#define TOR_S10					_LATF0
#define TOR_S11					_LATF1
#define TOR_S12					_LATG1

typedef struct
{
	unsigned bSortie :1;
	unsigned bEntree :1;
	unsigned bInvert :1;
	unsigned bChargeTimer :1;
	unsigned bImageE1 :1;
	unsigned bImageE2 :1;
	unsigned bImageE3 :1;
	unsigned bImageE4 :1;
	unsigned bImageE5 :1;
	unsigned bImageE6 :1;
	unsigned bImageE7 :1;
	unsigned bImageE8 :1;
	unsigned bImageE9 :1;
	unsigned bImageE10:1;
	unsigned bImageE11:1;
	unsigned bImageE12:1;
}bittor;

// Prototypes des fonctions publiques
void tor_init();
void tor_gestion(void);
void tor_SetSorties(int num, int val);
void tor_Echant(void);
void tor_Timer10ms(void);
#endif //TOR_H
