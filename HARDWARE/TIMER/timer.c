#include "timer.h"
#include "led.h"
#include "key.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

KeyHandler myKeyHandler;


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  //��ռ���ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}

//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2!
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; 				//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
 	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ

	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);		//ʹ�ܶ�ʱ��2���´����ж�
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
 	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  				//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  			//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); 
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
//		printf("TIM3���.......\r\n");
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

//��ʱ��2�жϷ�����
void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
	{
		static uint16_t count = 0;
	//**********************************************************//
		if(count%5 == 0 )
		{
//			if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) || (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) == 0) ||
//				(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == 0) || (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0))
			if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)  &&
				GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)) == 0)
			{
				myKeyHandler.status = KEY_DOWN;
			}
			key_handler(&myKeyHandler);
			
			if (myKeyHandler.event == KEY1_PRESS)
				Key_Scan = key1_down;
			else if (myKeyHandler.event == KEY2_PRESS)
				Key_Scan = key2_down;
			else if (myKeyHandler.event == KEY3_PRESS)
				Key_Scan = key3_down;
			else if (myKeyHandler.event == KEY4_PRESS)
				Key_Scan = key4_down;
			
			count=0;
		}
		count++;
	//*************************************************************//
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
}












