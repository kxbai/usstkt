/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2022,Sean Song,China,USST.
  *                            All Rights Reserved                          
  *                         By 上海理工大学 Sean Song
  *                          https://github.com/kxbai
  *                              szc980@163.com
  *
  * FileName   : DA17.c                
  * Version    : v1.0		
  * Author     : Sean Song		
  * Date       : 2022-02-02         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */
#include "DA17.h"
#include "myiic.h"
#include "string.h"
#include "delay.h"


uint8_t  Read_Data[5];
uint8_t  Read2_Data[5];
uint8_t  Read3_Data[5];
uint32_t data_send[3];
uint16_t j=0;
uint16_t TempData=0;

uint32_t Pressure_Dis_Cun;
uint32_t Pressure_Dis_Guan;
uint32_t Pressure_Dis_Chi;

extern uint32_t  send[6];
extern uint8_t  Data_Pressure[20];
void Send_Read_commond(void)
{
    IIC_Start();
    IIC_Send_Byte(0xda);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x30);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x0a);
    IIC_Wait_Ack();
    IIC_Stop();
}
uint8_t Read_Status(void)
{
    uint8_t Status=0;
    IIC_Start();
    IIC_Send_Byte(0xda);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x02);
    IIC_Wait_Ack();

    IIC_Start();
    IIC_Send_Byte(0xdb);
    IIC_Wait_Ack();
    Status=IIC_Read_Byte(1);
    IIC_Stop();

    return  Status;
}
uint32_t Read_Pressure_Data(void)
{
    memset(Read_Data,0,5);
    uint32_t Pressure_data=0;
    IIC_Start();
    IIC_Send_Byte(0xda);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x06);
    IIC_Wait_Ack();

    IIC_Start();
    IIC_Send_Byte(0xdb);
    IIC_Wait_Ack();
    Read_Data[0]=IIC_Read_Byte(1);//23:16
    Read_Data[1]=IIC_Read_Byte(1);//15:8
    Read_Data[2]=IIC_Read_Byte(1);//7:0
    Read_Data[3]=IIC_Read_Byte(1);
    Read_Data[4]=IIC_Read_Byte(0);
    IIC_Stop();

    Pressure_data = (Read_Data[0]<<16)|(Read_Data[1]<<8)|Read_Data[2];
    Pressure_data = Pressure_data/32;

//	TempData = ((Read_Data[3]<<8)|Read_Data[4])/256;
    return Pressure_data;
}

void Send_Read2_commond(void)
{
    IIC2_Start();
    IIC2_Send_Byte(0xda);
    IIC2_Wait_Ack();
    IIC2_Send_Byte(0x30);
    IIC2_Wait_Ack();
    IIC2_Send_Byte(0x0a);
    IIC2_Wait_Ack();
    IIC2_Stop();
}
uint8_t Read2_Status(void)
{
    uint8_t Status=0;
    IIC2_Start();
    IIC2_Send_Byte(0xda);
    IIC2_Wait_Ack();
    IIC2_Send_Byte(0x02);
    IIC2_Wait_Ack();

    IIC2_Start();
    IIC2_Send_Byte(0xdb);
    IIC2_Wait_Ack();
    Status=IIC2_Read_Byte(1);
    IIC2_Stop();

    return  Status;
}
uint32_t Read2_Pressure_Data(void)
{
    memset(Read2_Data,0,5);
    uint32_t Pressure_data=0;
    IIC2_Start();
    IIC2_Send_Byte(0xda);
    IIC2_Wait_Ack();
    IIC2_Send_Byte(0x06);
    IIC2_Wait_Ack();
    IIC2_Start();
    IIC2_Send_Byte(0xdb);
    IIC2_Wait_Ack();
    Read2_Data[0]=IIC2_Read_Byte(1);//23:16
    Read2_Data[1]=IIC2_Read_Byte(1);//15:8
    Read2_Data[2]=IIC2_Read_Byte(1);//7:0
    Read2_Data[3]=IIC2_Read_Byte(1);
    Read2_Data[4]=IIC2_Read_Byte(0);
    IIC2_Stop();

    Pressure_data = (Read2_Data[0]<<16)|(Read2_Data[1]<<8)|Read2_Data[2];
    Pressure_data = Pressure_data/32;

    TempData = ((Read2_Data[3]<<8)|Read2_Data[4])/256;
    return Pressure_data;
}

void Send_Read3_commond(void)
{
    IIC3_Start();
    IIC3_Send_Byte(0xda);
    IIC3_Wait_Ack();
    IIC3_Send_Byte(0x30);
    IIC3_Wait_Ack();
    IIC3_Send_Byte(0x0a);
    IIC3_Wait_Ack();
    IIC3_Stop();
}
uint8_t Read3_Status(void)
{
    uint8_t Status=0;
    IIC3_Start();
    IIC3_Send_Byte(0xda);
    IIC3_Wait_Ack();
    IIC3_Send_Byte(0x02);
    IIC3_Wait_Ack();

    IIC3_Start();
    IIC3_Send_Byte(0xdb);
    IIC3_Wait_Ack();
    Status=IIC3_Read_Byte(1);
    IIC3_Stop();

    return  Status;
}
uint32_t Read3_Pressure_Data(void)
{

    memset(Read3_Data,0,5);

    uint32_t Pressure_data=0;
//  uint32_t TempData=0;
    IIC3_Start();
    IIC3_Send_Byte(0xda);
    IIC3_Wait_Ack();
    IIC3_Send_Byte(0x06);
    IIC3_Wait_Ack();

    IIC3_Start();
    IIC3_Send_Byte(0xdb);
    IIC3_Wait_Ack();
    Read3_Data[0]=IIC3_Read_Byte(1);//23:16
    Read3_Data[1]=IIC3_Read_Byte(1);//15:8
    Read3_Data[2]=IIC3_Read_Byte(1);//7:0
    Read3_Data[3]=IIC3_Read_Byte(1);
    Read3_Data[4]=IIC3_Read_Byte(0);
    IIC3_Stop();

    Pressure_data = (Read3_Data[0]<<16)|(Read3_Data[1]<<8)|Read3_Data[2];
    Pressure_data = Pressure_data/32;
//	TempData = ((Read_Data[3]<<8)|Read_Data[4])/256;

    return Pressure_data;
}

void Pressure_Data(void)
{
    
    Send_Read2_commond();
    Send_Read3_commond();
    Send_Read_commond();
    delay_ms(3);
    
    data_send[0] = Read_Pressure_Data();
    data_send[1] = Read2_Pressure_Data();
    data_send[2] = Read3_Pressure_Data();
//    Pressure_Dis_Cun  = data_send[0];
//    Pressure_Dis_Guan = data_send[1];
//    Pressure_Dis_Chi  = data_send[2];
    Data_Pressure[1]=byte2(data_send[0]);
    Data_Pressure[2]=byte1(data_send[0]);
    Data_Pressure[3]=byte0(data_send[0]);
    Data_Pressure[4]=byte2(data_send[1]);
    Data_Pressure[5]=byte1(data_send[1]);
    Data_Pressure[6]=byte0(data_send[1]);
    Data_Pressure[7]=byte2(data_send[2]);
    Data_Pressure[8]=byte1(data_send[2]);
    Data_Pressure[9]=byte0(data_send[2]);

    
    Send_Read2_commond();
    Send_Read3_commond();
    Send_Read_commond();
    delay_ms(3);
    data_send[0] = Read_Pressure_Data();
    data_send[1] = Read2_Pressure_Data();
    data_send[2] = Read3_Pressure_Data();
    Pressure_Dis_Cun  = data_send[0];
    Pressure_Dis_Guan = data_send[1];
    Pressure_Dis_Chi  = data_send[2];
    Data_Pressure[10]=byte2(data_send[0]);
    Data_Pressure[11]=byte1(data_send[0]);
    Data_Pressure[12]=byte0(data_send[0]);
    Data_Pressure[14]=byte2(data_send[1]);
    Data_Pressure[14]=byte1(data_send[1]);
    Data_Pressure[15]=byte0(data_send[1]);
    Data_Pressure[16]=byte2(data_send[2]);
    Data_Pressure[17]=byte1(data_send[2]);
    Data_Pressure[18]=byte0(data_send[2]);

//    RedBuffer[j] = data_send[1];
//	  j++;
//    if(j==256)
//      {
//        j=0;
//        HeartRate=GetHeartRate();
//        OLED_Dis_Heart(TempData*10,HeartRate,3,1,1);
//      }

//    if(k==5)
//    {
//        k=0;
//        RedBuffer[j] = data_send[1];
//        j++;
//        if(j==256)
//        {
//            j=0;
//            HeartRate=GetHeartRate();
//            OLED_Dis_Heart(TempData*10,HeartRate,3,1,1);
//        }
//    }


//		Data_Pressure[0]=Read_Data[0];
//		Data_Pressure[1]=Read_Data[1];
//		Data_Pressure[2]=Read_Data[2];
//		Data_Pressure[3]=Read2_Data[0];
//		Data_Pressure[4]=Read2_Data[1];
//		Data_Pressure[5]=Read2_Data[2];
//		Data_Pressure[6]=Read3_Data[0];
//		Data_Pressure[7]=Read3_Data[1];
//		Data_Pressure[8]=Read3_Data[2];

}

void Pluse_Test(void)
{
//  uint8_t t;
//    Data_Pressure[1]=byte2(send[0]);
//    Data_Pressure[2]=byte1(send[0]);
//    Data_Pressure[3]=byte0(send[0]);
//    Data_Pressure[4]=byte2(send[1]);
//    Data_Pressure[5]=byte1(send[1]);
//    Data_Pressure[6]=byte0(send[1]);
//    Data_Pressure[7]=byte2(send[2]);
//    Data_Pressure[8]=byte1(send[2]);
//    Data_Pressure[9]=byte0(send[2]);
//    Data_Pressure[10]=byte2(send[0]);
//    Data_Pressure[11]=byte1(send[0]);
//    Data_Pressure[12]=byte0(send[0]);
//    Data_Pressure[14]=byte2(send[1]);
//    Data_Pressure[14]=byte1(send[1]);
//    Data_Pressure[15]=byte0(send[1]);
//    Data_Pressure[16]=byte2(send[2]);
//    Data_Pressure[17]=byte1(send[2]);
//    Data_Pressure[18]=byte0(send[2]);
//    for(t=0;t<20;t++)
//		{
//				USART_SendData(USART1,Data_Pressure[t]);//Ïò´®¿Ú1·¢ËÍÊý¾Ý
////        USART_SendData(USART2,Data_Pressure[t]);//Ïò´®¿Ú1·¢ËÍÊý¾Ý
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø				
////				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//µÈ´ý·¢ËÍ½áÊø
//		}
//    return 0;

    Data_Pressure[1]=byte2(send[0]);
    Data_Pressure[2]=byte1(send[0]);
    Data_Pressure[3]=byte0(send[0]);
    Data_Pressure[4]=byte2(send[1]);
    Data_Pressure[5]=byte1(send[1]);
    Data_Pressure[6]=byte0(send[1]);
    Data_Pressure[7]=byte2(send[2]);
    Data_Pressure[8]=byte1(send[2]);
    Data_Pressure[9]=byte0(send[2]);
    Data_Pressure[10]=byte2(send[0]);
    Data_Pressure[11]=byte1(send[0]);
    Data_Pressure[12]=byte0(send[0]);
    Data_Pressure[14]=byte2(send[1]);
    Data_Pressure[14]=byte1(send[1]);
    Data_Pressure[15]=byte0(send[1]);
    Data_Pressure[16]=byte2(send[2]);
    Data_Pressure[17]=byte1(send[2]);
    Data_Pressure[18]=byte0(send[2]);
}




