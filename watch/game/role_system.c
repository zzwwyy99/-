#include <stdio.h>
#include <string.h>

#include "role.h"
#include "role_manager.h"

PGameRole g_ptDefaultPLAYERRole;

static PGameRole SetDefaultPLAYERDinosaur(void)
{
	printf("获得默认玩家角色.\r\n");
    
    return g_ptDefaultPLAYERRole = __GetGameRole("dinosaur");
}

void AddRole(void)
{
	AddRoleToManager();

	SetDefaultPLAYERDinosaur();
}

//一对多驱动,pin驱动,hello驱动
//role = hello_dev,role_manager + role_system = struct rt_device hello_drv.
void DefaultDinosaurJUMP(void)
{
	g_ptDefaultPLAYERRole->control_ops(g_ptDefaultPLAYERRole,CONTROL_OPS_CMD_UP);
}

int DefaultDinosaurCollisionJUDGE(void)
{
	//恐龙在仙人掌到达前两帧起跳.迭代器每迭代一次为一帧
	//恐龙高度为43
	if(g_ptDefaultPLAYERRole->iCurrentY > 43 ){
		return 1;
	}else{
		return 0;
	}
}

//这个可以加到恐龙的结构体里面,作为ROLE的子类．
extern int speed_index;
void RecoverSpeedIndex(void)
{
	speed_index = 9;
}

