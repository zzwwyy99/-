#include <stdio.h>
#include <string.h>

#include "game_system.h"
#include "role_manager.h"
#include "role_system.h"
#include "drive_delay.h"

#include "led_system.h"

//setcollisionJUDGE,setcollisionEXCUTE
extern struct Game g_tGame;

PLedDevice g_ptIndicateLed;

void IndicateLedOpen(void)
{
    g_ptIndicateLed = GetLedDevice("WHITE");
    g_ptIndicateLed->control(g_ptIndicateLed,LED_ON);
}

void IndicateLedClose(void)
{
    g_ptIndicateLed = GetLedDevice("WHITE");
    g_ptIndicateLed->control(g_ptIndicateLed,LED_OFF);
}

//DefaultDinosaurCollisionEXCUTE
static void DefaultDinosaurCollisionEXCUTE(void)
{
    IndicateLedClose();
    GameStop(&g_tGame);
}

//role结构体中有一个excute_ops接口,memento_ops接口,所以这里也有两个
//聚集器是role的上层,是游戏机的下层
static void excute_item(void *arg,int index)
{
	struct Aggregate *ptAggregate = (struct Aggregate *)arg;
	
	if(ptAggregate->role_list[index]->iCurrentFlag & FLAG_ENABLE){
		ptAggregate->role_list[index]->excute_ops(ptAggregate->role_list[index]);

		/* FLAG_ENEMY:判断是否碰撞 */
		if(ptAggregate->role_list[index]->iCurrentFlag & FLAG_ENEMY){
			if(DefaultDinosaurCollisionJUDGE()){
				if(ptAggregate->role_list[index]->iCurrentX<28 && ptAggregate->role_list[index]->iCurrentX>0){
					/* 发生碰撞回调函数 */
					DefaultDinosaurCollisionEXCUTE();
                }
			}	
		}
	}
}

static void recover_item(void *arg,int index)
{
	struct Aggregate *ptAggregate = (struct Aggregate *)arg;
	
	if(ptAggregate->role_list[index]){						
		ptAggregate->role_list[index]->recover_data(ptAggregate->role_list[index]);
	}
}

struct Aggregate g_tAggregate  = {
	.excute_item = excute_item,
	.recover_item = recover_item
};

void RoleRegister(struct GameRole *ptGameRole)
{
	if(g_tAggregate.num > ROLE_LIST_MAX - 1){
		return;
	}

	/* 初始化ROLE */
	ptGameRole->memento.iOriginFlag = ptGameRole->iCurrentFlag;
	ptGameRole->memento.iOriginX = ptGameRole->iCurrentX;
	ptGameRole->memento.iOriginY= ptGameRole->iCurrentY;
	
	g_tAggregate.role_list[g_tAggregate.num++] = ptGameRole;
    
    printf("注册第%d个ROLE成功.\r\n",g_tAggregate.num);
}

PGameRole __GetGameRole(char *name)
{
	PGameRole ptGameRole;

	for(int i=0;i<g_tAggregate.num;i++){
		ptGameRole = g_tAggregate.role_list[i];
		if(strcmp(ptGameRole->name,name) == 0){
			return ptGameRole;			
		}
	}

	return NULL;
}


/*****************
 *迭代器:其实就一个for语句
 *****************/
static void traversal_excute_item(void *arg)
{
	Iterator *ptIterator = (Iterator *)arg;
	
    ptIterator->index = 0;
	while(ptIterator->index < ptIterator->ptAggregate->num){
		ptIterator->ptAggregate->excute_item(ptIterator->ptAggregate,ptIterator->index);
		ptIterator->index++; 
	}	
}

static void traversal_recover_item(void *arg)
{
	Iterator *ptIterator = (Iterator *)arg;
	
    ptIterator->index = 0;
	while(ptIterator->index < ptIterator->ptAggregate->num){
		ptIterator->ptAggregate->recover_item(ptIterator->ptAggregate,ptIterator->index);
		ptIterator->index++;
	}	
	
	//复原恐龙的speed_index
	RecoverSpeedIndex();
    
    IndicateLedOpen();
}

struct Iterator g_tIterator = {
	.traversal_excute_item = traversal_excute_item,
	.traversal_recover_item = traversal_recover_item,
	.ptAggregate = &g_tAggregate,
	.index = 0  //这里的index没有用到
};

