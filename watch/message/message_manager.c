#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "message_manager.h"

#define SUBSCRIBER_LIST_MAX	3
#define TOPIC_NUM_MAX	10


//话题名称, 信息
struct TopicMSG{
	char *TopicName;
	void *MessageData;	//最多一个消息
};

//话题名称, 订阅该话题的订阅者
struct TopicSUBSCRIBER{
	char *TopicName;
	int num;
	struct Subcriber *subciber_list[SUBSCRIBER_LIST_MAX];	//最多三个订阅者
};

//话题表
struct TopicMessageTable{
	int num;
	struct TopicMSG TopicMessage_list[TOPIC_NUM_MAX];	//最多十个话题
};

struct TopicSubcriberTable{
	int num;
	struct TopicSUBSCRIBER TopicSubcriber_list[TOPIC_NUM_MAX];	//最多十个话题
};
static struct TopicMessageTable g_ptTopicMessageTable;		
static struct TopicSubcriberTable g_ptTopicSubcriberTable;


/******************/
//消息中心
#if 0 
struct MessageCenter{
	int (*RegisterMessage)(char *pcTopicName);
	int (*RegisterSubscriber)(char *pcTopicName,struct Subcriber* ptSubcriber);
	//void (*CancelSubscriber)(char *pcTopicName,struct Subcriber* ptSubcriber);

	//thread:处理线程.发送消息会唤醒此线程.
	void (*CoreProcss)(void);
};
#endif 

int RegisterMessage(char *pcTopicName)
{
	int i;
	int ret;
	
	for(i=0;i<g_ptTopicMessageTable.num;i++){
		ret = strcmp(g_ptTopicMessageTable.TopicMessage_list[i].TopicName, pcTopicName);
		/* 找到同名话题 */
		if(ret == 0){
			printf("RegisterMessage:已经存在名为%s话题.\r\n",pcTopicName);
			return -1;
		}
		
	}
    /* TopicMessageTable,TopicSubcriberTable中增加名为pcTopicName的话题 */
    printf("RegisterMessage:注册名为%s话题.\r\n",pcTopicName);
					
    g_ptTopicMessageTable.TopicMessage_list[g_ptTopicMessageTable.num].TopicName = pcTopicName;
    g_ptTopicMessageTable.num++;		
    g_ptTopicSubcriberTable.TopicSubcriber_list[g_ptTopicSubcriberTable.num].TopicName = pcTopicName;
    g_ptTopicSubcriberTable.num++;
    
	return 0;
}

int RegisterSubscriber(char *pcTopicName,struct Subcriber* ptSubcriber)
{
	int i;
	int ret;
	
	for(i=0;i<g_ptTopicMessageTable.num;i++){
		ret = strcmp(g_ptTopicMessageTable.TopicMessage_list[i].TopicName, pcTopicName);
		
		/* 找到同名话题,将subscriber加到TopicSUBSCRIBER */
		if(ret == 0){
            break;
		}
	}
    
    if(ret == 0){
        printf("RegisterSubscriber:加入subscriber到话题%s的链表.\r\n",pcTopicName);
        
        g_ptTopicSubcriberTable.TopicSubcriber_list[i].subciber_list[g_ptTopicSubcriberTable.TopicSubcriber_list[i].num] = ptSubcriber; 
        g_ptTopicSubcriberTable.TopicSubcriber_list[i].num++;    
        return 0;
    }
    /* 否则注册一个名为pcTopicName的话题 */
    else{
        printf("RegisterSubscriber:不存在名称为:%s的话题,请先注册话题.\r\n",pcTopicName);
        return -1;
    }
}


void CoreProcss(void *arg)
{
	int i,j;
    int ret;
	struct TopicMSG *ptTopicMessage = (struct TopicMSG *)arg;
		
	//每当发布者发布消息,唤醒此线程
	//由TopicName,找到更新消息的话题,并更新Message
	for(i=0;i<g_ptTopicMessageTable.num;i++){
		ret = strcmp(g_ptTopicMessageTable.TopicMessage_list[i].TopicName,ptTopicMessage->TopicName);
        
        if(ret == 0){
            break;
        }    
	}

    /* 找到同名话题 */
    if(ret == 0){
        //printf("CoreProcss:存在名为%s话题.\r\n",ptTopicMessage->TopicName);

        /* 保存数据 */
        g_ptTopicMessageTable.TopicMessage_list[i].MessageData = ptTopicMessage->MessageData;

        /* 执行Handle */
        for(j=0;j<g_ptTopicSubcriberTable.TopicSubcriber_list[i].num;j++){
            //printf("CoreProcss:%s话题下%d号订阅者处理.\r\n",ptTopicMessage->TopicName,j);
            g_ptTopicSubcriberTable.TopicSubcriber_list[i].subciber_list[j]->HandleEvent(ptTopicMessage->MessageData);
        }

        if(j == 0){
            printf("CoreProcss:%s话题下没有订阅者.\r\n",ptTopicMessage->TopicName);
        }
    }
    else{
        printf("CoreProcss:不存在名为%s话题.\r\n",ptTopicMessage->TopicName);
        return;
    }	
}

