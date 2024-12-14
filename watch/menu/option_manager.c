#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <rtthread.h>

#include "drive_delay.h"
#include "node_manager.h"
#include "option.h"
#include "option_manager.h"
#include "message_manager.h"

//����node��ַ�õ�option��ַ
#define node_obtain_option(node_addr,type,member) \
			option_container_of(node_addr,type,member)

#define option_container_of(ptr,type,member) \
            ((type *)((char *)ptr-(unsigned long)(&(((type *)0)->member))))
            

#define OPTION_LIST_MAX	20

/* ����һ��dev������ */ //��
typedef struct OptionManager{
	unsigned int num;		
	//unsigned int index;		
	struct Option *option_list[OPTION_LIST_MAX];	
}OptionManager,*POptionManager;

//dev������
static struct OptionManager g_tOptionManager;

/* ע�ắ�� */
void OptionRegister(struct Option *ptOption,char *name)
{
	/* ��ʼ��dev */
	ptOption->name = name;
	
    /* ��option��node�ӵ�TREE��*/
    NodeRegisterToTREE(&ptOption->node);
    
	/*  ���dev��dev������ */
	g_tOptionManager.option_list[g_tOptionManager.num++] = ptOption;
}

/* ���һ��dev */
POption __GetOption(char *name)
{
	POption ptOption;

	for(int i=0;i<g_tOptionManager.num;i++){
		ptOption = g_tOptionManager.option_list[i];
		if(strcmp(ptOption->name,name) == 0){
            printf("�õ�ѡ��:%s\r\n",name);
            
			return ptOption;			
		}
	}
    printf("û�еõ�ѡ��:%s\r\n",name);
    
	return NULL;
}

struct Option *current_option;

static void GetBackOption(struct Option *ptOption)
{
	PNode ptNode;
	
    ptNode = GetNodeParent(&ptOption->node);

	/* �ı�ѡ��״̬ */
	if(current_option->status == OPTION_ENTER){
		//printf("�л���OPTION_CHOOSE״̬.\r\n");
		current_option->status = OPTION_CHOOSE;
	}	
	/* �и���� */
	else if(ptNode){
		//printf("�и��ڵ�.\r\n");
		current_option = node_obtain_option(ptNode, Option, node);
	}
	/* û�и���� */
	else{
		//printf("û�и����.\r\n");
	}
}

static void GetNextOption(struct Option *ptOption)
{
	PNode ptNode;
	
    ptNode = GetNodeRightSibling(&ptOption->node); //������ȡ�����
    
    if(current_option->status == OPTION_ENTER){
		//printf("�л���OPTION_CHOOSE״̬.\r\n");
		current_option->status = OPTION_CHOOSE;
	}
	/* ���ֵܽ�� */
	if(ptNode){
		//printf("�л����ֵܽ��.\r\n");
		current_option = node_obtain_option(ptNode, Option, node);
	}
	/* û���ֵܽ�� */
	else{
		//printf("û���ֵܽ��.\r\n");
	}
}

static void GetEnterOption(struct Option *ptOption)
{
	PNode ptNode;
	
    ptNode = GetNodeFirstChild(&ptOption->node);
	/* ���ӽ�� */
	if(ptNode){
		//printf("���ӽ��.\r\n");
		current_option = node_obtain_option(ptNode, Option, node);
	}
	/* û���ӽ�� */
	else{
		//printf("û���ӽ��.\r\n");
		
		current_option->status = OPTION_ENTER;
	}
}


void OptionControl(int cmd)
{
	switch (cmd){
		case OPTION_CMD_BACK:	
			GetBackOption(current_option);
			break;
		case OPTION_CMD_NEXT: 
			GetNextOption(current_option);
			break;
		case OPTION_CMD_ENTER:
			GetEnterOption(current_option);
			break;
		default:
			break;	
	}	
}


void InitMenuOption(void)
{
	/* ����豸���豸������ */
	AddOptionToManager();

	//��ʼ��current_option
	current_option = __GetOption("home");
}

//����ѡ��״̬��ҵ����ϵͳ
extern int enter_node_count;  //������0,��ˢ��һ����Ļ.�����ӽ��Ĵ���.
extern int enter_status_count;

//��Ϣ����
struct menu_status menu_data;

struct msg_topic g_tMsgMenu = {
	.name = "menu_topic",
	.msg_data = &menu_data
};

extern rt_mailbox_t g_tMsgCentrerMb;
//������
void MenuPublish(void *arg)
{
	/* ��������,������Ϣ�����߳� */
    if(g_tMsgCentrerMb){
        rt_mb_send(g_tMsgCentrerMb,(rt_uint32_t)&g_tMsgMenu);
    }
    
    
}

struct Publisher g_tPublisherMenu = {
	.Publish = MenuPublish
};

void MenuThreadInit(void)
{
	printf("Menu Thread Init.\r\n");
    
    //ע��MSG
    RegisterMessage("menu_topic");
}

void MenuThreadEntry(void *arg)
{
	printf("Menu Thread����.\r\n");
	
    rt_thread_delay(300);
	while(1){
		//printf("Menu Thread.\r\n");
		
		if(current_option->status == OPTION_CHOOSE){
			current_option->choose(current_option);
		}else{
			current_option->enter(current_option);
		}
 
        //����msg��ҵ����ϵͳ
        menu_data.EnterNodeCount = enter_node_count;
        menu_data.EnterStatusCount = enter_status_count;
        g_tPublisherMenu.Publish(&g_tMsgMenu);
	}
}

