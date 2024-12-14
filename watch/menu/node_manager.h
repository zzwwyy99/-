#ifndef __NODE_MANAGER_H
#define __NODE_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

/*struct dev */
//结点自身没有数据,只有父结点,子结点,兄弟结点的位置
typedef struct Node{
	int  parent;		/* 父结点位置 */
	int  first_child; 		/* 子结点位置 */
	int  right_sibling;  	/* 右兄弟结点位置 */
}Node,*PNode;


PNode GetNodeParent(PNode tNode);
PNode GetNodeFirstChild(PNode tNode);
PNode GetNodeRightSibling(PNode tNode);
void NodeRegisterToTREE(struct Node *ptNode);


#endif	/*__NODE_MANAGER_H*/

