//////////////////////////////////////////////////////////////
// Date de création : 14 Février 2007						//
// Auteur			: Faouzi								//
// Description		: Fonctions pour gérer le CAN			//
//////////////////////////////////////////////////////////////

#include <plib.h>
#include "can.h"
#include "main.h"
#include "variables.h"

unsigned int  can_NumEntreeDirect=0;
bitcan	canbit;

#if(CAN_MUX_PRESENT)
octetflags can_OCtet2flags;
unsigned int  can_NumEntreeMux=0;
unsigned int MuxOrder[] = { 0x0, 0x1, 0x3, 0x2, 0x6, 0x7, 0x5, 0x4};
#endif

//--------------------------------------------------------------------------------------------
// Fonction : can_Init
// Utilité : Initialisation des IO et les registres analogique
//--------------------------------------------------------------------------------------------
void can_Init(void)
{

	can_NumEntreeDirect=0;

    if(GetlReg(RNV_TEMPO_MUX) == 0xFFFFFFFF)
    {
        SetlReg(RNV_TEMPO_MUX, 5);
    }


	_TRISB6 = INPUT;
	_TRISB7 = INPUT;
	_TRISB8 = INPUT;

	#if(CAN_MUX_PRESENT)
	Mux_SetNum(0);
	CAN_TRIS_MUX_ADD_0 = OUTPUT;
	CAN_TRIS_MUX_ADD_1 = OUTPUT;
	CAN_TRIS_MUX_ADD_2 = OUTPUT;
	#endif

	// configure and enable the ADC
	CloseADC10();	// ensure the ADC is off before setting the configuration

	// define setup parameters for OpenADC10
	// 				Turn module on | ouput in integer | trigger mode auto | enable autosample
	#define PARAM1  ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON

	// define setup parameters for OpenADC10
	#define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_SAMPLES_PER_INT_3 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF

	// define setup parameters for OpenADC10
	//				  use ADC internal clock | set sample time
	#define PARAM3  ADC_CONV_CLK_SYSTEM | ADC_SAMPLE_TIME_31 | 0xFF


	// define setup parameters for OpenADC10
	//               set AN4 and AN5 as analog inputs

	#define PARAM4	ENABLE_AN6_ANA | \
					ENABLE_AN7_ANA | \
					ENABLE_AN8_ANA

	// define setup parameters for OpenADC10
	// do not assign channels to scan
	#define PARAM5	ENABLE_AN0_ANA | \
					ENABLE_AN1_ANA | \
					ENABLE_AN2_ANA | \
					ENABLE_AN3_ANA | \
					ENABLE_AN4_ANA | \
					ENABLE_AN5_ANA | \
					ENABLE_AN9_ANA | \
					ENABLE_AN10_ANA | \
					ENABLE_AN11_ANA | \
					ENABLE_AN12_ANA | \
					ENABLE_AN13_ANA | \
					ENABLE_AN14_ANA | \
					ENABLE_AN15_ANA


	// use ground as neg ref for A | use AN4 for input A      |  use ground as neg ref for A | use AN5 for input B

	// configure to sample AN4 & AN5
	SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN6 |  ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN6);
	OpenADC10(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5); // configure ADC using the parameters defined above


	AD1CON1bits.CLRASAM = 1;

	#if(CAN_MUX_PRESENT)
	can_NumEntreeMux=0;
	Mux_SetNum(MuxOrder[can_NumEntreeMux]);
	#endif

	canbit.bScan = 3;

	SetTimer(TIMER_CAN, 1);
	EnableADC10(); // Enable the ADC
}

void can_Stop()
{
	CAN_TRIS_MUX_ADD_0 = INPUT;
	CAN_TRIS_MUX_ADD_1 = INPUT;
	CAN_TRIS_MUX_ADD_2 = INPUT;

	CloseADC10();
}

char can_Valide()
{
	return !canbit.bScan;
}

#define CAN_START_CONVERSION        0
#define CAN_READ_CONVERSION         1

unsigned char can_MAEConversion = CAN_READ_CONVERSION;

void can_Gestion()
{
	if(IFS1bits.AD1IF)
	{
		SetlReg(RV_AN01 + MuxOrder[can_NumEntreeMux], ReadADC10(0));
		SetlReg(RV_AN09 + MuxOrder[can_NumEntreeMux], ReadADC10(1));
		SetlReg(RV_A1 + MuxOrder[can_NumEntreeMux], ReadADC10(2));

		can_NumEntreeMux++;
		if(can_NumEntreeMux >= 8)
		{
			can_NumEntreeMux = 0;
			if(canbit.bScan) canbit.bScan--;
		}
		Mux_SetNum(MuxOrder[can_NumEntreeMux]);
		canbit.b1Convert = 1;
		SetTimer(TIMER_CAN, GetlReg(RNV_TEMPO_MUX));
		IFS1bits.AD1IF = 0;
	}

	if(canbit.b1Convert)
	{
		if(!GetTimer(TIMER_CAN))
		{
			AD1CON1bits.ASAM = 1;
			canbit.b1Convert = 0;
			SetTimer(TIMER_CAN, 2);
		}
	}

//    switch(can_MAEConversion)
//    {
//        case CAN_START_CONVERSION:
//        {
//            if(!GetTimer(TIMER_CAN))
//            {
//                AD1CON1bits.ASAM = 1;
//                can_MAEConversion = CAN_READ_CONVERSION;
//            }
//            break;
//        }
//        case CAN_READ_CONVERSION:
//        {
//            if(IFS1bits.AD1IF)
//            {
//                IFS1bits.AD1IF = 0;
//
//                Nop();
//                Nop();
//                Nop();
//
//                SetlReg(RV_AN01 + MuxOrder[can_NumEntreeMux], ReadADC10(0));
//                SetlReg(RV_AN09 + MuxOrder[can_NumEntreeMux], ReadADC10(1));
//                SetlReg(RV_A1 + MuxOrder[can_NumEntreeMux], ReadADC10(2));
//
//                can_NumEntreeMux++;
//                if(can_NumEntreeMux >= 8)
//                {
//                    can_NumEntreeMux = 0;
//                    if(canbit.bScan) canbit.bScan--;
//                }
//                Mux_SetNum(MuxOrder[can_NumEntreeMux]);
//                SetTimer(TIMER_CAN, GetlReg(RNV_TEMPO_MUX));
//
//                can_MAEConversion = CAN_START_CONVERSION;
//            }
//            break;
//        }
//    }
}
