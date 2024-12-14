#include <stm32f10x.h>
#include <stdio.h>

#include "button_system.h"
#include "input_system.h"
#include "input_buffer.h"

void input_test(void)
{
    /* 0x080011E8 &Region$$Table$$Limit */
    //InputEvent *pev;  //ʹ��ָ��ĳ�ʼֵ��0x080011E8
    InputEvent event;
    
    //pev = &event; ���û�г�ʼ��ָ��,ָ���ʼֵΪ0x080011E8
    //*pev = event; GetInputBuffer(pev)��ִ��*ptInputEvent = g_tInputEventBuffer.buffer[g_tInputEventBuffer.pRead]; \
                        �൱��û�г�ʼ��ָ��pev,�͸�ָ��pevָ��ĵ�ַ��ֵ,Ҳ������0x080011E8��ֵ
    //ָ��������ʱ,�Ӻ��������ʱ��Ҫ��ָ��ָ��ĵ�ַ��ֵ,��ʱָ������Ѿ���ʼ��.
    
	//��ʼ�������豸
	AddButtonDevice();
    
	//��ʼ�������豸
    AddInputDevice();
    InitInputDevice();
    
    printf("input_test\r\n");
    
	//���԰�����������,���
    #if 0
	while(1){
        while(GetInputBuffer(&event) == 0){
            printf("�������������¼�----------\r\n");
            printf("��������:%d\r\n",event.etype);
			printf("����ֵ:%d\r\n",event.iKey);
			printf("�����¼�:%d\r\n",event.iPressure);
        }
    }
    #endif
}

