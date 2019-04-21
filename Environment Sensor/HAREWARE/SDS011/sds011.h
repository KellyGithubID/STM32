#ifndef __SDS011_H
#define __SDS011_H

#include "sys.h"
#include "delay.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8 READ_VALUE_CMD[8];
extern u8 READ_PRECISION_CMD[8];

void SDS011_Init(void);
void SDS011_Get_Value(u16 *pm2, u16 *pm10);

#endif
