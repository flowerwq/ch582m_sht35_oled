#include "sht35.h"
#include <CH58x_gpio.h>
#include  <CH58x_i2c.h>
#include <CH58x_common.h>
#include <stdint.h>
#include <stdbool.h>
#include "oled.h"

#define SHT35_READ_HUMITURE (uint16_t)0x2c06
#define POLYNOMIAL_CXDZ 0x31

uint8_t read_buffer[Size];
static volatile int time_out_flag = 0;

__INTERRUPT
__HIGH_CODE
void TMR2_IRQHandler(void) // TMR0 定时中断
{
    
    if(TMR2_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR2_Disable();
        TMR2_ClearITFlag(TMR0_3_IT_CYC_END);// 清除中断标志
        time_out_flag=1;
    }
}
static int sht35_timer_init(){
	TMR2_ITCfg(ENABLE, TMR0_3_IT_CYC_END); // 开启中断
	PFIC_EnableIRQ(TMR2_IRQn);
	return 0;
}

static int sht35_timer_start(uint32_t timeout_ms){
	time_out_flag = 0;
	TMR2_TimerInit(GetSysClock() / 1000  * timeout_ms);
	return 0;
}
static int sht35_delay_ms(uint32_t ms){
	sht35_timer_start(ms);
	return 0;
}

int count=0;
static void print_function(){
    count++;
    PRINT("error:wait status timeout \r\n");
    PRINT("error place:%d\r\n",count);
}
static bool timeout_flag_function(){
	if(time_out_flag == 1){
		print_function();
		TMR2_Disable();
		
	
		return FALSE;
	}
	else{
		return TRUE;
	}
}

/*    sht35 initialization  */
void sht35_init()
{
    //senior AFIOalternate function IO ( PB12 and PB13 )
	I2C_SoftwareResetCmd(ENABLE);
	DelayMs(50);
	I2C_SoftwareResetCmd(DISABLE);
    GPIOB_ModeCfg(GPIO_Pin_13 | GPIO_Pin_12, GPIO_ModeIN_PU);
    I2C_Init(I2C_Mode_I2C, 100000, I2C_DutyCycle_16_9, I2C_Ack_Enable,
        I2C_AckAddr_7bit,0x44); 

	sht35_timer_init();
}

static uint32_t sht35_write(uint16_t cmd,uint8_t address){
	sht35_delay_ms(500);
    while(1){
		if(I2C_GetFlagStatus(I2C_FLAG_BUSY) == RESET){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
	}
    I2C_GenerateSTART(ENABLE);
	sht35_delay_ms(100);
    while(1){
		if(I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT) != 0){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
	}
    I2C_Send7bitAddress(address, I2C_Direction_Transmitter);
	
	sht35_delay_ms(500);
	while(1){
		if(I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != 0){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
	}
    if(I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET){
		I2C_SendData((cmd>>8)&0xff);//  send  command MSB
    }
    if(I2C_GetFlagStatus(I2C_FLAG_TXE) != RESET){
        I2C_SendData(cmd&0xff);//send command lSB
		
    }
	sht35_delay_ms(500);
	while(1){
		if(I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED ) !=0){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
	}
    I2C_GenerateSTOP(ENABLE);
    DelayMs(50);
	return 0;
}


uint32_t sht35_read(uint8_t address){
	volatile uint8_t i=0;

	sht35_write(SHT35_READ_HUMITURE,0x44<<1);
	I2C_AcknowledgeConfig(ENABLE);
	sht35_delay_ms(100);
    while(1){
		if(I2C_GetFlagStatus(I2C_FLAG_BUSY) == RESET){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
	}
    I2C_GenerateSTART(ENABLE);//send start signal (start = 1)
	sht35_delay_ms(100);
	while(1){
		if(I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT) != 0){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		 }
	 }

    I2C_Send7bitAddress(address, I2C_Direction_Receiver);//send slave  address to datar
	sht35_delay_ms(500);
	while(1){
		if(I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != 0){
			break;
		}
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
	}
	
	sht35_delay_ms(500);
    while(i < Size){
        if (I2C_GetFlagStatus(I2C_FLAG_RXNE) != RESET){
			read_buffer[i] = I2C_ReceiveData();//receive  senior original data
            i++;
        }
		else{
			if(timeout_flag_function() != TRUE){
				break;
			}
		}
    }
//	*read_len =sizeof(*read_buffer);
    I2C_AcknowledgeConfig(DISABLE);//clear ACK
    I2C_GenerateSTOP(ENABLE);	
	return 0;
}


uint32_t sht35_read_temphumi(int *temp,int *humi){
	sht35_read(0x44<<1);
	if(SHT3X_CRC(&read_buffer[0],2) == read_buffer[2] &&
	    SHT3X_CRC(&read_buffer[3],2) == read_buffer[5])
	{
		//calculate temp and humi
	  	*temp=(-45+(175.0*((read_buffer[0]<<8)+read_buffer[1])/65535.0))*10;
	    *humi=10*(100*((read_buffer[3]<<8)+read_buffer[4])/65535.0);
	    PRINT("%s:temp value %d\r\n", __FUNCTION__, *temp);
	    PRINT("%s:rh value %d\r\n", __FUNCTION__ , *humi);
	    if(*temp>1250)
	    {
	        *temp=1250;
	    }
	    else if(*temp<-400)
	    {
	        *temp=-400;
	    }
	    return 0;
	}
	else{
	     return 1;
	}
	
}

//crc 校验算法，data 要校验的数据，len 校验的数据的长度
unsigned char SHT3X_CRC(uint8_t *data, uint8_t len)
{
    unsigned char bit;        // bit mask
    unsigned char crc = 0xFF; // calculated checksum
    unsigned char byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial @GZCXDZ
    for(byteCtr = 0; byteCtr < len; byteCtr++) {
            crc ^= (data[byteCtr]);
            for(bit = 8; bit > 0; --bit) {
                    if(crc & 0x80) {
                            crc = (crc << 1) ^ POLYNOMIAL_CXDZ;
                    }  else {
                            crc = (crc << 1);
                    }
            }
    }
  return crc;
}


void read_display_data(){
	float ftemp=0;
	float fhumi=0;
	int temp;
	int humi;

	sht35_read_temphumi(&temp,&humi);
	ftemp=temp/(10.0);
	fhumi=humi/(10.0);
	PRINT("temp=%.1f,humi=%.1f\r\n",ftemp,fhumi);
	PRINT("\r\n");
	oled_change_data_show(ftemp,fhumi);
	OLED_Refresh();
}
