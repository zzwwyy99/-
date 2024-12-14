#ifndef __NODE_MANAGER_H
#define __NODE_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

/*struct dev */
//�������û������,ֻ�и����,�ӽ��,�ֵܽ���λ��
typedef struct Node{
	int  parent;		/* �����λ�� */
	int  first_child; 		/* �ӽ��λ�� */
	int  right_sibling;  	/* ���ֵܽ��λ�� */
}Node,*PNode;


PNode GetNodeParent(PNode tNode);
PNode GetNodeFirstChild(PNode tNode);
PNode GetNodeRightSibling(PNode tNode);
void NodeRegisterToTREE(struct Node *ptNode);


#endif	/*__NODE_MANAGER_H*/

