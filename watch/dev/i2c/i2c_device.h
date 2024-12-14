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

/* ���I2C */		  		/* Ӳ��I2C */	
//I2C_Start();			    I2C_GenerateSTART(I2C1,ENABLE);	
//I2C_WriteByte(data);  	I2C_SendData(I2C1,data);
//I2C_Stop();			    I2C_GenerateSTOP(I2C1,ENABLE);
/* I2C�豸�ṹ�� */
typedef struct I2CDevice{
	char *name;  		/* �豸�� */
	int  iPin_SCL;		/* SCL pin */
	int  iPin_SDA;		/* SDA pin */
	void *iPort; 		/* Ӳ����port��� */
	
    void (*init)(struct I2CDevice *ptI2CDevice);    /* �豸��ʼ������ */
    
#ifdef SOFT_I2C    
	void (*i2c_start)(void);
    void (*i2c_writebyte)(unsigned char *data);
    void (*i2c_stop)(void);
#endif

#ifdef HARD_I2C 
#ifdef I2C_DMA_TRANSMIT    
    void *iDMA;  		/* I2C����DMA�ı��*/
    
	unsigned int iDMAperBaseAddr;	/* DMA����Ĵ�����ַ */
	unsigned int iDMAmemBaseAddr;  	/* DMAmemory��ַ */
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

