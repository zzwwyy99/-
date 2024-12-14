#ifndef __DISPLAY_DEVICE_H
#define __DISPLAY_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

enum TIMER_FLUSH_CMD
{
    TIMER_FLUSH_CMD_ENABLE,
    TIMER_FLUSH_CMD_DISABLE
};

/* Display设备结构体 */
typedef struct DisplayDevice{
	char *name;  		/* 设备名 */
	void *FBBase;		/* CPU能直接读写的显存 */
	int  iXres;		 	/* X方向分辨率 */
	int  iYres;		 	/* y方向分辨率 */
	int  iBpp;          /* 像素位 */
	int	 iSize;			/* 显存大小 */
	
	void (*init)(struct DisplayDevice *ptDisplayDevice);		/* 设备初始化函数 */
	void (*flush)(struct DisplayDevice *ptDisplayDevice); 		/* 将FBBase的数据刷到LCD的显存里 */

	/* 设置FBBase中的数据, 把(iX,iY)的像素设置为颜色dwColor 
	 * dwColor的格式:0x00RRGGBB
	 */
	int (*SetPixel)(struct DisplayDevice *ptDisplayDevice,int iX,int iY,unsigned int dwcolor);
}DisplayDevice,*PDisplayDevice;

void __DisplayDevTimerFlushControl(int arg);
void DisplayDeviceRegister(struct DisplayDevice *ptDisplayDevice,char *name);
PDisplayDevice __GetDisplayDevice(char *name);

#endif	/*__DISPLAY_DEVICE_H*/

