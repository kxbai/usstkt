#include "myiic.h"

//初始化IIC  ---暂时用不到
void DA17_IIC_Init(void)
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//	__HAL_RCC_GPIOA_CLK_ENABLE();	//使能GPIOA时钟
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);
//	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12; //PB14--SCL  PB15--SDA
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);	//SDA
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50MGPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_5);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_SetBits(GPIOA,GPIO_Pin_8); 

}
//产生IIC起始信号

void Delay()		//@24.000MHz --2us
{
    unsigned char i;
    i = 33;
    while (--i);
}

void IIC_Start(void)
{
    SDA_OUT();     //sda线输出
    IIC_SDA1;
    IIC_SCL1;
    Delay();
    IIC_SDA0;//START:when CLK is high,DATA change form high to low
    Delay();
    IIC_SCL0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
    SDA_OUT();//sda线输出
    IIC_SCL0;
    IIC_SDA0;//STOP:when CLK is high DATA change form low to high
    Delay();
    IIC_SCL1;
    IIC_SDA1;//发送I2C总线结束信号
    Delay();
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      //SDA设置为输入
    IIC_SDA1;
    Delay();
    IIC_SCL1;
    Delay();
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL0;//时钟输出0
    return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
    IIC_SCL0;
    SDA_OUT();
    IIC_SDA0;
    Delay();
    IIC_SCL1;
    Delay();
    IIC_SCL0;
}
//不产生ACK应答
void IIC_NAck(void)
{
    IIC_SCL0;
    SDA_OUT();
    IIC_SDA1;
    Delay();
    IIC_SCL1;
    Delay();
    IIC_SCL0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL0;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC_SDA1;
        else
            IIC_SDA0;
        txd<<=1;
        Delay();  //对TEA5767这三个延时都是必须的
        IIC_SCL1;
        Delay();
        IIC_SCL0;
        Delay();
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC_SCL0;
        Delay();
        IIC_SCL1;
        receive<<=1;
        if(READ_SDA)receive++;
        Delay();
    }
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK
    return receive;
}


//产生IIC起始信号
void IIC2_Start(void)
{
    SDA2_OUT();     //sda线输出
    IIC2_SDA1;
    IIC2_SCL1;
    Delay();
    IIC2_SDA0;//START:when CLK is high,DATA change form high to low
    Delay();
    IIC2_SCL0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC2_Stop(void)
{
    SDA2_OUT();//sda线输出
    IIC2_SCL0;
    IIC2_SDA0;//STOP:when CLK is high DATA change form low to high
    Delay();
    IIC2_SCL1;
    IIC2_SDA1;//发送I2C总线结束信号
    Delay();
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC2_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA2_IN();      //SDA设置为输入
    IIC2_SDA1;
    Delay();
    IIC2_SCL1;
    Delay();
    while(READ2_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC2_Stop();
            return 1;
        }
    }
    IIC2_SCL0;//时钟输出0
    return 0;
}
//产生ACK应答
void IIC2_Ack(void)
{
    IIC2_SCL0;
    SDA2_OUT();
    IIC2_SDA0;
    Delay();
    IIC2_SCL1;
    Delay();
    IIC2_SCL0;
}
//不产生ACK应答
void IIC2_NAck(void)
{
    IIC2_SCL0;
    SDA2_OUT();
    IIC2_SDA1;
    Delay();
    IIC2_SCL1;
    Delay();
    IIC2_SCL0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC2_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA2_OUT();
    IIC2_SCL0;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC2_SDA1;
        else
            IIC2_SDA0;
        txd<<=1;
        Delay();  //对TEA5767这三个延时都是必须的
        IIC2_SCL1;
        Delay();
        IIC2_SCL0;
        Delay();
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC2_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA2_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC2_SCL0;
        Delay();
        IIC2_SCL1;
        receive<<=1;
        if(READ2_SDA)receive++;
        Delay();
    }
    if (!ack)
        IIC2_NAck();//发送nACK
    else
        IIC2_Ack(); //发送ACK
    return receive;
}

//产生IIC起始信号
void IIC3_Start(void)
{
    SDA3_OUT();     //sda线输出
    IIC3_SDA1;
    IIC3_SCL1;
    Delay();
    IIC3_SDA0;//START:when CLK is high,DATA change form high to low
    Delay();
    IIC3_SCL0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC3_Stop(void)
{
    SDA3_OUT();//sda线输出
    IIC3_SCL0;
    IIC3_SDA0;//STOP:when CLK is high DATA change form low to high
    Delay();
    IIC3_SCL1;
    IIC3_SDA1;//发送I2C总线结束信号
    Delay();
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC3_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA3_IN();      //SDA设置为输入
    IIC3_SDA1;
    Delay();
    IIC3_SCL1;
    Delay();
    while(READ3_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC3_Stop();
            return 1;
        }
    }
    IIC3_SCL0;//时钟输出0
    return 0;
}
//产生ACK应答
void IIC3_Ack(void)
{
    IIC3_SCL0;
    SDA3_OUT();
    IIC3_SDA0;
    Delay();
    IIC3_SCL1;
    Delay();
    IIC3_SCL0;
}
//不产生ACK应答
void IIC3_NAck(void)
{
    IIC3_SCL0;
    SDA3_OUT();
    IIC3_SDA1;
    Delay();
    IIC3_SCL1;
    Delay();
    IIC3_SCL0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC3_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA3_OUT();
    IIC3_SCL0;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC3_SDA1;
        else
            IIC3_SDA0;
        txd<<=1;
        Delay();  //对TEA5767这三个延时都是必须的
        IIC3_SCL1;
        Delay();
        IIC3_SCL0;
        Delay();
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC3_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA3_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        IIC3_SCL0;
        Delay();
        IIC3_SCL1;
        receive<<=1;
        if(READ3_SDA)receive++;
        Delay();
    }
    if (!ack)
        IIC3_NAck();//发送nACK
    else
        IIC3_Ack(); //发送ACK
    return receive;
}











