
#if !defined(ADS7846_H)
#define ADS7846_H

    #include "Graphics.h"


#define TP_TRIS_IRQ		_TRISD12
#define TP_TRIS_CLK		_TRISD10
#define TP_TRIS_SDI		_TRISC4
#define TP_TRIS_SDO		_TRISD0
#define TP_TRIS_CS		_TRISD9

#define TP_IRQ			_RD12
#define TP_CLK			_LATD10
#define TP_SDI			_RC4
#define TP_SDO			_LATD0
#define TP_CS			_LATD9


#define ADS7846_AXE_Z1	0
#define ADS7846_AXE_Z2	1
#define ADS7846_AXE_X	2
#define ADS7846_AXE_Y	3

    #if (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V1)

//#define SWAP_X_AND_Y
        #define FLIP_X

//#define FLIP_Y
    #elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V2)

		#include "EEPROM.h"

//#define SWAP_X_AND_Y
//#define FLIP_X
        #define FLIP_Y

    #elif (GRAPHICS_HARDWARE_PLATFORM == GFX_PICTAIL_V3) || (GRAPHICS_HARDWARE_PLATFORM == DA210_DEV_BOARD)

		#include "SST25VF016.h"

//#define SWAP_X_AND_Y
//#define FLIP_X
//#define FLIP_Y
    #endif


// Default calibration values
//    #define XMINCAL 	251  //192
//    #define XMAXCAL 	3845 //3900
//    #define YMINCAL 	291  //192
//    #define YMAXCAL 	3755 //3800

    #define XMINCAL 	340  //192
    #define XMAXCAL 	3950 //3900
    #define YMINCAL 	290  //192
    #define YMAXCAL 	3755 //3800

// Max/Min ADC values for each direction
extern volatile WORD    _calXMin;
extern volatile WORD    _calXMax;
extern volatile WORD    _calYMin;
extern volatile WORD    _calYMax;

    #if (GRAPHICS_HARDWARE_PLATFORM < GFX_PICTAIL_V3)

// Addresses for calibration and version values in EEPROM on Explorer 16
        #define ADDRESS_VERSION (unsigned)0x7FFE
        #define ADDRESS_XMIN    (unsigned)0x7FFC
        #define ADDRESS_XMAX    (unsigned)0x7FFA
        #define ADDRESS_YMIN    (unsigned)0x7FF8
        #define ADDRESS_YMAX    (unsigned)0x7FF6
    #else

// Addresses for calibration and version values in SPI Flash on Graphics PICtail 3 & PIC24FJ256DA210 Development Board.
        #define ADDRESS_VERSION (unsigned long)0xFFFFFFFE
        #define ADDRESS_XMIN    (unsigned long)0xFFFFFFFC
        #define ADDRESS_XMAX    (unsigned long)0xFFFFFFFA
        #define ADDRESS_YMIN    (unsigned long)0xFFFFFFF8
        #define ADDRESS_YMAX    (unsigned long)0xFFFFFFF6
    #endif

// Current ADC values for X and Y channels and potentiometer R6
extern volatile SHORT   adcX;
extern volatile SHORT   adcY;
extern volatile SHORT   adcAct;

extern volatile SHORT   adcX1;
extern volatile SHORT   adcY1;
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
void ads7846_Init(void);

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
SHORT                   TouchGetX(void);

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
SHORT                   TouchGetY(void);

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
SHORT                   TouchActive(void);

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
//void                    ads7846_GetMsg(GOL_MSG *pMsg);

/*********************************************************************
* Function: void TouchCalibration()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Runs the calibration routine.
*
* Note: none
*
********************************************************************/
void                    TouchCalibration(void);

/*********************************************************************
* Function: void TouchStoreCalibration(void)
*
* PreCondition: EEPROMInit() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: stores calibration parameters into EEPROM
*
* Note: none
*
********************************************************************/
void                    TouchStoreCalibration(void);

/*********************************************************************
* Function: void TouchLoadCalibration(void)
*
* PreCondition: EEPROMInit() must be called before
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: loads calibration parameters from EEPROM
*
* Note: none
*
********************************************************************/
void                    TouchLoadCalibration(void);

/*********************************************************************
* Macros: ADCGetX()
*
* PreCondition: none
*
* Input: none
*
* Output: ADC result
*
* Side Effects: none
*
* Overview: returns ADC value for x direction if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
    #define ADCGetX()   adcX
    #define ADCGetX1()   adcX1

/*********************************************************************
* Macros: ADCGetY()
*
* PreCondition: none
*
* Input: none
*
* Output: ADC result
*
* Side Effects: none
*
* Overview: returns ADC value for y direction if touch screen is pressed
*           and -1 if not
*
* Note: none
*
********************************************************************/
    #define ADCGetY()   adcY
    #define ADCGetY1()   adcY1

/*********************************************************************
* Macros: ADCGetPot()
*
* PreCondition: none
*
* Input: none
*
* Output: ADC result
*
* Side Effects: none
*
* Overview: returns ADC value for potentiometer
*
* Note: none
*
********************************************************************/
    #define ADCGetPot() adcPot

/*********************************************************************
* Overview: This structure defines the types of input devices
*			used in the library.
*
*********************************************************************/
typedef enum
{
    TYPE_UNKNOWN    = 0,            // Unknown device.
    TYPE_KEYBOARD,                  // Keyboard.
    TYPE_TOUCHSCREEN,               // Touchscreen.
    TYPE_MOUSE,                     // Mouse.
    TYPE_TIMER,                     // Timer.
    TYPE_SYSTEM                     // System Messages.
} INPUT_DEVICE_TYPE;

/*********************************************************************
* Overview: This structure defines the types of GOL message events
*  			used in the library.
*
*********************************************************************/
typedef enum
{
    EVENT_INVALID   = 0,            // An invalid event.
    EVENT_MOVE,                     // A move event.
    EVENT_PRESS,                    // A press event.
    EVENT_STILLPRESS,               // A continuous press event.
    EVENT_RELEASE,                  // A release event.
    EVENT_KEYSCAN,                  // A keyscan event, parameters has the object ID keyboard scan code.
    EVENT_CHARCODE,                 // Character code is presented at the parameters.
    EVENT_SET,                      // A generic set event.
    EVENT_SET_STATE,                // A set state event.
    EVENT_CLR_STATE                 // A clear state event.
} INPUT_DEVICE_EVENT;

/*********************************************************************
* Overview: This structure defines the GOL message used in the library.
*	- The types must be one of the INPUT_DEVICE_TYPE:
*		- TYPE_UNKNOWN
*		- TYPE_KEYBOARD
*		- TYPE_TOUCHSCREEN
*		- TYPE_MOUSE
*	- uiEvent must be one of the INPUT_DEVICE_EVENT.
*   	- for touch screen:
*			- EVENT_INVALID
*			- EVENT_MOVE
*			- EVENT_PRESS
*			- EVENT_STILLPRESS
*			- EVENT_RELEASE
*   	- for keyboard:
*			- EVENT_KEYSCAN (param2 contains scan code)
*   		- EVENT_KEYCODE (param2 contains character code)
* 	- param1:
*	 	- for touch screen is the x position
*	    - for keyboard ID of object receiving the message
*	- param2
*	 	- for touch screen y position
*       - for keyboard scan or key code
*
*********************************************************************/
typedef struct
{
    BYTE    type;                   // Type of input device.
    BYTE    uiEvent;                // The generic events for input device.
    SHORT   param1;                 // Parameter 1 meaning is dependent on the type of input device.
    SHORT   param2;                 // Parameter 2 meaning is dependent on the type of input device.
} GOL_MSG;

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
void ads7846_GetMsg(GOL_MSG *pMsg);
void ads7846_Click(void);
void ads7846_Gestion(void);

#endif //ADS7846_H
