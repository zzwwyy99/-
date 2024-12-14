#ifndef __ROLE_MANAGER_H
#define __ROLE_MANAGER_H

#include "show_graph.h"

#define	FLAG_DISABLE			0x00000000	//失能
#define	FLAG_ENABLE				0x10000000	//使能
#define	FLAG_NEUTRAL			0x01000000  //中立
#define	FLAG_ENEMY				0x00100000	//敌对
#define	FLAG_CONTROL_DEFAULT	0x00010000	//控制_默认
#define	FLAG_CONTROL_INVIN	 	0x00001000	//控制_无敌
#define	FLAG_CONTROL_ATACK		0x00000100	//控制_攻击
#define	FLAG_CONTROL_DOWN		0x00000010	//控制_下降
#define	FLAG_CONTROL_UP			0x00000001	//控制_上升

#define ROLE_LIST_MAX	10

//enum MEMENTO_OPS_CMD{
//	MEMENTO_OPS_CMD_RESERVE_DATA,	
//	MEMENTO_OPS_CMD_RECOVER_DATA
//};
//

enum CONTROL_OPS_CMD{
	CONTROL_OPS_CMD_ENABLE,		//使能
	CONTROL_OPS_CMD_DISABLE,	//失能
	CONTROL_OPS_CMD_UP,			//跳跃
};

/* 备忘录模式 */
struct memento_data{
	int iOriginX;		/* 初始坐标 */
	int iOriginY;			
	int iOriginFlag;	/* 初始属性 */
};

typedef struct GameRole{
	char *name;			/* 名字 */
	int  iCurrentFlag;	/* 当前属性 */
	struct GraphBitMap *ptGraphBitMap;	/* 图片位图 */
	int  iCurrentX;		/* 当前坐标 */
	int  iCurrentY;	

	struct memento_data memento;		/* 备忘录 */

	int (*control_ops)(void *role,int cmd);
	int  (*excute_ops)(void *arg);		/* 运行接口 */
	void (*recover_data)(void *arg);	/* 恢复数据 */
}GameRole,*PGameRole;

/* 聚集器:role类型的聚集器 */
//类似于Dev管理器.Aggregate(播放器,管理歌曲),Iterator(播放器控制台)
typedef struct Aggregate{
	void (*excute_item)(void *arg,int index);		/* 迭代器index所指向的role */ 
	void (*recover_item)(void *arg,int index);	
	
	struct GameRole *role_list[ROLE_LIST_MAX];
	int num;
}Aggregate;

/* 迭代器:就一个for语句 */
typedef struct Iterator{
	void (*traversal_excute_item)(void *arg);	//this指针
	void (*traversal_recover_item)(void *arg);	//this指针

	struct Aggregate *ptAggregate;	//聚集器
	int index;	//索引
}Iterator;	

void RoleRegister(struct GameRole *ptGameRole); /* 注册函数 */
PGameRole __GetGameRole(char *name);


void IndicateLedOpen(void);
#endif	/*__ROLE_MANAGER_H*/

