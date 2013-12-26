/*
 * ss_lcd_nh012864.c
 *
 *  Created on: 2013-4-2
 *      Author: WLD-LuoWenjie
 */
#include "stm32f30x.h"
#include "common.h"
#include "pal.h"
#include "sensor.h"
#include "global.h"

#ifdef SS_LCD_NH012864

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define MAX_COLUMN	0x3F			// 128/2-1 (Total Columns Devided by 2)
#define MAX_ROW		0x3F			// 64-1
#define	BRIGHTNESS	0x7F

static void nh012684_soft_init(void);
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static SS_LCD_NH012864_INFO g_sstLCDNH012864Info;
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void ss_lcd_nh012864_delay_ms(uint32_t u32Nms)
{
	uint32_t u32I, u32J;

	for(u32I=0; u32I<u32Nms; u32I++)
		for(u32J=0; u32J<(SystemCoreClock / 1000 / 72); u32J++)
		{
			__NOP();
		}
}

static void ss_lcd_nh012864_delay_ns(uint32_t u32NS)
{
	uint32_t u32I, u32J;

	for(u32I=0; u32I<u32NS; u32I++)
		for(u32J=0; u32J<(SystemCoreClock / 1000000); u32J++)
		{
			__NOP();
		}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
ErrorStatus ss_lcd_nh012864_init(const SS_LCD_NH012864_CFG *pConfig)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	if(!pConfig)
		return ERROR;

	memset(&g_sstLCDNH012864Info, 0x00, sizeof(SS_LCD_NH012864_INFO));

	g_sstLCDNH012864Info.pPalConfig = pConfig;

	if(pConfig->u32APB1Clk)
		RCC_APB1PeriphClockCmd(pConfig->u32APB1Clk, ENABLE);
	if(pConfig->u32APB2Clk)
		RCC_APB2PeriphClockCmd(pConfig->u32APB2Clk, ENABLE);
	if(pConfig->u32AHBClk)
		RCC_AHBPeriphClockCmd(pConfig->u32AHBClk, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = pConfig->u16CSPin;
	GPIO_Init(pConfig->pCSPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = pConfig->u16CDPin;
	GPIO_Init(pConfig->pCDPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = pConfig->u16WRPin;
	GPIO_Init(pConfig->pWRPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = pConfig->u16RDPin;
	GPIO_Init(pConfig->pRDPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = pConfig->u16RESTPin;
	GPIO_Init(pConfig->pRESTPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = pConfig->u16DBusPin;
	GPIO_Init(pConfig->pDBusPort, &GPIO_InitStructure);

	ss_lcd_nh012864_cs_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_rd_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_wr_high(&g_sstLCDNH012864Info);

	nh012684_soft_init();

	return SUCCESS;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
/*
static uint8_t ss_lcd_nh012864_ReadCmd(void)
{
	uint8_t u8Data;
	ss_lcd_nh012864_dbus_input(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cd_cmd(&g_sstLCDNH012864Info);				//command port
	ss_lcd_nh012864_cs_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_rd_low(&g_sstLCDNH012864Info);				//read
	ss_lcd_nh012864_dbus_read(&g_sstLCDNH012864Info, &u8Data);
	ss_lcd_nh012864_rd_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_high(&g_sstLCDNH012864Info);
	return u8Data;
}
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Write_Command(uint8_t u8Cmd)
{
	ss_lcd_nh012864_dbus_output(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cd_cmd(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_dbus_write(&g_sstLCDNH012864Info, u8Cmd);
	ss_lcd_nh012864_wr_low(&g_sstLCDNH012864Info);

	ss_lcd_nh012864_wr_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_high(&g_sstLCDNH012864Info);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

static uint8_t Read_Data(void)
{
	uint8_t u8Data;

	// 经过调试，需要连续读两次才能读出正确的值
	ss_lcd_nh012864_dbus_input(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cd_data(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_rd_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_low(&g_sstLCDNH012864Info);
//	ss_lcd_nh012864_dbus_read(&g_sstLCDNH012864Info, &u8Data);
	ss_lcd_nh012864_rd_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_high(&g_sstLCDNH012864Info);

//	ss_lcd_nh012864_cd_data(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_rd_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_dbus_read(&g_sstLCDNH012864Info, &u8Data);
	ss_lcd_nh012864_rd_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_high(&g_sstLCDNH012864Info);

	return u8Data;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Write_Data(uint8_t u8Data)
{
	ss_lcd_nh012864_dbus_output(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cd_data(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_dbus_write(&g_sstLCDNH012864Info, u8Data);
	ss_lcd_nh012864_wr_low(&g_sstLCDNH012864Info);

	ss_lcd_nh012864_wr_high(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_cs_high(&g_sstLCDNH012864Info);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Column_Address(uint8_t a, uint8_t b)
{
	Write_Command(0x15);		// Set Column Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x3F (Total Columns Devided by 2)
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Row_Address(uint8_t a, uint8_t b)
{
	Write_Command(0x75);		// Set Row Address
	Write_Command(a);			//   Default => 0x00
	Write_Command(b);			//   Default => 0x4F
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Contrast_Current(uint8_t d)
{
	Write_Command(0x81);		// Set Contrast Value
	Write_Command(d);			//   Default => 0x40
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Current_Range(uint8_t d)
{
	Write_Command(0x84|d);		// Set Current Range
								//   Default => 0x84
								//     0x84 (0x00) => Quarter Current Range
								//     0x85 (0x01) => Half Current Range
								//     0x86 (0x02) => Full Current Range
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Remap_Format(uint8_t d)
{
	Write_Command(0xA0);		// Set Re-Map & Data Format
	Write_Command(d);			//   Default => 0x00
								// Column Address 0 Mapped to SEG0
								// Disable Nibble Re-Map
								// Horizontal Address Increment
								// Scan from COM0 to COM[N-1]
								// Disable COM Split Odd Even
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Start_Line(uint8_t d)
{
	Write_Command(0xA1);		// Set Display Start Line
	Write_Command(d);			//   Default => 0x00
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Display_Offset(uint8_t d)
{
	Write_Command(0xA2);		// Set Display Offset
	Write_Command(d);			//   Default => 0x00
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Display_Mode(uint8_t d)
{
	Write_Command(0xA4|d);		// Set Display Mode
								//   Default => 0xA4
								//     0xA4 (0x00) => Normal Display
								//     0xA5 (0x01) => Entire Display On, All Pixels Turn On at GS Level 15
								//     0xA6 (0x02) => Entire Display Off, All Pixels Turn Off
								//     0xA7 (0x03) => Inverse Display
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Multiplex_Ratio(uint8_t d)
{
	Write_Command(0xA8);		// Set Multiplex Ratio
	Write_Command(d);			//   Default => 0x5F
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Master_Config(uint8_t d)
{
	Write_Command(0xAD);		// Set Master Configuration
	Write_Command(0x02|d);		//   Default => 0x03
								//     0x02 (0x00) => Select External VCC Supply
								//     0x03 (0x01) => Select Internal DC/DC Voltage Converter
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Display_On_Off(uint8_t d)
{
	Write_Command(0xAE|d);		// Set Display On/Off
								//   Default => 0xAE
								//     0xAE (0x00) => Display Off
								//     0xAF (0x01) => Display On
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Phase_Length(uint8_t d)
{
	Write_Command(0xB1);		// Phase 1 & 2 Period Adjustment
	Write_Command(d);			//   Default => 0x53 (5 Display Clocks [Phase 2] / 3 Display Clocks [Phase 1])
								//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
								//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Frame_Frequency(uint8_t d)
{
	Write_Command(0xB2);		// Set Frame Frequency (Row Period)
	Write_Command(d);			//   Default => 0x25 (37 Display Clocks)
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Display_Clock(uint8_t d)
{
	Write_Command(0xB3);		// Display Clock Divider/Osciallator Frequency
	Write_Command(d);			//   Default => 0x41
								//     D[3:0] => Display Clock Divider
								//     D[7:4] => Oscillator Frequency
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Precharge_Compensation(uint8_t a, uint8_t b)
{
	Write_Command(0xB4);		// Set Pre-Charge Compensation Level
	Write_Command(b);			//   Default => 0x00 (No Compensation)

	if(a == 0x20)
	{
		Write_Command(0xB0);	// Set Pre-Charge Compensation Enable
		Write_Command(0x08|a);	//   Default => 0x08
								//     0x08 (0x00) => Disable Pre-Charge Compensation
								//     0x28 (0x20) => Enable Pre-Charge Compensation
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_Precharge_Voltage(uint8_t d)
{
	Write_Command(0xBC);		// Set Pre-Charge Voltage Level
	Write_Command(d);			//   Default => 0x10 (Connect to VCOMH)
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_VCOMH(uint8_t d)
{
	Write_Command(0xBE);		// Set Output Level High Voltage for COM Signal
	Write_Command(d);			//   Default => 0x1D (0.81*VREF)
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void Set_VSL(uint8_t d)
{
	Write_Command(0xBF);		// Set Segment Low Voltage Level
	Write_Command(0x02|d);		//   Default => 0x0E
								//     0x02 (0x00) => Keep VSL Pin Floating
								//     0x0E (0x0C) => Connect a Capacitor between VSL Pin & VSS
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//static void Set_NOP()
//{
//	Write_Command(0xE3);		// Command for No Operation
//}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Graphic Acceleration
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void GA_Option(unsigned char d)
{
	Write_Command(0x23);			// Graphic Acceleration Command Options
	Write_Command(d);			//   Default => 0x01
						//     Enable Fill Rectangle
						//     Disable Wrap around in Horizontal Direction During Copying & Scrolling
						//     Disable Reverse Copy
}

void Draw_Rectangle(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	Write_Command(0x24);			// Draw Rectangle
	Write_Command(a);			//   Column Address of Start
	Write_Command(c);			//   Row Address of Start
	Write_Command(b);			//   Column Address of End (Total Columns Devided by 2)
	Write_Command(d);			//   Row Address of End
	Write_Command(e);			//   Gray Scale Level
	ss_lcd_nh012864_delay_ns(200);
}

void Copy(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f)
{
	Write_Command(0x25);			// Copy
	Write_Command(a);			//   Column Address of Start
	Write_Command(c);			//   Row Address of Start
	Write_Command(b);			//   Column Address of End (Total Columns Devided by 2)
	Write_Command(d);			//   Row Address of End
	Write_Command(e);			//   Column Address of New Start
	Write_Command(f);			//   Row Address of New Start
	ss_lcd_nh012864_delay_ns(200);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)
//
//    a: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM(unsigned char a)
{
	unsigned char k,j;

	Set_Column_Address(0,MAX_COLUMN);    //col+7);
	Set_Row_Address(0, MAX_ROW);

	for(j=0;j<0x40;j++)
		for(k=0;k<0x40;k++)
			Write_Data(a);

	// NHO12864-5 没调试通过 下面这种方法
	/*
	GA_Option(0x01);
	Draw_Rectangle(0x00,0x3f,0x00,0x3f,a);
	*/
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//    e: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)
{
	GA_Option(0x01);
	Draw_Rectangle(a,b,c,d,e);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard()
{
	unsigned char i,j;

	Set_Column_Address(0x00,0x3F);
	Set_Row_Address(0x00,0x5F);

	for(i=0;i<40;i++)
	{
		for(j=0;j<64;j++)
		{
			Write_Data(0xF0);
		}
		for(j=0;j<64;j++)
		{
			Write_Data(0x0F);
		}
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale()
{
	//   Level 16 => Column 1~8
		Fill_Block(0x00,0x03,0x00,0x3F,0xFF);

	//   Level 15 => Column 9~16
		Fill_Block(0x04,0x07,0x00,0x3F,0xEE);

	//   Level 14 => Column 17~24
		Fill_Block(0x08,0x0B,0x00,0x3F,0xDD);

	//   Level 13 => Column 25~32
		Fill_Block(0x0C,0x0F,0x00,0x3F,0xCC);

	//   Level 12 => Column 33~40
		Fill_Block(0x10,0x13,0x00,0x3F,0xBB);

	//   Level 11 => Column 41~48
		Fill_Block(0x14,0x17,0x00,0x3F,0xAA);

	//   Level 10 => Column 49~56
		Fill_Block(0x18,0x1B,0x00,0x3F,0x99);

	//   Level 9 => Column 57~64
		Fill_Block(0x1C,0x1F,0x00,0x3F,0x88);

	//   Level 8 => Column 65~72
		Fill_Block(0x20,0x23,0x00,0x3F,0x77);

	//   Level 7 => Column 73~80
		Fill_Block(0x24,0x27,0x00,0x3F,0x66);

	//   Level 6 => Column 81~88
		Fill_Block(0x28,0x2B,0x00,0x3F,0x55);

	//   Level 5 => Column 89~96
		Fill_Block(0x2C,0x2F,0x00,0x3F,0x44);

	//   Level 4 => Column 97~104
		Fill_Block(0x30,0x33,0x00,0x3F,0x33);

	//   Level 3 => Column 105~112
		Fill_Block(0x34,0x37,0x00,0x3F,0x22);

	//   Level 2 => Column 113~120
		Fill_Block(0x38,0x3B,0x00,0x3F,0x11);

	//   Level 1 => Column 121~128
		Fill_Block(0x3C,0x3F,0x00,0x3F,0x00);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Frame()
{
	GA_Option(0x00);
	Draw_Rectangle(0x00,0x3F,0x00,0x3F,0xFF);
	Fill_Block(0x00,0x00,0x01,0x3E,0x0F);
	Fill_Block(0x3F,0x3F,0x01,0x3E,0xF0);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned int i,j;

	switch(a)
	{
		case 0:
			for(i=0;i<80;i+=b)
			{
				Set_Start_Line(i);
				for(j=0;j<c;j++)
				{
					ss_lcd_nh012864_delay_ns(200);
				}
			}
			break;
		case 1:
			for(i=0;i<80;i+=b)
			{
				Set_Start_Line(80-i);
				for(j=0;j<c;j++)
				{
					ss_lcd_nh012864_delay_ns(200);
				}
			}
			break;
	}
	Set_Start_Line(0x00);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	GA_Option(0x03);
	Write_Command(0x26);			// Horizontal Scroll Setup
	Write_Command(a);
	Write_Command(b);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	ss_lcd_nh012864_delay_ms(d);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Fade Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	GA_Option(0x01);
	Write_Command(0x26);			// Horizontal Scroll Setup
	Write_Command(a);
	Write_Command(b);
	Write_Command(c);
	Write_Command(0x2F);			// Activate Scrolling
	ss_lcd_nh012864_delay_ms(d);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll()
{
	Write_Command(0x2E);			// Deactivate Scrolling
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In()
{
	unsigned int i;

	Set_Display_On_Off(0x01);
	for(i=0;i<(BRIGHTNESS+1);i++)
	{
		Set_Contrast_Current(i);
		ss_lcd_nh012864_delay_ns(200);
		ss_lcd_nh012864_delay_ns(200);
		ss_lcd_nh012864_delay_ns(200);
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out()
{
	unsigned int i;

	for(i=(BRIGHTNESS+1);i>0;i--)
	{
		Set_Contrast_Current(i-1);
		ss_lcd_nh012864_delay_ns(200);
		ss_lcd_nh012864_delay_ns(200);
		ss_lcd_nh012864_delay_ns(200);
	}
	Set_Display_On_Off(0x00);
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void OLEDSleep(unsigned char a)
{
	switch(a)
	{
		case 0:
			Set_Display_On_Off(0x00);
			Set_Display_Mode(0x01);
			break;
		case 1:
			Set_Display_Mode(0x00);
			Set_Display_On_Off(0x01);
			break;
	}
}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table()
{
	Write_Command(0xB8);			// Set Gray Scale Table
	Write_Command(0x01);			//   Gray Scale Level 1
	Write_Command(0x11);			//   Gray Scale Level 3 & 2
	Write_Command(0x22);			//   Gray Scale Level 5 & 4
	Write_Command(0x32);			//   Gray Scale Level 7 & 6
	Write_Command(0x43);			//   Gray Scale Level 9 & 8
	Write_Command(0x54);			//   Gray Scale Level 11 & 10
	Write_Command(0x65);			//   Gray Scale Level 13 & 12
	Write_Command(0x76);			//   Gray Scale Level 15 & 14
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
static void nh012684_soft_init(void)
{
	ss_lcd_nh012864_rest_low(&g_sstLCDNH012864Info);
	ss_lcd_nh012864_delay_ms(200);						// Reset 200ms
	ss_lcd_nh012864_rest_high(&g_sstLCDNH012864Info);

	Set_Display_On_Off(0x00);			// Display Off (0x00/0x01)
	Set_Display_Clock(0x91);			// Set Clock as 135 Frames/Sec
	Set_Multiplex_Ratio(0x3F);			// 1/64 Duty (0x0F~0x5F)
	Set_Display_Offset(0x4C);			// Shift Mapping RAM Counter (0x00~0x5F)
	Set_Start_Line(0x00);				// Set Mapping RAM Display Start Line (0x00~0x5F)
	Set_Master_Config(0x00);			// Disable Embedded DC/DC Converter (0x00/0x01)
	Set_Remap_Format(0x50);				// Set Column Address 0 Mapped to SEG0
										//     Disable Nibble Remap
										//     Horizontal Address Increment
										//     Scan from COM[N-1] to COM0
										//     Enable COM Split Odd Even
	Set_Current_Range(0x02);			// Set Full Current Range
	Set_Gray_Scale_Table();				// Set Pulse Width for Gray Scale Table
	Set_Contrast_Current(BRIGHTNESS);	// Set Scale Factor of Segment Output Current Control
	Set_Frame_Frequency(0x46);			// Set Frame Frequency
	Set_Phase_Length(0x22);				// Set Phase 1 as 2 Clocks & Phase 2 as 2 Clocks
	Set_Precharge_Voltage(0x10);			// Set Pre-Charge Voltage Level
	Set_Precharge_Compensation(0x20,0x07);	// Set Pre-Charge Compensation
	Set_VCOMH(0x02);					// Set High Voltage Level of COM Pin
	Set_VSL(0x0C);						// Set Low Voltage Level of SEG Pin
	Set_Display_Mode(0x00);				// Normal Display Mode (0x00/0x01/0x02/0x03)

	Fill_RAM(0xFF);						// Clear Screen

	Set_Display_On_Off(0x01);			// Display On (0x00/0x01)
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//功能：在坐标（X,Y）出绘制一个点
//输入：
//		u32X ------- 点的横坐标，0~127
//		u32Y ------- 点的纵坐标，0~63
//		u8Data ---- 点的灰度值，低四位有效。
//输出:无
void ss_lcd_nh012864_draw_point(uint32_t u32X, uint32_t u32Y, uint8_t u8Constrast)
{
	uint8_t u8Temp;
	uint8_t u8Col;
	uint8_t u8Row;
	uint8_t u8X;

	u8X = (uint8_t)u32X;
	u8Col = u32X >> 1;
	u8Row = (uint8_t)u32Y;

	Set_Column_Address(u8Col, 0x3F);
	Set_Row_Address(u8Row, 0x3F);

	u8Temp = Read_Data();

	if(u8X & 0x01)
	{
		u8Temp &= 0x0F;
		u8Temp |= (u8Constrast << 4);
	}
	else
	{
		u8Temp &= 0xF0;
		u8Temp |= (u8Constrast & 0x0F);
	}

	Set_Column_Address(u8Col, 0x3F);
	Set_Row_Address(u8Row, 0x3F);
	Write_Data(u8Temp);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//功能：将屏幕全屏填充
//输入：
//		u8Constrast ---- 填充时每个像素点的灰度值，低四位有效
void ss_lcd_nh012864_fill_screen(uint8_t u8Constrast)
{
	Fill_RAM(((u8Constrast & 0x0F) | (u8Constrast << 4)));
//	Fill_RAM(u8Data);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//功能：在坐标（X，Y）处绘制一个固定宽度的矩形区域
//取模方式：横向模式，逐行式，顺序左上->右上->左下->右下，液晶从左到右第一点在字节的高位。
//输入：
//		u32X ------------ X坐标，必须为0、2、4、8......
//		u32Y ------------ Y坐标，0~MAX_ROW
//		*pu8Data ------- 点阵模
//		u32Width -------- 点阵宽度，单位像素。8的倍数
//		u32High --------- 点阵高度，单位像素
//		u8Constrast ---- 显示灰度，只有低4位有效
//		bRev ----------- 是否反色显。true ---- 反显；false ---- 正常显示
//
void ss_lcd_nh012864_draw_area(uint32_t u32X, uint32_t u32Y, uint8_t *pu8Data, uint32_t u32Width, uint32_t u32High, uint8_t u8Constrast, bool bRev)
{
	uint8_t u8I, u8J;
	uint8_t u8Data;
	uint8_t u8Temp;
	uint8_t u8InputData;
	uint8_t u8Cons;
	uint8_t u8X, u8Y, u8Width, u8High;

	if(!pu8Data)
		return;

	if(((MAX_COLUMN + 1) << 1) < (u32X + u32Width))
		return;

	if((MAX_ROW  + 1) < (u32Y + u32High))
		return;

	if((uint8_t)u32X & 0x01)
		return;

	if((uint8_t)u32Width & 0x07)
		return;

	u8X = (uint8_t)u32X;
	u8Y = (uint8_t)u32Y;
	u8Width = (uint8_t)u32Width;
	u8High = (uint8_t)u32High;

	u8Cons = 0x0F & u8Constrast;

	for(u8I=0; u8I<u8High; u8I++)
	{
		Set_Column_Address(u8X>>1, MAX_COLUMN);
		Set_Row_Address(u8Y+u8I, MAX_ROW);

		for(u8J=0; u8J<(u8Width >> 3); u8J++)
		{
			u8InputData = bRev? (~(*pu8Data)):(*pu8Data);
			pu8Data++;

			u8Data = (u8InputData & 0x80)? (u8Cons):(0x00);
			u8Temp = (u8InputData & 0x40)? (u8Cons << 4):(0x00);
			u8Data |= u8Temp;
			Write_Data(u8Data);

			u8Data = (u8InputData & 0x20)? (u8Cons):(0x00);
			u8Temp = (u8InputData & 0x10)? (u8Cons << 4):(0x00);
			u8Data |= u8Temp;
			Write_Data(u8Data);

			u8Data = (u8InputData & 0x08)? (u8Cons):(0x00);
			u8Temp = (u8InputData & 0x04)? (u8Cons << 4):(0x00);
			u8Data |= u8Temp;
			Write_Data(u8Data);

			u8Data = (u8InputData & 0x02)? (u8Cons):(0x00);
			u8Temp = (u8InputData & 0x01)? (u8Cons << 4):(0x00);
			u8Data |= u8Temp;
			Write_Data(u8Data);
		}
	}
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void ss_lcd_nh012864_get_info(uint32_t *pu32MaxCol, uint32_t *pu32MaxRow)
{
	if((!pu32MaxCol) || (!pu32MaxRow))
		return;

	*pu32MaxCol = (MAX_COLUMN + 1) * 2 - 1;
	*pu32MaxRow = MAX_ROW;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
#endif
