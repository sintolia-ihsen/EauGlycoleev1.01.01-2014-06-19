
#include "main.h"
#include "graphics.h"
#include "s1d13700.h"


SHORT	graphics_LineType;
BYTE	graphics_lineThickness;
SHORT	graphics_cursorX;
SHORT	graphics_cursorY;
BYTE    graphics_fontOrientation;

void    *graphics_font;
// First and last characters in the font
WORD    graphics_fontFirstChar; // First character in the font table.
WORD    graphics_fontLastChar;  // Last character in the font table.
// Installed font height
SHORT   graphics_fontHeight;

void graphics_ClearDevice(void)
{
    //while(graphics_Bar(0, 0, GetMaxX(), GetMaxY()) == 0);
    s1d13700_Clear();
    graphics_MoveTo(0, 0);
}


/*********************************************************************
* Function:  void InitGraph(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: initializes LCD controller,
*           sets cursor position to upper left corner,
*           sets active and visual pages to page 0,
*           clears active page with BLACK,
*           sets color to WHITE,
*           disables clipping
*
* Note: none
*
********************************************************************/

void graphics_Init(void)
{

    // Current line type
    graphics_SetLineType(GRAPHICS_SOLID_LINE);

    // Current line thickness
    graphics_SetLineThickness(GRAPHICS_NORMAL_LINE);

    // Current cursor coordinates to 0,0
    graphics_MoveTo(0, 0);

    // Reset device
	s1d13700_Init();

    // Set color to BLACK
    s1d13700_SetColor(S1D13700_WHITE);

    // Clear screen
    graphics_ClearDevice();

    // Set color to WHITE
    s1d13700_SetColor(S1D13700_WHITE);

    // Set font orientation
    graphics_SetFontOrientation(GRAPHICS_ORIENT_HOR);
}


/*********************************************************************
* Function: WORD graphics_Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
*
* PreCondition: none
*
* Input: x1,y1 - starting coordinates, x2,y2 - ending coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws line
*
* Note: none
*
********************************************************************/

WORD graphics_Line(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
    SHORT   deltaX, deltaY;
    SHORT   error, stepErrorLT, stepErrorGE;
    SHORT   stepX, stepY;
    SHORT   steep;
    SHORT   temp;
    SHORT   style, type;


    // Move cursor
    graphics_MoveTo(x2, y2);

    if(x1 == x2)
    {
        if(y1 > y2)
        {
            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = y1; temp < y2 + 1; temp++)
        {
            if((++style) == graphics_LineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                s1d13700_PutPixel(x1, temp);
                if(graphics_lineThickness)
                {
                    s1d13700_PutPixel(x1 + 1, temp);
                    s1d13700_PutPixel(x1 - 1, temp);
                }
            }
        }

        return (1);
    }

    if(y1 == y2)
    {
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }

        style = 0;
        type = 1;
        for(temp = x1; temp < x2 + 1; temp++)
        {
            if((++style) == graphics_LineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                s1d13700_PutPixel(temp, y1);
                if(graphics_lineThickness)
                {
                    s1d13700_PutPixel(temp, y1 + 1);
                    s1d13700_PutPixel(temp, y1 - 1);
                }
            }
        }

        return (1);
    }

    stepX = 0;
    deltaX = x2 - x1;
    if(deltaX < 0)
    {
        deltaX = -deltaX;
        --stepX;
    }
    else
    {
        ++stepX;
    }

    stepY = 0;
    deltaY = y2 - y1;
    if(deltaY < 0)
    {
        deltaY = -deltaY;
        --stepY;
    }
    else
    {
        ++stepY;
    }

    steep = 0;
    if(deltaX < deltaY)
    {
        ++steep;
        temp = deltaX;
        deltaX = deltaY;
        deltaY = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = stepX;
        stepX = stepY;
        stepY = temp;
        s1d13700_PutPixel(y1, x1);
    }
    else
    {
        s1d13700_PutPixel(x1, y1);
    }

    // If the current error greater or equal zero
    stepErrorGE = deltaX << 1;

    // If the current error less than zero
    stepErrorLT = deltaY << 1;

    // Error for the first pixel
    error = stepErrorLT - deltaX;

    style = 0;
    type = 1;

    while(--deltaX >= 0)
    {
        if(error >= 0)
        {
            y1 += stepY;
            error -= stepErrorGE;
        }

        x1 += stepX;
        error += stepErrorLT;

        if((++style) == graphics_LineType)
        {
            type ^= 1;
            style = 0;
        }

        if(type)
        {
            if(steep)
            {
                s1d13700_PutPixel(y1, x1);
                if(graphics_lineThickness)
                {
                    s1d13700_PutPixel(y1 + 1, x1);
                    s1d13700_PutPixel(y1 - 1, x1);
                }
            }
            else
            {
                s1d13700_PutPixel(x1, y1);
                if(graphics_lineThickness)
                {
                    s1d13700_PutPixel(x1, y1 + 1);
                    s1d13700_PutPixel(x1, y1 - 1);
                }
            }
        }
    }   // end of while

    return (1);
}

/*********************************************************************
* Function: WORD Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* PreCondition: None
*
* Input: x1, y1 - coordinate position of the upper left center of the 
* 				  circle that draws the rounded corners,
*		 x2, y2 - coordinate position of the lower right center of the 
* 				  circle that draws the rounded corners,
*        rad - defines the redius of the circle,
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Overview: Draws a beveled figure on the screen. 
*           For a pure circular object x1 = x2 and y1 = y2. 
*           For a rectangular object radius = 0.
*
* Note: none
*
********************************************************************/
WORD graphics_Bevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
{
    SHORT       style, type, xLimit, xPos, yPos, error;
    DWORD_VAL   temp;

    
    temp.Val = GRAPHICS_SIN45 * rad;
    xLimit = temp.w[1] + 1;
    temp.Val = (DWORD) (GRAPHICS_ONEP25 - ((LONG) rad << 16));
    error = (SHORT) (temp.w[1]);
    yPos = rad;

    style = 0;
    type = 1;

    if(rad)
    {
        for(xPos = 0; xPos <= xLimit; xPos++)
        {
            if((++style) == graphics_LineType)
            {
                type ^= 1;
                style = 0;
            }

            if(type)
            {
                s1d13700_PutPixel(x2 + xPos, y1 - yPos);         // 1st quadrant
                s1d13700_PutPixel(x2 + yPos, y1 - xPos);
                s1d13700_PutPixel(x2 + xPos, y2 + yPos);         // 2nd quadrant
                s1d13700_PutPixel(x2 + yPos, y2 + xPos);
                s1d13700_PutPixel(x1 - xPos, y2 + yPos);         // 3rd quadrant
                s1d13700_PutPixel(x1 - yPos, y2 + xPos);
                s1d13700_PutPixel(x1 - yPos, y1 - xPos);         // 4th quadrant
                s1d13700_PutPixel(x1 - xPos, y1 - yPos);

                if(graphics_lineThickness)
                {
                    s1d13700_PutPixel(x2 + xPos, y1 - yPos - 1); // 1st quadrant
                    s1d13700_PutPixel(x2 + xPos, y1 - yPos + 1);
                    s1d13700_PutPixel(x2 + yPos + 1, y1 - xPos);
                    s1d13700_PutPixel(x2 + yPos - 1, y1 - xPos);
                    s1d13700_PutPixel(x2 + xPos, y2 + yPos - 1); // 2nd quadrant
                    s1d13700_PutPixel(x2 + xPos, y2 + yPos + 1);
                    s1d13700_PutPixel(x2 + yPos + 1, y2 + xPos);
                    s1d13700_PutPixel(x2 + yPos - 1, y2 + xPos);
                    s1d13700_PutPixel(x1 - xPos, y2 + yPos - 1); // 3rd quadrant
                    s1d13700_PutPixel(x1 - xPos, y2 + yPos + 1);
                    s1d13700_PutPixel(x1 - yPos + 1, y2 + xPos);
                    s1d13700_PutPixel(x1 - yPos - 1, y2 + xPos);
                    s1d13700_PutPixel(x1 - yPos + 1, y1 - xPos); // 4th quadrant
                    s1d13700_PutPixel(x1 - yPos - 1, y1 - xPos);
                    s1d13700_PutPixel(x1 - xPos, y1 - yPos + 1);
                    s1d13700_PutPixel(x1 - xPos, y1 - yPos - 1);
                }
            }

            if(error > 0)
            {
                yPos--;
                error += 5 + ((xPos - yPos) << 1);
            }
            else
                error += 3 + (xPos << 1);
        }
    }
    // Must use lines here since this can also be used to draw focus of round buttons
    if(x2 - x1)
    {
        while(!graphics_Line(x1, y1 - rad, x2, y1 - rad));
        // draw top
    }

    if(y2 - y1)
    {
        while(!graphics_Line(x1 - rad, y1, x1 - rad, y2));
        // draw left
    }

    if((x2 - x1) || (y2 - y1))
    {
        while(!graphics_Line(x2 + rad, y1, x2 + rad, y2));
        // draw right
        while(!graphics_Line(x1, y2 + rad, x2, y2 + rad));
        // draw bottom
    }

    return (1);
}

/*********************************************************************
* Function: WORD FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
*
* PreCondition: None
*
* Input: x1, y1 - coordinate position of the upper left center of the 
* 				  circle that draws the rounded corners,
*		 x2, y2 - coordinate position of the lower right center of the 
* 				  circle that draws the rounded corners,
*        rad - defines the redius of the circle,
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Overview: Draws a filled beveled figure on the screen. 
*           For a filled circular object x1 = x2 and y1 = y2. 
*           For a filled rectangular object radius = 0.
*
* Note: none
*
********************************************************************/
WORD graphics_FillBevel(SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad)
{
    #ifndef USE_NONBLOCKING_CONFIG

    SHORT       yLimit, xPos, yPos, err;
    SHORT       xCur, yCur, yNew;
    DWORD_VAL   temp;

    // note that octants here is defined as:
    // from yPos=-radius, xPos=0 in the clockwise direction octant 1 to 8 are labeled
    // assumes an origin at 0,0. Quadrants are defined in the same manner
    if(rad)
    {
        temp.Val = GRAPHICS_SIN45 * rad;
        yLimit = temp.w[1];
        temp.Val = (DWORD) (GRAPHICS_ONEP25 - ((LONG) rad << 16));
        err = (SHORT) (temp.w[1]);
        xPos = rad;
        yPos = 0;

        xCur = xPos;
        yCur = yPos;
        yNew = yPos;

        while(yPos <= yLimit)
        {

            // Drawing of the rounded panel is done only when there is a change in the
            // x direction. Bars are drawn to be efficient.
            // detect changes in the x position. Every change will mean a bar will be drawn
            // to cover the previous area. y1New records the last position of y before the
            // change in x position.
            // y1New records the last y position
            yNew = yPos;

            if(err > 0)
            {
                xPos--;
                err += 5 + ((yPos - xPos) << 1);
            }
            else
                err += 3 + (yPos << 1);
            yPos++;

            if(xCur != xPos)
            {

                // 6th octant to 3rd octant
                graphics_Bar(x1 - xCur, y2 + yCur, x2 + xCur, y2 + yNew);

                // 5th octant to 4th octant
                graphics_Bar(x1 - yNew, y2 + xPos, x2 + yNew, y2 + xCur);

                // 8th octant to 1st octant
                graphics_Bar(x1 - yNew, y1 - xCur, x2 + yNew, y1 - xPos);

                // 7th octant to 2nd octant
                graphics_Bar(x1 - xCur, y1 - yNew, x2 + xCur, y1 - yCur);

                // update current values
                xCur = xPos;
                yCur = yPos;
            }
        }
    }

    // this covers both filled rounded object and filled rectangle.
    if((x2 - x1) || (y2 - y1))
        graphics_Bar(x1 - rad, y1, x2 + rad, y2);
    return (1);
    #else

    typedef enum
    {
        BEGIN,
        CHECK,
        Q8TOQ1,
        Q7TOQ2,
        Q6TOQ3,
        Q5TOQ4,
        WAITFORDONE,
        FACE
    } FILLCIRCLE_STATES;

    DWORD_VAL temp;
    static LONG err;
    static SHORT yLimit, xPos, yPos;
    static SHORT xCur, yCur, yNew;

    static FILLCIRCLE_STATES state = BEGIN;

    while(1)
    {
        if(IsDeviceBusy())
            return (0);
        switch(state)
        {
            case BEGIN:
                if(!rad)
                {   // no radius object is a filled rectangle
                    state = FACE;
                    break;
                }

                // compute variables
                temp.Val = GRAPHICS_SIN45 * rad;
                yLimit = temp.w[1];
                temp.Val = (DWORD) (GRAPHICS_ONEP25 - ((LONG) rad << 16));
                err = (SHORT) (temp.w[1]);
                xPos = rad;
                yPos = 0;
                xCur = xPos;
                yCur = yPos;
                yNew = yPos;
                state = CHECK;

            case CHECK:
                bevel_fill_check : if(yPos > yLimit)
                {
                    state = FACE;
                    break;
                }

                // y1New records the last y position
                yNew = yPos;

                // calculate the next value of x and y
                if(err > 0)
                {
                    xPos--;
                    err += 5 + ((yPos - xPos) << 1);
                }
                else
                    err += 3 + (yPos << 1);
                yPos++;
                state = Q6TOQ3;

            case Q6TOQ3:
                if(xCur != xPos)
                {

                    // 6th octant to 3rd octant
                    if(graphics_Bar(x1 - xCur, y2 + yCur, x2 + xCur, y2 + yNew) == 0)
                        return (0);
                    state = Q5TOQ4;
                    break;
                }

                state = CHECK;
                goto bevel_fill_check;

            case Q5TOQ4:

                // 5th octant to 4th octant
                if(graphics_Bar(x1 - yNew, y2 + xPos, x2 + yNew, y2 + xCur) == 0)
                    return (0);
                state = Q8TOQ1;
                break;

            case Q8TOQ1:

                // 8th octant to 1st octant
                if(graphics_Bar(x1 - yNew, y1 - xCur, x2 + yNew, y1 - xPos) == 0)
                    return (0);
                state = Q7TOQ2;
                break;

            case Q7TOQ2:

                // 7th octant to 2nd octant
                if(graphics_Bar(x1 - xCur, y1 - yNew, x2 + xCur, y1 - yCur) == 0)
                    return (0);

                // update current values
                xCur = xPos;
                yCur = yPos;
                state = CHECK;
                break;

            case FACE:
                if((x2 - x1) || (y2 - y1))
                {
                    if(graphics_Bar(x1 - rad, y1, x2 + rad, y2) == 0)
                        return (0);
                    state = WAITFORDONE;
                }
                else
                {
                    state = BEGIN;
                    return (1);
                }

            case WAITFORDONE:
                if(IsDeviceBusy())
                    return (0);
                state = BEGIN;
                return (1);
        }           // end of switch
    }               // end of while
    #endif // end of USE_NONBLOCKING_CONFIG
}

/*********************************************************************
* Function: WORD graphics_Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/

WORD graphics_Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
    SHORT   x, y;

    for(y = top; y < bottom + 1; y++)
        for(x = left; x < right + 1; x++)
            s1d13700_PutPixel(x, y);

    return (1);
}


/*********************************************************************
* Function: void graphics_SetFont(void* font)
*
* PreCondition: none
*
* Input: pointer to the font image
*
* Output: none
*
* Side Effects: none
*
* Overview: defines current font
*
* Note: none
*
********************************************************************/
void graphics_SetFont(void *font)
{
    GRAPHICS_FONT_HEADER *pHeader;


        
    graphics_font = font;
    switch(*((SHORT *)font))
    {

        case FLASH:
            pHeader = (GRAPHICS_FONT_HEADER *) ((GRAPHICS_FONT_FLASH *)font)->address;
            break;                 

        default:
            return;
    }

    graphics_fontFirstChar = pHeader->firstChar;
    graphics_fontLastChar = pHeader->lastChar;
    graphics_fontHeight = pHeader->height;
}

/*********************************************************************
* Function: WORD OutText(GRAPHICS_XCHAR* textString)
*
* PreCondition: none
*
* Input: textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from current position
*
* Note: none
*
********************************************************************/
WORD graphics_OutText(GRAPHICS_XCHAR *textString)
{
    GRAPHICS_XCHAR       ch;
    static WORD counter = 0;

    while((unsigned GRAPHICS_XCHAR)(ch = *(textString + counter)) > (unsigned GRAPHICS_XCHAR)15)
    {
        if(graphics_OutChar(ch) == 0)
            return (0);
        counter++;
    }

    counter = 0;
    return (1);
}

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* PreCondition: none
*
* Input: x,y - starting coordinates, textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from x,y position
*
* Note: none
*
********************************************************************/
WORD graphics_OutTextXY(SHORT x, SHORT y, GRAPHICS_XCHAR *textString)
{
    static BYTE start = 1;

    if(start)
    {
        graphics_MoveTo(x, y);
        start = 0;
    }

    if(graphics_OutText(textString) == 0)
    {
        return (0);
    }
    else
    {
        start = 1;
        return (1);
    }
}

/*********************************************************************
* Function: WORD OutChar(GRAPHICS_XCHAR ch)
*
* PreCondition: none
*
* Input: character code
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs a character
*
* Note: none
*
********************************************************************/
WORD graphics_OutChar(GRAPHICS_XCHAR ch)
{

    GRAPHICS_GLYPH_ENTRY *pChTable = NULL;
    BYTE        *pChImage = NULL;

    SHORT       chWidth = 0;
    SHORT       xCnt, yCnt, x = 0, y;
    BYTE        temp = 0, mask;

    if((unsigned GRAPHICS_XCHAR)ch < (unsigned GRAPHICS_XCHAR)graphics_fontFirstChar)
        return (-1);
    if((unsigned GRAPHICS_XCHAR)ch > (unsigned GRAPHICS_XCHAR)graphics_fontLastChar)
        return (-1);

    switch(*((SHORT *)graphics_font))
    {
		case FLASH:
       	
            pChTable = (GRAPHICS_GLYPH_ENTRY *) (((GRAPHICS_FONT_FLASH *)graphics_font)->address + sizeof(GRAPHICS_FONT_HEADER)) + ((unsigned GRAPHICS_XCHAR)ch - (unsigned GRAPHICS_XCHAR)graphics_fontFirstChar);

            pChImage = (BYTE *) (((GRAPHICS_FONT_FLASH *)graphics_font)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);

            chWidth = pChTable->width;

            break;

        default:
            break;
    }

    if(graphics_fontOrientation == GRAPHICS_ORIENT_HOR)
    {
        y = graphics_GetY();
        for(yCnt = 0; yCnt < graphics_fontHeight; yCnt++)
        {
            x = graphics_GetX();
            mask = 0;
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01;
                }

                if(temp & mask)
                {
                    s1d13700_PutPixel(x, y);
                }

                x++;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        graphics_cursorX = x;
    }
    else
    {
        y = graphics_GetX();
        for(yCnt = 0; yCnt < graphics_fontHeight; yCnt++)
        {
            x = graphics_GetY();
            mask = 0; 
            for(xCnt = 0; xCnt < chWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *pChImage++;
                    mask = 0x01; 
                }

                if(temp & mask)
                {
                    s1d13700_PutPixel(y, x);
                }

                x--;
                mask <<= 1;
            }

            y++;
        }

        // move cursor
        graphics_cursorY = x;
    }

    return (1);
}

/*********************************************************************
* Function: SHORT GetTextWidth(GRAPHICS_XCHAR* textString, void* font)
*
* PreCondition: none
*
* Input: textString - pointer to the text string,
*        font - pointer to the font
*
* Output: text width in pixels
*
* Side Effects: none
*
* Overview: returns text width for the font
*
* Note: none
*
********************************************************************/
SHORT graphics_GetTextWidth(GRAPHICS_XCHAR *textString, void *font)
{
    GRAPHICS_GLYPH_ENTRY *pChTable;
    GRAPHICS_FONT_HEADER *pHeader;
    SHORT       textWidth;

    //SHORT        temp;
    GRAPHICS_XCHAR       ch;
    GRAPHICS_XCHAR       fontFirstChar;
    GRAPHICS_XCHAR       fontLastChar;

    switch(*((SHORT *)font))
    {

        case FLASH:
            pHeader = (GRAPHICS_FONT_HEADER *) ((GRAPHICS_FONT_FLASH *)font)->address;
            fontFirstChar = pHeader->firstChar;
            fontLastChar = pHeader->lastChar;
            pChTable = (GRAPHICS_GLYPH_ENTRY *) (pHeader + 1);
            textWidth = 0;
            while((unsigned GRAPHICS_XCHAR)15 < (unsigned GRAPHICS_XCHAR)(ch = *textString++))
            {
                if((unsigned GRAPHICS_XCHAR)ch < (unsigned GRAPHICS_XCHAR)fontFirstChar)
                    continue;
                if((unsigned GRAPHICS_XCHAR)ch > (unsigned GRAPHICS_XCHAR)fontLastChar)
                    continue;
                textWidth += (pChTable + ((unsigned GRAPHICS_XCHAR)ch - (unsigned GRAPHICS_XCHAR)fontFirstChar))->width;
            }

            return (textWidth);
               

        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetTextHeight(void* font)
*
* PreCondition: none
*
* Input: pointer to the font
*
* Output: character height in pixels
*
* Side Effects: none
*
* Overview: returns characters height for the font
*
* Note: none
*
********************************************************************/
SHORT graphics_GetTextHeight(void *font)
{
    switch(*((SHORT *)font))
    {

        case FLASH:
            return ((GRAPHICS_FONT_HEADER *) ((GRAPHICS_FONT_FLASH *)font)->address)->height;
  
        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetImageWidth(void* bitmap)
*
* PreCondition: none
*
* Input: bitmap - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image width
*
* Note: none
*
********************************************************************/
SHORT graphics_GetImageWidth(void *bitmap)
{
    switch(*((SHORT *)bitmap))
    {
        case FLASH:
            return (*((GRAPHICS_FLASH_WORD *) ((GRAPHICS_BITMAP_FLASH *)bitmap)->address + 2));
 
        default:
            return (0);
    }
}

/*********************************************************************
* Function: SHORT GetImageHeight(void* bitmap)
*
* PreCondition: none
*
* Input: bitmap - image pointer
*
* Output: none
*
* Side Effects: none
*
* Overview: returns image height
*
* Note: none
*
********************************************************************/
SHORT graphics_GetImageHeight(void *bitmap)
{

    switch(*((SHORT *)bitmap))
    {
        case FLASH:
            return (*((GRAPHICS_FLASH_WORD *) ((GRAPHICS_BITMAP_FLASH *)bitmap)->address + 1));
        
        default:
            return (0);
    }
}

/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, GRAPHICS_FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void graphics_PutImage1BPP(SHORT left, SHORT top, GRAPHICS_FLASH_BYTE *bitmap, BYTE stretch, BYTE invert)
{
    register GRAPHICS_FLASH_BYTE *flashAddress;
    register GRAPHICS_FLASH_BYTE *tempFlashAddress;
    BYTE                temp = 0;
    WORD                sizeX, sizeY;
    WORD                x, y;
    WORD                xc, yc;
    BYTE                stretchX, stretchY;
    WORD                pallete[2];
    BYTE                mask;

    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Read image size
    sizeY = *((GRAPHICS_FLASH_WORD *)flashAddress);
    flashAddress += 2;
    sizeX = *((GRAPHICS_FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[0] = *((GRAPHICS_FLASH_WORD *)flashAddress);
    flashAddress += 2;
    pallete[1] = *((GRAPHICS_FLASH_WORD *)flashAddress);
    flashAddress += 2;

    yc = top;
    for(y = 0; y < sizeY; y++)
    {
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY < stretch; stretchY++)
        {
            flashAddress = tempFlashAddress;
            mask = 0;
            xc = left;
            for(x = 0; x < sizeX; x++)
            {

                // Read 8 pixels from flash
                if(mask == 0)
                {
                    temp = *flashAddress;
                    flashAddress++;
                    mask = 0x80;
                }

                // Set color
                if(mask & temp)
                {
	                // Set color
	                #ifdef USE_PALETTE
	                    s1d13700_SetColor(1);
	                #else
	                    s1d13700_SetColor(pallete[invert?0:1]);
	                #endif                
                }
                else
                {
	                // Set color
	                #ifdef USE_PALETTE
	                    s1d13700_SetColor(0);
	                #else
						s1d13700_SetColor(pallete[invert?1:0]);
	                #endif
                }

                // Write pixel to screen
                for(stretchX = 0; stretchX < stretch; stretchX++)
                {
                    s1d13700_PutPixel(xc++, yc);
                }

                // Shift to the next pixel
                mask >>= 1;
            }

            yc++;
        }
    }
}


/*********************************************************************
* Function: WORD PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the image is not yet completely drawn.
*         - Returns 1 when the image is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
WORD graphics_PutImage(SHORT left, SHORT top, void *bitmap, BYTE stretch, BYTE invert)
{
    GRAPHICS_FLASH_BYTE  *flashAddress;
	BYTE        colorDepth;
    WORD        colorTemp;


    // Save current color
    colorTemp = s1d13700_GetColor();

    switch(*((SHORT *)bitmap))
    {
        case FLASH:

            // Image address
            flashAddress = ((GRAPHICS_BITMAP_FLASH *)bitmap)->address;

            // Read color depth
            colorDepth = *(flashAddress + 1);

            // Draw picture
            switch(colorDepth)
            {
                case 1:     graphics_PutImage1BPP(left, top, flashAddress, stretch, invert); break; 
     
                default:    break;
            }

            break;
        default:
            break;
    }

    // Restore current color
    s1d13700_SetColor(colorTemp);
    return (1);
}
