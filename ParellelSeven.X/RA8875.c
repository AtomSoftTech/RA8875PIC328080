//----------------------------------------------------------------------
//EASTRISING TECHNOLOGY CO,.LTD.//
// Module    : ER-TFTM070-5  7.0 INCH TFT LCD  800*480   7 INCH  Resistive Touch Screen
// Lanuage   : C51 Code
// Create    : JAVEN LIU
// Date      : Nov-30-2013
// Drive IC  : RA8875    Font chip  23L32S4W    Flash:128M char
// INTERFACE : 4-Wire SPI
// MCU 		 : STC12LE5C60S2  //51 series  1T MCU
// MCU VDD		 : 3.3V
//----------------------------------------------------------------------
#include "ra8875.h"

void Delay1us(uint i)
{
    unsigned int j;
    while(i--)
        for(j=0;j<48;j++);
}

void Delay1ms(uint i)
{
    unsigned int j;
    while(i--)
        for(j=0;j<48000;j++);
}


void Delay10ms(uint i)
{
    while(i--)
        Delay1ms(10);
}

void Delay100ms(uint i)
{
    while(i--)
	Delay1ms(100);
}

void NextStep(void)
{
     Delay10ms(2);
}

void LCD_CmdWrite(unsigned char Cmd)
{//8080 mode
	  RD = 1;
	  LCD_CS = 0;
	  RS = 1;
	  LATC = Cmd;
	  WR = 0;   Delay1us(1);
	  WR = 1;
	  LCD_CS = 1;
	  LATC = 0xff;
	  RS = 0;
	  /*
//6800 mode
	  cs =0; // chip enable.
	  wr = 0; //
	  rs = 1; // rs = 0;
	  DATA_BUS = Cmd;
	  rd = 1; //
	  ;
	  ;
	  rd = 0; //
	  cs =1; // chip disable.
	*/
}

///////////write data or parameter
void LCD_DataWrite(unsigned char Data)
{//8080 mode
	  RD = 1;
	  LCD_CS = 0;
	  RS = 0;
	  LATC = Data;
	  WR = 0;   Delay1us(1);
	  WR = 1;
	  LCD_CS = 1;
	  LATC = 0xff;
	  RS = 1;
	  /*
//6800 mode
	  cs =0; // chip enable.
	  wr = 0; //
	  rs = 1; // rs = 0;
	  DATA_BUS = Data;
	  rd = 0; //
	  ;
	  ;
	  rd = 0; //
	  cs =1; // chip disable
	*/
}

////////////////write command and parameter
void Write_Dir(uchar Cmd,uchar Data)
{
  	LCD_CmdWrite(Cmd);
  	LCD_DataWrite(Data);
}

///////////////read  status
uchar LCD_StatusRead(void)
{//8080 mode
	  uchar Data;
          TRISC = 0XFF;
          LCD_CS = 0;
	  RS= 1;
	  WR = 1;
	  RD = 0;   Delay1us(1);
	  Data = PORTC;
	  RD = 1;
	  LCD_CS = 1;
	  TRISC = 0x00;
	  RS = 0;
	  return Data;
	  /*
//6800 mode
	  uchar reg_rddata;
	  DATA_BUS = 0xff;
	  cs =0; // chip enable.
	  wr = 1; //
	  rs = 1; //
	  rd = 1; // rd = 0;
	  reg_rddata = DATA_BUS;
	  ;
	  ;
	  rd = 0; // rd = 1;
	  cs =1; // chip disable.
	  return(reg_rddata);
	  	*/
}

/////////////////read data parameter
uchar LCD_DataRead(void)
{
	//8080 mode
	  uchar Data;
          TRISC = 0xFF;

	  WR = 1;
	  LCD_CS = 0;
	  RS = 0;
	  RD = 0;   Delay1us(1);
	  Data = PORTC;
	  RD = 1;
	  LCD_CS = 1;
	  RS = 1;
          TRISC = 0x00;
	  return Data;
	/*
//6800 mode
	  uchar reg_rddata;
	  DATA_BUS = 0xff;
	  cs =0; // chip enable.
	  wr = 1; //
	  rs = 0; //
	  rd = 1; // rd = 0;
	  reg_rddata = DATA_BUS;
	  ;
	  ;
	  rd = 0; // rd = 1;
	  cs =1; // chip disable.
	  return(reg_rddata);
	  	*/
}
void ClearScreen(char area)
{
    char val = 0x80;

    if(area)
        val |= 0x40;

    LCD_CmdWrite(0x8E);     //Memory Clear Control Register
    LCD_DataWrite(val);
}
void SetColors(uint f_color, uint b_color)
{
    LCD_CmdWrite(0x60);//BGCR0
    LCD_DataWrite((unsigned char)(b_color>>11));

    LCD_CmdWrite(0x61);//BGCR0
    LCD_DataWrite((unsigned char)(b_color>>5));

    LCD_CmdWrite(0x62);//BGCR0
    LCD_DataWrite((unsigned char)(b_color));

    LCD_CmdWrite(0x63);//BGCR0
    LCD_DataWrite((unsigned char)(f_color>>11));

    LCD_CmdWrite(0x64);//BGCR0
    LCD_DataWrite((unsigned char)(f_color>>5));

    LCD_CmdWrite(0x65);//BGCR0
    LCD_DataWrite((unsigned char)(f_color));
}
////////LCM reset
void LCD_Reset(void)
{
	RESET = 0;
	Delay1ms(1);
	RESET = 1;
	Delay1ms(1);
}

///////////////check busy
void Chk_Busy(void)
{
	uchar temp;
	do
	{
	temp=LCD_StatusRead();
	}while((temp&0x80)==0x80);
}
///////////////check bte busy
void Chk_BTE_Busy(void)
{
	uchar temp;
	do
	{
	temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);
}
///////////////check dma busy
void Chk_DMA_Busy(void)
{
	uchar temp;
	do
	{
	LCD_CmdWrite(0xBF);
	temp =LCD_DataRead();
	}while((temp&0x01)==0x01);
}

/////////////PLL setting
void PLL_ini(void)
{
    LCD_CmdWrite(0x88);
    LCD_DataWrite(0x0C);
    Delay1ms(1);
    LCD_CmdWrite(0x89);
    LCD_DataWrite(0x02);
    Delay1ms(1);
}

/////////////////Set the working window area
void Active_Window(uint XL,uint XR ,uint YT ,uint YB)
{
	uchar temp;
    //setting active window X
	temp=XL;
    LCD_CmdWrite(0x30);//HSAW0
	LCD_DataWrite(temp);
	temp=XL>>8;
    LCD_CmdWrite(0x31);//HSAW1
	LCD_DataWrite(temp);

	temp=XR;
    LCD_CmdWrite(0x34);//HEAW0
	LCD_DataWrite(temp);
	temp=XR>>8;
    LCD_CmdWrite(0x35);//HEAW1
	LCD_DataWrite(temp);

    //setting active window Y
	temp=YT;
    LCD_CmdWrite(0x32);//VSAW0
	LCD_DataWrite(temp);
	temp=YT>>8;
    LCD_CmdWrite(0x33);//VSAW1
	LCD_DataWrite(temp);

	temp=YB;
    LCD_CmdWrite(0x36);//VEAW0
	LCD_DataWrite(temp);
	temp=YB>>8;
    LCD_CmdWrite(0x37);//VEAW1
	LCD_DataWrite(temp);
}


/////////////LCM initial
void LCD_Initial(void)
{
    PLL_ini();
	LCD_CmdWrite(0x10);	 //
	LCD_DataWrite(0x0C);   //     65k   8bit mcu interface

	LCD_CmdWrite(0x04);    //PCLK
	LCD_DataWrite(0x81);   //
	Delay1ms(1);

	 //Horizontal set
	LCD_CmdWrite(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]
	LCD_DataWrite(0x63);//Horizontal display width(pixels) = (HDWR + 1)*8
	LCD_CmdWrite(0x15);//Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
	LCD_DataWrite(0x00);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]
	LCD_CmdWrite(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]
	LCD_DataWrite(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
	LCD_CmdWrite(0x17); //HSTR//HSYNC Start Position[4:0]
	LCD_DataWrite(0x03);//HSYNC Start Position(PCLK) = (HSTR + 1)*8
	LCD_CmdWrite(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.
	LCD_DataWrite(0x0B);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
	 //Vertical set
	LCD_CmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]
	LCD_DataWrite(0xdf);//Vertical pixels = VDHR + 1
	LCD_CmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]
	LCD_DataWrite(0x01);//Vertical pixels = VDHR + 1
	LCD_CmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]
	LCD_DataWrite(0x20);//Vertical Non-Display area = (VNDR + 1)
	LCD_CmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]
	LCD_DataWrite(0x00);//Vertical Non-Display area = (VNDR + 1)
	LCD_CmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]
	LCD_DataWrite(0x16);//VSYNC Start Position(PCLK) = (VSTR + 1)
	LCD_CmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]
	LCD_DataWrite(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)
	LCD_CmdWrite(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	LCD_DataWrite(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)



	Active_Window(0,799,0,479);

	LCD_CmdWrite(0x8a);//PWM setting
	LCD_DataWrite(0x80);
	LCD_CmdWrite(0x8a);//PWM setting
	LCD_DataWrite(0x81);//open PWM
	LCD_CmdWrite(0x8b);//Backlight brightness setting
	LCD_DataWrite(0xff);//Brightness parameter 0xff-0x00
}



///////////////Background color settings
void Text_Background_Color1(uint b_color)
{

	LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite((uchar)(b_color>>11));

	LCD_CmdWrite(0x61);//BGCR0
	LCD_DataWrite((uchar)(b_color>>5));

	LCD_CmdWrite(0x62);//BGCR0
	LCD_DataWrite((uchar)(b_color));
}

///////////////Background color settings
void Text_Background_Color(uchar setR, uchar setG, uchar setB)
{
    LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite(setR);

    LCD_CmdWrite(0x61);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x62);//BGCR0
	LCD_DataWrite(setB);
}

////////////////Foreground color settings
void Text_Foreground_Color1(uint b_color)
{

	LCD_CmdWrite(0x63);//BGCR0
	LCD_DataWrite((uchar)(b_color>>11));

	LCD_CmdWrite(0x64);//BGCR0
	LCD_DataWrite((uchar)(b_color>>5));

	LCD_CmdWrite(0x65);//BGCR0
	LCD_DataWrite((uchar)(b_color));
}

////////////////Foreground color settings
void Text_Foreground_Color(uchar setR,uchar setG,uchar setB)
{
    LCD_CmdWrite(0x63);//BGCR0
	LCD_DataWrite(setR);

    LCD_CmdWrite(0x64);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x65);//BGCR0¡¤
	LCD_DataWrite(setB);
}
//////////////////BTE area size settings
void BTE_Size(UINT width,UINT height)
{
    uchar temp;
	temp=width;
    LCD_CmdWrite(0x5c);//BET area width literacy
	LCD_DataWrite(temp);
	temp=width>>8;
    LCD_CmdWrite(0x5d);//BET area width literacy
	LCD_DataWrite(temp);

	temp=height;
    LCD_CmdWrite(0x5e);//BET area height literacy
	LCD_DataWrite(temp);
	temp=height>>8;
    LCD_CmdWrite(0x5f);//BET area height literacy
	LCD_DataWrite(temp);
}

////////////////////BTE starting position
void BTE_Source(uint SX,uint DX ,uint SY ,uint DY)
{
	uchar temp,temp1;

	temp=SX;
    LCD_CmdWrite(0x54);//BTE horizontal position of read/write data
	LCD_DataWrite(temp);
	temp=SX>>8;
    LCD_CmdWrite(0x55);//BTE horizontal position of read/write data
	LCD_DataWrite(temp);

	temp=DX;
    LCD_CmdWrite(0x58);//BET written to the target horizontal position
	LCD_DataWrite(temp);
	temp=DX>>8;
    LCD_CmdWrite(0x59);//BET written to the target horizontal position
	LCD_DataWrite(temp);

	temp=SY;
    LCD_CmdWrite(0x56);//BTE vertical position of read/write data
	LCD_DataWrite(temp);
	temp=SY>>8;
    LCD_CmdWrite(0x57);
	temp1 = LCD_DataRead();
	temp1 &= 0x80;
    temp=temp|temp1;
	LCD_CmdWrite(0x57);//BTE vertical position of read/write data
	LCD_DataWrite(temp);

	temp=DY;
    LCD_CmdWrite(0x5a);//BET written to the target  vertical  position
	LCD_DataWrite(temp);
	temp=DY>>8;
    LCD_CmdWrite(0x5b);
	temp1 = LCD_DataRead();
	temp1 &= 0x80;
	temp=temp|temp1;
	LCD_CmdWrite(0x5b);//BET written to the target  vertical  position
	LCD_DataWrite(temp);
}
///////////////Memory write position
void MemoryWrite_Position(uint X,uint Y)
{
	uchar temp;

	temp=X;
    LCD_CmdWrite(0x46);
	LCD_DataWrite(temp);
	temp=X>>8;
    LCD_CmdWrite(0x47);
	LCD_DataWrite(temp);

	temp=Y;
    LCD_CmdWrite(0x48);
	LCD_DataWrite(temp);
	temp=Y>>8;
    LCD_CmdWrite(0x49);
	LCD_DataWrite(temp);
}

////////////////Text write position
void FontWrite_Position(uint X,uint Y)
{
	uchar temp;
	temp=X;
    LCD_CmdWrite(0x2A);
	LCD_DataWrite(temp);
	temp=X>>8;
    LCD_CmdWrite(0x2B);
	LCD_DataWrite(temp);

	temp=Y;
    LCD_CmdWrite(0x2C);
	LCD_DataWrite(temp);
	temp=Y>>8;
    LCD_CmdWrite(0x2D);
	LCD_DataWrite(temp);
}

//////////////writing text
void String(uchar *str)
{
    Write_Dir(0x40,0x80);//Set the character mode
	LCD_CmdWrite(0x02);
	while(*str != '\0')
	{
	 LCD_DataWrite(*str);
	 ++str;
	 Chk_Busy();
	}
}



/////////////////Scroll window size
void Scroll_Window(uint XL,uint XR ,uint YT ,uint YB)
{
	uchar temp;
	temp=XL;
    LCD_CmdWrite(0x38);//HSSW0
	LCD_DataWrite(temp);
	temp=XL>>8;
    LCD_CmdWrite(0x39);//HSSW1
	LCD_DataWrite(temp);

	temp=XR;
    LCD_CmdWrite(0x3c);//HESW0
	LCD_DataWrite(temp);
	temp=XR>>8;
    LCD_CmdWrite(0x3d);//HESW1
	LCD_DataWrite(temp);

	temp=YT;
    LCD_CmdWrite(0x3a);//VSSW0
	LCD_DataWrite(temp);
	temp=YT>>8;
    LCD_CmdWrite(0x3b);//VSSW1
	LCD_DataWrite(temp);

	temp=YB;
    LCD_CmdWrite(0x3e);//VESW0
	LCD_DataWrite(temp);
	temp=YB>>8;
    LCD_CmdWrite(0x3f);//VESW1
	LCD_DataWrite(temp);
}

///////////////Window scroll offset Settings
void Scroll(uint X,uint Y)
{
	uchar temp;

	temp=X;
    LCD_CmdWrite(0x24);//HOFS0
	LCD_DataWrite(temp);
	temp=X>>8;
    LCD_CmdWrite(0x25);//HOFS1
	LCD_DataWrite(temp);

	temp=Y;
    LCD_CmdWrite(0x26);//VOFS0
	LCD_DataWrite(temp);
	temp=Y>>8;
    LCD_CmdWrite(0x27);//VOFS1
	LCD_DataWrite(temp);
}

///////////////The FLASH reading area   setting
void DMA_block_mode_size_setting(uint BWR,uint BHR,uint SPWR)
{
  	LCD_CmdWrite(0xB4);
  	LCD_DataWrite(BWR);
  	LCD_CmdWrite(0xB5);
  	LCD_DataWrite(BWR>>8);

  	LCD_CmdWrite(0xB6);
  	LCD_DataWrite(BHR);
  	LCD_CmdWrite(0xB7);
  	LCD_DataWrite(BHR>>8);

  	LCD_CmdWrite(0xB8);
  	LCD_DataWrite(SPWR);
  	LCD_CmdWrite(0xB9);
  	LCD_DataWrite(SPWR>>8);
}

/////////////FLASH read start position Settings
void DMA_Start_address_setting(ulong set_address)
{
  	LCD_CmdWrite(0xB0);
  	LCD_DataWrite(set_address);

  	LCD_CmdWrite(0xB1);
  	LCD_DataWrite(set_address>>8);

	LCD_CmdWrite(0xB2);
  	LCD_DataWrite(set_address>>16);

  	LCD_CmdWrite(0xB3);
  	LCD_DataWrite(set_address>>24);
}
///////////drawing circle
void  Draw_Circle(uint X,uint Y,uint R)
{
	uchar temp;

	temp=X;
    LCD_CmdWrite(0x99);
	LCD_DataWrite(temp);
	temp=X>>8;
    LCD_CmdWrite(0x9a);
	LCD_DataWrite(temp);

	temp=Y;
    LCD_CmdWrite(0x9b);
	LCD_DataWrite(temp);
	temp=Y>>8;
    LCD_CmdWrite(0x9c);
	LCD_DataWrite(temp);

	temp=R;
    LCD_CmdWrite(0x9d);
	LCD_DataWrite(temp);
}

///////////drawing elliptic curve
void  Draw_Ellipse(uint X,uint Y,uint R1,uint R2)
{
	uchar temp;
	temp=X;
    LCD_CmdWrite(0xA5);
	LCD_DataWrite(temp);
	temp=X>>8;
    LCD_CmdWrite(0xA6);
	LCD_DataWrite(temp);

	temp=Y;
    LCD_CmdWrite(0xA7);
	LCD_DataWrite(temp);
	temp=Y>>8;
    LCD_CmdWrite(0xA8);
	LCD_DataWrite(temp);

	temp=R1;
    LCD_CmdWrite(0xA1);
	LCD_DataWrite(temp);
	temp=R1>>8;
    LCD_CmdWrite(0xA2);
	LCD_DataWrite(temp);

	temp=R2;
    LCD_CmdWrite(0xA3);
	LCD_DataWrite(temp);
	temp=R2>>8;
    LCD_CmdWrite(0xA4);
	LCD_DataWrite(temp);
}

///////////drawing line, rectangle, triangle
void Draw_Line(uint XS,uint XE ,uint YS,uint YE)
{
    uchar temp;
	temp=XS;
    LCD_CmdWrite(0x91);
	LCD_DataWrite(temp);
	temp=XS>>8;
    LCD_CmdWrite(0x92);
	LCD_DataWrite(temp);

	temp=XE;
    LCD_CmdWrite(0x95);
	LCD_DataWrite(temp);
	temp=XE>>8;
    LCD_CmdWrite(0x96);
	LCD_DataWrite(temp);

	temp=YS;
    LCD_CmdWrite(0x93);
	LCD_DataWrite(temp);
	temp=YS>>8;
    LCD_CmdWrite(0x94);
	LCD_DataWrite(temp);

	temp=YE;
    LCD_CmdWrite(0x97);
	LCD_DataWrite(temp);
	temp=YE>>8;
    LCD_CmdWrite(0x98);
	LCD_DataWrite(temp);
}

////////////draw a triangle of three point
void Draw_Triangle(uint X3,uint Y3)
{
    uchar temp;
	temp=X3;
    LCD_CmdWrite(0xA9);
	LCD_DataWrite(temp);
	temp=X3>>8;
    LCD_CmdWrite(0xAA);
	LCD_DataWrite(temp);

	temp=Y3;
    LCD_CmdWrite(0xAB);
	LCD_DataWrite(temp);
	temp=Y3>>8;
    LCD_CmdWrite(0xAC);
	LCD_DataWrite(temp);
}