#ifndef __OLED_COMMAND_H
#define __OLED_COMMAND_H

#define WAIT_FOR_I2CDMA_BUSY()   while(OLED_I2C1_DMA_BUSYFLAG)

extern int OLED_I2C1_DMA_BUSYFLAG;

void oled_init(char *pFramBuffer,int iSize);
void oled_gram_refresh_dma(void);

void oled_setcursor(int x,int y);
void oled_clear(void);
void WaitForI2CDMABusy(void);

#endif	/*__OLED_COMMAND_H*/

