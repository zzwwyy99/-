#ifndef __ROLE_MANAGER_H
#define __ROLE_MANAGER_H

#include "show_graph.h"

#define	FLAG_DISABLE			0x00000000	//ʧ��
#define	FLAG_ENABLE				0x10000000	//ʹ��
#define	FLAG_NEUTRAL			0x01000000  //����
#define	FLAG_ENEMY				0x00100000	//�ж�
#define	FLAG_CONTROL_DEFAULT	0x00010000	//����_Ĭ��
#define	FLAG_CONTROL_INVIN	 	0x00001000	//����_�޵�
#define	FLAG_CONTROL_ATACK		0x00000100	//����_����
#define	FLAG_CONTROL_DOWN		0x00000010	//����_�½�
#define	FLAG_CONTROL_UP			0x00000001	//����_����

#define ROLE_LIST_MAX	10

//enum MEMENTO_OPS_CMD{
//	MEMENTO_OPS_CMD_RESERVE_DATA,	
//	MEMENTO_OPS_CMD_RECOVER_DATA
//};
//

enum CONTROL_OPS_CMD{
	CONTROL_OPS_CMD_ENABLE,		//ʹ��
	CONTROL_OPS_CMD_DISABLE,	//ʧ��
	CONTROL_OPS_CMD_UP,			//��Ծ
};

/* ����¼ģʽ */
struct memento_data{
	int iOriginX;		/* ��ʼ���� */
	int iOriginY;			
	int iOriginFlag;	/* ��ʼ���� */
};

typedef struct GameRole{
	char *name;			/* ���� */
	int  iCurrentFlag;	/* ��ǰ���� */
	struct GraphBitMap *ptGraphBitMap;	/* ͼƬλͼ */
	int  iCurrentX;		/* ��ǰ���� */
	int  iCurrentY;	

	struct memento_data memento;		/* ����¼ */

	int (*control_ops)(void *role,int cmd);
	int  (*excute_ops)(void *arg);		/* ���нӿ� */
	void (*recover_data)(void *arg);	/* �ָ����� */
}GameRole,*PGameRole;

/* �ۼ���:role���͵ľۼ��� */
//������Dev������.Aggregate(������,�������),Iterator(����������̨)
typedef struct Aggregate{
	void (*excute_item)(void *arg,int index);		/* ������index��ָ���role */ 
	void (*recover_item)(void *arg,int index);	
	
	struct GameRole *role_list[ROLE_LIST_MAX];
	int num;
}Aggregate;

/* ������:��һ��for��� */
typedef struct Iterator{
	void (*traversal_excute_item)(void *arg);	//thisָ��
	void (*traversal_recover_item)(void *arg);	//thisָ��

	struct Aggregate *ptAggregate;	//�ۼ���
	int index;	//����
}Iterator;	

void RoleRegister(struct GameRole *ptGameRole); /* ע�ắ�� */
PGameRole __GetGameRole(char *name);


void IndicateLedOpen(void);
#endif	/*__ROLE_MANAGER_H*/

