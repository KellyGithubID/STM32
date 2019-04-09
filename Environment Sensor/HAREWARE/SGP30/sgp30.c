#include "sgp30.h"
#include "delay.h"
#include "usart.h"

//初始化IIC总线，查找SGP30
void SGP30_Init(void)
{	
	IIC_Init();		//初始化IIC总线
	
	//查看SGP30的序列号，确认已与总线相连
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

//初始化SGP30的空气质量检测功能
void SGP30_Start_Measure(void)
{	
	//发送Init_air_quality命令
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
	
	delay_ms(10000);		//需要时间间隔进行初始化
	
	//发送Measure_air_quality命令
	IIC_Start();
	IIC_Send_Byte(0x58<<1);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x20);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x08);
	IIC_Wait_Ack();
	IIC_Stop();
}

//读取CO2与TVOC浓度
void Read_Air_Quality(u16 *co2, u16 *tvoc)
{
	u8 co2_high = 0;
	u8 co2_low = 0;
	u8 tvoc_high = 0;
	u8 tvoc_low = 0;
	
	//发送Measure_air_quality命令
	IIC_Start();
	IIC_Send_Byte(0x58<<1);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x20);
	IIC_Wait_Ack();
	IIC_Send_Byte(0x08);
	IIC_Wait_Ack();
	
	delay_ms(50000);		//需要时间间隔进行检测
	
	//发送读地址并读取两个16位的浓度值及相应的8位CRC检查值
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
