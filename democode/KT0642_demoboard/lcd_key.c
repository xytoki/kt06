//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <interface.h>										// SFR declarations
#include <intrins.h>
#include <stdio.h>

extern uchar IAL;
extern uchar TRL;
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
uchar code LCD_Welcome1[8]={0x57,0x65,0x6c,0x63,0x6f,0x6d,0x65,0x20};												// Welcome
#ifdef DEMO_RX
uchar code LCD_Welcome2[14]={0x46,0x4D,0x2D,0x52,0x65,0x63,0x65,0x69,0x76,0x65,0x72,0x20,0x20,0x20};				// FM-Receiver
#endif 
#ifdef DEMO_TX
uchar code LCD_Welcome2[14]={0x46,0x4D,0x2D,0x54,0x72,0x61,0x6e,0x73,0x6d,0x69,0x74,0x74,0x65,0x72};				// FM-Transmitter
#endif
uchar code LCD_Welcome3[16]={0x20,0x20,0x20,0x20,0x4B,0x54,0x20,0x4D,0x69,0x63,0x72,0x6F,0x20,0x20,0x20,0x20};		// KT Micro
uchar code LCD_Welcome4[10]={0x32,0x30,0x30,0x38,0x2E,0x20,0x37,0x2E,0x20,0x20};									// 2008��7��
uchar code LCD_Frequency_Title[6]={0x46,0x72,0x65,0x2E,0xA1,0xC3};						// Frequency��
uchar code LCD_Number[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};			// 0,1,2,3,4,5,6,7,8,9
uchar code LCD_RSSI_Title[6]={0x52,0x53,0x53,0x49,0xA1,0xC3};						// RSSI
uchar code LCD_Rfgain_Title[8]={0x50,0x41,0x20,0x47,0x61,0x69,0x6e,0x3a};				// PA Gain��
uchar code LCD_PGA_Title[8]={0x41,0x75,0x20,0x47,0x61,0x69,0x6e,0x3a};		// AudioGain:
uchar code LCD_PGA[22]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x31,0x30,0x31,0x31,0x31,0x32,0x31,0x33,0x31,0x34,0x31,0x35};	//0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
uchar code LCD_PETCS_Title[8]={0x50,0x72,0x65,0x2d,0x65,0x6d,0x70,0x3a};				// Pre-emp:
uchar code LCD_PETCS[4]={0x37,0x35,0x35,0x30};											// 75,50


uchar HexToASCII(uint num)
{
		 switch (num&0x0f){
		 	case 0x00: {return(0x30);}break;
		 	case 0x01: {return(0x31);}break;
		 	case 0x02: {return(0x32);}break;
		 	case 0x03: {return(0x33);}break;
		 	case 0x04: {return(0x34);}break;
		 	case 0x05: {return(0x35);}break;
		 	case 0x06: {return(0x36);}break;
		 	case 0x07: {return(0x37);}break;
		 	case 0x08: {return(0x38);}break;
		 	case 0x09: {return(0x39);}break;
		 	case 0x0a: {return(0x41);}break;
		 	case 0x0b: {return(0x42);}break;
		 	case 0x0c: {return(0x43);}break;
		 	case 0x0d: {return(0x44);}break;
		 	case 0x0e: {return(0x45);}break;
		 	case 0x0f: {return(0x46);}break;
		 }
}
//------------------------------------------------------------------------------------
// KEY_Scan
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
//��ֵ��
//Next	: 0x48,01001000		Previous: 0x28,00101000		Autoscan: 0x18,00011000
//Up	: 0x44,01000100		Down	: 0x24,00100100		RDS		: 0x14,00010100
//3D	: 0x42,01000010		Bass	: 0x22,00100010		S/M		: 0x12,00010010
//EQU	: 0x41,01000001		Save	: 0x21,00100001		Mute	: 0x11,00010001
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// ����ɨ��ȥ������ʱ����
//------------------------------------------------------------------------------------
#ifdef C8051F340
void dlms(void)												// ����ɨ��ȥ������ʱ����
{
	unsigned int i,j;
	for (i=0x1000;i>0;i--)
		{
		 for (j=7;j>0;j--);
		}
}
#endif


//------------------------------------------------------------------------------------
// ����ɨ�����
//------------------------------------------------------------------------------------
uchar KEY_Scan (void)
{
	uchar sccode,recode;
	KeyInterface=0xF0;
	delay();												// ��ɨ���ź�ȫ��0
	if((KeyInterface&0xF0)!=0xF0)										// �ж��Ƿ��м�����
		{
		 delay();
		 dlms();											// ��ʱȥ����
		 delay();
		 if ((KeyInterface&0xF0)!=0xF0)								// ���м�����
			{
			 delay();
			 sccode=0xFE;									// ����ɨ���ֵ
			 delay();
			 while((sccode&0x10)!=0)
				{delay();
				 KeyInterface=sccode;									// �����ɨ����
				 delay();
				 if((KeyInterface & 0xF0) != 0xF0)					// ��������м�����
					{
					 delay();
					 recode=(KeyInterface & 0xF0) | 0x0F;				// �����ɨ����
					 delay();
					 return ((~sccode)+(~recode));			// �����ֵ�������ؼ�ֵ
					 delay();
					}
				 else sccode=(sccode<<1)|0x01;				// ��������޼����£���ɨ��һ��
				}
			}
		}
	return(0);												// �޼����£����ؼ�ֵΪ0
}

#ifdef C8051F314

void Delay_ms(uint time_number)
{
	uint i;
	uint j;

	for (j=0;j<time_number;j++)
	{
			for (i=0;i<0x900;i++)
		{
		}
	}
}

#endif

#ifdef C8051F340

void Delay_ms(uint time_number)
{
	uint i;
	uint j;

	for (j=time_number;j>0;j--)
	{
			for (i=0x1118;i>0;i--)
		{
		}
	}
}
/**/
#endif
//------------------------------------------------------------------------------------
// delay
//------------------------------------------------------------------------------------

void delay(void	)		//<<<�޸�11
{
	uchar i;	//<<<�޸�14
	for(i=0;i<=1;i++)
		{
		 _nop_();
		}
}

//------------------------------------------------------------------------------------
// LCD��ʾ����
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// LCD��ʾ��ʼ���ӳ���
//------------------------------------------------------------------------------------
void LCD_Init(void)
{
	LCD_Reset=0;
	delay();												// LCD��ʼ����ʱ
	LCD_Reset=1;											// ��ֹ��λ
	LCD_RW=0;												// д��ģʽ
	LCD_Enalbe=0;
	CODE_Send(0x30);										// ���书���趨��RE=0����ָ�������G=0��ͼ��ʾ��
	CODE_Send(0x01);										// �����ʾ
	CODE_Send(0x06);										// ���趨��I/D=1������ơ�S=0 DDRAMΪ��״̬��������ʾ���ƶ�
	CODE_Send(0x0c);										// ��ʾ״̬D=������ʾ����C=�α�ء�B=�α�λ�ù�
	CODE_Send(0x01);										// �����ʾ
	DISPLAY_Start();										// ��ʾ��ӭ����
	DELAY_Display();										// ��ʾ��ӭ������ʱ
	CODE_Send(0x01);										// �����ʾ
//	Display1();												// ��һ����ʾ�ӳ���
//	Display2();												// �ڶ�����ʾ�ӳ���
//	Display3();												// ��������ʾ�ӳ���
//	Display4();												// ��������ʾ�ӳ���
}

void Display_Start(void)
{
	uchar wide,offset;										// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x82);
	for(wide=0;wide<4;wide++)								// ��ӭ�����һ�У���ʾ����ӭʹ��
		{
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_Welcome1[offset]);
		 DATA_Send(LCD_Welcome1[offset+1]);
		}

	CODE_Send(0x90);
	for(wide=0;wide<7;wide++)								// ��ӭ����ڶ��У���ʾ����Ƶ�����ϵͳ  
		{
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_Welcome2[offset]);
		 DATA_Send(LCD_Welcome2[offset+1]);
		}

	CODE_Send(0x88);
	for(wide=0;wide<8;wide++)								// ��ӭ��������У���ʾ��KT Micro���׿�
		{
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_Welcome3[offset]);
		 DATA_Send(LCD_Welcome3[offset+1]);
		}

	CODE_Send(0x9a);
	for(wide=0;wide<5;wide++)								// ��ӭ��������У���ʾ��2007��11��
		{	
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_Welcome4[offset]);
		 DATA_Send(LCD_Welcome4[offset+1]);
		}
}

/*
//------------------------------------------------------------------------------------
//��һ����ʾ�ӳ���
//------------------------------------------------------------------------------------

void Display1(void)
{
	uchar wide,offset;										// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x80);										// Ƶ�ʣ�
	for(wide=0;wide<3;wide++)
		{
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_Frequency_Title[offset]);
		 DATA_Send(LCD_Frequency_Title[offset+1]);
		}
   DATA_Send(0x2E);										// .
   DATA_Send(0x2E);										// .
   DATA_Send(0x2E);										// .
   DATA_Send(0x2E);										// .
   DATA_Send(0x2E);										// .
	DATA_Send(0x4D);										// M
	DATA_Send(0x48);										// H
	DATA_Send(0x7A);										// z
}

//------------------------------------------------------------------------------------
//�ڶ�����ʾ�ӳ���
//------------------------------------------------------------------------------------
#ifdef DEMO_RX
void Display2(void)   
{
	uchar wide,offset;										// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x90);										// ��Ƶ���棺
	for(wide=0;wide<5;wide++)
		{
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_PGA_Title[offset]);
		 DATA_Send(LCD_PGA_Title[offset+1]);
		}
//	offset=IAL*3;											// LCD��ʾ����ÿ3λ1��
	CODE_Send(0x94);										// ��ʾ��xxx��-12-12��dB
	if(IAL<10)
		{
		DATA_Send(0x20);
		DATA_Send(LCD_PGA[IAL]);
		}
	else
		{
		DATA_Send(LCD_PGA[IAL/10]);
		DATA_Send(LCD_PGA[IAL%10]);
		}
//		DATA_Send(LCD_PGA[offset+1]);
//		DATA_Send(LCD_PGA[offset+2]);
//		DATA_Send(0x64);
//		DATA_Send(0x42);
}
#endif
#ifdef DEMO_TX
void Display2(void)
{
	uchar offset;											// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x90);										// ���书�ʣ�
	for(offset=0;offset<8;offset++)
	{
		DATA_Send(LCD_Rfgain_Title[offset]);
	}

	CODE_Send(0x94);										// ��ʾ��x��1-15��level
	DATA_Send( (TRL/10)+ 0x30);
	DATA_Send( (TRL%10)+ 0x30);
//	DATA_Send(0x4C);
//	DATA_Send(0x65);
//	DATA_Send(0x76);
//	DATA_Send(0x65);
//	DATA_Send(0x6C);
}
#endif
//------------------------------------------------------------------------------------
//��������ʾ�ӳ���	RSSI
//------------------------------------------------------------------------------------
#ifdef DEMO_RX
void Display3(void)
{
	uchar wide,offset;										// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x88);										// ���书�ʣ�
	for(wide=0;wide<3;wide++)
		{
		 offset=wide<<1;									// LCD��ʾ����ÿ2λ1��
		 DATA_Send(LCD_RSSI_Title[offset]);
		 DATA_Send(LCD_RSSI_Title[offset+1]);
		}
}
#endif
#ifdef DEMO_TX
void Display3(void)   
{
	uchar offset;											// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x88);										// ��Ƶ���棺
	for(offset=0;offset<8;offset++)
	{
		DATA_Send(LCD_PGA_Title[offset]);
	}

	CODE_Send(0x8C);										// ��ʾ-/+xx
	if(IAL<15)
	{
		DATA_Send('-');
		DATA_Send( (15-IAL)/10 + 0x30 );
		DATA_Send( (15-IAL)%10 + 0x30 );
	}
	else if(IAL==15)
	{
		DATA_Send(' ');
		DATA_Send(' ');
		DATA_Send('0');
	}
	else if(IAL>15)
	{
		DATA_Send('+');
		DATA_Send( (IAL-15)/10 + 0x30  );
		DATA_Send( (IAL-15)%10 + 0x30  );
	}

	DATA_Send(0x64);
	DATA_Send(0x42);
}
#endif
//------------------------------------------------------------------------------------
//��������ʾ�ӳ���
//------------------------------------------------------------------------------------

#ifdef DEMO_RX
void Display4(void)											// ��ʾ��Ԥ���غ;���
{
#ifdef TST
	uint reg12;
	reg12 = KT_Bus_Read(0x12);
	reg12 = reg12 &	0x0200;
	if (reg12 == 0x0200)
		{
		 CODE_Send(0x98);										// LCD��ʾ"S"����
		 DATA_Send(0x53);
		 DATA_Send(0x74);
		 DATA_Send(0x65);
		 DATA_Send(0x72);
		 DATA_Send(0x65);
		 DATA_Send(0x6f);
		}
	else
		{
		 CODE_Send(0x98);										// LCD��ʾ"M"����
		 DATA_Send(0x4D);
		 DATA_Send(0x6f);
		 DATA_Send(0x6e);
		 DATA_Send(0x6f);
		 DATA_Send(0x20);
		 DATA_Send(0x20);
		}

#else
	CODE_Send(0x98);										// LCD��ʾ"S"����
	DATA_Send(0x53);
	DATA_Send(0x74);
	DATA_Send(0x65);
	DATA_Send(0x72);
	DATA_Send(0x65);
	DATA_Send(0x6f);

	CODE_Send(0x9C);										// ��ʾB0-B3
	DATA_Send(0x42);
	DATA_Send(0x41);
	DATA_Send(0x53);
	DATA_Send(0x53);
	DATA_Send(BASS+0x30);

#endif
	CODE_Send(0x9F);
	DATA_Send(0x0E);										//��ʾ���ַ���
}
#endif
#ifdef DEMO_TX
void Display4(void)											// ��ʾ��Ԥ���غ;���
{
//	uchar offset;											// �����ַ�/������ʾ���ȣ�������ʾƫ����
	CODE_Send(0x98);										// Ԥ���أ�
//	for(offset=0;offset<8;offset++)
// 	{
//		DATA_Send(LCD_PETCS_Title[offset]);
//	}

	if( (I2C_Byte_Read(KT0806r_address, 0x02) & 0x01) == 0)	// ���Ԥ���ر�־λ�������־λΪ�͵�ƽ˵��Ԥ����Ϊ75us
	{
		DATA_Send('7');
		DATA_Send('5');
	}
	else													// ���Ԥ���ر�־λ�������־λΪ�ߵ�ƽ˵��Ԥ����Ϊ50us
	{
		DATA_Send('5');
		DATA_Send('0');
	}
	DATA_Send('u');
	DATA_Send('s');

	CODE_Send(0x9B);										// LCD��ʾ��BASS��
	DATA_Send('B');
	DATA_Send('A');
	DATA_Send('S');
	DATA_Send('S');
	DATA_Send(BASS_LEVEL+ 0x30);

	CODE_Send(0x9F);
	if( (I2C_Byte_Read(KT0806r_address, 0x04) & 0x40) == 0)	// ���Mono��־λ������ǵ�ǰ������������ʾ��ST��
	{
		DATA_Send('S');
		DATA_Send('T');

	}
	else													// ���Mono��־λ������ǵ�ǰ�ǵ���������ʾ�� M��
	{
		DATA_Send(' ');
		DATA_Send('M');
	}
}
#endif
*/
//------------------------------------------------------------------------------------
// ��ʾ��ʱ
//------------------------------------------------------------------------------------
void DELAY_Display(void)									// ��ʾ��ӭ������ʱ
{
	unsigned int i,j;
	for (i=0x2000;i>0;i--)
		{
		 for (j=50;j>0;j--);
		}
}

//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void DELAY_Write(void)										// ��ʾ��ӭ������ʱ
{
	uchar i;
	for(i=0;i<100;i++)
    {}
}

//------------------------------------------------------------------------------------
// ���ݷ����ӳ���
//------------------------------------------------------------------------------------
void DATA_Send(uchar data mydata)
{
	BUSY_Check();											// ����Ƿ���æ״̬
	DELAY_Write();
	LCDInterface=BYTE_Reversal(mydata);								// д��mydata���������
	DELAY_Write();
	LCD_RS=1;												// ����ģʽ
	DELAY_Write();
	LCD_RW=0;												// д��ģʽ
	DELAY_Write();
	LCD_Enalbe=1;											// ����д��
	DELAY_Write();
	LCD_Enalbe=0;											// ��ֹд��
	DELAY_Write();
	LCDInterface=0xff;												// P2����ȫ1
}
//------------------------------------------------------------------------------------
// ������ӳ���
//------------------------------------------------------------------------------------
void CODE_Send(uchar data command)
{
	BUSY_Check();											// ����Ƿ���æ״̬
	DELAY_Write();
	LCDInterface=BYTE_Reversal(command);								// д��command�����ָ����
	DELAY_Write();
	LCD_RS=0;												// ָ��ģʽ
	DELAY_Write();
	LCD_RW=0;												// д��ģʽ
	DELAY_Write();
	LCD_Enalbe=1;											// ����д��
	DELAY_Write();
	LCD_Enalbe=0;											// ��ֹд��
	DELAY_Write();
	LCDInterface=0xff;												// P2����ȫ1

}
//------------------------------------------------------------------------------------
// ���æ�ź��ӳ���
//------------------------------------------------------------------------------------
void BUSY_Check(void)										// æ�źż���ӳ���
{
	DELAY_Write();
	LCD_RS=0;												// ָ��ģʽ
	DELAY_Write();
	LCD_RW=1;												// ����ģʽ
	DELAY_Write();
	LCD_Enalbe=1;											// �������
	DELAY_Write();
	while(LCD_BF){;}										// �ȴ���ֱ����æΪֹ
	DELAY_Write();
	LCD_Enalbe=0;											// ��ֹ����
	DELAY_Write();
}
//------------------------------------------------------------------------------------
// 1Byte ��ת����
//------------------------------------------------------------------------------------
uchar BYTE_Reversal(uchar Reversal_data)
{
	uchar temp_data=0;
	bit Reversal_data_0=0;
	bit Reversal_data_1=0;
	bit Reversal_data_2=0;
	bit Reversal_data_3=0;
	bit Reversal_data_4=0;
	bit Reversal_data_5=0;
	bit Reversal_data_6=0;
	bit Reversal_data_7=0;

	Reversal_data_0 = Reversal_data & 0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_1 = (Reversal_data)&0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_2 = (Reversal_data)&0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_3 = (Reversal_data)&0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_4 = (Reversal_data)&0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_5 = (Reversal_data)&0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_6 = (Reversal_data)&0x01;
	Reversal_data=Reversal_data>>1;
	Reversal_data_7 = (Reversal_data)&0x01;

	temp_data=temp_data | Reversal_data_0;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_1;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_2;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_3;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_4;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_5;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_6;

	temp_data=temp_data<<1;
	temp_data=(temp_data) | Reversal_data_7;

	return (temp_data);
}
//-----------------------------------------------------------------------------
// LCD_RSSI_Number_Calculator
//-----------------------------------------------------------------------------
//
//
// 
//
/*
void LCD_RSSI_Display (uchar RSSI_Number)
{
	uchar RSSI_Hundred_Remainder,RSSI_Hundred,RSSI_Ten_Remainder,RSSI_Ten;
	uchar data RSSI_high,RSSI_mid,RSSI_low;


	RSSI_Hundred_Remainder = RSSI_Number % 100;
	RSSI_Hundred = RSSI_Number / 100;
	RSSI_high = 0x30 + RSSI_Hundred;

	RSSI_Ten_Remainder = RSSI_Hundred_Remainder % 10;
	RSSI_Ten = RSSI_Hundred_Remainder / 10;
	RSSI_mid = 0x30 + RSSI_Ten;
	RSSI_low = 0x30 + RSSI_Ten_Remainder;
	 CODE_Send(0x8B);										// ��ʾ��-XXX
	 DATA_Send(0x2D);//"-"
	 DATA_Send(RSSI_high);//
	 DATA_Send(RSSI_mid);
	 DATA_Send(RSSI_low);

}


void Display_Channel (uint LCD_Number)
{
	uint HundredMHz_Remainder,TenMHz_Remainder,MHz_Remainder,HundredKHz_Remainder;
	uchar HundredMHz,TenMHz,MHz,HundredKHz;

	HundredMHz_Remainder = LCD_Number % 10000;
	HundredMHz = LCD_Number / 10000;

	TenMHz_Remainder = HundredMHz_Remainder % 1000;
	TenMHz = HundredMHz_Remainder / 1000;

	MHz_Remainder = TenMHz_Remainder % 100;
	MHz = TenMHz_Remainder / 100;

	HundredKHz_Remainder = MHz_Remainder % 10;
	HundredKHz = MHz_Remainder / 10;

	CODE_Send(0x83);										// ��ʾxxx.x
	if(HundredMHz == 0)
		DATA_Send(0x20);
	else
		DATA_Send(0x30 + HundredMHz);
	DATA_Send(0x30 + TenMHz);
	DATA_Send(0x30 + MHz);
	DATA_Send(0x2E);
	DATA_Send(0x30 + HundredKHz);
//	DATA_Send(0x30 + HundredKHz_Remainder);
}
 */