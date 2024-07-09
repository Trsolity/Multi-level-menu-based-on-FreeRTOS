#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
extern u8 Key_Scan;

extern u8 key1_down;
extern u8 key2_down;
extern u8 key3_down;
extern u8 key4_down;

#define KEY1_PORT		GPIOA
#define KEY1_PIN		GPIO_Pin_15
#define KEY1_PORT_RCC	RCC_APB2Periph_GPIOA

#define KEY2_PORT		GPIOA
#define KEY2_PIN		GPIO_Pin_14
#define KEY2_PORT_RCC	RCC_APB2Periph_GPIOA

#define KEY3_PORT		GPIOA
#define KEY3_PIN		GPIO_Pin_13
#define KEY3_PORT_RCC	RCC_APB2Periph_GPIOA

#define KEY4_PORT		GPIOA
#define KEY4_PIN		GPIO_Pin_12
#define KEY4_PORT_RCC	RCC_APB2Periph_GPIOA

//#define KEY1	PAin(15)
//#define KEY2	PAin(14)
//#define KEY3	PAin(13)
//#define KEY4	PAin(12)

//#define KEY1_PRESS		1
//#define KEY2_PRESS		2
//#define KEY3_PRESS		3
//#define KEY4_PRESS		4

void KEY_Init(void);
//u8 Key_Scan(u8 mode);


/*���尴��״̬ö�����ö�ٶ����˰������ܵ�����״̬��KEY_DOWN ��ʾ�������£�KEY_UP ��ʾ����̧��*/
typedef enum {
    KEY_DOWN,
    KEY_UP,
} KeyStatus;
 
/*���尴���¼�ö�٣����ö�ٶ����˰������ܵ������¼���NULL_PRESS ��ʾû�а����¼���SHORT_PRESS ��ʾ�̰���LONG_PRESS ��ʾ������*/
//���Ұ���Щ�¼��ĳ��˷ֱ���KEY1,KET2,KEY3,KEY4���ް�������
typedef enum {
	KEY1_PRESS,
    KEY2_PRESS,
	KEY3_PRESS,
	KEY4_PRESS,
	NULL_PRESS
} KeyEvent;
 
/*���尴�����ýṹ�壬����ṹ������˰�����״̬ (status)���¼����� (event) �ͳ������� (longPressCount)�������ڴ洢�����������Ϣ��*/
typedef struct {
    KeyStatus status;
    KeyEvent event;
    uint16_t longPressCount;
} KeyHandler;
 
void initKeyHandler(KeyHandler *handler);
void key_handler(KeyHandler *handler);
/*
Key_Init �����������ڳ�ʼ������Ӳ���������ṩ�Ĵ����в�û�о����ʵ�֡�
initKeyHandler �������ڳ�ʼ�� KeyHandler �ṹ���ʵ������������״̬���¼��ͼ���������Ϊ��ʼֵ��
key_handler �������ڴ�������״̬���¼���������һ�� KeyHandler �ṹ���ʵ����Ϊ������������ʵ��������½ṹ��ĳ�Ա��
��Щ�����ṩ��һ�������Ŀ�ܣ��������ڴ�������״̬���¼�������Ҫ���ݾ����Ӳ��������ʵ�� Key_Init �����Գ�ʼ������Ӳ����������ʵ�����ʵ�� key_handler ������*/

#endif

