#include <stdio.h>
#include <string.h>

#include "role.h"
#include "role_manager.h"

PGameRole g_ptDefaultPLAYERRole;

static PGameRole SetDefaultPLAYERDinosaur(void)
{
	printf("���Ĭ����ҽ�ɫ.\r\n");
    
    return g_ptDefaultPLAYERRole = __GetGameRole("dinosaur");
}

void AddRole(void)
{
	AddRoleToManager();

	SetDefaultPLAYERDinosaur();
}

//һ�Զ�����,pin����,hello����
//role = hello_dev,role_manager + role_system = struct rt_device hello_drv.
void DefaultDinosaurJUMP(void)
{
	g_ptDefaultPLAYERRole->control_ops(g_ptDefaultPLAYERRole,CONTROL_OPS_CMD_UP);
}

int DefaultDinosaurCollisionJUDGE(void)
{
	//�����������Ƶ���ǰ��֡����.������ÿ����һ��Ϊһ֡
	//�����߶�Ϊ43
	if(g_ptDefaultPLAYERRole->iCurrentY > 43 ){
		return 1;
	}else{
		return 0;
	}
}

//������Լӵ������Ľṹ������,��ΪROLE�����࣮
extern int speed_index;
void RecoverSpeedIndex(void)
{
	speed_index = 9;
}

