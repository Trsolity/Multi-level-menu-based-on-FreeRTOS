#include "rtc.h"
#include "display.h"

float Pitch,Roll,Yaw;


//��ʾ�˵���ҳ��
//��ʾ�˵���������������ƶ�
void Display_Menu(u8 x)
{
//	u8 first = 0;
//	u8 second = 0;
//	u8 third = 0;

	OLED_ShowString(50,0,"Menu",16);
	if(x == 0)
	{
		OLED_ShowString(5,18,"time",12);	
		OLED_ShowString(5,32,"display",12);	
		OLED_ShowString(5,46,"game",12);
	}
	if(x == 1)
	{
		OLED_ShowString(5,18,"display",12);	
		OLED_ShowString(5,32,"game",12);	
		OLED_ShowString(5,46,"pwm",12);
	}
	if(x == 2)
	{
		OLED_ShowString(5,18,"game",12);	
		OLED_ShowString(5,32,"pwm",12);
		OLED_ShowString(5,46,"music",12);
	}
	OLED_Refresh_Gram();
}

//��ʾ���(���ο�
/*	x:��ʾ�����ĸ�����
	y:��ʾ�˵��б�ڼ���ѡ��;
	z:��ʾ�������Ļ��������λ��;
	i:1��ʾ��䣬0��ʾ���*/
void Display_Cursor(u8 x,u8 y,u8 z,u8 i)
{
	//������	  
	//(x1,y1),(x2,y2):���εĶԽ�����
	u8 x1 = 2;
	u8 y1 ;
	u8 x2;    //y:��ʾ�ò˵��б���
	u8 y2;
	int tools_length[]={4,7,4,3,5};
	int time_length[]={5,9,11};
	int high[4]={16,30,45,60};//{16,31,47,62};
	
	if(x == 0) x2 = x1+7*tools_length[y];
	if(x == 1) 
	{
		x2 = x1+7*time_length[y];
	}
	y1 = high[z];
	y2 = high[z+1];

	OLED_DrawRectangle(x1, y1, x2, y2, i);			//��Ȧ
	OLED_DrawRectangle(x1+1, y1+1, x2-1, y2-1, i);	//��Ȧ
	OLED_Refresh_Gram();

}

//��ʾ�ײ��Ĺ��(���ο�
/*	x:��ʾ�����ĸ�ѡ�����	
	i:1��ʾ��䣬0��ʾ���*/
void Display_Down_Cursor(u8 x,u8 i)
{
	u8 x1 = 93;
	u8 y1 = 47;
	u8 x2;   
	u8 y2;

	int Down_length[]={4};		
	int Down_high[]={48,63};	
	if((x == 0) || (x == 1))
	{
		x2 = x1+7*Down_length[0];
	}
	y1 = Down_high[0];
	y2 = Down_high[1];
	
	OLED_DrawRectangle(x1, y1, x2, y2, i);			//��Ȧ
	OLED_DrawRectangle(x1+1, y1+1, x2-1, y2-1, i);	//��Ȧ
	OLED_Refresh_Gram();
}

//��ʾʱ���ҳ��
void Display_Time(void)
{
	u8 y=0;

	//��ʾʱ��			 
	OLED_ShowString(24,0,"/",12);
	OLED_ShowString(42,0,"/",12);
	OLED_ShowString(90,0,":",12);
	OLED_ShowString(108,0,":",12);
	if(y!=calendar.sec)
	{
		y=calendar.sec;
		OLED_ShowNum(0,0,calendar.w_year,4,12);									  
		OLED_ShowNum(30,0,calendar.w_month,2,12);									  
		OLED_ShowNum(48,0,calendar.w_date,2,12);	 
			
		OLED_ShowNum(78,0,calendar.hour,2,12);									  
		OLED_ShowNum(96,0,calendar.min,2,12);									  
		OLED_ShowNum(114,0,calendar.sec,2,12);
		OLED_Refresh_Gram();
	}
	OLED_ShowString(5,18,"count",12);	
	OLED_ShowString(5,32,"clock_set",12);	
	OLED_ShowString(5,46,"time_change",12);
	Display_Choice(1);

	OLED_Refresh_Gram();
}
//��ʾmpu6050����
void Display_MPU6050(void)
{
	u8 x;
	u8 y;
	u8 z;
	
	Read_DMP(&Pitch,&Roll,&Yaw);
	x = (int)100*Pitch;
	y = (int)100*Roll;
	z = (int)100*Yaw;
	
	OLED_ShowString(0,0,"Pitch:",16);
	OLED_ShowString(0,16,"Roll:",16);
	OLED_ShowString(0,32,"Yaw:",16);


	OLED_ShowNum(50,0,(int)Pitch,3,16);		//Pitch��������
	OLED_ShowNum(84,0,(int)x%100,2,16);		//PitchС������
	OLED_ShowNum(50,16,(int)Roll,3,16);		//Roll��������
	OLED_ShowNum(84,16,(int)y%100,2,16);	//RollС������
	OLED_ShowNum(50,32,(int)Yaw,3,16);		//Yaw��������
	OLED_ShowNum(84,32,(int)z%100,2,16);	//YawС������
	
	OLED_ShowString(74,0,".",16);	//��С����
	OLED_ShowString(74,16,".",16);
	OLED_ShowString(74,32,".",16);
	
	Display_Choice(2);				//��ʾ�ײ��ķ��ؼ�
	
	OLED_Refresh_Gram();
}

//��ʾ�����·���ѡ��
void Display_Choice(u8 c)
{
	switch (c)
	{
		case 0:
			OLED_ShowString(0,7,"Menu",8); 
			OLED_ShowString(78,58,"Change",8);
			break;
		case 1:
			OLED_ShowString(96,50,"Back",12);
			break;
		case 2:
			OLED_ShowString(96,50,"Back",12);
			break;
		case 3:
			OLED_ShowString(0,7,"stop ",8);
			OLED_ShowString(96,7,"Bye",8);
			break;
		case 4:
//			OLED_ShowImage(0,0,128,64,end);
			OLED_ShowString(0,7,"Again",8);
			OLED_ShowString(96,7,"Bye",8);
			break;
		case 5:
//			OLED_ShowTimes(2,10,1);
			OLED_ShowString(96,7,"Bye",8);
			OLED_ShowString(94,0,"Init",8);
			break;
		case 6:
			OLED_ShowString(0,7,"OK",8);
			OLED_ShowString(78,7,"Delete",8);
			break;
		default:
			break;
	}
}
