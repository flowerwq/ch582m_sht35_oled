/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : 串口1收发演示
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/



#include "CH58x_common.h"
#include "sht35.h"
#include "oled.h"
#include <stdio.h>
#include "stdlib.h"


/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main()
{

	float ftemp=0;
	float fhumi=0;
    uint8_t len;
    int temp;
    int humi;
  

    SetSysClock(CLK_SOURCE_PLL_60MHz);

    /* 配置串口1：先配置IO口模式，再配置串口 */
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-配置上拉输入
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART1_DefInit();

    sht35_init();
	OLED_Clear();
    oled_init();
	OLED_DisPlay_On();
	OLED_DisplayTurn(1);
    oled_show();

    while(1){
		sht35_read_temphumi(&temp,&humi);
		ftemp=temp/(10.0);
		fhumi=humi/(10.0);
		PRINT("temp=%.1f,humi=%.1f\r\n",ftemp,fhumi);
		PRINT("\r\n");
//		OLED_ShowFNum(44,16,ftemp,16,1);
//		OLED_ShowFNum(44,32,fhumi,16,1);
//    	oled_show(ftemp,fhumi);
		oled_change_data_show(ftemp,fhumi);
		OLED_Refresh();


	}

}


