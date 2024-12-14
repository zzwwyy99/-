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

//role�ṹ������һ��excute_ops�ӿ�,memento_ops�ӿ�,��������Ҳ������
//�ۼ�����role���ϲ�,����Ϸ�����²�
static void excute_item(void *arg,int index)
{
	struct Aggregate *ptAggregate = (struct Aggregate *)arg;
	
	if(ptAggregate->role_list[index]->iCurrentFlag & FLAG_ENABLE){
		ptAggregate->role_list[index]->excute_ops(ptAggregate->role_list[index]);

		/* FLAG_ENEMY:�ж��Ƿ���ײ */
		if(ptAggregate->role_list[index]->iCurrentFlag & FLAG_ENEMY){
			if(DefaultDinosaurCollisionJUDGE()){
				if(ptAggregate->role_list[index]->iCurrentX<28 && ptAggregate->role_list[index]->iCurrentX>0){
					/* ������ײ�ص����� */
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

	/* ��ʼ��ROLE */
	ptGameRole->memento.iOriginFlag = ptGameRole->iCurrentFlag;
	ptGameRole->memento.iOriginX = ptGameRole->iCurrentX;
	ptGameRole->memento.iOriginY= ptGameRole->iCurrentY;
	
	g_tAggregate.role_list[g_tAggregate.num++] = ptGameRole;
    
    printf("ע���%d��ROLE�ɹ�.\r\n",g_tAggregate.num);
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
 *������:��ʵ��һ��for���
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
	
	//��ԭ������speed_index
	RecoverSpeedIndex();
    
    IndicateLedOpen();
}

struct Iterator g_tIterator = {
	.traversal_excute_item = traversal_excute_item,
	.traversal_recover_item = traversal_recover_item,
	.ptAggregate = &g_tAggregate,
	.index = 0  //�����indexû���õ�
};

