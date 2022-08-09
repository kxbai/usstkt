#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "draw_api.h"
#include "test.h"
#include "led.h"
#include "lcd.h"
#include "common.h"
#include "i2c_soft.h"
#include "ADC.h"
#include "DS3231.h"
#include "NVIC.h"
#include "bme280.h"
#include "DA17.h"
#include "myiic.h"
#include "dma.h"

//#include "test.h"
//#include "OLED.h"
//#include "spi.h"


//extern unsigned char wifi_time_cnt;
#define SEND_BUF_SIZE 20	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.
enum
{
	LA =262,
	LB =294,
	LC =330,
	LD =349,
	LE =392,
	LF =440,
	LG =494,
	
	MA =523,
	MB =578,
	MC =659,
	MD =698,
	ME =784,
	MF =880,
	MG =988,
	
	HA =1064,
	HB =1175,
	HC =1318,
	HD =1397,
	HE =1568,
	HF =1760,
	HG =1976
};


//void power_pin_init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//	 PB12 POWER控制端口
// 	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//速度
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_12);	 
//}
extern bool bme_flag;
uint8_t  Data_Pressure[20];
//uint8_t  SendBuff[SEND_BUF_SIZE]={0};	//发送数据缓冲区
void bme_update(void)
{
	if(bme_flag)
		{
			bme_flag=0;
			readTrim();
			bme280CompensateH();
			bme280CompensateP();
			bme280CompensateT();
		}
}

extern short pitch_a,roll_a,yaw_a;
void c_setup()
{
	char i=0,count=0;
//	power_pin_init();
	SystemInit();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2	  
	delay_init();	    	 //延时函数初始?
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
  uart_init(115200);	 	//串口初始化为115200
  MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)Data_Pressure,SEND_BUF_SIZE);//DMA1通道4,外设为串口1,存储器为SendBuff,长度SEND_BUF_SIZE.
	LCD_Init();               //初始化OLED接口
	Adc_Init();		  		//ADC初始化
	KEY_INT_INIT();
	millis_init();
	console_log(500,"init_OLED: OK");
	console_log(10,"--- welcome !---");
	console_log(10,"  _   _    _    ");
	console_log(10," | | | |  |_|   ");
	console_log(10," | |_| |   _    ");
	console_log(10," |  _  |  | |   ");
	console_log(10," | | | |  | |   ");
	console_log(10," |_| |_|  |_|   ");
	console_log(10,"                ");
	I2C_GPIO_Config();
	delay_ms(50);
	i = MPU_Init();
	delay_ms(50);
	i=mpu_dmp_init();
	while((++count)<5&&i)
	{
		console_log(1,"MPU_Error:%d",i);
		delay_ms(100);
		i=mpu_dmp_init();
	}
	console_log(500,"init_MPU: OK");
	milliseconds=0;
	
	memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
	
	DS3231_Init();             //DS3231初始化 
	appconfig_init();
	//led_init();              //初始化LED
	buzzer_init();	
	buttons_init();
	Bme280_Init();
	//global_init();
	alarm_init();              //无法储存闹钟，每次重启以后需要自定义
	//time_init();
	//Date_init();
	Set_DS3231_date();
	pwrmgr_init();
//  uart_init(115200);
  DA17_IIC_Init(); 
  Data_Pressure[0]=0x00;
  Data_Pressure[19]=0xaa;
	console_log(500,"START !");
//	while(1)
//	{
//		if(MPU_Get_Gyroscope(&pitch_a,&roll_a,&yaw_a)==0)
//		console_log(20,"roll_a = %d",roll_a);
//		i=MPU_Roll_Detect();
//		console_log(20,"roll_detect = %d",i);
//	}
	// Set watchface
	display_set(watchface_normal);
	display_load();//启动表盘	
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送      

}

bool bme_enable = 1;
bool DA17_enable = 0;
void c_loop()
{
    time_update();
	if(pwrmgr_userActive())
	{
		//battery_update();
		buttons_update();
	}
  
	//mpu_updata(); 

  	buzzer_update();
    //led_update();
	
	
#if COMPILE_STOPWATCH
	stopwatch_update();
#endif
//  global_update();
	
	if(pwrmgr_userActive())
	{
		alarm_update();
		display_update();
	}
	if(bme_enable)
		bme_update();

	pwrmgr_update();
//显示完成后清除缓冲区
	memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
  GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5)));
//if(DA17_enable)		
//  Pluse_Test();
}


int main(void)
{ 
	
	//printf("begin>>>\r\n");
	
	c_setup();	    //初始化
	while(1)
	{

		c_loop(); //循环
	}
	
}



const uint32_t STAY[]={

	//5353
	ME<<16 | 250,
	MC<<16 | 250,
	ME<<16 | 250,
	MC<<16 | 250,
	//222321
	MB<<16 | 125,
	MB<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	MB<<16 | 125,
	MA<<16 | 250,
	//7115
	LG<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	ME<<16 | 500,
	//177777111
	MA<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 250,
	//1715
	MA<<16 | 125,
	LG<<16 | 125,
	MA<<16 | 125,
	ME<<16 | 500,
	//177777111
	MA<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 250,
	//1715
	MA<<16 | 125,
	LG<<16 | 125,
	MA<<16 | 125,
	ME<<16 | 500,
	//177777111
	MA<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	LG<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 250,
	//1715
	MA<<16 | 125,
	LG<<16 | 125,
	MA<<16 | 125,
	ME<<16 | 500,
	//71275
	LG<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MG<<16 | 125,
	ME<<16 | 500,
	
	
	
	TONE_REPEAT

};  //旋律


const uint32_t TUNE[]={
	
	
	LF<<16 | 250,
	LC<<16 | 250,
	HF<<16 | 250,
	MC<<16 | 250,
	
	LD<<16 | 250,
	MA<<16 | 250,
	MD<<16 | 250,
	MA<<16 | 250,
	
	LE<<16 | 250,
	MB<<16 | 250,
	ME<<16 | 250,
	MB<<16 | 250,
	
	MA<<16 | 250,
	ME<<16 | 250,
	HA<<16 | 250,
	ME<<16 | 250,
	
	LF<<16 | 250,
	LC<<16 | 250,
	HF<<16 | 250,
	MC<<16 | 250,
	
	LD<<16 | 250,
	MA<<16 | 250,
	MD<<16 | 250,
	MA<<16 | 250,
	
	LE<<16 | 250,
	MB<<16 | 250,
	ME<<16 | 250,
	MB<<16 | 250,
	//1 5123
	MA<<16 | 500,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	//2111
	MB<<16 | 250,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 500,
	
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	
	//212233
	MB<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 250,
	MC<<16 | 125,
	
	//35123
	MC<<16 | 500,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	
	//2111
	MB<<16 | 250,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 500,
	
	
	//05123
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	//212253
	MB<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	MB<<16 | 125,
	ME<<16 | 250,
	MC<<16 | 125,
	
	//334
	MC<<16 | 500,
	MC<<16 | 250,
	MD<<16 | 250,
	
	//55555
	ME<<16 | 250,
	ME<<16 | 125,
	ME<<16 | 125,
	ME<<16 | 250,
	ME<<16 | 250,
	
	//531134
	ME<<16 | 250,
	MC<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	MC<<16 | 250,
	MD<<16 | 125,
	
	//55555
	ME<<16 | 250,
	ME<<16 | 125,
	ME<<16 | 125,
	ME<<16 | 250,
	ME<<16 | 250,
	//531 112
	ME<<16 | 250,
	MC<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	
	//33333
	MC<<16 | 250,
	MC<<16 | 125,
	MC<<16 | 125,
	MC<<16 | 250,
	MC<<16 | 250,
	//366321
	MC<<16 | 250,
	LF<<16 | 250,
	MF<<16 | 125,
	MC<<16 | 125,
	MB<<16 | 125,
	LA<<16 | 250,
	
	//20
	MB<<16 | 500,
	TONE_PAUSE<<16 | 125,
	
	//05123
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	//2111
	MB<<16 | 250,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 500,
	//05123
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	
	//212233
	MB<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 250,
	MC<<16 | 125,
	
	//35123
	MC<<16 | 500,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	//2111
	MB<<16 | 250,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 500,
	//05123
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	//212253
	MB<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	MB<<16 | 125,
	ME<<16 | 250,
	MC<<16 | 125,
	
	
	
	//334
	MC<<16 | 500,
	MC<<16 | 250,
	MD<<16 | 250,
	
	//55555
	ME<<16 | 250,
	ME<<16 | 125,
	ME<<16 | 125,
	ME<<16 | 250,
	ME<<16 | 250,
	
	//531134
	ME<<16 | 250,
	MC<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	MC<<16 | 250,
	MD<<16 | 125,
	
	//55555
	ME<<16 | 250,
	ME<<16 | 125,
	ME<<16 | 125,
	ME<<16 | 250,
	ME<<16 | 250,
	//531 112
	ME<<16 | 250,
	MC<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	
	//33333
	MC<<16 | 250,
	MC<<16 | 125,
	MC<<16 | 125,
	MC<<16 | 250,
	MC<<16 | 250,
	//363216
	MC<<16 | 500,
	MF<<16 | 125,
	MC<<16 | 125,
	MB<<16 | 125,
	MA<<16 | 125,
	LF<<16 | 125,
	
	//10
	MA<<16 | 500,
	TONE_PAUSE<<16 | 250,
	//0
	//TONE_PAUSE<<16 | 3000,
	
	//55555
	ME<<16 | 250,
	ME<<16 | 125,
	ME<<16 | 125,
	ME<<16 | 250,
	ME<<16 | 250,
	
	//531134
	ME<<16 | 250,
	MC<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 125,
	MC<<16 | 250,
	MD<<16 | 125,
	
	//55555
	ME<<16 | 250,
	ME<<16 | 125,
	ME<<16 | 125,
	ME<<16 | 250,
	ME<<16 | 250,
	//531 112
	ME<<16 | 250,
	MC<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	
	
	
	//33333
	MC<<16 | 250,
	MC<<16 | 125,
	MC<<16 | 125,
	MC<<16 | 250,
	MC<<16 | 250,
	//366321
	MC<<16 | 250,
	LF<<16 | 250,
	MF<<16 | 125,
	MC<<16 | 125,
	MB<<16 | 125,
	LA<<16 | 250,
	
	//20
	MB<<16 | 500,
	TONE_PAUSE<<16 | 125,
	
	//05123
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	//2111
	MB<<16 | 250,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 500,
	//05123
	TONE_PAUSE<<16 | 125,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	
	//212233
	MB<<16 | 250,
	MA<<16 | 125,
	MB<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 250,
	MC<<16 | 125,
	
	//35123
	MC<<16 | 500,
	LE<<16 | 125,
	MA<<16 | 125,
	MB<<16 | 125,
	MC<<16 | 125,
	
	//2111
	MB<<16 | 250,
	MA<<16 | 125,
	MA<<16 | 125,
	MA<<16 | 500,
	
	TONE_PAUSE<<16 | 2000,
	
	TONE_REPEAT

};  //旋律

