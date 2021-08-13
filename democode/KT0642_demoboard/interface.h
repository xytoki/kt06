//#define C8051F314
#define C8051F340

#ifdef C8051F314

	#include <c8051f310.h>										// SFR declarations

#endif

#ifdef C8051F340

	#include <c8051f340.h>										// SFR declarations

#endif

#define uchar unsigned char
#define ulong unsigned long
#define uint unsigned int

//#define SPI3
#define I2C

#define DEMO_RX
//#define DEMO_TX

#ifdef C8051F314
sbit SDA = P0^4;											// ����P1.0��SDA
sbit SCL = P0^5;											// ����P1.0��SCL
sbit GPIO3 = P0^6;
sbit GPIO2 = P0^7;
sbit GPIO1 = P0^1;											// Slave output, master input
sbit SW_FM = P3^1;											// SWITCH_1='1' means switch pressed
sbit SCK = P0^0;											// SPI clock
sbit MOSI = P0^2;											// Master output, slave input
sbit NSS = P0^3;											// SPI select
#endif

#ifdef C8051F340
sbit SDA = P3^0;											// ����P3.0��SDA
sbit SCL = P2^7;											// ����P2.7��SCL

sbit GPIO3 = P2^5;
sbit GPIO2 = P2^4;
sbit GPIO1 = P2^3;											// Slave output, master input
sbit SW_FM = P3^1;											// SWITCH_1='1' means switch pressed
sbit SCK = P3^2;											// SPI clock
sbit MOSI = P3^3;											// Master output, slave input
sbit NSS = P2^6;											// SPI select
#endif


#ifdef SPI3	
void SPI_Write (uchar Reg_addr, uint spi_data);
uint SPI_Read_3wire (uchar Reg_addr);
#endif

#ifdef I2C
void I2C_Word_Write(uchar device_address, uchar reg_add, uint writeword);
uint I2C_Word_Read(uchar device_address, uchar reg_add);
void I2C_Byte_Write(uchar device_address, uchar reg_add, uchar writedata);
uchar I2C_Byte_Read(uchar device_address, uchar reg_add);
#endif

//------------------------------------------------------------------------------------
//LCD�������Ŷ���
//------------------------------------------------------------------------------------
#ifdef C8051F314
sbit LCD_RS = P3^2;											// RS=1�����ݣ� RS=0��ָ��
sbit LCD_RW = P3^3;											// RW=1������ RW=0��д
sbit LCD_Enalbe = P3^4;										// L������H����ֹ
sbit LCD_Reset = P1^7;										// LCD_Reset ='1' means LCD reset
sbit LCD_BF = P2^0;											// æ�źż��
sfr LCDInterface = 0xA0;									// P2
#endif

#ifdef C8051F340
sbit LCD_RS = P2^2;											// RS=1�����ݣ� RS=0��ָ��
sbit LCD_RW = P2^1;											// RW=1������ RW=0��д
sbit LCD_Enalbe = P2^0;										// L������H����ֹ
sbit LCD_Reset = P0^7;										// LCD_Reset ='1' means LCD reset
sbit LCD_BF = P1^0;											// æ�źż��
sfr LCDInterface = 0x90;									// P1
#endif
//------------------------------------------------------------------------------------
//KEY���Ŷ���
//------------------------------------------------------------------------------------
#ifdef C8051F314
sfr KeyInterface = 0x90;									// P1
#endif

#ifdef C8051F340
sfr KeyInterface = 0x80;									// P0
#endif

//------------------------------------------------------------------------------------
//LCD��ʾ���ֺ�������
//------------------------------------------------------------------------------------
void LCD_Init(void);										// LCD��ʾ��ʼ���ӳ���//
void BUSY_Check(void);										// ���æ�ź��ӳ���
void DISPLAY_Start(void);									// ��ʾ��ӭ����
void Display1(void);										// ��һ����ʾ�ӳ���
void Display2(void);										// ��һ����ʾ�ӳ���
void Display3(void);										// ��������ʾ�ӳ���
void Display4(void);										// ��������ʾ�ӳ���
void CODE_Send(uchar data command);							// ������ӳ���
void DATA_Send(uchar data mydata);							// ���ݷ����ӳ���
void DELAY_Write(void);										// д����ʱ
void DELAY_Display(void);									// ��ʾ��ӭ������ʱ
uchar BYTE_Reversal(uchar Reversal_data);					// 1�ֽڵ�8Ϊ��ת����
void LCD_Number_Cal (uint LCD_Number);
void LCD_RSSI_Display (uchar RSSI_Number);
uchar HexToASCII(uint num);
uchar KEY_Scan (void);
void dlms(void);											// ����ɨ��ȥ������ʱ����
void delay(void);											// ɨ����LCD��ʱ
void Display_Channel (uint LCD_Number);
void Delay_ms(uint time_number);

