#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//#define LED0 PAout(6)	// PA6
//#define LED1 PAout(5)	// PA5

//����꣬����������ֲ�͹���ֻ��Ҫ���ĺ������Ƽ���
#define LED0_PIN		GPIO_Pin_0
#define LED1_PIN		GPIO_Pin_1
#define LED2_PIN		GPIO_Pin_2
#define LED3_PIN		GPIO_Pin_3
#define LED4_PIN		GPIO_Pin_4
#define LED5_PIN		GPIO_Pin_5
#define LED6_PIN		GPIO_Pin_6
#define LED7_PIN		GPIO_Pin_7

#define LED0	PAout(0)
#define LED1	PAout(1)
#define LED2	PAout(2)
#define LED3	PAout(3)
#define LED4	PAout(4)
#define LED5	PAout(5)
#define LED6	PAout(6)
#define LED7	PAout(7)

void LED_Init(void);//��ʼ��

		 				    
#endif
