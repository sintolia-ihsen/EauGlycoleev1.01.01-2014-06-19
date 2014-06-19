//#define OLD_RTC_CODE

#ifdef OLD_RTC_CODE

#include <p32xxxx.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <plib.h>

#include "main.h"
#include "variables.h"
#include "rtc.h"


unsigned char rtcc_buf       ;   // global data buffer for the RTCC
unsigned char err_cnt = 0x00 ;   // error counter ; slave's I2C_NACK
unsigned char err_flg = 0x00 ;   // error flag ; reacts at slave's NACK & MSSP1IF
int compt;

int BcdToDecimal(unsigned char bcd)
{
	unsigned char result;

	result = (bcd &0x0F) + (((bcd & 0xF0)>>4) * 10);

	return(result);
}

unsigned char DecimalToBcd(unsigned char dec)
{
	unsigned char result,d,x;

	result = ((dec / 10) << 4) + (dec % 10);

	return(result);
}

void IdleI2C2(void)
{
    /* Wait until I2C Bus is Inactive */
    while(I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RSEN || I2C2CONbits.RCEN || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT);
}


unsigned int MasterWriteI2C2(unsigned char data_out)
{
    I2C2TRN = data_out;

    if(I2C2STATbits.IWCOL)        /* If write collision occurs,return -1 */
        return -1;
    else
	{
		while(I2C2STATbits.TBF);   // wait until write cycle is complete
		IdleI2C2();                 // ensure module is idle

		if ( I2C2STATbits.ACKSTAT ) // test for ACK condition received
             return ( -2 );
        else
			return ( 0 );
    }
}

unsigned char MasterReadI2C2(void)
{
    I2C2CONbits.RCEN = 1;
    while(I2C2CONbits.RCEN);
    I2C2STATbits.I2COV = 0;
    return(I2C2RCV);
}

//................................................................................
void i2c_start(void)            // START I2C communication
{
	StartI2C2();//Send the Start Bit
	IdleI2C2();//Wait to complete
}
//.................................................................................
void i2c_restart(void)         	// RESTART I2C communication (change to 'reads')
{
	RestartI2C2();
	IdleI2C2();
}
//.................................................................................
void i2c_stop(void)       		// STOP I2C communication
{
	StopI2C2();//Send the Stop condition
	IdleI2C2();//Wait to complete
}


//.................................................................................
unsigned char i2c_rd(void)                	// reads a byte on I2C bus ; returns 'rtcc_buf'
{
	unsigned char rtc_data;

	rtc_data = MasterReadI2C2();
	IdleI2C2();

	return rtc_data;
}

//.................................................................................
void i2c_wr(unsigned char rtc_data)                	// reads a byte on I2C bus ; returns 'rtcc_buf'
{

	MasterWriteI2C2(rtc_data);
	IdleI2C2();//Wait to complete
}

//..................................................................................
void i2c_ack(void)               	// ACK=0 from MASTER
{
	AckI2C2();
	IdleI2C2();//Wait to complete

}
//...................................................................................
void i2c_nack(void)              	// NACK=1 from MASTER
{
	NotAckI2C2();
	IdleI2C2();//Wait to complete
}
//************************************************************************************
//                      I2C RTCC DRIVERS
//************************************************************************************
void rtcc_wr(unsigned char time_var, unsigned char rtcc_reg) // writes a constant/variable in a RTCC register
{
	i2c_start()             ;  // start I2C communication
  	i2c_wr(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
   	i2c_wr(rtcc_reg)        ;  // write the register's ADDRESS
   	err_cnt = 0x03          ;  // set error variable = 03
   	i2c_wr(time_var)        ;  // write byte variable in the register
   	i2c_stop()              ;  // stop I2C communication
}
//.....................................................................................
unsigned char rtcc_rd(unsigned char rtcc_reg)  // reads a register of the RTCC
{
	i2c_start()             ;  // start I2C communication
    i2c_wr(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
   	i2c_wr(rtcc_reg)        ;  // write the register ADDRESS
   	i2c_restart()           ;  // RESTART for READS
   	i2c_wr(ADDR_RTCC_READ)  ;  // send the DEVICE ADDRESS for RTCC READS.
   	rtcc_buf = i2c_rd()     ;  // read register (stored in 'rtcc_buf')
   	i2c_nack()              ;  // NOACK from MASTER (last read byte)
   	i2c_stop()              ;  // stop I2C communication

   	return rtcc_buf    ;       // store the read byte in 'time_var'
}

 //.........................................................................................
void rtc_init(void)              // initialization of time/date:2010_01_01 , 00:00:00.
{
    unsigned char day, sec;

    RTC_TRIS_SCL = 1;
    RTC_TRIS_SDA = 1;
    RTC_TRIS_MFP = 1;

	OpenI2C2(I2C_EN|I2C_SM_DIS|I2C_SLW_DIS, (GetPeripheralClock() / 2ul / 100000) - 2);// (GetPeripheralClock() / 2ul / EEPROMCLOCK) - 2

    day = rtcc_rd(ADDR_DAY);
    sec = rtcc_rd(ADDR_SEC);

    if(!(sec & START_32KHZ) ||  !(day & VBATEN))
    {
        rtcc_wr(SQWE + ALM_NO + MFP_01H, ADDR_CTRL);

        rtcc_wr(sec | START_32KHZ, ADDR_SEC);
        rtcc_wr(day | VBATEN, ADDR_DAY);
    }
}

int rtc_Read()
{
	struct tm	STime;

	if(rtcc_rd(ADDR_SEC) & 0x80)
	{
		STime.tm_sec = BcdToDecimal(rtcc_rd(ADDR_SEC) & MASK_SEC);
		STime.tm_min = BcdToDecimal(rtcc_rd(ADDR_MIN) & MASK_MIN);
		STime.tm_hour = BcdToDecimal(rtcc_rd(ADDR_HOUR) & MASK_HOUR);
		//clock_wday = BcdToDecimal(rtcc_rd(ADDR_DAY) & 0x07);
		STime.tm_mday = BcdToDecimal(rtcc_rd(ADDR_DATE) & MASK_DATE);
		STime.tm_mon = BcdToDecimal(rtcc_rd(ADDR_MNTH) & MASK_MNTH) - 1;
		STime.tm_year = BcdToDecimal(rtcc_rd(ADDR_YEAR) & MASK_YEAR) + 100;


		return mktime(&STime);
	}
	return -1;
}

void rtc_Write(long DateHeure)
{
	struct tm *pSTime = localtime(&DateHeure);

	rtcc_wr(DecimalToBcd(pSTime->tm_sec)|START_32KHZ, ADDR_SEC);
    rtcc_wr(DecimalToBcd(pSTime->tm_min), ADDR_MIN);
    rtcc_wr(DecimalToBcd(pSTime->tm_hour)|HOUR_24, ADDR_HOUR);
    rtcc_wr(DecimalToBcd(pSTime->tm_wday + 1)|VBATEN, ADDR_DAY);
	rtcc_wr(DecimalToBcd(pSTime->tm_mday), ADDR_DATE);
	rtcc_wr(DecimalToBcd(pSTime->tm_mon + 1), ADDR_MNTH);
	rtcc_wr(DecimalToBcd(pSTime->tm_year - 100), ADDR_YEAR);
}

#else
/**********************************************************************************
// File Name    : i2c_rtcc_func.c
// PIC24 family, C language.
// Dependencies : p24fj128ga010.h, i2c_rtcc.h, i2c.h
// Processor    : PIC24FJ128GA010
// Hardware     : Explorer 16 demo board, MCP79410 I2C RTCC
// I.D.E.       : MPLAB + C30 compiler v3.30b
// Company      : Microchip Technology , Inc.
//.................................................................................
//                   SOFTWARE  LICENSE AGREEMENT
//.................................................................................
// "Microchip Technology Inc." ("Microchip") licenses this software to you
// solely for use with Microchip's Serial RTCC products.
// The software is owned by Microchip and/or its licensors, and is protected
// under applicable copyright laws.  All rights reserved.
// SOFTWARE IS PROVIDED "AS IS."  MICROCHIP AND ITS LICENSOR EXPRESSLY
// DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE,OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
// AND ITS LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
// CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT,
// COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY
// CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
// THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS."
//.......................................................................................
* Author                    Date            Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Alexandru Valeanu        	11/7/2011		Initial release for MCP79410 I2C RTCC
*
****************************************************************************************/
#include <plib.h>
#include <time.h>
#include "main.h"
#include "rtc.h"

//..................................................................................................
// Init the I2C1 module
//..................................................................................................
unsigned char err_flg; // global error flag
unsigned char err_code; // global error flag

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ini_tim1(void) { // initialization of TIMER1
    T1CON = 0x0010; // stop TMR1, psc = 08 / 0.5usec
    PR1 = 1500; // 5ms
    TMR1 = 0x0000; // clear the timer
}

//...................................................................................................
// This function checks the I2C/Timer1 Interrupt Flags
//...................................................................................................

void poll_tim1(void) {
    T1CONbits.TON = 1; // start Timer1
    while (!(IFS0bits.T1IF | IFS1bits.I2C2MIF));
    if (IFS0bits.T1IF) { // if Timer1 overflowed (the I2C device is unplugged)
        IFS0bits.T1IF = 0; // clear Timer1 & I2C Interrupt Flags
        IFS1bits.I2C2MIF = 0;
        err_flg = 0x01; // set the error flag if a time-out occured
    }
    else {
        err_flg = 0x00; // if no error in the I2C comm, clear the error flag
        IFS1bits.I2C2MIF = 0; // and the I2C general flag
    }
    T1CONbits.TON = 0; // stop Timer1 in any case

    TMR1 = 0x0000; // and clear it
}

void ini_i2c2(void) {
    I2C2CONbits.I2CEN = 0; // enable the I2C1 module
    RTC_SCL = 1;
    RTC_SDA = 1;
    RTC_TRIS_SCL = 0;
    RTC_TRIS_SDA = 0;
    poll_tim1();

    I2C2BRG = (GetPeripheralClock() / 2ul / 100000) - 2; //0x0076; // Fcy = 24 MHz -> Fscl = 100 kHz
    I2C2STAT = 0X0000;
    I2C2CON = 0x1200; // release SCL1 clock, 7-bit slave address
    // slew rate control disabled
    I2C2RCV = 0x0000; // clear the transmit and receive registers
    I2C2TRN = 0x0000;
    I2C2CONbits.I2CEN = 1; // enable the I2C1 module

}

//..................................................................................................
// This function generates an I2C Start Condition
//..................................................................................................

void i2c_start(void) {
    while (I2C2STATbits.TRSTAT); // Wait for bus Idle
    I2C2CONbits.SEN = 1; // Generate Start Condition
    poll_tim1(); // Wait for I2C/Timer1 interrupt flag
    if (err_flg)
    {
        err_code |= I2C_ERR_START;
    }
}
//..................................................................................................
// This function generates a Restart Condition (for reads)
//..................................................................................................

void i2c_restart(void) {
    while (I2C2STATbits.TRSTAT); // Wait for bus Idle
    I2C2CONbits.RSEN = 1; // Generate a Restart
    poll_tim1(); // Wait for I2C/Timer1 interrupt flag
    if (err_flg)
    {
        err_code |= I2C_ERR_RESTART;
    }
}
//..................................................................................................
// This function generates an I2C Stop Condition
//..................................................................................................

void i2c_stop(void) {
    while (I2C2STATbits.TRSTAT); // Wait for bus Idle
    I2C2CONbits.PEN = 1; // Generate Stop Condition
    poll_tim1(); // Wait for I2C/Timer1 interrupt flag
    if (err_flg)
    {
        err_code |= I2C_ERR_STOP;
    }
}
//..................................................................................................
// Writes a byte to the I2C bus
//..................................................................................................

void i2c_wr(unsigned char i2c_data) {
    while (I2C2STATbits.TRSTAT); // Wait for bus Idle
    I2C2TRN = i2c_data; // Load byte in the transmit buffer
    poll_tim1(); // Wait for I2C/Timer1 interrupt flag
    if (err_flg)
    {
        err_code |= I2C_ERR_WR;
    }
}
//..................................................................................................
// Reads a byte from the I2C bus
//..................................................................................................

unsigned char i2c_rd(void) {
    while (I2C2STATbits.TRSTAT); // Wait for bus Idle
    I2C2CONbits.RCEN = 1; // Enable Master receive
    poll_tim1(); // Wait for I2C/Timer1 interrupt flag
    if (err_flg)
    {
        err_code |= I2C_ERR_RD;
    }
    return (I2C2RCV); // Return data in buffer
}
//...................................................................................................
// Generates a Master No Acknowledge on the Bus
//...................................................................................................

void i2c_nack(void) {
    I2C2CONbits.ACKDT = 1; // Set   the related flag for NotAck
    I2C2CONbits.ACKEN = 1; // Start Ack sequence
    poll_tim1(); // Wait  for I2C/Timer1 interrupt flag
    I2C2CONbits.ACKDT = 0; // Clear the related flag for ACk
    if (err_flg)
    {
        err_code |= I2C_ERR_NACK;
    }
}
//..................................................................................................
// Generates a Master Acknowledge on the Bus
//..................................................................................................

void i2c_ack(void) {
    I2C2CONbits.ACKDT = 0; // Clear  the related flag for Ack
    I2C2CONbits.ACKEN = 1; // Start Ack sequence
    poll_tim1(); // Wait for I2C/Timer1 interrupt flag
    if (err_flg)
    {
        err_code |= I2C_ERR_ACK;
    }
}




//********************************************************************************************
//                         I2C RTCC DRIVERS
//********************************************************************************************
//extern unsigned char err_flg  ;   // the error flag will be used in this file
//............................................................................................
// The below function writes a data byte in the I2C RTCC
//............................................................................................

void i2c_rtcc_wr(unsigned char rtcc_reg, unsigned char time_var) { // writes a data byte to the I2C RTCC
    if(err_code)
    {
        err_code = 0;
        ini_i2c2();
    }
    i2c_start(); // start I2C communication
    i2c_wr(ADDR_RTCC_WRITE); // write DEVICE ADDR for RTCC WRITES
    i2c_wr(rtcc_reg); // write the register's ADDRESS
    i2c_wr(time_var); // write byte variable in the register
    i2c_stop(); // stop I2C communication
}
//..............................................................................................
// The below function reads a data byte from the I2C RTCC
//..............................................................................................

unsigned char i2c_rtcc_rd(unsigned char rtcc_reg) { // reads a data byte from the I2C RTCC
    unsigned char rtcc_buf; // general data buffer for the i2c rtcc
    if(err_code)
    {
        err_code = 0;
        ini_i2c2();
    }
    i2c_start(); // start I2C communication
    i2c_wr(ADDR_RTCC_WRITE); // write DEVICE ADDR for RTCC WRITES
    if (err_flg) // if an error occured at a PICTAIL removal,
    {
        return rtcc_buf; // leave fast the function
    }
    i2c_wr(rtcc_reg); // write the register ADDRESS
    if (err_flg) // if an error occured at a PICTAIL removal,
    {
        return rtcc_buf; // leave fast the function
    }
    i2c_restart(); // RESTART for READS
    i2c_wr(ADDR_RTCC_READ); // send the DEVICE ADDRESS for RTCC READS.
    if (err_flg) // if an error occured at a PICTAIL removal,
    {
        return rtcc_buf; // leave fast the function
    }
    rtcc_buf = i2c_rd(); // read register (stored in 'rtcc_buf')
    i2c_nack(); // NOACK from MASTER (last read byte)
    i2c_stop(); // stop I2C communication
    return rtcc_buf; // return the read byte, stored in the general rtcc buffer
}


//................................................................................................
// The below function initializes the I2C RTCC
//................................................................................................

void ini_i2c_rtcc(long DateHeureDefault) { // initialization of the I2C RTCC: enables the battery circuit                                     // START bit is located in the Sec register
    unsigned char day = 0; // local variable (stores the RTCC DAY register)
    unsigned char sec = 0; // local variable (stores the RTCC SEC register)

    sec = i2c_rtcc_rd(ADDR_SEC); // read day + OSCON bit
    day = i2c_rtcc_rd(ADDR_DAY); // read day + OSCON bit

    i2c_rtcc_wr(ADDR_CTRL, ALM_NO | MFP_01H | SQWE);

    if ((day & OSCON) == OSCON &&
        (sec & START_32KHZ) == START_32KHZ
        ) // if oscillator = already running, do nothing.
    {
    } else { // if oscillator = not running, set time/date(arbitrary)
        rtc_Write(DateHeureDefault);
    }
}


unsigned char BcdToDecimal(unsigned char bcd) {
    unsigned char result;

    result = (bcd & 0x0F) + (((bcd & 0xF0) >> 4) * 10);

    return (result);
}

unsigned char DecimalToBcd(unsigned char dec) {
    unsigned char result;

    result = ((dec / 10) << 4) + (dec % 10);

    return (result);
}

//.........................................................................................
void rtc_init(long DateHeureDefault) // initialization of time/date:2010_01_01 , 00:00:00.
{
    ini_tim1();

    ini_i2c2();

    ini_i2c_rtcc(DateHeureDefault);

    // init input capture
    _TRISD11 = 1;
    IC4CON = 0x8002;        // TMR2, int every cap, fall'n edge
    mIC4ClearIntFlag();     // clear the interrupt flag
    mIC4SetIntPriority( 3);
    mIC4IntEnable( 1);      // enable the IC1 interrupt
}

int rtc_Read() {
    struct tm STime;

    if (i2c_rtcc_rd(ADDR_SEC) & 0x80) {
        STime.tm_sec = BcdToDecimal(i2c_rtcc_rd(ADDR_SEC) & MASK_SEC);
        STime.tm_min = BcdToDecimal(i2c_rtcc_rd(ADDR_MIN) & MASK_MIN);
        STime.tm_hour = BcdToDecimal(i2c_rtcc_rd(ADDR_HOUR) & MASK_HOUR);
        //clock_wday = BcdToDecimal(rtcc_rd(ADDR_DAY) & 0x07);
        STime.tm_mday = BcdToDecimal(i2c_rtcc_rd(ADDR_DATE) & MASK_DATE);
        STime.tm_mon = BcdToDecimal(i2c_rtcc_rd(ADDR_MNTH) & MASK_MNTH) - 1;
        STime.tm_year = BcdToDecimal(i2c_rtcc_rd(ADDR_YEAR) & MASK_YEAR) + 100;
        return mktime(&STime);
    }

    return -1;
}

void rtc_Write(long DateHeure) {
    struct tm *pSTime = localtime(&DateHeure);

    i2c_rtcc_wr(ADDR_SEC, DecimalToBcd(pSTime->tm_sec) | START_32KHZ);
    i2c_rtcc_wr(ADDR_MIN, DecimalToBcd(pSTime->tm_min));
    i2c_rtcc_wr(ADDR_HOUR, DecimalToBcd(pSTime->tm_hour) | HOUR_24);
    i2c_rtcc_wr(ADDR_DAY, DecimalToBcd(pSTime->tm_wday + 1) | VBATEN);
    i2c_rtcc_wr(ADDR_DATE, DecimalToBcd(pSTime->tm_mday));
    i2c_rtcc_wr(ADDR_MNTH, DecimalToBcd(pSTime->tm_mon + 1));
    i2c_rtcc_wr(ADDR_YEAR, DecimalToBcd(pSTime->tm_year - 100));
}

int rtc_SetAlarme() {

        long DateHeure;

        i2c_rtcc_wr(ADDR_CTRL, ALM_0);

        DateHeure = rtc_Read();

        DateHeure += 60; // Alarme aprés une minute.

        struct tm *pSTime = localtime(&DateHeure);

        i2c_rtcc_wr(ADDR_ALM0HR, DecimalToBcd(pSTime->tm_hour));
        i2c_rtcc_wr(ADDR_ALM0MIN, DecimalToBcd(pSTime->tm_min));
        i2c_rtcc_wr(ADDR_ALM0SEC, DecimalToBcd(pSTime->tm_sec));

        i2c_rtcc_wr(ADDR_ALM0DAT, DecimalToBcd(pSTime->tm_mday));
        i2c_rtcc_wr(ADDR_ALM0MTH, DecimalToBcd(pSTime->tm_mon + 1));
        i2c_rtcc_wr(ADDR_ALM0CTL, DecimalToBcd(pSTime->tm_wday + 1)|ALMxC_SEC|ALMx_POL);

        return err_code;
}

void __ISR( _INPUT_CAPTURE_4_VECTOR, ipl3) IC4Interrupt( void)
{
    int temp;

    clock_MAJ();

    // this should clean the interrupt flag
    temp = IC4BUF;

    mIC4ClearIntFlag();
}
#endif