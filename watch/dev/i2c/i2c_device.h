#ifndef __I2C_DEVICE_H
#define __I2C_DEVICE_H

//#include <stm32f10x.h>

#ifndef NULL
#define NULL (void *)0
#endif

#define	OLEDInfDev	I2CDevice
#define	POLEDInfDev	PI2CDevice

#define HARD_I2C
#define I2C_DMA_TRANSMIT

/* 软件I2C */		  		/* 硬件I2C */	
//I2C_Start();			    I2C_GenerateSTART(I2C1,ENABLE);	
//I2C_WriteByte(data);  	I2C_SendData(I2C1,data);
//I2C_Stop();			    I2C_GenerateSTOP(I2C1,ENABLE);
/* I2C设备结构体 */
typedef struct I2CDevice{
	char *name;  		/* 设备名 */
	int  iPin_SCL;		/* SCL pin */
	int  iPin_SDA;		/* SDA pin */
	void *iPort; 		/* 硬件的port编号 */
	
    void (*init)(struct I2CDevice *ptI2CDevice);    /* 设备初始化函数 */
    
#ifdef SOFT_I2C    
	void (*i2c_start)(void);
    void (*i2c_writebyte)(unsigned char *data);
    void (*i2c_stop)(void);
#endif

#ifdef HARD_I2C 
#ifdef I2C_DMA_TRANSMIT    
    void *iDMA;  		/* I2C所用DMA的编号*/
    
	unsigned int iDMAperBaseAddr;	/* DMA外设寄存器地址 */
	unsigned int iDMAmemBaseAddr;  	/* DMAmemory地址 */
	unsigned int iBufferSize;
    
	void (*dam_cfg)(struct I2CDevice *ptI2CDevice);
	void (*dam_enable)(struct I2CDevice *ptI2CDevice);
#endif
#endif  
}I2CDevice,*PI2CDevice;


//void I2CDeviceRegister(struct I2CDevice *ptI2CDevice,char *name,unsigned int membaseaddr,unsigned int bufferszie);
void I2CDeviceRegister(struct I2CDevice *ptI2CDevice,char *name);
PI2CDevice __GetI2CDevice(char *name);

#endif	/*__I2C_DEVICE_H*/

