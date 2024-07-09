#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "rtc.h" 
#include "key.h"
#include "IIC.h"
#include "oled.h"
#include "MPU6050.h"

#include "mytask.h"

#include "FreeRTOS.h"
#include "task.h"

u8 ID;

/*----------------------------������-------------------------------*/

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������
	LED_Init();		  					//��ʼ��LED
	KEY_Init();
	RTC_Init();
	printf("uart ok!\r\n");
	IIC_Init();							//IIC��ʼ��
	
	OLED_Init();
	OLED_Clear();
	
	MPU6050_initialize();				//MPU6050��ʼ��
	DMP_Init();							//DMP��ʼ��
	TIM2_Init(10,7200-1);		//��ʱ����ʼ�������MPU6050��ʱ�ɼ�DMMP����
	TIM_Cmd(TIM2,ENABLE);				//������ʱ��
	
	ID = MPU6050_getDeviceID();
	printf("mpu6050��ID��Ϊ��%d\r\n",ID);
	
	
	printf("����һ��FreeRTOS��ʵ��\r\n");
	
	FreeRTOS_Init();
}


