#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "node_manager.h"

#define NODE_LIST_MAX	20

/* ����һ��dev������ */ //��
typedef struct NodeManager{
	unsigned int num;			/* ������� */
	//unsigned int index;			/* ��ǰ��� */
	struct Node *node_list[NODE_LIST_MAX];		/* ������ĵ�ַ */
}NodeTREE,*PNodeTREE;

//dev������
static struct NodeManager g_tNodeTREE;

/* ���ĸ���� */
PNode GetNodeParent(PNode tNode)
{
	if(tNode->parent == -1){
		return 0;
	}
	return g_tNodeTREE.node_list[tNode->parent]; 
}


/* �����ӽ�� */
PNode GetNodeFirstChild(PNode tNode)
{
	if(tNode->first_child == -1){
		return 0;
	}
	return g_tNodeTREE.node_list[tNode->first_child]; 
}

/* �����ֵܽ�� */
PNode GetNodeRightSibling(PNode tNode)
{
	if(tNode->right_sibling == -1){
		return 0;
	}
	return g_tNodeTREE.node_list[tNode->right_sibling]; 
}

/*ע�ắ��*/
void NodeRegisterToTREE(struct Node *ptNode)
{	
	/*  ���dev��dev������ */
	g_tNodeTREE.node_list[g_tNodeTREE.num++] = ptNode;
}

