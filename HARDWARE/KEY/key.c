#include "key.h"
#include "delay.h"
#include "usart.h"

//������ʼ������ 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);//ʹ�ܶ˿�ʱ��
	RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//�˿���ӳ��(���ܹرգ�,ʹ��ΪGPIO�ܽ�ʹ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//����GPIO�ܽ�ģʽ(��������)�����ţ��ٶ�	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN;//����1
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY2_PIN;//����2
	GPIO_Init(KEY2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY3_PIN;//����3
	GPIO_Init(KEY3_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=KEY4_PIN;//����4
	GPIO_Init(KEY4_PORT,&GPIO_InitStructure);
}


void initKeyHandler(KeyHandler *handler) 
{
    handler->status = KEY_UP;
    handler->event = NULL_PRESS;
    handler->longPressCount = 0;
}
 
void key_handler(KeyHandler *handler)
{
	unsigned char keyStatus=1;
	
	static unsigned char KEY1=1;
	static unsigned char KEY2=1;
	static unsigned char KEY3=1;
	static unsigned char KEY4=1;

	if (handler->longPressCount == 0 && handler->status != KEY_DOWN)//�����������Ϊ0�Ұ���״̬��ΪNULL_PRESS
	{
		handler->event = NULL_PRESS; // ���ð���״̬��־λ��û�а������£�
	}
    if (handler->status == KEY_DOWN) //����а�������
	{
		handler->status = KEY_UP;
		
//		printf("��ʼ���µİ���״̬Ϊ%d  %d  %d  %d\r\n",GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14),
//						GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));

		keyStatus=(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) &&
				GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));//��ȡʵ�ʵİ���״̬��keyStatus������
		//		&& GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) &&
    } 
	
	if (keyStatus == 0) //��ʱ�������ڱ��ְ��µ�״̬
	{
		handler->longPressCount++;
//		printf("key��ֵ�ڰ������¿�ʼʱΪ%d\r\n",KEY1);
//		printf("����ʱ�İ���״̬Ϊ%d  %d  %d  %d\r\n",GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14),
//						GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) KEY1 = 0;
//		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) == 0) KEY2 = 0;
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == 0) KEY3 = 0;
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) KEY4 = 0;
	}
	else    //�����ɿ���ʼ�жϰ��������ľ�����Ϣ
	{
		if (handler->longPressCount >= 4 && handler->longPressCount < 150) 
		{
			if(KEY1 == 0) 
			{
//				printf("key��ֵ�ڰ����ɿ�ʱΪ%d\r\n",KEY1);
				handler->event = KEY1_PRESS;
			}
			else if (KEY2 == 0) handler->event = KEY2_PRESS;
			else if (KEY3 == 0) handler->event = KEY3_PRESS;
			else if (KEY4 == 0) handler->event = KEY4_PRESS;
			
		} 
		else 
		{
			handler->event = NULL_PRESS; 
		}
		KEY1 = 1;
		KEY2 = 1;
		KEY3 = 1;
		KEY4 = 1;
		
		handler->longPressCount = 0;
    }
}

//u8 Key_Scan(u8 mode)
//{
//	static u8 key=1;//������һ�ε�ֵ
//	if(mode==1)key=1;
//		
//	if(key==1 && (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0))  //(key=1����Ч)
//	{
//		delay_ms(10);//�������£���ʱ�жϷ���
//		key=0;  //key=0�����ã���ֹ��������������ʱ�����ν��뺯����ɶ�μ��ɨ�践�ذ�����������
//		if(KEY1==0)
//			return KEY1_PRESS;
//		else if(KEY2==0)
//			return KEY2_PRESS;
//		else if(KEY3==0)
//			return KEY3_PRESS;
//		else if(KEY4==0)
//			return KEY4_PRESS;
//	}
//	else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1)//�ް�������
//	{
//		delay_ms(10);//��ʱ�жϷ���
//		if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1)
//		key=1;
//	}
//	return 0;
//}
