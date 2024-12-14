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

/* Display�豸�ṹ�� */
typedef struct DisplayDevice{
	char *name;  		/* �豸�� */
	void *FBBase;		/* CPU��ֱ�Ӷ�д���Դ� */
	int  iXres;		 	/* X����ֱ��� */
	int  iYres;		 	/* y����ֱ��� */
	int  iBpp;          /* ����λ */
	int	 iSize;			/* �Դ��С */
	
	void (*init)(struct DisplayDevice *ptDisplayDevice);		/* �豸��ʼ������ */
	void (*flush)(struct DisplayDevice *ptDisplayDevice); 		/* ��FBBase������ˢ��LCD���Դ��� */

	/* ����FBBase�е�����, ��(iX,iY)����������Ϊ��ɫdwColor 
	 * dwColor�ĸ�ʽ:0x00RRGGBB
	 */
	int (*SetPixel)(struct DisplayDevice *ptDisplayDevice,int iX,int iY,unsigned int dwcolor);
}DisplayDevice,*PDisplayDevice;

void __DisplayDevTimerFlushControl(int arg);
void DisplayDeviceRegister(struct DisplayDevice *ptDisplayDevice,char *name);
PDisplayDevice __GetDisplayDevice(char *name);

#endif	/*__DISPLAY_DEVICE_H*/

