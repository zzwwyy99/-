#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <rtthread.h>

#include "drive_delay.h"
#include "node_manager.h"
#include "option.h"
#include "option_manager.h"
#include "message_manager.h"

//根据node地址得到option地址
#define node_obtain_option(node_addr,type,member) \
			option_container_of(node_addr,type,member)

#define option_container_of(ptr,type,member) \
            ((type *)((char *)ptr-(unsigned long)(&(((type *)0)->member))))
            

#define OPTION_LIST_MAX	20

/* 定义一个dev管理器 */ //树
typedef struct OptionManager{
	unsigned int num;		
	//unsigned int index;		
	struct Option *option_list[OPTION_LIST_MAX];	
}OptionManager,*POptionManager;

//dev管理器
static struct OptionManager g_tOptionManager;

/* 注册函数 */
void OptionRegister(struct Option *ptOption,char *name)
{
	/* 初始化dev */
	ptOption->name = name;
	
    /* 将option的node加到TREE中*/
    NodeRegisterToTREE(&ptOption->node);
    
	/*  添加dev到dev管理器 */
	g_tOptionManager.option_list[g_tOptionManager.num++] = ptOption;
}

/* 获得一个dev */
POption __GetOption(char *name)
{
	POption ptOption;

	for(int i=0;i<g_tOptionManager.num;i++){
		ptOption = g_tOptionManager.option_list[i];
		if(strcmp(ptOption->name,name) == 0){
            printf("得到选项:%s\r\n",name);
            
			return ptOption;			
		}
	}
    printf("没有得到选项:%s\r\n",name);
    
	return NULL;
}

struct Option *current_option;

static void GetBackOption(struct Option *ptOption)
{
	PNode ptNode;
	
    ptNode = GetNodeParent(&ptOption->node);

	/* 改变选项状态 */
	if(current_option->status == OPTION_ENTER){
		//printf("切换到OPTION_CHOOSE状态.\r\n");
		current_option->status = OPTION_CHOOSE;
	}	
	/* 有父结点 */
	else if(ptNode){
		//printf("有父节点.\r\n");
		current_option = node_obtain_option(ptNode, Option, node);
	}
	/* 没有父结点 */
	else{
		//printf("没有父结点.\r\n");
	}
}

static void GetNextOption(struct Option *ptOption)
{
	PNode ptNode;
	
    ptNode = GetNodeRightSibling(&ptOption->node); //从树中取出结点
    
    if(current_option->status == OPTION_ENTER){
		//printf("切换到OPTION_CHOOSE状态.\r\n");
		current_option->status = OPTION_CHOOSE;
	}
	/* 有兄弟结点 */
	if(ptNode){
		//printf("切换到兄弟结点.\r\n");
		current_option = node_obtain_option(ptNode, Option, node);
	}
	/* 没有兄弟结点 */
	else{
		//printf("没有兄弟结点.\r\n");
	}
}

static void GetEnterOption(struct Option *ptOption)
{
	PNode ptNode;
	
    ptNode = GetNodeFirstChild(&ptOption->node);
	/* 有子结点 */
	if(ptNode){
		//printf("有子结点.\r\n");
		current_option = node_obtain_option(ptNode, Option, node);
	}
	/* 没有子结点 */
	else{
		//printf("没有子结点.\r\n");
		
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
	/* 添加设备到设备管理器 */
	AddOptionToManager();

	//初始化current_option
	current_option = __GetOption("home");
}

//发送选项状态给业务子系统
extern int enter_node_count;  //不等于0,则刷新一次屏幕.进入子结点的次数.
extern int enter_status_count;

//消息数据
struct menu_status menu_data;

struct msg_topic g_tMsgMenu = {
	.name = "menu_topic",
	.msg_data = &menu_data
};

extern rt_mailbox_t g_tMsgCentrerMb;
//发布者
void MenuPublish(void *arg)
{
	/* 发送邮箱,唤醒消息中心线程 */
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
    
    //注册MSG
    RegisterMessage("menu_topic");
}

void MenuThreadEntry(void *arg)
{
	printf("Menu Thread启动.\r\n");
	
    rt_thread_delay(300);
	while(1){
		//printf("Menu Thread.\r\n");
		
		if(current_option->status == OPTION_CHOOSE){
			current_option->choose(current_option);
		}else{
			current_option->enter(current_option);
		}
 
        //发送msg到业务子系统
        menu_data.EnterNodeCount = enter_node_count;
        menu_data.EnterStatusCount = enter_status_count;
        g_tPublisherMenu.Publish(&g_tMsgMenu);
	}
}

