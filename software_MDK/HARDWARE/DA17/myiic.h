#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

//寸 A12--SDA  A11--SCL
//关 B8--SDA   B9--SCL
//尺 A5--SDA   A1--SCL


//after change mcu
//寸 B4--SDA   B3--SCL
//关 B6--SDA   B7--SCL
//尺 B9--SDA   B8--SCL



#define  SDA_IN()  {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=(uint32_t)8<<16;}
#define  SDA_OUT() {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=(uint32_t)3<<16;}
#define  IIC_SCL0  GPIO_ResetBits(GPIOB,GPIO_Pin_3)
#define  IIC_SDA0  GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define  IIC_SCL1	 GPIO_SetBits(GPIOB,GPIO_Pin_3)	
#define  IIC_SDA1	 GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define  READ_SDA  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==1 

#define  SDA2_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=(uint32_t)8<<24;}
#define  SDA2_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=(uint32_t)3<<24;}
#define  IIC2_SCL0  GPIO_ResetBits(GPIOB,GPIO_Pin_7)	
#define  IIC2_SDA0  GPIO_ResetBits(GPIOB,GPIO_Pin_6)	
#define  IIC2_SCL1	GPIO_SetBits(GPIOB,GPIO_Pin_7)	
#define  IIC2_SDA1	GPIO_SetBits(GPIOB,GPIO_Pin_6)	
#define  READ2_SDA  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==1


#define  SDA3_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(uint32_t)8<<4;}
#define  SDA3_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(uint32_t)3<<4;}
#define  IIC3_SCL0  GPIO_ResetBits(GPIOB,GPIO_Pin_8)	
#define  IIC3_SDA0  GPIO_ResetBits(GPIOB,GPIO_Pin_9)	
#define  IIC3_SCL1	GPIO_SetBits(GPIOB,GPIO_Pin_8)	
#define  IIC3_SDA1	GPIO_SetBits(GPIOB,GPIO_Pin_9)	
#define  READ3_SDA  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==1

//IIC所有操作函数
void DA17_IIC_Init(void);                      //初始化IIC的IO?
void IIC_Start(void);			              	//发送IIC开始信号
void IIC_Stop(void);	  		            	//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);	    		//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack); //IIC读取一个字节
uint8_t IIC_Wait_Ack (void); 				      //IIC等待ACK信号
void IIC_Ack(void);				              	//IIC发送ACK信号
void IIC_NAck(void);			              	//IIC不发送ACK信号  

//IIC所有操作函数
//void IIC2_Init(void);                      //初始化IIC的IO口				 
void IIC2_Start(void);			              	//发送IIC开始信号
void IIC2_Stop(void);	  		            	//发送IIC停止信号
void IIC2_Send_Byte(uint8_t txd);	    		//IIC发送一个字节
uint8_t IIC2_Read_Byte(unsigned char ack); //IIC读取一个字节
uint8_t IIC2_Wait_Ack (void); 				      //IIC等待ACK信号
void IIC2_Ack(void);				              	//IIC发送ACK信号
void IIC2_NAck(void);			              	//IIC不发送ACK信号  

//IIC所有操作函数
//void IIC3_Init(void);                      //初始化IIC的IO口				 
void IIC3_Start(void);			              	//发送IIC开始信号
void IIC3_Stop(void);	  		            	//发送IIC停止信号
void IIC3_Send_Byte(uint8_t txd);	    		//IIC发送一个字节
uint8_t IIC3_Read_Byte(unsigned char ack); //IIC读取一个字节
uint8_t IIC3_Wait_Ack (void); 				      //IIC等待ACK信号
void IIC3_Ack(void);				              	//IIC发送ACK信号
void IIC3_NAck(void);			              	//IIC不发送ACK信号  

#endif
















