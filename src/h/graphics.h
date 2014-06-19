
#if !defined(GRAPHICS_H)
#define GRAPHICS_H

#include "main.h"

// Solid Line Style
#define GRAPHICS_SOLID_LINE  0
// Dotted Line Style
#define GRAPHICS_DOTTED_LINE 1
// Dashed Line Style
#define GRAPHICS_DASHED_LINE 4

// Normal Line (thickness is 1 pixel)
#define GRAPHICS_NORMAL_LINE 0
// Thick Line (thickness is 3 pixels)
#define GRAPHICS_THICK_LINE  1

#define GRAPHICS_ORIENT_HOR  0
#define GRAPHICS_ORIENT_VER  1

#define GRAPHICS_IMAGE_NORMAL    1 // Normal image stretch code
#define GRAPHICS_IMAGE_X2    	 2 // Stretched image stretch code

// constants used for circle/arc computation
#define GRAPHICS_SIN45   46341   // sin(45) * 2^16)
#define GRAPHICS_ONEP25  81920   // 1.25 * 2^16


#define GRAPHICS_FLASH_BYTE  char const
#define GRAPHICS_FLASH_WORD  short int const
        
// Memory type enumeration to determine the source of data.
// Used in interpreting bitmap and font from different memory sources.
typedef enum
{
    FLASH   = 0,            // internal flash
    EXTERNAL= 1,            // external memory
    RAM		= 2,			// RAM
    VIDEOBUF= 3             // video buffer
} GRAPHICS_TYPE_MEMORY;

/*********************************************************************
* Overview: Structure describing the font header.
*
*********************************************************************/
typedef struct
{
    BYTE    fontID;     // User assigned value
    BYTE    res1   : 4;         // Reserved for future use (must be set to 0).
    BYTE    orient : 2;         // Orientation
    BYTE    res2   : 2;         // Reserved for future use (must be set to 0).
    WORD    firstChar;  // Character code of first character (e.g. 32).
    WORD    lastChar;   // Character code of last character in font (e.g. 3006).
    BYTE    height;     // Font characters height in pixels.
    BYTE    reserved;   // Reserved for future use (must be set to 0).
} GRAPHICS_FONT_HEADER;

/*********************************************************************
* Overview: Structure for font stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GRAPHICS_TYPE_MEMORY type;       // must be FLASH
    const char  *address;   // font image address
} GRAPHICS_FONT_FLASH;

// Structure describing font glyph entry
typedef struct
{
    BYTE    width;
    BYTE    offsetLSB;
    WORD    offsetMSB;
} GRAPHICS_GLYPH_ENTRY;

/*********************************************************************
* Overview: Structure for bitmap stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GRAPHICS_TYPE_MEMORY type;       // must be FLASH
    GRAPHICS_FLASH_BYTE  *address;   // bitmap image address
} GRAPHICS_BITMAP_FLASH;


#define GRAPHICS_XCHAR   char

extern SHORT	graphics_LineType;
extern BYTE		graphics_lineThickness;
extern SHORT	graphics_cursorX;
extern SHORT	graphics_cursorY;
extern BYTE		graphics_fontOrientation;

#define graphics_SetLineType(lnType) graphics_LineType = lnType;
#define graphics_SetLineThickness(lnThickness)	graphics_lineThickness = lnThickness;
#define graphics_GetX()  graphics_cursorX
#define graphics_GetY()  graphics_cursorY
#define graphics_MoveTo(x, y)	graphics_cursorX = x; \
								graphics_cursorY = y;
#define graphics_SetFontOrientation(orient)  graphics_fontOrientation = orient;
#define graphics_Rectangle(left, top, right, bottom) graphics_Bevel(left, top, right, bottom, 0)
#define graphics_FillRectangle(left, top, right, bottom) graphics_FillBevel(left, top, right, bottom, 0)
#define graphics_Circle(x1, y1, rad) graphics_Bevel(x1, y1, x1, y1, rad);
#define graphics_FillCircle(x1, y1, rad) graphics_FillBevel(x1, y1, x1, y1, rad);

void graphics_Init(void);
void graphics_ClearDevice(void);

WORD graphics_Bar(SHORT left, SHORT top, SHORT right, SHORT bottom);
WORD graphics_Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
WORD graphics_Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad);
WORD graphics_FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad);

void graphics_SetFont(void *font);
WORD graphics_OutChar(GRAPHICS_XCHAR ch);
WORD graphics_OutTextXY(SHORT x, SHORT y, GRAPHICS_XCHAR *textString);
SHORT graphics_GetTextHeight(void *font);
SHORT graphics_GetTextWidth(GRAPHICS_XCHAR *textString, void *font);
WORD graphics_OutText(GRAPHICS_XCHAR *textString);

SHORT graphics_GetImageWidth(void *bitmap);
SHORT graphics_GetImageHeight(void *bitmap);
void graphics_PutImage1BPP(SHORT left, SHORT top, GRAPHICS_FLASH_BYTE *bitmap, BYTE stretch, BYTE invert);
WORD graphics_PutImage(SHORT left, SHORT top, void *bitmap, BYTE stretch, BYTE invert);



#endif
