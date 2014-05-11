/*
 * File:   main.c
 * Author: Jason
 *
 * Created on March 4, 2014, 3:45 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>
#include <plib.h>

#include "fubmini.h"
#include "ra8875.h"

#include "main.h"

#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
#pragma config FUSBIDIO = OFF           // USB USID Selection (Controlled by Port Function)
#pragma config FVBUSONIO = OFF          // USB VBUS ON Selection (Controlled by Port Function)
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (24x Multiplier)
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)
#pragma config UPLLEN = ON              // USB PLL Enable (Enabled)
#pragma config FPLLODIV = DIV_2         // System PLL Output CLOCK2 Divider (PLL Divide by 2)
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral CLOCK2 Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSECME           // CLOCK2 Switching and Monitor Selection (CLOCK2 Switch Enable, FSCM Enabled)
#pragma config WDTPS = PS1024           // Watchdog Timer Postscaler (1:1024)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#define ALL_BITS 0xFFFFFFFF

#define SYS_FREQ    (48000000L)


uint fColor = color_black;
uint bColor = color_blue;

void main ()
{
#ifdef DEBUG_ON
    DBINIT();
#endif

    init();
    configIO();
    C86_LOW();

    LCD_Initial();
    //Backlight(RA8875_PWM_CLK_DIV8,80);

    LCD_CmdWrite(0X01);//display on
    LCD_DataWrite(0X80);
    //LCD_WriteRegister(0x01,0x80);
    
    Active_Window(0,799,0,479);//Set the working window size
    SetColors(fColor,bColor);
    ClearScreen(0);
    Chk_Busy();

    FontWrite_Position(10,10);
    String("AtomSoftTech - RA8875 7\" TFT with Capacitive Touch");

    while(1)
    {

    }
}

uint RGB16(char R,char G,char B)
{
 return ((B >> 3) + ((G >> 2)<<5) + ((R >> 3)<<11));
}

void WriteString (char *text, uint x, uint y, uint fcolor, uint bcolor)
{
    Text_Foreground_Color1(fcolor);//Set the foreground color
    Text_Background_Color1(bcolor);//Set the background color

    FontWrite_Position(x,y);
    String(text);

    Text_Foreground_Color1(fColor);//Set the foreground color
    Text_Background_Color1(bColor);//Set the background color
}

void delay()
{
    UINT tt;
    tt = 100;
    while(tt--)
        Nop();
}

void configIO()
{
    U1PWRCbits.USBPWR = 0; //USB OFF

    PORTSetPinsDigitalOut(IOPORT_A, ALL_BITS);
    PORTSetPinsDigitalOut(IOPORT_B, ALL_BITS);
    PORTSetPinsDigitalOut(IOPORT_C, ALL_BITS);
}

void init()
{
    SYSTEMConfig(SYS_FREQ, SYS_CFG_ALL);
    OSCConfig(OSC_POSC_PLL, OSC_PLL_MULT_24,OSC_PLL_POST_2,0);
    mOSCSetPBDIV( OSC_PB_DIV_1 );    // Configure the PB bus to run at 1/4 the CPU frequency
}