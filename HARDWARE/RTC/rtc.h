#ifndef __RTC_H
#define __RTC_H	    
//Mini STM32开发板
//RTC实时时钟 驱动代码			 
//正点原子@ALIENTEK
//2010/6/6

//时间结构体
typedef struct 
{
	volatile unsigned char hour;
	volatile unsigned char min;
	volatile unsigned char sec;			
	//公历日月年周
	volatile unsigned short w_year;
	volatile unsigned char  w_month;
	volatile unsigned char  w_date;
	volatile unsigned char  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体

extern unsigned char const mon_table[12];	//月份日期数据表
void Disp_Time(unsigned char x,unsigned char y,unsigned char size);//在制定位置开始显示时间
void Disp_Week(unsigned char x,unsigned char y,unsigned char size,unsigned char lang);//在指定位置显示星期
unsigned char RTC_Init(void);       //初始化RTC,返回0,失败;1,成功;
unsigned char Is_Leap_Year(volatile unsigned short year);//平年,闰年判断
unsigned char RTC_Get(void);         //更新时间   
unsigned char RTC_Get_Week(volatile unsigned short year,unsigned char month,unsigned char day);
unsigned char RTC_Set(volatile unsigned short syear,unsigned char smon,unsigned char sday,unsigned char hour,unsigned char min,unsigned char sec);//设置时间			 
#endif


