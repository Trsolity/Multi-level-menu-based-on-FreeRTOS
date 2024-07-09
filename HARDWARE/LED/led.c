#include "led.h"

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

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
	 GPIO_InitStructure.GPIO_Pin = LED0_PIN;				 //LED0-->PA.0 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.0
	 GPIO_SetBits(GPIOA,LED0_PIN);						 	 //PA.0 �����

	 GPIO_InitStructure.GPIO_Pin = LED1_PIN;	    		 //LED1-->PA.1 �˿�����, �������
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	 GPIO_SetBits(GPIOA,LED1_PIN); 						 	 //PA.1 ����� 
	
	GPIO_InitStructure.GPIO_Pin=LED2_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED2_PIN);						     //���ùܽ�Ϊ�ߵ�ƽ��ʹLED��ʼ״̬ΪϨ��״̬
	
	GPIO_InitStructure.GPIO_Pin=LED3_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED3_PIN);						     //���ùܽ�Ϊ�ߵ�ƽ��ʹLED��ʼ״̬ΪϨ��״̬
	
	GPIO_InitStructure.GPIO_Pin=LED4_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED4_PIN);						     //���ùܽ�Ϊ�ߵ�ƽ��ʹLED��ʼ״̬ΪϨ��״̬
	
	GPIO_InitStructure.GPIO_Pin=LED5_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED5_PIN);						     //���ùܽ�Ϊ�ߵ�ƽ��ʹLED��ʼ״̬ΪϨ��״̬
	
	GPIO_InitStructure.GPIO_Pin=LED6_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED6_PIN);						     //���ùܽ�Ϊ�ߵ�ƽ��ʹLED��ʼ״̬ΪϨ��״̬
	
	GPIO_InitStructure.GPIO_Pin=LED7_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED7_PIN);						     //���ùܽ�Ϊ�ߵ�ƽ��ʹLED��ʼ״̬ΪϨ��״̬
}
 
