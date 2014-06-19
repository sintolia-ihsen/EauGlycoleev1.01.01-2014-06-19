#include <plib.h>
#include "ads7846.h"
#include "s1d13700.h"
#include "graphics.h"
#include "variables.h"
#include "terminal.h"
//////////////////////// LOCAL PROTOTYPES ////////////////////////////
// Max/Min ADC values for each derection
volatile WORD   	_calXMin = XMINCAL;
volatile WORD       _calXMax = XMAXCAL;
volatile WORD       _calYMin = YMINCAL;
volatile WORD       _calYMax = YMAXCAL;

// Current ADC values for X and Y channels
volatile SHORT      adcOld = -1;
volatile SHORT      adcTemp = -1;
volatile SHORT      adcX = -1;
volatile SHORT      adcY = -1;
volatile SHORT      adcZ1 = -1;
volatile SHORT      adcZ2 = -1;
volatile SHORT      adcRt = -1;
volatile SHORT      adcPot = 0;
volatile SHORT      hyst = 50;
volatile SHORT      nbrOffset = 0;

volatile SHORT      adcX1 = -1;
volatile SHORT      adcY1 = -1;

BYTE_VAL affEtat;

#define TIME_BASE   14400  // 100ms

unsigned char tempdelay = 1;
void delay(unsigned char temp)
{
	while(temp--);
}

//=======================================
unsigned int ads7846_Cmd(unsigned int data)
{

   	SPI1BUF=data;
   	while(!SPI1STATbits.SPIRBF);		// wait receive data available
	return(SPI1BUF>>11);
}



/*********************************************************************
* Function: void ads7846_Init(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: sets ADC
*
* Note: none
*
********************************************************************/
void ads7846_Init(void)
{
	TP_TRIS_IRQ = 1;
	TP_TRIS_CS  = 0;
	TP_TRIS_SDI = 1;
	TP_TRIS_SDO = 0;
	TP_TRIS_CLK = 0;

	TP_CS = 0;

	SpiChnOpen(SPI_CHANNEL1, SPI_OPEN_MODE32|SPI_OPEN_ON|SPI_OPEN_MSTEN|SPI_OPEN_CKE_REV, 512);

        // init input capture
        _TRISD12 = 1;
        IC5CON = 0x8002;        // TMR2, int every cap, fall'n edge
        mIC5ClearIntFlag();     // clear the interrupt flag
        mIC5SetIntPriority( 5);
        mIC5IntEnable( 1);      // enable the IC1 interrupt


//    if(GetlReg(20694) == 0xFFFFFFFF)
//    {
//        SetlReg(20694, XMINCAL);
//        SetlReg(20695, XMAXCAL);
//        SetlReg(20696, YMINCAL);
//        SetlReg(20697, YMAXCAL);
//    }
}

int IRQ_OK = 1;
double Rt;

void ads7846_Click()
{
    if (!IRQ_OK)
    {
        IRQ_OK = 1;
    }
}

void ads7846_Gestion()
{
	//if(IRQ_OK)
	if(IRQ_OK)
	{
		SetTimer(TIMER_RETROECLAIRAGE, GetlReg(RNV_DUREE_RETROECLAIRAGE)*10);
//		SetTimer(TIMER_VEILLE_AFFICHEUR, GetlReg(RNV_DUREE_REVEILLE_AFFICHEUR)*10);
        SetTimer(TIMER_DUREE_VIE_ECRAN, GetlReg(RNV_DUREE_VIE_ECRAN)*10);
        
		TP_CS = 0;

		//Z1
		adcZ1 = ads7846_Cmd(0xB0000000);

		if(adcZ1)
		{
			//Z2
			adcZ2 = ads7846_Cmd(0xC0000000);

			//X
			adcX = ads7846_Cmd(0xD0000000);

			Rt = 400;
			Rt *= ((double)adcX/4096);
			Rt *= (((double)adcZ2/(double)adcZ1)-1);
			adcRt = Rt;

			if(adcRt < 1000)
			{
				//Y
				adcY = ads7846_Cmd(0x90000000);
			}
			else
			{
				adcRt = -1;
				adcX = -1;
				adcY = -1;
				adcZ1 = -1;
				adcZ2 = -1;
			}
		}
		else
		{
			adcRt = -1;
			adcX = -1;
			adcY = -1;
			adcZ1 = -1;
			adcZ2 = -1;
			IRQ_OK = 0;
		}

		TP_CS = 1;
	}
}

/*********************************************************************
* Function: SHORT TouchGetX()
*
* PreCondition: none
*
* Input: none
*
* Output: x coordinate
*
* Side Effects: none
*
* Overview: returns x coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetX(void)
{
    long    result;

//    _calXMin = GetlReg(20694);
//    _calXMax = GetlReg(20695);

    result = ADCGetX();
    if(result >= 0)
    {
        result = (S1D13700_GET_MAX_X() * (result - _calXMin)) / (_calXMax - _calXMin);

        result = S1D13700_GET_MAX_X() - result;
    }

    return (result);
}

/*********************************************************************
* Function: SHORT TouchGetY()
*
* PreCondition: none
*
* Input: none
*
* Output: y coordinate
*
* Side Effects: none
*
* Overview: returns y coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchGetY(void)
{
    long    result;

//    _calYMin = GetlReg(20696);
//    _calYMax = GetlReg(20697);

    result = ADCGetY();
    if(result >= 0)
    {
        result = (S1D13700_GET_MAX_Y() * (result - _calYMin)) / (_calYMax - _calYMin);

        result = S1D13700_GET_MAX_Y() - result;
    }

    return (result);
}

/*********************************************************************
* Function: SHORT TouchGetY()
*
* PreCondition: none
*
* Input: none
*
* Output: y coordinate
*
* Side Effects: none
*
* Overview: returns y coordinate if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
SHORT TouchActive(void)
{
    if(ADCGetX() >= 0 && ADCGetY() >= 0)
    {
        return (1);
    }

    return (0);
}

/*********************************************************************
* Function: void ads7846_GetMsg(GOL_MSG* pMsg)
*
* PreCondition: none
*
* Input: pointer to the message structure to be populated
*
* Output: none
*
* Side Effects: none
*
* Overview: populates GOL message structure
*
* Note: none
*
********************************************************************/
void ads7846_GetMsg(GOL_MSG *pMsg)
{
    static SHORT    prevX = -1;
    static SHORT    prevY = -1;

    SHORT           x, y;

    //if(!adcRt || adcRt > 400) return;

    x = TouchGetX();
    y = TouchGetY();

    pMsg->type = TYPE_TOUCHSCREEN;
    pMsg->uiEvent = EVENT_INVALID;

    if(x == -1)
    {
        y = -1;
    }
    else
    {
        if(y == -1)
            x = -1;
    }

    if((prevX == x) && (prevY == y))
    {
        pMsg->uiEvent = EVENT_STILLPRESS;
        pMsg->param1 = x;
        pMsg->param2 = y;
        return;
    }

    if((prevX != -1) || (prevY != -1))
    {
        if((x != -1) && (y != -1))
        {

            // Move
            pMsg->uiEvent = EVENT_MOVE;
        }
        else
        {

            // Released
            pMsg->uiEvent = EVENT_RELEASE;
            pMsg->param1 = prevX;
            pMsg->param2 = prevY;
            prevX = x;
            prevY = y;
            return;
        }
    }
    else
    {
        if((x != -1) && (y != -1))
        {

            // Pressed
            pMsg->uiEvent = EVENT_PRESS;
        }
        else
        {

            // No message
            pMsg->uiEvent = EVENT_INVALID;
        }
    }

    pMsg->param1 = x;
    pMsg->param2 = y;
    prevX = x;
    prevY = y;
}

void __ISR( _INPUT_CAPTURE_5_VECTOR, ipl5) IC5Interrupt( void)
{
    int temp;

    trmbit.bSleepAfficheur = 0;
    SetTimer(TIMER_VEILLE_AFFICHEUR, GetlReg(RNV_DUREE_REVEILLE_AFFICHEUR)*10);
    
    ads7846_Click();

    // this should clean the interrupt flag
    temp = IC5BUF;

    mIC5ClearIntFlag();
}