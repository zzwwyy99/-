#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "message_manager.h"

#define SUBSCRIBER_LIST_MAX	3
#define TOPIC_NUM_MAX	10


//��������, ��Ϣ
struct TopicMSG{
	char *TopicName;
	void *MessageData;	//���һ����Ϣ
};

//��������, ���ĸû���Ķ�����
struct TopicSUBSCRIBER{
	char *TopicName;
	int num;
	struct Subcriber *subciber_list[SUBSCRIBER_LIST_MAX];	//�������������
};

//�����
struct TopicMessageTable{
	int num;
	struct TopicMSG TopicMessage_list[TOPIC_NUM_MAX];	//���ʮ������
};

struct TopicSubcriberTable{
	int num;
	struct TopicSUBSCRIBER TopicSubcriber_list[TOPIC_NUM_MAX];	//���ʮ������
};
static struct TopicMessageTable g_ptTopicMessageTable;		
static struct TopicSubcriberTable g_ptTopicSubcriberTable;


/******************/
//��Ϣ����
#if 0 
struct MessageCenter{
	int (*RegisterMessage)(char *pcTopicName);
	int (*RegisterSubscriber)(char *pcTopicName,struct Subcriber* ptSubcriber);
	//void (*CancelSubscriber)(char *pcTopicName,struct Subcriber* ptSubcriber);

	//thread:�����߳�.������Ϣ�ỽ�Ѵ��߳�.
	void (*CoreProcss)(void);
};
#endif 

int RegisterMessage(char *pcTopicName)
{
	int i;
	int ret;
	
	for(i=0;i<g_ptTopicMessageTable.num;i++){
		ret = strcmp(g_ptTopicMessageTable.TopicMessage_list[i].TopicName, pcTopicName);
		/* �ҵ�ͬ������ */
		if(ret == 0){
			printf("RegisterMessage:�Ѿ�������Ϊ%s����.\r\n",pcTopicName);
			return -1;
		}
		
	}
    /* TopicMessageTable,TopicSubcriberTable��������ΪpcTopicName�Ļ��� */
    printf("RegisterMessage:ע����Ϊ%s����.\r\n",pcTopicName);
					
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
		
		/* �ҵ�ͬ������,��subscriber�ӵ�TopicSUBSCRIBER */
		if(ret == 0){
            break;
		}
	}
    
    if(ret == 0){
        printf("RegisterSubscriber:����subscriber������%s������.\r\n",pcTopicName);
        
        g_ptTopicSubcriberTable.TopicSubcriber_list[i].subciber_list[g_ptTopicSubcriberTable.TopicSubcriber_list[i].num] = ptSubcriber; 
        g_ptTopicSubcriberTable.TopicSubcriber_list[i].num++;    
        return 0;
    }
    /* ����ע��һ����ΪpcTopicName�Ļ��� */
    else{
        printf("RegisterSubscriber:����������Ϊ:%s�Ļ���,����ע�Ự��.\r\n",pcTopicName);
        return -1;
    }
}


void CoreProcss(void *arg)
{
	int i,j;
    int ret;
	struct TopicMSG *ptTopicMessage = (struct TopicMSG *)arg;
		
	//ÿ�������߷�����Ϣ,���Ѵ��߳�
	//��TopicName,�ҵ�������Ϣ�Ļ���,������Message
	for(i=0;i<g_ptTopicMessageTable.num;i++){
		ret = strcmp(g_ptTopicMessageTable.TopicMessage_list[i].TopicName,ptTopicMessage->TopicName);
        
        if(ret == 0){
            break;
        }    
	}

    /* �ҵ�ͬ������ */
    if(ret == 0){
        //printf("CoreProcss:������Ϊ%s����.\r\n",ptTopicMessage->TopicName);

        /* �������� */
        g_ptTopicMessageTable.TopicMessage_list[i].MessageData = ptTopicMessage->MessageData;

        /* ִ��Handle */
        for(j=0;j<g_ptTopicSubcriberTable.TopicSubcriber_list[i].num;j++){
            //printf("CoreProcss:%s������%d�Ŷ����ߴ���.\r\n",ptTopicMessage->TopicName,j);
            g_ptTopicSubcriberTable.TopicSubcriber_list[i].subciber_list[j]->HandleEvent(ptTopicMessage->MessageData);
        }

        if(j == 0){
            printf("CoreProcss:%s������û�ж�����.\r\n",ptTopicMessage->TopicName);
        }
    }
    else{
        printf("CoreProcss:��������Ϊ%s����.\r\n",ptTopicMessage->TopicName);
        return;
    }	
}

