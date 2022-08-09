/**
  ************************************* Copyright ******************************   
  *                 (C) Copyright 2022,Sean Song,China,USST.
  *                            All Rights Reserved                          
  *                         By 上海理工大学 Sean Song
  *                          https://github.com/kxbai
  *                              szc980@163.com
  *
  * FileName   : DA17.h                
  * Version    : v1.0		
  * Author     : Sean Song		
  * Date       : 2022-02-02         
  * Description:    
  *                                                                                            
  * 责任声明： 使用该代码所造成的一切后果均由使用者承担，作者不负任何法律责任。
  * All consequences caused by the use of this code shall be borne by the user, 
  * and the author shall not bear any legal responsibility 
  ******************************************************************************
 */

#ifndef __DA17_H
#define __DA17_H
#include "sys.h"

//osr define
#define OSR_1024X  0
#define OSR_2048X  1
#define OSR_4096X  2
#define OSR_8192X  3
#define OSR_256X   4
#define OSR_512X   5
#define OSR_16384X 6
#define OSR_32768X 7

#define byte0(dwTemp) (*(char *)(&dwTemp))
#define byte1(dwTemp) (*((char *)(&dwTemp)+1))
#define byte2(dwTemp) (*((char *)(&dwTemp)+2))
#define byte3(dwTemp) (*((char *)(&dwTemp)+3))



void     Send_Read_commond(void);
uint8_t  Read_Status(void);
uint32_t Read_Pressure_Data(void);

void     Send_Read2_commond(void);
uint8_t  Read2_Status(void);
uint32_t Read2_Pressure_Data(void);

void     Send_Read3_commond(void);
uint8_t  Read3_Status(void);  
uint32_t Read3_Pressure_Data(void);

void     Pressure_Data(void);
void     Pluse_Test(void);

#endif





