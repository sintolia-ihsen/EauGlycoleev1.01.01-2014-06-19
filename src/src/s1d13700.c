#include "main.h"
#include "s1d13700.h"


unsigned char s1d13700_MemImage[9600];
unsigned char s1d13700_EtatMemImage[1200];
WORD_VAL s1d13700_color;

int CptMax = 0x00;

void Delay10us()
{
//	int Cpt;
//	for(Cpt = 0; Cpt < CptMax; Cpt++)
//	Nop();
}

// Purpose:    Send an 8 bit command
// Inputs:     The command to send
void s1d13700_SendCMD(BYTE cmd)
{
	S1D13700_TRIS_DATA &= 0xFFFFFF00;
	
    //S1D13700_WR = 0;
    S1D13700_A0 = 1;
    S1D13700_CS = 0;
    Delay10us();
    S1D13700_E = 1;
    Delay10us();

    S1D13700_WR_DATA = cmd;
    Delay10us();
    S1D13700_E = 0;
    Delay10us();
    //S1D13700_WR = 1;
    S1D13700_CS = 1;
    S1D13700_E = 1;
}

// Purpose:    Write a BYTE of data
// Inputs:     The BYTE of data to write
void s1d13700_SendByte(BYTE data)
{
   	S1D13700_TRIS_DATA &= 0xFFFFFF00;
   	
    //S1D13700_WR = 0;
    S1D13700_A0 = 0;
    S1D13700_CS = 0;
	Delay10us();
    S1D13700_E = 1;
	Delay10us();
    
    S1D13700_WR_DATA = data;
	Delay10us();
    S1D13700_E = 0;
    Delay10us();
    //S1D13700_WR = 1;
    S1D13700_CS = 1;
    S1D13700_E = 1;   
}

// Purpose:    Set the cursor address
// Inputs:     A 16 bit integer containing the new cursor address
void s1d13700_setCursorAddress(WORD addr)
{
	WORD_VAL IntConvert;
	IntConvert.Val = addr;
   	s1d13700_SendCMD(S1D13700_CMD_SET_CSR_ADDR);
   	s1d13700_SendByte(IntConvert.byte.LB);
	s1d13700_SendByte(IntConvert.byte.HB);
}

void s1d13700_OFF()
{ 
	S1D13700_WR_DATA &= 0xFFFFFF00;
	S1D13700_RD_DATA &= 0xFFFFFF00;

	S1D13700_BACKLIGHT = 0;
	S1D13700_RST = 0;
	S1D13700_E = 0;
	S1D13700_WR = 0;
	S1D13700_CS = 0;
	S1D13700_A0 = 0;
	S1D13700_DSF = 0;

	S1D13700_TRIS_DATA &= 0xFFFFFFFF;
	S1D13700_TRIS_BACKLIGHT = 1;
	S1D13700_TRIS_RST = 1;
	S1D13700_TRIS_E = 1;
	S1D13700_TRIS_WR = 1;
	S1D13700_TRIS_CS = 1;
	S1D13700_TRIS_A0 = 1;
	S1D13700_TRIS_DSF = 1;

    S1D13700_POWER = 0;
}

void s1d13700_Init(void)
{ 
	DWORD i;
	WORD_VAL IntConvert;
	
	S1D13700_POWER = 0;
	
	S1D13700_TRIS_BACKLIGHT = 0;
	S1D13700_TRIS_POWER = 0;
	S1D13700_TRIS_RST = 0;
	S1D13700_TRIS_E = 0;
	S1D13700_TRIS_WR = 0;
	S1D13700_TRIS_CS = 0;
	S1D13700_TRIS_A0 = 0;
	S1D13700_TRIS_DSF = 0;
	
	S1D13700_POWER = 1;

	S1D13700_CS = 0;
    S1D13700_DSF = 0;
	S1D13700_E = 0;
	S1D13700_WR = 0;

	S1D13700_A0 = 0;
	
	S1D13700_RST = 0;
	
	for(i = 0; i < 0xFFFFF; i++)
	{
		Nop();
	}
	
	S1D13700_RST = 1;
	
	for(i = 0; i < 0xFFFFF; i++)
	{
		Nop();
	}

	s1d13700_SendCMD(S1D13700_CMD_DISP_OFF);           // Turn the display off
	s1d13700_SendByte(0x56);
	
	// Setup the system
	s1d13700_SendCMD(S1D13700_CMD_SYSTEM);          // Setup the system
   	s1d13700_SendByte(0x30);                    // No offset
   	s1d13700_SendByte(0x7F + S1D13700_CHAR_WIDTH);  // Set character width
   	s1d13700_SendByte(S1D13700_CHAR_HEIGHT - 1);    // Set character height
   	s1d13700_SendByte(S1D13700_CR);                 // Display line address range
   	s1d13700_SendByte(0x2F);                    // TC/R
   	s1d13700_SendByte(S1D13700_HEIGHT - 1);         // Number of lines per frame
   	s1d13700_SendByte(S1D13700_CR + 1);             // Horizontal address range LSB (APL)
   	s1d13700_SendByte((S1D13700_CR + 1) / 0xFF);    // Horizontal address range MSB (APH)


	// Currently setup for a text and graphics layer
	s1d13700_SendCMD(S1D13700_CMD_SCROLL);          // Setup scrolling
	
	IntConvert.Val= S1D13700_GRAPHICS1_ADDR;
	s1d13700_SendByte(IntConvert.byte.LB);     		// SAD1L
	s1d13700_SendByte(IntConvert.byte.HB);			// SAD1H
	s1d13700_SendByte(S1D13700_HEIGHT - 1);			// SL1
	
//	IntConvert.Val = S1D13700_GRAPHICS2_ADDR;
//	s1d13700_SendByte(IntConvert.byte.LB);     		// SAD2L
//	s1d13700_SendByte(IntConvert.byte.HB);			// SAD2H
//	s1d13700_SendByte(S1D13700_HEIGHT - 1);         // SL2
	s1d13700_SendByte(0x00);     					// SAD2L
	s1d13700_SendByte(0x00);						// SAD2H
	s1d13700_SendByte(0x00);         				// SL2
	s1d13700_SendByte(0x00);                        // SAD3L
	s1d13700_SendByte(0x00);                        // SAD3H
	s1d13700_SendByte(0x00);                        // SAD4L
	s1d13700_SendByte(0x00);                        // SAD4H


	// Currently setup for a single graphics layer
	s1d13700_SendCMD(0x5B);              // Text / graphic overlay mode
	s1d13700_SendByte(0x04);             // Area 1 graphics, others graphics
	
	//hDotScr
	s1d13700_SendCMD(0x5A);                                
	s1d13700_SendByte(0x00);
	
	
	// Purpose:    Set the direction the cursor moves after
	//             writing to dispaly memory
	// Inputs:     Use one of the following to set the direction:
	//             S1D13700_CMD_CSRDIR_RIGHT
	//             S1D13700_CMD_CSRDIR_LEFT
	//             S1D13700_CMD_CSRDIR_UP
	//             S1D13700_CMD_CSRDIR_DOWN
	s1d13700_SendCMD(S1D13700_CMD_CSRDIR_RIGHT);
	
	// Clear layer graphics
	s1d13700_setCursorAddress(S1D13700_GRAPHICS1_ADDR);
	s1d13700_SendCMD(S1D13700_CMD_DISPLAY_WRITE);
   
	for(i = S1D13700_GRAPHICS1_ADDR; i < S1D13700_GRAPHICS1_ADDR_END; ++i)
	{
		s1d13700_SendByte(0x00);
	}

	// Clear layer text
	s1d13700_setCursorAddress(S1D13700_GRAPHICS2_ADDR);
	s1d13700_SendCMD(S1D13700_CMD_DISPLAY_WRITE);
	
	for(i = S1D13700_GRAPHICS2_ADDR; i < S1D13700_GRAPHICS2_ADDR_END; ++i)
	{
	   s1d13700_SendByte(0x00);
	}
	
	s1d13700_setCursorAddress(0);
	
	// Purpose:    Set the size of the cursor
	// Inputs:     1) The width in pixels - 1  Valid numbers: (0 - 15)
	//             2) The height in pixels - 1 Valid numbers: (1 - 15)
	s1d13700_SendCMD(S1D13700_CMD_CSR_FORM);             // Cursor form and size
	s1d13700_SendByte(0);
	s1d13700_SendByte(0x80 + 0);

	s1d13700_SendCMD(S1D13700_CMD_DISP_ON);           // Turn the display on
	s1d13700_SendByte(0x56);

	S1D13700_DSF = 1;

}

BYTE_VAL temp;

void s1d13700_PutPixel(SHORT x, SHORT y)
{
	BYTE_VAL data;
	
	if(x < 0 || x >= 320 || y < 0 || y >= 240) return;
	
	unsigned int addr = (x/8 + y*40);
	
 	data.Val = s1d13700_MemImage[addr];

	temp.bits.b0 = s1d13700_GetColor()==S1D13700_WHITE?0:1;

	// Turn the pixel on or off
	//temp.bits.b1 = 0;
	switch(x%8)
	{
		case 0 :
		{
			if(data.bits.b7 != temp.bits.b0)
			{
				data.bits.b7 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 1 :
		{
			if(data.bits.b6 != temp.bits.b0)
			{
				data.bits.b6 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 2 : 
		{
			if(data.bits.b5 != temp.bits.b0)
			{
				data.bits.b5 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 3 : 
		{
			if(data.bits.b4 != temp.bits.b0)
			{
				data.bits.b4 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 4 :
		{
			if(data.bits.b3 != temp.bits.b0)
			{
				data.bits.b3 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 5 : 
		{
			if(data.bits.b2 != temp.bits.b0)
			{
				data.bits.b2 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 6 : 
		{
			if(data.bits.b1 != temp.bits.b0)
			{
				data.bits.b1 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		case 7 : 
		{
			if(data.bits.b0 != temp.bits.b0)
			{
				data.bits.b0 = temp.bits.b0;
				temp.bits.b1 = 1;
			}
			break;
		}
		
	}
	
	// Write the new data byte to display memory
	if(temp.bits.b1)
	{
		s1d13700_EtatMemImage[addr/8] |= 0x01<<addr%8;
	}
	s1d13700_MemImage[addr] = data.Val;
}

void s1d13700_Reffrache()
{
//	char Etat;
	unsigned int addr = 0;
//	unsigned int Oldaddr = 0;
	
	if(temp.bits.b1)
	{
		// Clear layer graphics
		s1d13700_setCursorAddress(S1D13700_GRAPHICS1_ADDR);
		s1d13700_SendCMD(S1D13700_CMD_DISPLAY_WRITE);
		
		for(addr = 0; addr < 9600; addr++)
		{
			s1d13700_SendByte(s1d13700_MemImage[addr]);
		}
	}
 /*  
	for(addr = 0; addr < 9600; addr++)
	{
		if(addr%8 == 0)
		{
			Etat = s1d13700_EtatMemImage[addr/8];
			if(!Etat)
			{
				addr += 7;
				continue;
			}
		}
		
		if(Etat & (0x01 << (addr%8)))
		{
			if(addr > Oldaddr + 1)
			{
				s1d13700_setCursorAddress(S1D13700_GRAPHICS1_ADDR + addr);
				s1d13700_SendCMD(S1D13700_CMD_DISPLAY_WRITE);
			}
			
			s1d13700_EtatMemImage[addr/8] &= ~(0x01<<addr%8);
			s1d13700_SendByte(s1d13700_MemImage[addr]);
			Oldaddr = addr;
		}
	}*/
}


void s1d13700_Clear()
{
	unsigned int addr;

	for(addr = 0; addr < 9600; addr++)
	{
		s1d13700_MemImage[addr] = 0x00;
	}
	for(addr = 0; addr < 1200; addr++)
	{
		s1d13700_EtatMemImage[addr] = 0xFF;
	}
}

void s1d13700_SetBackLight(BYTE etat)
{
   S1D13700_BACKLIGHT = etat;
}
