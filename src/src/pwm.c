#include <plib.h>
#include "pwm.h"
#include "main.h"



void pwm_init()
{
	PWM_TRIS_PWM1 = 0;
	PWM_TRIS_PWM2 = 0;
	PWM_TRIS_PWM3 = 0;
	PWM_TRIS_PWM4 = 0;
	
	/* Open Timer2 with Period register value */
	OpenTimer3(T3_ON, 4000);

	SetlReg(RV_DUTY_CYCLE_PWM1, 2000);
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
	OpenOC2( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , 2000, 2000 );

	SetlReg(RV_DUTY_CYCLE_PWM2, 2000);
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
	OpenOC3( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , 2000, 2000 );

	SetlReg(RV_DUTY_CYCLE_PWM3, 2000);
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
	OpenOC4( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , 2000, 2000 );

	SetlReg(RV_DUTY_CYCLE_PWM4, 2000);
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
	OpenOC5( OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE , 2000, 2000 );
}

void pwm_Arret()
{
	SetlReg(RV_DUTY_CYCLE_PWM1, 0);
	SetlReg(RV_DUTY_CYCLE_PWM2, 0);
	SetlReg(RV_DUTY_CYCLE_PWM3, 0);
	SetlReg(RV_DUTY_CYCLE_PWM4, 0);
	pwm_Gestion();
}

void pwm_Gestion()
{
	int DutyCycle;
	
	DutyCycle = GetlReg(RV_DUTY_CYCLE_PWM1);
	if(DutyCycle != ReadDCOC2PWM())
	{
		SetDCOC2PWM(DutyCycle);
	}

	DutyCycle = GetlReg(RV_DUTY_CYCLE_PWM2);
	if(DutyCycle != ReadDCOC3PWM())
	{
		SetDCOC3PWM(DutyCycle);
	}


	DutyCycle = GetlReg(RV_DUTY_CYCLE_PWM3);
	if(DutyCycle != ReadDCOC4PWM())
	{
		SetDCOC4PWM(DutyCycle);
	}

        DutyCycle = GetlReg(RV_DUTY_CYCLE_PWM4);
	if(DutyCycle != ReadDCOC5PWM())
	{
		SetDCOC5PWM(DutyCycle);
	}

}


