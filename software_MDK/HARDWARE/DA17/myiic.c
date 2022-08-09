#include "myiic.h"

//��ʼ��IIC  ---��ʱ�ò���
void DA17_IIC_Init(void)
{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//	__HAL_RCC_GPIOA_CLK_ENABLE();	//ʹ��GPIOAʱ��
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);
//	GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12; //PB14--SCL  PB15--SDA
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);	//SDA
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50MGPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB,GPIO_Pin_5);
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PB�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_SetBits(GPIOA,GPIO_Pin_8); 

}
//����IIC��ʼ�ź�

void Delay()		//@24.000MHz --2us
{
    unsigned char i;
    i = 33;
    while (--i);
}

void IIC_Start(void)
{
    SDA_OUT();     //sda�����
    IIC_SDA1;
    IIC_SCL1;
    Delay();
    IIC_SDA0;//START:when CLK is high,DATA change form high to low
    Delay();
    IIC_SCL0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
    SDA_OUT();//sda�����
    IIC_SCL0;
    IIC_SDA0;//STOP:when CLK is high DATA change form low to high
    Delay();
    IIC_SCL1;
    IIC_SDA1;//����I2C���߽����ź�
    Delay();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      //SDA����Ϊ����
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
    IIC_SCL0;//ʱ�����0
    return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC_SDA1;
        else
            IIC_SDA0;
        txd<<=1;
        Delay();  //��TEA5767��������ʱ���Ǳ����
        IIC_SCL1;
        Delay();
        IIC_SCL0;
        Delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}


//����IIC��ʼ�ź�
void IIC2_Start(void)
{
    SDA2_OUT();     //sda�����
    IIC2_SDA1;
    IIC2_SCL1;
    Delay();
    IIC2_SDA0;//START:when CLK is high,DATA change form high to low
    Delay();
    IIC2_SCL0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC2_Stop(void)
{
    SDA2_OUT();//sda�����
    IIC2_SCL0;
    IIC2_SDA0;//STOP:when CLK is high DATA change form low to high
    Delay();
    IIC2_SCL1;
    IIC2_SDA1;//����I2C���߽����ź�
    Delay();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC2_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA2_IN();      //SDA����Ϊ����
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
    IIC2_SCL0;//ʱ�����0
    return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC2_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA2_OUT();
    IIC2_SCL0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC2_SDA1;
        else
            IIC2_SDA0;
        txd<<=1;
        Delay();  //��TEA5767��������ʱ���Ǳ����
        IIC2_SCL1;
        Delay();
        IIC2_SCL0;
        Delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC2_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA2_IN();//SDA����Ϊ����
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
        IIC2_NAck();//����nACK
    else
        IIC2_Ack(); //����ACK
    return receive;
}

//����IIC��ʼ�ź�
void IIC3_Start(void)
{
    SDA3_OUT();     //sda�����
    IIC3_SDA1;
    IIC3_SCL1;
    Delay();
    IIC3_SDA0;//START:when CLK is high,DATA change form high to low
    Delay();
    IIC3_SCL0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC3_Stop(void)
{
    SDA3_OUT();//sda�����
    IIC3_SCL0;
    IIC3_SDA0;//STOP:when CLK is high DATA change form low to high
    Delay();
    IIC3_SCL1;
    IIC3_SDA1;//����I2C���߽����ź�
    Delay();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC3_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA3_IN();      //SDA����Ϊ����
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
    IIC3_SCL0;//ʱ�����0
    return 0;
}
//����ACKӦ��
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
//������ACKӦ��
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC3_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA3_OUT();
    IIC3_SCL0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            IIC3_SDA1;
        else
            IIC3_SDA0;
        txd<<=1;
        Delay();  //��TEA5767��������ʱ���Ǳ����
        IIC3_SCL1;
        Delay();
        IIC3_SCL0;
        Delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
uint8_t IIC3_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA3_IN();//SDA����Ϊ����
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
        IIC3_NAck();//����nACK
    else
        IIC3_Ack(); //����ACK
    return receive;
}











