#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "node_manager.h"

#define NODE_LIST_MAX	20

/* 定义一个dev管理器 */ //树
typedef struct NodeManager{
	unsigned int num;			/* 结点总数 */
	//unsigned int index;			/* 当前结点 */
	struct Node *node_list[NODE_LIST_MAX];		/* 保存结点的地址 */
}NodeTREE,*PNodeTREE;

//dev管理器
static struct NodeManager g_tNodeTREE;

/* 结点的父结点 */
PNode GetNodeParent(PNode tNode)
{
	if(tNode->parent == -1){
		return 0;
	}
	return g_tNodeTREE.node_list[tNode->parent]; 
}


/* 结点的子结点 */
PNode GetNodeFirstChild(PNode tNode)
{
	if(tNode->first_child == -1){
		return 0;
	}
	return g_tNodeTREE.node_list[tNode->first_child]; 
}

/* 结点的兄弟结点 */
PNode GetNodeRightSibling(PNode tNode)
{
	if(tNode->right_sibling == -1){
		return 0;
	}
	return g_tNodeTREE.node_list[tNode->right_sibling]; 
}

/*注册函数*/
void NodeRegisterToTREE(struct Node *ptNode)
{	
	/*  添加dev到dev管理器 */
	g_tNodeTREE.node_list[g_tNodeTREE.num++] = ptNode;
}

