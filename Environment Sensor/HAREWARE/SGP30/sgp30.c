#include "sgp30.h"
#include "delay.h"
#include "usart.h"

//��ʼ��IIC���ߣ�����SGP30
void SGP30_Init(void)
{	
	IIC_Init();		//��ʼ��IIC����
	
	//�鿴SGP30�����кţ�ȷ��������������
	IIC_Start();
	IIC_Send_Byte(0x58<<1);
	if(IIC_Wait_Ack()==1){}
	else{}
	IIC_Send_Byte(0x36);
	if(IIC_Wait_Ack()==1){}
	else{}
	IIC_Send_Byte(0x82);
	if(IIC_Wait_Ack()==1){}
	else{}
	IIC_Stop();
}

//��ʼ��SGP30�Ŀ���������⹦��
void SGP30_Start_Measure(void)
{	
	//����Init_air_quality����
	IIC_Start();
	IIC_Send_Byte(0x58<<1);
	if(IIC_Wait_Ack()==1){}
	else{}
	IIC_Send_Byte(0x20);
	if(IIC_Wait_Ack()==1){}
	else{}
	IIC_Send_Byte(0x03);
	if(IIC_Wait_Ack()==1){}
	else{}
	IIC_Stop();
	
	delay_ms(10000);		//��Ҫʱ�������г�ʼ��
	
	//����Measure_air_quality����
	IIC_Start();
	IIC_Send_Byte(0x58<<1);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x20);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x08);
	IIC_Wait_Ack();
	IIC_Stop();
}

//��ȡCO2��TVOCŨ��
void Read_Air_Quality(u16 *co2, u16 *tvoc)
{
	u8 co2_high = 0;
	u8 co2_low = 0;
	u8 tvoc_high = 0;
	u8 tvoc_low = 0;
	
	//����Measure_air_quality����
	IIC_Start();
	IIC_Send_Byte(0x58<<1);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x20);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x08);
	IIC_Wait_Ack();
	
	delay_ms(50000);		//��Ҫʱ�������м��
	
	//���Ͷ���ַ����ȡ����16λ��Ũ��ֵ����Ӧ��8λCRC���ֵ
	IIC_Start();
	IIC_Send_Byte((0x58<<1)|0x01);
	IIC_Wait_Ack();
	co2_high = IIC_Read_Byte(1);
	co2_low = IIC_Read_Byte(1);
	IIC_Read_Byte(1);
	tvoc_high = IIC_Read_Byte(1);
	tvoc_low = IIC_Read_Byte(1);
	IIC_Read_Byte(0);
	IIC_Stop();
	
	*co2 = (co2_high<<8)|co2_low;
	*tvoc = (tvoc_high<<8)|tvoc_low;
}
