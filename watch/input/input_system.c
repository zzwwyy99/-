#include <string.h>

#include "input_device.h"
#include "button_input.h" 

/* ��������豸 */
void AddInputDevice(void)
{
    /* ���Button�����豸 */
    AddButtonInputDevice();
}

/* �������ֻ��һ�������豸 */
PInputDevice GetInputDevice(char *name)
{
    return __GetInputDevice(name);
}

/* ��ʼ�������豸 */
void InitInputDevice(void)
{
    __InitInputDevice();
}
