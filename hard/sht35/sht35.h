#include "CH58x_common.h"

#ifndef HARD_SHT35_SHT35_H_
#define HARD_SHT35_SHT35_H_
#define Size 6


//uint8_t write_buffer,write_len;
//uint8_t read_buffer,read_len;

void sht35_init();
uint32_t sht35_read_temphumi(int *temp,int *humi);

unsigned char SHT3X_CRC(uint8_t *data, uint8_t len);
void read_display_data();


#endif /* HARD_SHT35_SHT35_H_ */
