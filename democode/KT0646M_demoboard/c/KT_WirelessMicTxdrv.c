//*****************************************************************************
//  File Name:    KT_WirelessMicTxdrv.c
//  Function:    KT Wireless Mic Transmitter Products Driver For Customer
//                (KT064xM)
//*****************************************************************************
//        Revision History
//  Version Date        Description
//  V0.1    2016-04-26  For KT0646M_VX
//  V0.2    2016-06-01  ����VX��VX2�ı���ѡ��    
//  V0.3    2016-08-18  For KT0646M_VX4
//  V0.3.1  2016-08-24  ɾ��KT_WirelessMicTx_Init������һЩ�ظ�����
//  V0.4    2016-08-26  ������XTAL_DUAL����
//  V0.5    2016-10-11  �޸��˳�ʼ�������е�һЩ���ã���0x25,0x26�Ĵ��������üӵ��˳�ʼ���������棬
//                  �ڳ�ʼ����������set pilot frequency
//  V0.5.1  2016-10-17  �����޸���һ������ĺ궨�壬������RXISKT0616M����BPSK�ľ�ģʽ��Ȼ���ٸ���
//                  �Ƿ���˫�����KT0616Mѡ����ĳ���ӽ�ȥ��
//  V0.5.2  2016-11-15  ��tunęǰ��lo_fine_vref_sel��Ϊ3��tune���ĳ�0.
//  V0.5.3  2016-12-15  �ڳ�ʼ���а�lo_fine_vref_sel��Ϊ3��������tunę��ʱ���������޸���.Ϊ���¶�
//                  �仯�����������⣬�������DOUBLE_KVCO��Χ�Ĵ���
//  V0.6    2017-02-08  ��ʽ�淶������
//  V1.1    2017-04-27  ɾ����һЩ��ʼ����������û�õĶ���������Щ��Сд��ĸ����ĺ궨��ĳ��˴�д����ĸ
//  V1.2    2017-05-24  ��ʼ���У���HARD_LIMIT��14�ĳ���15��COMPANDOR_TC�ĳ���3����48ms��
//  V1.3    2017-09-18	COMPEN_GAIN��3�ĳ�1,MIC_SENS_GAIN��9�ĳ�5,COMPANDOR_TC_48ms�ĳ�COMPANDOR_TC_12ms
//  V1.4    2017-10-10  echo�رյ�ʱ�򣬲�������echo���ǰ�Echo_Ratioд��0,soft_rst�����ڼĴ���0x3e��bit15�ˣ�������0x1e��bit4.
//  V1.5    2017-12-14  ���ݽ�����KT0616Mʱ����BPSK_NEW_MODE�ĳ�0������Ϊ1
//*****************************************************************************

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "KT_WirelessMicTxdrv.h"
#include <math.h>

//-----------------------------------------------------------------------------
//�� �� ����KT_Bus_Write                                                             
//��������������д����                                                                
//����˵����                                                                        
//ȫ�ֱ�������                                                                        
//��    �룺UINT8 Register_Address, UINT16 Word_Data                                    
//��    �أ���                                                                        
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
void KT_Bus_Write(UINT8 Register_Address, UINT16 Word_Data)
{
    I2C_Word_Write(KTWirelessMicTxw_address,Register_Address,Word_Data);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_Bus_Read                                                                 
//�������������߶�����                                                                
//����˵����                                                                        
//ȫ�ֱ�������                                                                        
//��    �룺UINT8 Register_Address                                                    
//��    �أ�I2C_Word_Read(KTWirelessMicTxr_address, Register_Address)                
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
UINT16 KT_Bus_Read(UINT8 Register_Address)
{
    return( I2C_Word_Read(KTWirelessMicTxr_address, Register_Address) );
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_PreInit                                                 
//����������оƬ����ʼ������                                                        
//����˵�����ж��Ƿ�оƬ�Ƿ������ϵ磬I2C�����Ƿ��д����                            
//ȫ�ֱ�����INIT_FAIL_TH                                                            
//��    �룺��                                                                        
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_PreInit(void)              
{
    UINT16 regx;
    UINT8 i;

    for (i = 0; i < INIT_FAIL_TH; i++)
    {
        regx = KT_Bus_Read(0x01); //Read Manufactory ID 
        Delay_ms(10);
          if (regx == 0x4B54)
            return(1);
    }
    return(0);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Init                                                     
//����������оƬ��ʼ������                                                            
//����˵����                                                                        
//ȫ�ֱ�������                                                                        
//��    �룺��                                                                        
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-26                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26                                         
//��    ����V0.4  
//��    ����V1.2    HARD_LIMIT��14�ĳ���15��COMPANDOR_TC�ĳ���3����48ms                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_Init(void)
{
    UINT16 regx;

    Delay_ms(50);

    regx = KT_Bus_Read(0x03);
    KT_Bus_Write(0x03, (regx & 0xfffe) | PA_SEL);

    regx = KT_Bus_Read(0x24);
    KT_Bus_Write(0x24, (regx & 0xEFFF) | (BATTERY_METER_ENABLE << 12));

    //ref_vtr_vth_sel = 1
    regx=KT_Bus_Read(0x47);                        
    KT_Bus_Write(0x47,(regx | 0x0200));

    //vref_mon_en=1
    regx = KT_Bus_Read(0x0a);
    KT_Bus_Write(0x0a, regx | 0x0200);            

    regx = KT_Bus_Read(0x1C);
    KT_Bus_Write(0x1C, (regx & 0xF1E1) | ( FDEV_MONITOR_TC_250ms << 10 ) | (COMPANDOR_ENABLE<<4) | 
                 ( PRE_EMPHASIS_ENABLE << 9 ) | ( COMPANDOR_TC_12ms << 1 ));

    KT_WirelessMicTx_Mic_Sens(MIC_SENS_GAIN_5);        

    //cic overflow detect enable
    regx = KT_Bus_Read(0x1e); 
    KT_Bus_Write(0x1e, (regx & 0xffdf)|(1<<5)); 

    regx=KT_Bus_Read(0x30);
    KT_Bus_Write(0x30, ( regx & 0x8000 ) | (AGC_VHIGH << 11) | (AGC_VLOW << 7) | (AGC_ATTACK << 4) | 
                 (AGC_RELEASE << 1) | AGC_DIS );
 
    regx=KT_Bus_Read(0x31);
    KT_Bus_Write( 0x31, ( regx & 0xff00 ) | (GAIN_SEL << 6) | (COMPEN_GAIN << 4) | 
                  (BLANK_EN << 3) | BLANK_TIME );

    regx=KT_Bus_Read(0x39);
    KT_Bus_Write(0x39,(regx & 0xFC00) | (HARD_LIMIT << 6) | (CPRS_1XLPF_BP << 5) | 
                 (CPRS_KNEE_DIS << 4) | CPRS_THRSH);

    KT_Bus_Write( 0x3a,(ALC_DIS << 15) | (ALC_SOFTKNEE << 14) | (ALC_VMAX << 7) | 
                  (ALC_ATTACK << 4) | ALC_RELEASE );

	#ifdef SILENCE_MUTE
    regx=KT_Bus_Read(0x25);
    KT_Bus_Write( 0x25,(regx&0xc000)|(SLNC_MUTE_DIS << 13) | (SLNC_MUTE_TIME << 8) | 
                 (SLNC_MUTE_LOW_LEVEL << 4) | SLNC_MUTE_HIGH_LEVEL );
    regx=KT_Bus_Read(0x26);
    KT_Bus_Write( 0x26,(regx&0xfffb)|(SILENCE_MUTE_ACT_MCU << 2));
	#endif

    #ifdef AUX_CH
        regx=KT_Bus_Read(0x1f);
        KT_Bus_Write(0x1f,(regx&0x80f0)|(AUXDATA_EN << 14)|(AUX_REG_NUM << 12) |
                     (AUX_CARRY_NUM << 9)|BPSK_NEW_MODE); 
        KT_Bus_Write(0x20,(AUX_ADDRB << 8) | AUX_ADDRA);
        KT_Bus_Write(0x21,(AUX_ADDRD << 8) | AUX_ADDRC);    
    #endif

    #ifdef OTHER_RX
        #ifdef XTAL_24M_ONLY
        KT_WirelessMicTx_Set_Pilot_Freq(XTAL_24M_FREQ); //set pilot frequency
        #endif
        #ifdef XTAL_24P576M_ONLY
        KT_WirelessMicTx_Set_Pilot_Freq(XTAL_24P576M_FREQ); //set pilot frequency
        #endif
    #endif

    //for VCO unlock because of temperature change
    regx=KT_Bus_Read(0x10);
    KT_Bus_Write(0x10,(regx|0x0010)); //vco_ldo_calicode=1.4v
    regx=KT_Bus_Read(0x45);
    KT_Bus_Write(0x45,(regx&0xfff0|0x0007));

    regx=KT_Bus_Read(0x2d);
    KT_Bus_Write(0x2d,(regx&0xfff8)|3); //lofine_vref_sel=0.4v

    #ifdef pll_unlock 
        regx = KT_Bus_Read(0x2f);                
        KT_Bus_Write(0x2f, regx|0x0080); //pll_unlock_en
        regx = KT_Bus_Read(0x44);                
        KT_Bus_Write(0x44, (regx&0x1fff)|(1<<13)); //lo_lock_hth_vsel
        regx = KT_Bus_Read(0x44);                
        KT_Bus_Write(0x44, (regx&0xe3ff)|(3<<10)); //lo_lock_lth_vsel
        regx = KT_Bus_Read(0x44);                
        KT_Bus_Write(0x44, regx&0xfdff); //lo_lock_det_pd
    #endif

    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Standby                                                 
//������������������                                                                
//����˵����                                                                        
//ȫ�ֱ�����                                                                        
//��    �룺��                                                                        
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_Standby(void)
{
    KT_WirelessMicTx_PowerDownProcedure();
    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_WakeUp                                                     
//�������������ѳ���                                                                
//����˵�����������ģʽ                                                        
//ȫ�ֱ�����                                                                        
//��    �룺��                                                                        
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_WakeUp(void)
{
    UINT16 reg3;
    reg3 = KT_Bus_Read(0x03);
    KT_Bus_Write(0x03, (reg3 & 0x7FFF) | (WAKEUP << 15)); //Write Standby bit to 0
    Delay_ms(50);
    KT_WirelessMicTx_Init();
//  wakeUp�Ժ���Ҫ��һЩtunę��������Ч������PA�ȹ������ɲο�main.c�����KT_MicTX_Init����
    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_PASW                                                     
//����������PA�򿪡��رճ���                                                        
//����˵������������оƬ�򿪻��߹ر�PA                                                
//ȫ�ֱ�����                                                                        
//��    �룺bPA_Switch                                                                
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-05                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26                                         
//��    ����V0.4                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_PASW(BOOL bPA_Switch)
{
    UINT16 regF;

    regF = KT_Bus_Read(0x0F);
    KT_Bus_Write(0x0F, regF & 0xFFF7 | ( (UINT8)bPA_Switch << 3 ));
    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_PAGain                                                     
//����������PA�������                                                                
//����˵����cPaGain��ΧΪ0-63����64��                                    
//ȫ�ֱ�����                                                                        
//��    �룺cPaGain                                                                    
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-05                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V0.1                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_PAGain(UINT8 cPaGain)
{
    UINT16 reg11;

    reg11 = KT_Bus_Read(0x11);
    KT_Bus_Write(0x11, reg11 & 0xFF00 | cPaGain );        

    return(1);    
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Fdev_Monitor                                             
//����������ʵʱƵƫ��ȡ����                                                        
//����˵������ȡ��ǰ״̬��Ƶƫ��С                                                    
//ȫ�ֱ�����                                                                        
//��    �룺��                                                                        
//��    �أ�(reg1C & 0xF000) >> 12 ��Ϊ0-15��������                                    
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_Fdev_Monitor(void)
{
    UINT16 reg1C;

    reg1C = KT_Bus_Read(0x1C);

    return( (reg1C & 0xF000) >> 12 );
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Mic_Sens                                                 
//����������Mic�����ȵ���                                                            
//����˵����cMicSensΪ0-15����16��                                                
//ȫ�ֱ�����                                                                        
//��    �룺cMicSens                                                                
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_Mic_Sens(UINT8 cMicSens)
{
    UINT16 reg1C;

    reg1C = KT_Bus_Read(0x1C);
    KT_Bus_Write(0x1C, (reg1C & 0xFE1F) | ( (UINT16)cMicSens << 5 ));

    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Comp_Dis                                                 
//����������ѹ�����ܴ򿪡��رճ���                                                    
//����˵������������оƬ�򿪻��߹ر�ѹ������                                        
//ȫ�ֱ�����                                                                        
//��    �룺bComp_Dis                                                                
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
//BOOL KT_WirelessMicTx_Comp_Dis(BOOL bComp_Dis)                            
//{
//    UINT16 reg1C;
//
//    reg1C = KT_Bus_Read(0x1C);
//    KT_Bus_Write(0x1C, (reg1C & 0xFFEF) | ( (UINT8)bComp_Dis << 4 ));
//
//    return(1);
//}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_MuteSel                                                 
//�����������������ܴ򿪡��رճ���                                                    
//����˵��������ѡ��򿪻��߹رվ�������                                            
//ȫ�ֱ�����                                                                        
//��    �룺bMute_Sel                                                                
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_MuteSel(BOOL bMute_Sel)
{
    UINT16 reg1C;

    reg1C = KT_Bus_Read(0x1C);
    KT_Bus_Write(0x1C, (reg1C & 0xFFFE) | bMute_Sel); //Write Mute bit

    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Pilot                                                     
//����������32.768KHz��Ƶ�źŴ򿪡��رճ���                                            
//����˵��������ѡ���Ƿ�ʹ��32.768KHz��Ƶ�ź�                                        
//ȫ�ֱ�����                                                                        
//��    �룺bPilot_Dis                                                                
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_Pilot(BOOL bPilot_Dis)
{
    UINT16 reg1F;

    reg1F = KT_Bus_Read(0x1F);
#ifdef OTHER_RX
    KT_Bus_Write(0x1F, (reg1F & 0xFFBF) | ( (UINT8)bPilot_Dis << 6 ));
#endif
#ifdef KT_RX
    KT_Bus_Write(0x1F, (reg1F & 0x7FFF) | ( (UINT16)bPilot_Dis << 15 ));
#endif
    Delay_ms(20);

    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Pilot_Fdev                                                 
//����������32.768KHz��Ƶ�źŵ���Ƶƫ��С���ó���                                    
//����˵����cPilot_FdevΧΪ0-3����4��                                                
//ȫ�ֱ�����                                                                        
//��    �룺cPilot_Fdev                                                                
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_Pilot_Fdev(UINT8 cPilot_Fdev)
{
    UINT16 reg1F;

    reg1F = KT_Bus_Read(0x1F);
#ifdef OTHER_RX
    KT_Bus_Write(0x1F, (reg1F & 0xFFCF) | (cPilot_Fdev << 4));
#endif
#ifdef KT_RX
    KT_Bus_Write(0x1F, (reg1F & 0xFE7F) | ((UINT16)cPilot_Fdev << 7));
#endif
    Delay_ms(20);

    return(1);
}

#ifdef OTHER_RX
//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Set_Pilot_Freq                                             
//�������������õ�ƵƵ��                                                            
//����˵�������õ�ƵƵ��                                                
//ȫ�ֱ�����                                                                        
//��    �룺xtal_sel: 0 or 1                                                        
//��    �أ��ɹ���1�� ʧ�ܣ�0                                                 
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V1.0                                                                     
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_Set_Pilot_Freq(BOOL bXtal_Sel)
{
    UINT16 regx;
    if (bXtal_Sel == XTAL_24M_FREQ)
    {
        KT_Bus_Write(0x2C, ((PILOT_FREQ << 13) / 9375));
    }
    else
    {
        KT_Bus_Write(0x2C, ((PILOT_FREQ << 13) / 9600));
    }
    return(1);
}
#endif

#ifdef RXISKT0616M_XTAL_DUAL
//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Calc_ChanReg                                    
//���������������Ƶ�ʿ����ּ���                                                
//����˵����������KHzΪ��λ��VCO��Ƶ��;                                            
//            ����������*chan_ptr,*chan_frac_ptr,*chan_frac_msb_ptr��                
//ȫ�ֱ�����                                                                        
//��    �룺Freq ��������KHzΪ��λ��VCOƵ�ʣ�                                        
//��    �أ���ȷ��1    ����0                                                            
//�� �� �ߣ�YANG Pei                ʱ�䣺2012-04-19                            
//�� �� �ߣ�KANG Hekai              ʱ�䣺2013-03-29                            
//��    ����V2.0                                                                    
//          V2.5 �޸�����<40�����xtal-40��bug                                                                    
//-----------------------------------------------------------------------------
BOOL KT_WirelessMicTx_Calc_ChanReg_Old(INT32 Freq, UINT16 *chan_ptr, INT16 *chan_frac_ptr, 
                                       UINT8 *chan_frac_msb_ptr, UINT16 xtal_freq)
{
    *chan_ptr = Freq / xtal_freq;
    Freq = Freq % xtal_freq; 
    *chan_frac_ptr = (Freq << 16) / xtal_freq;
    if ((Freq <= 40) && (Freq >= 0))
    {
        *chan_frac_ptr = 0xffff;
        *chan_frac_msb_ptr =3;
    }
    else if ((Freq < xtal_freq ) && (Freq >= xtal_freq - 40))
    {
        (*chan_ptr)++; 
        *chan_frac_ptr = 0xffff;
        *chan_frac_msb_ptr = 3;
    }
    else if ( (Freq >= (xtal_freq / 2 - 40)) && (Freq <= (xtal_freq / 2 + 40)) )
    {
        *chan_frac_ptr = 0x7fff;
        *chan_frac_msb_ptr = 0;
    }
    else if ( Freq > (xtal_freq >> 1) )
    {
        (*chan_ptr)++; 
        *chan_frac_msb_ptr = 3;
    }
    else    
    {
        *chan_frac_msb_ptr = 0;
    }
    return(1);
}

bit selectXtalOld(INT32 Freq)
{
    UINT16 chan0,chan1;
    INT16 chan_frac0,chan_frac1;
    UINT8 chan_frac_msb0,chan_frac_msb1;
    INT16 mod0,mod1,mod2,mod3;

    Freq<<=1;
    KT_WirelessMicTx_Calc_ChanReg_Old(Freq, &chan0, &chan_frac0, &chan_frac_msb0,24000);
    KT_WirelessMicTx_Calc_ChanReg_Old(Freq, &chan1, &chan_frac1, &chan_frac_msb1,24576);
    mod0 = chan_frac0;
    mod1 = chan_frac1;
    mod2 = chan_frac0 << 1;
    mod3 = chan_frac1 << 1;
    if(mod0 < 0)
        mod0=~mod0;             //mod0=abs(mod0);
    if(mod1 < 0)
        mod1=~mod1;             //mod1=abs(mod1);
    if(mod2 < 0)
        mod2=~mod2;             //mod2=abs(mod2);
    if(mod3 < 0)
        mod3=~mod3;             //mod3=abs(mod3);
    if(mod2 < mod0)
        mod0 = mod2;
    if(mod3 < mod1)
        mod1 = mod3;
    if(mod0<mod1)
    {
        return(XTAL_24P576M_FREQ);
    }
    else 
        return(XTAL_24M_FREQ);
}
#endif

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_BatteryMeter_Read                                         
//������������ص�ѹ��ȡ����                                                        
//����˵������ȡ��ǰ�ĵ�ص�ѹ������ֵΪ0-2047������                                
//ȫ�ֱ�����                                                                        
//��    �룺��                                                                        
//��    �أ�reg7 & 0x07FF��Ϊ0-2047��������                                            
//�� �� �ߣ�KANG Hekai              ʱ�䣺2011-06-10                                
//�� �� �ߣ�KANG Hekai              ʱ�䣺2011-06-10                                
//��    ����V1.0                                                                    
//-----------------------------------------------------------------------------
UINT16 KT_WirelessMicTx_BatteryMeter_Read(void)
{
    UINT16 reg7;

    reg7 = KT_Bus_Read(0x07);

    return( reg7 & 0x07FF );
}

//�� �� ����KT_WirelessMicTx_Calc_ChanReg                                    
//���������������Ƶ�ʿ����ּ���                                                
//����˵����������KHzΪ��λ��VCO��Ƶ��;                                            
//            ����������*chan_ptr,*chan_frac_ptr,*chan_frac_msb_ptr��                
//ȫ�ֱ�����                                                                        
//��    �룺Freq ��������KHzΪ��λ��VCOƵ�ʣ�                                        
//��    �أ���ȷ��1    ����0                                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-05                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-09-02                                     
//��    ����V0.1    For KT0646M_VX
//            V0.2    �޸��˱�������ķ�ʽ
//            V0.3    �޸���vco_lowth
//            V0.4    ͳһ�˱��������ʽ
//-----------------------------------------------------------------------------
//lopa_div1 * lopa_div2
UINT8 code lopa_div_tab[13]=  
{
    6, 8, 10, 12,
      16, 20, 24,
      32, 40, 48,
      64, 80, 96
};

//reg0x08<bit9:8> and <bit7:6>
UINT8 code lopa_div_tab2[13][2]= 
{
     {0,0},{0,1}, {0,2}, {0,3},
           {1,1}, {1,2}, {1,3},
           {2,1}, {2,2}, {2,3},
           {3,1}, {3,2}, {3,3}
};

BOOL KT_WirelessMicTx_Calc_ChanReg(INT32 Freq, UINT8 data *lopa_div1_ptr,UINT8 data *lopa_div2_ptr,
                                   UINT16 data *chan_ptr, UINT16 data *chan_frac_ptr, 
                                   UINT8 data *chan_frac_msb_ptr, UINT32 *chan_frac_temp_ptr,INT32 xtal_freq)
{
    UINT32 chan_frac;
    INT32 Fvco;
    UINT32 temp,tempMin=xtal_freq;
//    double chan_frac_d;
//    double chan_frac_temp_d;
    UINT16 i = 12;
    UINT8 lopa_div;
//    UINT16 chan_temp;
    long vco_highth = 4320000; //VCO Range:4.32G-5.76G
    
    //lodiv ����ֵ
    lopa_div = vco_highth / Freq;

    //lodiv ʵ��ֵ
    for(i=0; i<12; i++)
    {
        if(lopa_div < lopa_div_tab[i])
        {            
            break;                                                   
        }
    }
    //���lopa_div1��lopa_div2
    *lopa_div1_ptr = lopa_div_tab2[i][1];
    *lopa_div2_ptr = lopa_div_tab2[i][0];
    
    //����VCO Ƶ��
    Fvco = Freq * lopa_div_tab[i];
    //����
    *chan_ptr = Fvco / (xtal_freq * 7) + 1; //dll_mode : *7
    //С��
//    chan_frac_d = Fvco;
//    chan_frac_d = chan_frac_d / (xtal_freq * 7);     //31.1221
//    chan_frac_d = chan_frac_d - (*chan_ptr);
//    chan_frac_d = chan_frac_d * 65536;                 //-57532.95
//    chan_frac = chan_frac_d;
    chan_frac= (((Fvco % (xtal_freq * 7))-(xtal_freq * 7))<<12)/((xtal_freq * 7)>>4);

    *chan_frac_msb_ptr = (chan_frac & 0x00030000) >> 16 ; //reg0x08<bit1:0>
    *chan_frac_ptr = chan_frac & 0x0000ffff; //reg0x09

//    chan_temp = Freq / xtal_freq + 1;
//    chan_frac_temp_d = Freq;
//    chan_frac_temp_d = chan_frac_temp_d / xtal_freq - chan_temp;
//    *chan_frac_temp_ptr = chan_frac_temp_d;

//    *chan_frac_temp_ptr= ((xtal_freq>>1)-abs(((Freq%xtal_freq)-(xtal_freq>>1))))*(3072000/xtal_freq);
    
    for(i=1;i<4;i++)
    {
        temp = ((xtal_freq/(2*i))-abs(((Freq%(xtal_freq/i))-(xtal_freq/(2*i)))))*i;
        if(temp<tempMin)
        {
            tempMin=temp;
        }
    }
    i=8;
    temp = ((xtal_freq/(2*i))-abs(((Freq%(xtal_freq/i))-(xtal_freq/(2*i)))))*i;
    if(temp<tempMin)
    {
        tempMin=temp;
    }
    *chan_frac_temp_ptr=tempMin*(3072000/xtal_freq);        
    return(1);
}
//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Tune                                                     
//��������������Ƶ�����ú���                                                        
//����˵����������KHzΪ��λ�ķ���Ƶ�ʣ�                                                
//ȫ�ֱ�����                                                                        
//��    �룺Freq ��������KHzΪ��λ�ķ���Ƶ�ʣ�                                        
//��    �أ���ȷ��1                 ����0                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-05                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26                                         
//��    ����V0.1    For KT0646M_VX
//          V0.2    �޸��˱�������ķ�ʽ��������VX��VX2�ĺ궨�壬
//                  �޸��˽��DLL���������ֳ����λ��
//                  ȥ���˲��õ�chan_cfg2����
//                  ȥ����MCU_POWER_OKλ�Ĳ�ѯ����
//                  ȥ����PLL�������ж�
//          V0.4    ͳһ�˱��������ʽ
//-----------------------------------------------------------------------------
//ʹ��24MHz����ķ���Ƶ��
UINT32 code use24M[26] = 
{   490500,492000,516000,541500,556000,565500,566000,590000,614000,615000,639000,651250,688000,
    688500,712000,712250,712500,722500,736500,760500,762000,787500,810000,811500,835500,859500
};

//ʹ��24.576MHz����ķ���Ƶ��
UINT32 code use24576M[14] = 
{
    7500,9000,10000,10500,12000,13500,14000,15000,16000,16500,18000,19500,20000,22000
};

BOOL KT_WirelessMicTx_Tune(INT32 Freq)
{
    UINT16 chan0;
    UINT16 chan_frac0;
    UINT8 chan_frac_msb0;
    UINT8 lopa_div10,lopa_div20;
    UINT16 regx,regy;
    UINT32 chan_frac_temp0;    
    UINT16 state;
#ifdef XTAL_DUAL
    UINT16 chan1;
    UINT16 chan_frac1;
    UINT8 chan_frac_msb1;
    UINT8 lopa_div11,lopa_div21;
    UINT16 use24M_flag=0,use24576M_flag=0;
    UINT32 chan_frac_temp1;
    #ifndef RXISKT0616M_XTAL_DUAL
    UINT8 i;
    UINT32 state_tmp;
    #endif
//    double mod0,mod1,mod2,mod3;
#endif

    #ifdef DOUBLE_KVCO
        regx=KT_Bus_Read(0x2d); //kvco_cali_bps=0
        KT_Bus_Write(0x2d, (regx&0xf7ff));
    
        regx=KT_Bus_Read(0x0a); //locpcali_bps=0
        KT_Bus_Write(0x0a, (regx&0xffef));
        regx=KT_Bus_Read(0x3d);
        KT_Bus_Write(0x3d, (regx&0xffbf));              
    
        regx=KT_Bus_Read(0x3c);
        KT_Bus_Write(0x3c, (regx&0xfffd)); //loamp_cali_bps=0
    #endif    

//    regx = KT_Bus_Read(0x2d);
//    KT_Bus_Write(0x2d, (regx&0xfff8)|0x0003); //lo_fine_vref_sel

#ifdef XTAL_24M_ONLY
    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div10, &lopa_div20, &chan0, &chan_frac0, 
                                  &chan_frac_msb0,&chan_frac_temp0, 24000);
#endif

#ifdef XTAL_24P576M_ONLY
    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div10, &lopa_div20, &chan0, &chan_frac0, 
                                  &chan_frac_msb0,&chan_frac_temp0,24576);
#endif

#ifdef XTAL_DUAL

    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div10, &lopa_div20, &chan0, &chan_frac0, 
                                  &chan_frac_msb0,&chan_frac_temp0,24000);
    KT_WirelessMicTx_Calc_ChanReg(Freq, &lopa_div11, &lopa_div21, &chan1, &chan_frac1, 
                                  &chan_frac_msb1,&chan_frac_temp1,24576);
    #ifndef RXISKT0616M_XTAL_DUAL
        for(i=0;i<26;i++)
        {
            if(Freq==use24M[i])    
            {
                use24M_flag=1;
                break;
            }
        }
    
        state_tmp = Freq%24000;
        for(i=0;i<14;i++)
        {
            if(state_tmp==use24576M[i])
            {
                use24576M_flag=1;
                break;
            }
        }
        if (use24M_flag)
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24M_FREQ);
        }
        else if(use24576M_flag)
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24P576M_FREQ);
            chan0 = chan1;
            lopa_div10 = lopa_div11;
            lopa_div20 = lopa_div21;
            chan_frac0 = chan_frac1;
            chan_frac_msb0 = chan_frac_msb1;
        }
        else 
        {    
            if(chan_frac_temp0>chan_frac_temp1)
            {
                KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24M_FREQ);    
            }
            else
            {
                KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24P576M_FREQ);
                chan0 = chan1;
                lopa_div10 = lopa_div11;
                lopa_div20 = lopa_div21;
                chan_frac0 = chan_frac1;
                chan_frac_msb0 = chan_frac_msb1;    
            }
        }
    #else
        if(selectXtalOld(Freq))
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24P576M_FREQ);
            chan0 = chan1;
            lopa_div10 = lopa_div11;
            lopa_div20 = lopa_div21;
            chan_frac0 = chan_frac1;
            chan_frac_msb0 = chan_frac_msb1;
        }
        else
        {
            KT_WirelessMicTx_SW_XTAL_Freq(XTAL_24M_FREQ);
        }
    #endif

#endif

    KT_Bus_Write(0x08,(chan0 << 10) | (lopa_div10 << 8) | (lopa_div20 << 6) | (0 << 5) | 
                 (0 << 4) | (0 <<2) | chan_frac_msb0); //
    KT_Bus_Write(0x09, chan_frac0|0x0001);

    //dllδ�������л��������Ҫrst
    regx = KT_Bus_Read(0x0e);
    KT_Bus_Write(0x0e, regx | 0x0080); //dll_rst is from regbank
    regx = KT_Bus_Read(0x0e);
    KT_Bus_Write(0x0e, regx | 0x0100); //dll_rst=1
    Delay_ms(1);
    regx = KT_Bus_Read(0x0e);
    KT_Bus_Write(0x0e, regx & 0xfeff); //dll_rst=0

    regx=KT_Bus_Read(0x0a);
    KT_Bus_Write(0x0a,regx|0x0020); //tune

    regx=KT_Bus_Read(0x0d);
    regx = (regx&0x0800)>>11;
    while(!regx) //wait pll ready
    {
        regx=KT_Bus_Read(0x0d);
        regx = (regx&0x0800)>>11; 
    }

//    regx = KT_Bus_Read(0x2d);
//    KT_Bus_Write(0x2d, (regx&0xfff8)); //lo_fine_vref_sel=0

    #ifdef DOUBLE_KVCO
        regx=KT_Bus_Read(0x2e); //double+16MHz/V locoarse_var_sel
        state=regx&0x1C00;
        state=state>>10;
        if(state >= 3)
        {
            state = 7;                                
        }
        else
        {
            state = (state<<1) + 3;
        }
        regx=regx&0xe3ff|(state<<10);
                                           
        regy=KT_Bus_Read(0x2d); //kvco_cali_bps=1
        KT_Bus_Write(0x2d, (regy|0x0800));              
    
        KT_Bus_Write(0x2e, regx); //write locoarse/lofine_var_sel
    
        regx=KT_Bus_Read(0x2a); //write cp_code
        state=(regx&0x007e)>>1;
        regx=KT_Bus_Read(0x3d);
        KT_Bus_Write(0x3d, (regx&0xff80)|(1<<6)|state);
    
        regx=KT_Bus_Read(0x0a); //locpcali_bps=1
        KT_Bus_Write(0x0a, (regx|0x0010));              
    
        regx=KT_Bus_Read(0x3c);
        KT_Bus_Write(0x3c, (regx|0x0002)); //loamp_cali_bps=1
    
        regx=KT_Bus_Read(0x0a);
        KT_Bus_Write(0x0a,regx|0x0020); //tune
    
        regx=KT_Bus_Read(0x0d);
        regx = (regx&0x0800)>>11;
        while(!regx) //wait pll ready
        {
            regx=KT_Bus_Read(0x0d);
            regx = (regx&0x0800)>>11; 
        }
    #endif

    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_Set_XTAL
//�������������þ���Ƶ��
//����˵�������þ���Ƶ��
//ȫ�ֱ�����
//��    �룺xtal_sel: 0 or 1
//��    �أ��ɹ���1�� ʧ�ܣ�0
//�� �� �ߣ�YANG Pei                ʱ�䣺2012-04-10
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26
//��    ����V0.4
//�� �� �ߣ�wu jinfeng	            ʱ�䣺2017-10-19
//��    ����V1.4     soft_rst�����ڼĴ���0x3e��bit15�ˣ�������0x1e��bit4.
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_Set_XTAL(BOOL bXtal_Sel)
{
    UINT16 regx;
    
	regx = KT_Bus_Read(0x1E);
    KT_Bus_Write(0x1E,regx|0x0010); //soft_rst=1      rst dsp part
    KT_Bus_Write(0x0E, 0x0002); //au_rst_bypass=1
    regx = KT_Bus_Read(0x0E);
    KT_Bus_Write(0x0E, regx | 0x0004); //au_dig_rst=1
    
    regx = KT_Bus_Read(0x47);
    KT_Bus_Write( 0x47, (regx & 0xFFDF) | ((UINT8)bXtal_Sel << 5) ); //bXtal_Sel=0

    Delay_ms(50);

    KT_Bus_Write(0x0E, 0x0000); //au_rst_bypass=0    au_dig_rst=0
	regx = KT_Bus_Read(0x1E);
    KT_Bus_Write(0x1E, regx&~0x0010); //soft_rst=0
#ifdef OTHER_RX
    KT_WirelessMicTx_Set_Pilot_Freq(bXtal_Sel); //set pilot frequency
#endif
    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_SW_XTAL_Freq                                                      
//�����������л�����Ƶ��                                                             
//����˵����                                                                     
//ȫ�ֱ�������                                                                     
//��    �룺xtal_sel;                                                             
//��    �أ��ɹ���1�� ʧ�ܣ�0                                                                 
//�� �� �ߣ�KANG Hekai              ʱ�䣺                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26                                         
//��    ����V0.4                                                                     
//-----------------------------------------------------------------------------
UINT8 KT_WirelessMicTx_SW_XTAL_Freq(BOOL bXtal_Sel)
{
    UINT16 regx;

    regx = KT_Bus_Read(0x47);
    if (bXtal_Sel==1) //24.576MHz
    {
        Display_Ch_Num(13,1);
        if ((regx & 0x0020)==0) //bXtal_Sel=0    24MHz
            KT_WirelessMicTx_Set_XTAL(bXtal_Sel);
    }
    else
    {
        Display_Ch_Num(13,0);
        if ((regx & 0x0020)!=0) //bXtal_Sel=1     24.576MHz
            KT_WirelessMicTx_Set_XTAL(bXtal_Sel);
    }
    return(1);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_PowerDownProcedure                                                      
//�����������ػ��������                                                             
//����˵����                                                                     
//ȫ�ֱ�������                                                                     
//��    �룺��;                                                             
//��    �أ���                                                                     
//�� �� �ߣ�Kang Hekai              ʱ�䣺2014-02-13                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26                                         
//��    ����V0.4                                                                     
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_PowerDownProcedure(void)
{
    UINT16 regx;

    KT_WirelessMicTx_MuteSel(AUDIO_MUTE);
//    Delay_ms(50);
    KT_WirelessMicTx_Pilot(PILOT_DISABLE);
//    Delay_ms(500);

//    KT_WirelessMicTx_PAGain(0);
    KT_WirelessMicTx_PASW(PA_OFF);
//    Delay_ms(5);

    regx=KT_Bus_Read(0x0a);
    KT_Bus_Write(0x0a,regx|0x1000); //recali

    regx = KT_Bus_Read(0x03);
    KT_Bus_Write(0x03, regx | (STANDBY << 15)); //Write Standby bit to 1
//    Delay_ms(20);
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_ECHO                                        
//����������ECHO�������ó���                                                
//����˵��������ECHO����Ч��                
//ȫ�ֱ�����                                                                        
//��    �룺bEcho_Dis    ��ECHO���أ�
//          Echo_Ratio     ��ECHO������                                        
//          Echo_Delay     ��ECHO��ʱ��
//��    �أ���                                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-26                                         
//�� �� �ߣ�                        ʱ�䣺                                         
//��    ����V0.1    For KT0646M_VX 
//��    ����V0.2    echo�رյ�ʱ�򣬲�������echo���ǰ�Echo_Ratioд��0                                                                  
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_ECHO(BOOL bEcho_Dis,UINT8 Echo_Ratio,UINT8 Echo_Delay)
{
    UINT16 regx;
	if(bEcho_Dis==1)//echo disable ��ʱ�򣬲�������echo���ǰ�Echo_Ratioд��0����redmine#11824
	{
		KT_Bus_Write( 0x32, (ECHO_MCU << 15) | (0 << 14) | (ECHO_STRU << 13) | 
                 (0 << 8) | (Echo_Delay << 3) | ECHO_GAINUP );
	}
	else
	{
		KT_Bus_Write( 0x32, (ECHO_MCU << 15) | ((UINT8)bEcho_Dis << 14) | (ECHO_STRU << 13) | 
                 (Echo_Ratio << 8) | (Echo_Delay << 3) | ECHO_GAINUP );
	}   
    regx = KT_Bus_Read(0x33);
    KT_Bus_Write( 0x33, (regx & 0xFFFC) | ECHO_GAINDOWN );                          
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_EQSW                                        
//����������EQ���������س���                                                
//����˵��������EQ�����������ر�                
//ȫ�ֱ�����                                                                        
//��    �룺bEq_Dis    ��EQ���أ�
//��    �أ���                                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26                                         
//�� �� �ߣ�                        ʱ�䣺
//��    ����V0.4
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_EQSW(BOOL bEq_Dis)
{
    UINT16 regx;

    regx = KT_Bus_Read(0x34);
    KT_Bus_Write(0x34, ((regx & 0x7FFF) | (UINT16)bEq_Dis << 15));
}

//-----------------------------------------------------------------------------
//�� �� ����KT_WirelessMicTx_EQGAIN                                        
//����������EQ���������ó���                                                
//����˵��������EQ��������Ƶ������Ч��                
//ȫ�ֱ�����                                                                        
//��    �룺Eq_Freq ��EQƵ�ʣ�                                        
//          Eq_Gain ��EQ���棩
//��    �أ���                                                            
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-04-26                                         
//�� �� �ߣ�Zhou Dongfeng           ʱ�䣺2016-08-26
//��    ����V0.4
//-----------------------------------------------------------------------------
void KT_WirelessMicTx_EQGAIN(UINT8 Eq_Freq,UINT8 Eq_Gain)
{
    UINT16 regx;
    UINT8 temp1,temp2;
    temp1=Eq_Freq/3+0x34;
    temp2=(2-(Eq_Freq%3))*5;
    regx = KT_Bus_Read(temp1);
    
    KT_Bus_Write(temp1, ((regx & ~(0x001f<<temp2)) | ( (UINT16)Eq_Gain << temp2 )));
/*    switch(Eq_Freq)
    {
        case EQ_25H:
        {
            regx = KT_Bus_Read(0x34);
            KT_Bus_Write(0x34, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_40H:
        {
            regx = KT_Bus_Read(0x34);
            KT_Bus_Write(0x34, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_63H:
        {
            regx = KT_Bus_Read(0x34);
            KT_Bus_Write(0x34, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_100H:
        {
            regx = KT_Bus_Read(0x35);
            KT_Bus_Write(0x35, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_160H:
        {
            regx = KT_Bus_Read(0x35);
            KT_Bus_Write(0x35, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_250H:
        {
            regx = KT_Bus_Read(0x35);
            KT_Bus_Write(0x35, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_400H:
        {
            regx = KT_Bus_Read(0x36);
            KT_Bus_Write(0x36, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_630H:
        {
            regx = KT_Bus_Read(0x36);
            KT_Bus_Write(0x36, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_1KH:
        {
            regx = KT_Bus_Read(0x36);
            KT_Bus_Write(0x36, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_1K6:
        {
            regx = KT_Bus_Read(0x37);
            KT_Bus_Write(0x37, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_2K5:
        {
            regx = KT_Bus_Read(0x37);
            KT_Bus_Write(0x37, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_4KH:
        {
            regx = KT_Bus_Read(0x37);
            KT_Bus_Write(0x37, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;

        case EQ_6K3:
        {
            regx = KT_Bus_Read(0x38);
            KT_Bus_Write(0x38, ((regx & 0x83FF) | ( (UINT16)Eq_Gain << 10 )));
        }break;
        case EQ_10K:
        {
            regx = KT_Bus_Read(0x38);
            KT_Bus_Write(0x38, ((regx & 0xFC1F) | ( (UINT16)Eq_Gain << 5 )));
        }break;
        case EQ_16K:
        {
            regx = KT_Bus_Read(0x38);
            KT_Bus_Write(0x38, ((regx & 0xFFE0) | ( (UINT16)Eq_Gain << 0 )));
        }break;
        default    :    break; 
    }     */
}

