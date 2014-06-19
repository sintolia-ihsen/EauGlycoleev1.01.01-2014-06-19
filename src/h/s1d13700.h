
#if !defined(S1D13700_H)
#define S1D13700_H

#include "main.h"

#define S1D13700_OFF                0
#define S1D13700_ON                 1
		

#define S1D13700_TRIS_BACKLIGHT		_TRISA7
#define S1D13700_BACKLIGHT			_LATA7

#define S1D13700_POWER				_LATD7
#define S1D13700_E					_LATA1
#define S1D13700_WR					mainbit.bUnused
#define S1D13700_CS					_LATA4
#define S1D13700_RST				_LATA5   //Apenas definir se n?tover resetado previamente(O driver faz o reset)
#define S1D13700_DSF                _LATA6
#define S1D13700_A0					_LATA9

#define S1D13700_TRIS_POWER			_TRISD7
#define S1D13700_TRIS_E				_TRISA1
#define S1D13700_TRIS_WR			mainbit.bUnused
#define S1D13700_TRIS_CS			_TRISA4
#define S1D13700_TRIS_RST			_TRISA5
#define S1D13700_TRIS_DSF   		_TRISA6
#define S1D13700_TRIS_A0			_TRISA9

#define S1D13700_TRIS_DATA			TRISE
#define S1D13700_WR_DATA			LATE
#define S1D13700_RD_DATA			PORTE

#define S1D13700_CMD_SYSTEM          0x40  // General system settings
#define S1D13700_CMD_SLEEP           0x53  // Enter into standy mode
#define S1D13700_CMD_DISP_OFF        0x58  // Turn the display off
#define S1D13700_CMD_DISP_ON         0x59  // Turn the display on
#define S1D13700_CMD_SCROLL          0x44  // Setup text and graphics address regions
#define S1D13700_CMD_CSR_FORM        0x5D  // Set cursor size
#define S1D13700_CMD_CSRDIR_RIGHT    0x4C  // Cursor moves right after write to display memory
#define S1D13700_CMD_CSRDIR_LEFT     0x4D  // Cursor moves left after write to display memory
#define S1D13700_CMD_CSRDIR_UP       0x4E  // Cursor moves up after write to display memory
#define S1D13700_CMD_CSRDIR_DN       0x4F  // Cursor moves down after write to display memory
#define S1D13700_CMD_CGRAM_ADDR      0x5C  // Configure character generator RAM address
#define S1D13700_CMD_HDOT_SCR        0x5A  // Set horizontal scroll rate
#define S1D13700_CMD_OVERLAY         0x5B  // Configure how layers overlay
#define S1D13700_CMD_SET_CSR_ADDR    0x46  // Set the cursor address
#define S1D13700_CMD_GET_CSR_ADDR    0x47  // Read the cursor address
#define S1D13700_CMD_DISPLAY_WRITE   0x42  // Write to display memory
#define S1D13700_CMD_DISPLAY_READ    0x43  // Read from display memory

#define S1D13700_WIDTH         320l
#define S1D13700_HEIGHT        240l
#define S1D13700_CHAR_WIDTH    8
#define S1D13700_CHAR_HEIGHT   16

#define S1D13700_GET_MAX_X()   (S1D13700_WIDTH - 1)
#define S1D13700_GET_MAX_Y()   (S1D13700_HEIGHT - 1)

#define S1D13700_CR (S1D13700_WIDTH/8 - 1)
	
#define S1D13700_GRAPHICS1_ADDR         (0x0000)
#define S1D13700_GRAPHICS1_ADDR_END     (S1D13700_GRAPHICS1_ADDR + ((S1D13700_WIDTH * S1D13700_HEIGHT) / 8))	//9600
#define S1D13700_GRAPHICS2_ADDR         (S1D13700_GRAPHICS1_ADDR_END)											//9600
#define S1D13700_GRAPHICS2_ADDR_END     (S1D13700_GRAPHICS2_ADDR + ((S1D13700_WIDTH * S1D13700_HEIGHT) / 8))	//9600
	
#define S1D13700_WHITE               0x00
#define S1D13700_BLACK               0x01

extern WORD_VAL s1d13700_color;
#define s1d13700_SetColor(color)	s1d13700_color.Val = color
#define s1d13700_GetColor() 		s1d13700_color.Val

extern unsigned char s1d13700_MemImage[9600];

void s1d13700_Init(void);
void s1d13700_Clear(void);
void s1d13700_SetBackLight(BYTE etat);
void s1d13700_PutPixel(SHORT x, SHORT y);
void s1d13700_Reffrache(void);

#endif

