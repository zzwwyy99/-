#include <string.h>

#include "input_device.h"
#include "button_input.h" 

/* 添加输入设备 */
void AddInputDevice(void)
{
    /* 添加Button输入设备 */
    AddButtonInputDevice();
}

/* 根据名字获得一个输入设备 */
PInputDevice GetInputDevice(char *name)
{
    return __GetInputDevice(name);
}

/* 初始化输入设备 */
void InitInputDevice(void)
{
    __InitInputDevice();
}
