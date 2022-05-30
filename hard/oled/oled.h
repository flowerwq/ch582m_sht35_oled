#ifndef HARD_OLED_OLED_H_
#define HARD_OLED_OLED_H_
#include "CH58x_common.h"


#define oled_scl_clr() GPIOA_ResetBits(GPIO_Pin_0)//SCL
#define oled_scl_set() GPIOA_SetBits(GPIO_Pin_0)

#define oled_sda_clr() GPIOA_ResetBits(GPIO_Pin_1)//mosi
#define oled_sda_set() GPIOA_SetBits(GPIO_Pin_1)

#define oled_res_clr() GPIOB_ResetBits(GPIO_Pin_20)//RES
#define oled_res_set() GPIOB_SetBits(GPIO_Pin_20)

#define oled_dc_clr()  GPIOB_ResetBits(GPIO_Pin_19)//DC
#define oled_dc_set()  GPIOB_SetBits(GPIO_Pin_19)

#define OLED_CMD  0 //写命令
#define OLED_DATA 1 //写数据
void oled_change_data_show(float m,float n);
void oled_show();
void oled_init(void);
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);

void OLED_ShowFNum(uint8_t x,uint8_t y,float Fnum,uint8_t size1,uint8_t mode);


#endif /* HARD_OLED_OLED_H_ */
