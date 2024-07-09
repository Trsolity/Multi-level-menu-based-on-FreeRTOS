#include "key.h"
#include "timer.h"
#include "display.h"
#include "mytask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

/*--------------------------������---------------------------------*/

static TaskHandle_t StartTask_Handler = NULL;				//����������
static TaskHandle_t Key_Task_Handler = NULL;				//Key_Task���
static TaskHandle_t Display_Menu_Task_Handler = NULL;		//Display_Menu_Task���
static TaskHandle_t Display_Time_Task_Handler = NULL;		//Display_Time_Task���
static TaskHandle_t Display_MPU6050_Task_Handler = NULL;	//Display_MPU6050_Task���

//static TaskHandle_t TaskGenericFunction_Handle = NULL;	//ͨ��������


/*-------------------------�ں˶�����------------------------------*/

//static SemaphoreHandle_t CountSem_Handle = NULL;  		//�������ź������
static SemaphoreHandle_t BinarySem_Handle = NULL;			//��ֵ�ź������
static QueueHandle_t Test_Queue = NULL;  					//��Ϣ���о��
static EventGroupHandle_t Event_Handle = NULL;  			//�¼�����


/*-------------------------ȫ�ֱ�������------------------------------*/

static volatile int AppTaskCreaterun = 0;  	// ������������ʱAppTaskCreaterun=1
static volatile int flagTask1run = 0;     	// ����1����ʱflagTask1run=1
static volatile int flagTask2run = 0;     	// ����2����ʱflagTask2run=1
u8 Key_Scan = 0;

u8 key1_down = 1;
u8 key2_down = 2;
u8 key3_down = 3;
u8 key4_down = 4;

/*--------------------------�������ȼ�-----------------------------*/

#define START_TASK_PRIO			1

#define KEY_TASK_PRIO			4
#define MENU_TASK_PRIO			4
#define TIME_TASK_PRIO			3
#define MPU6050_TASK_PRIO		3

#define TASK5_TASK_PRIO			2

/*-------------------------�����ջ��С----------------------------*/
	
#define START_STK_SIZE 			128 

#define KEY_TASK_SIZE 			64
#define MENU_TASK_SIZE 			128
#define TIME_TASK_SIZE 			200  
#define MPU6050_TASK_SIZE 		128  

#define TASK5_STK_SIZE 			64 

/*--------------------------�궨��---------------------------------*/

#define QUEUE_LEN			4	//���еĳ��ȣ����ɰ������ٸ���Ϣ
#define QUEUE_SIZE			4	//������ÿ����Ϣ�Ĵ�С���ֽڣ�

#define KEY1_EVENT (0x01 << 0)	//�����¼������λ0
#define KEY3_EVENT (0x01 << 1)	//�����¼������λ1
#define KEY4_EVENT (0x01 << 2)	//�����¼������λ2

/*--------------------------��������-------------------------------*/

void start_task(void *pvParameters);				//��������

void Key_Task(void *pvParameters);					//Key_Task�����ʵ��
void Display_Menu_Task(void *pvParameters);			//Display_Menu_Task�����ʵ��
void Display_Time_Task(void *pvParameters);			//Time_Task�����ʵ��
void Display_MPU6050_Task(void *pvParameters);		//Display_MPU6050_Task�����ʵ��

void FreeRTOS_Init(void)
{
	BaseType_t xReturn = pdPASS;

	//������ʼ����
    xReturn = xTaskCreate((TaskFunction_t )start_task,  //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������ 
	if(pdPASS == xReturn)
		vTaskStartScheduler();          //�����������
	
	while (1);
}
/*--------------------------��������������---------------------------------*/

//��ʼ������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

	/* ����BinarySem_Handle */
	BinarySem_Handle = xSemaphoreCreateBinary();
	if(NULL != BinarySem_Handle)
		printf("BinarySem_Handle��ֵ�ź��������ɹ�!\r\n");
	
	/* ����Test_Queue */
	Test_Queue = xQueueCreate((UBaseType_t) QUEUE_LEN,
								(UBaseType_t) QUEUE_SIZE);
	if(NULL != Test_Queue)
		printf("Test_Queue��Ϣ���д����ɹ�!\r\n");
	
	/* ����Event_Handle */
	Event_Handle = xEventGroupCreate();
	if(NULL != Test_Queue)
		printf("Event_Handle�¼��鴴���ɹ�!\r\n");		
			
	//����Key_Task����
    xTaskCreate((TaskFunction_t )Key_Task,             
                (const char*    )"Key_Task",           
                (uint16_t       )KEY_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&Key_Task_Handler); 			
	printf("����Key_Task�ɹ�!\r\n");
	
	//����OLED��ʾ����
    xTaskCreate((TaskFunction_t )Display_Menu_Task,             
                (const char*    )"Display_Menu_Task",           
                (uint16_t       )MENU_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MENU_TASK_PRIO,        
                (TaskHandle_t*  )&Display_Menu_Task_Handler);  
	printf("����Display_Menu_Task�ɹ�!\r\n");
				
	//����Display_Time_Task����
    xTaskCreate((TaskFunction_t )Display_Time_Task,             
                (const char*    )"Display_Time_Task",           
                (uint16_t       )TIME_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TIME_TASK_PRIO,        
                (TaskHandle_t*  )&Display_Time_Task_Handler); 			
	printf("����Display_Time_Task�ɹ�!\r\n");
				
	//����Display_Time_Tools_Task����
    xTaskCreate((TaskFunction_t )Display_MPU6050_Task,             
                (const char*    )"Display_MPU6050_Task",           
                (uint16_t       )MPU6050_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MPU6050_TASK_PRIO,        
                (TaskHandle_t*  )&Display_MPU6050_Task_Handler); 			
	printf("����Display_MPU6050_Task�ɹ�!\r\n");
				
				
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
				
}

//void Receive_Task(void *pvParameters)
//{
//	BaseType_t xReturn = pdTRUE;//����һ��������Ϣ����ֵ
//	u32 r_queue;
//	while(1)
//	{
//		xReturn = xQueueReceive(Test_Queue,		//��Ϣ���еľ��
//								&r_queue,		//���յ���Ϣ����
//								portMAX_DELAY);	//�ȴ�ʱ�䣺һֱ��
//		if (pdTRUE == xReturn)


//void Send_Task(void *pvParameters)
//{
//	BaseType_t xReturn = pdTRUE;//����һ��������Ϣ����ֵ
//	u32 send_data1 = 111111;
//	xReturn = xQueueSend(Test_Queue,		//��Ϣ���еľ��
//						&send_data1,		//���͵���Ϣ����
//						0);					//�ȴ�ʱ�䣺0		
//	if (pdPASS == xReturn)

	
void Key_Task(void *pvParameters)
{
	
	while(1)
	{		
//		printf("�Ѿ�������Key_task������\r\n");
		if(Key_Scan == key1_down)
		{
			/* KEY1������ */
			Key_Scan = 0;
			printf("KEY1������\r\n");
			xEventGroupSetBits(Event_Handle,KEY1_EVENT);
		}
		
		if(Key_Scan == key3_down)
		{
			/* KEY3������ */
			Key_Scan = 0;
			printf("KEY2������\r\n");
			xEventGroupSetBits(Event_Handle,KEY3_EVENT);
		}
		
		if(Key_Scan == key4_down)
		{
			/* KEY4������ */
			Key_Scan = 0;
			printf("KEY3������\r\n");
			xEventGroupSetBits(Event_Handle,KEY4_EVENT);
		}
		
		vTaskDelay(5);			//ÿ20msɨ��һ��
	}
}

//�˵���ʾ������
void Display_Menu_Task(void *pvParameters)
{
	
	EventBits_t r_event;
	BaseType_t xReturn = pdPASS;//����һ��������Ϣ����ֵ

	static signed char x = 0;//��ʾ�˵�ҳ������ѡ�����������0,1,2,3,4��
	static signed char y = 0;//��ʾ�������λ�ã�0,1,2��
	static signed char z = 0;//(z-1)��ʾ������³�����Ļ�Ĵ�����0,1,2��3��
	static signed char i = 0;//(i-1)��ʾ������ϳ�����Ļ�Ĵ�����0,1,2��3��
	
	static signed char j = 0;//��ʾ��ʾ�ڼ���ҳ�棨0,1,2��

	vTaskSuspend(Display_Time_Task_Handler);		//����ʱ�������ʾ����
	vTaskSuspend(Display_MPU6050_Task_Handler);		//����������������ʾ����

	Display_Menu(0);		//�״ν���˵����˵�ҳ���Լ����λ�õĳ�ʼ��
	Display_Cursor(0,0,0,1);

	while(1)
	{
//		printf("�Ѿ�������Display_Task������\r\n");
		xReturn = xSemaphoreTake(BinarySem_Handle,10);
		if(xReturn == pdTRUE)
		{
			Display_Menu(j);
			Display_Cursor(0,x,y,1);
			printf("����˵���ʾ��������ֵ�ź����ѱ���ȡ!\r\n");
		}
		
		r_event = xEventGroupWaitBits(Event_Handle,
									KEY1_EVENT|KEY3_EVENT|KEY4_EVENT,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY);
		
		if (r_event == KEY1_EVENT)
		{
//			OLED_Clear();					//����1���£���ʾҪ�л������ˣ�������				
	
			if(x == 0)						//������ͣ��ѡ��0����
			{
				OLED_Clear();					//����1���£���ʾҪ�л������ˣ�������	
				xSemaphoreGive(BinarySem_Handle);	//�ͷ�һ����ֵ�ź���
				printf("�˵���ʾ��������Ҫ����ʱ�����������!\r\n");

				vTaskResume(Display_Time_Task_Handler);		//�ָ�ʱ�������ʾ����
				printf("�ɹ��л���ʱ�������ʾ�����Ѿ�����!\r\n");
				vTaskSuspend(NULL);							//����˵���ʾ����

			}
			if(x == 1)						//������ͣ��ѡ��1����
			{
				OLED_Clear();					//����1���£���ʾҪ�л������ˣ�������
				printf("�˵���ʾ��������Ҫ����������������ʾ������!\r\n");

				vTaskResume(Display_MPU6050_Task_Handler);	//�ָ������ǽ�����ʾ����
				
				vTaskSuspend(NULL);							//����˵���ʾ����
			}

		}
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY3_EVENT)
		{
			i=0;
			Display_Cursor(0,x,y,0);		//��ʾ���λ�ã�����0��ʾ���
			x++,y++;
			if (x>4) x = 4;		
			if (y>2) y = 2;		
			if (y == 2) 
			{
				z++;			//������³�����Ļ������1�����������ϵĳ�������
			}
			//�����3 = (�ܹ���ѡ����) - (��Ļ�����ɵ�ѡ����) + 1
			if (z>3) z = 4;		//����z����4����ֹ�������°���3ʱ��������������䵼���䲻������,	
								//Ӧ����������������

			if((4>z) && (z>1) && (y == 2))	//��覴ã���굽�����һ��ҳ��ʱ���·��Ի�ˢ�������Ըĵ��͸�ӷ���ˣ����Ұ����������԰�
			{
				j++;
				if(j>2) j=2;
				OLED_Clear();				//Ҫ�л��˵�������ʾ��������
				Display_Menu(j);
			}
	
			printf("--�£�ѡ�еĹ���ѡ��= %d, ���λ��= %d, ������ʾҳ��= %d, ����ֵ= %d\r\n",x,y,j,i);
			
			if (z == 4) z = 3;				//�ڴ�ʱ�ı�z��ֵ
			Display_Cursor(0,x,y,1);		//����1��ʾ���
					
		}
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY4_EVENT)
		{
			z=0;
			Display_Cursor(0,x,y,0);		//��ʾ���λ�ã�����0��ʾ���
			x--,y--;
			if (x<0) x = 0;		
			if (y<0) y = 0;		
			if (y == 0) 
			{
				i++;			//���ϳ�����Ļ������1�����������µĳ�������
			}
			if (i>3) i = 4;		//����i����4����ֹ�������°���4ʱ��������������䵼���䲻������,	
								//Ӧ����������������

			if((4>i) && (i>1) && (y == 0))
			{
				j--;
				if(j<0) j=0;
				OLED_Clear();			//Ҫ�л��˵�������ʾ��������
				Display_Menu(j);
			}
			
			Display_Cursor(0,x,y,1);	//��ʾ���λ�ã�����1��ʾ���
			
			printf("++�ϣ�ѡ�еĹ���ѡ��= %d, ���λ��= %d, ����ֵ= %d, ������ʾҳ��= %d\r\n",x,y,z,j);
			if (i == 4) i = 3;			//�ڴ�ʱ�ı�i��ֵ
		}

		vTaskDelay(5);			//ÿ20msɨ��һ��
	}
}

//ʱ�������ʾ������
void Display_Time_Task(void *pvParameters)
{
	EventBits_t r_event;
	BaseType_t xReturn = pdPASS;//����һ��������Ϣ����ֵ

	static signed char x = 0;
	static signed char y = 0;
	printf("������ʱ�������ʾ����11111111!\r\n");
//	vTaskSuspend(Display_Menu_Task_Handler);		//����˵���ʾ����

//	Display_Time();

	while(1)
	{
		Display_Time();
		printf("������ʱ�������ʾ������������!\r\n");

		xReturn = xSemaphoreTake(BinarySem_Handle,5);
		if(xReturn == pdTRUE)		//����Ǹմӱ��������ת�����ģ���ˢ�µ���Ļ��ʾ��Ҫ��Ӧ��ʼ��
		{			
			x = 0;y = 0;
			Display_Cursor(1,x,x,1);		//��������񣬹����ڵ�һ������ѡ����
			printf("����ʱ����ʾ���񣬶�ֵ�ź����ѱ���ȡ!\r\n");
		}

		r_event = xEventGroupWaitBits(Event_Handle,
									KEY1_EVENT|KEY3_EVENT|KEY4_EVENT,
									pdTRUE,
									pdFALSE,
									10);
		
		if (r_event == KEY1_EVENT)
		{

			if(x == 3)					//������ͣ�ڡ�back��������
			{
				OLED_Clear();			//����1���£���ʾҪ�л�������				
				xSemaphoreGive(BinarySem_Handle);//�ͷ�һ����ֵ�ź���
				printf("��ʱ����ʾ�����У��ͷ�һ����ֵ�ź���!\r\n");

				vTaskResume(Display_Menu_Task_Handler);		//�ָ��˵���ʾ����
				vTaskSuspend(NULL);		//����ʱ����ʾ����
			}
		}
		
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY3_EVENT)
		{
			Display_Cursor(1,y,y,0);		//��ʾ��չ��
			Display_Down_Cursor(0,0);		//����ײ����

			x++,y++;
			if (y>2) y=2;	//2:��Ļ����ʾ�Ĺ���ѡ��
			if (x>3) x=3;	//3:�ܹ���ѡ��������ؼ���
			if (x==3) 
			{
				Display_Down_Cursor(0,1);	//����4�Σ���굽��ײ������ײ���ҳ��Ϊ0��
				Display_Cursor(1,y,y,0);		//��ʾ������
			}
			if((0<=x) && (x<3))
			{
				Display_Down_Cursor(0,0);		//����ײ����
				Display_Cursor(1,y,y,1);		//��ʾ�����
			}
			
						
		}
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY4_EVENT)
		{
			Display_Cursor(1,x,x,0);		//��ʾ��չ��
			Display_Down_Cursor(0,0);		//����ײ����
	
			if (x!=3) y--;
			x--;
			if (x<0) x=0;
			if (y<0) y=0;
			Display_Cursor(1,x,x,1);		//��ʾ�����
		}

		vTaskDelay(5);			//ÿ20msɨ��һ��
	}
}

//�����ǽ�����ʾ����
void Display_MPU6050_Task(void *pvParameters)
{
	EventBits_t r_event;
	
	while(1)
	{
		Display_MPU6050();
		Display_Down_Cursor(1,1);	//��ʾ�ײ��Ĺ��
		printf("������������������ʾ����!\r\n");

		r_event = xEventGroupWaitBits(Event_Handle,
									KEY1_EVENT|KEY3_EVENT|KEY4_EVENT,
									pdTRUE,
									pdFALSE,
									10);
		
		if (r_event == KEY1_EVENT)
		{
			OLED_Clear();				//����1���£���ʾҪ�л�������				
			xSemaphoreGive(BinarySem_Handle);	//�ͷ�һ����ֵ�ź���

			vTaskResume(Display_Menu_Task_Handler);		//�ָ��˵���ʾ����
			vTaskSuspend(NULL);			//������������ʾ����
		}		

		vTaskDelay(5);			//ÿ20msɨ��һ��
	}
}

//���������Ӻ���
void vApplicationIdleHook(void)
{
	AppTaskCreaterun = 0;  // ������������ʱAppTaskCreaterun=1
	flagTask1run = 0;     // ����1����ʱflagTask1run=1
	flagTask2run = 0;     // ����2����ʱflagTask2run=1	
}
