#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>

#include "message_manager.h"

//������Ϣ
void Publish(void *arg)
{
	//������Ϣ�����߳�
	CoreProcss(arg);
}

struct Publisher g_tPublisher = {
	.Publish = Publish
};

int publish_data[5] = {1,2,3,4,5};
//char publish_data[5] = {'a','b','c','d','e'};

struct msg_test{
    char *name;
    void *msg_data;
};

struct msg_test g_tMsgTest = {
	.name = "test",
	.msg_data = publish_data
};
	
//������Ϣ
void HandlerAsam(void *msgdata)
{
    int subscribe_data[5];
    
	printf("������Asam Handle.\r\n");
    for(int i=0;i<5;i++)
        printf("msgdata_%d = %d.\r\n",i,((int *)msgdata)[i]);

    memcpy(subscribe_data,msgdata,20);
    
    for(int i=0;i<5;i++){
       // subscribe_data[i] = ((char *)msgdata)[i];
        printf("subscribe_data_%d = %d.\r\n",i,subscribe_data[i]);
    }
}

void Subscribe(char *pcTopicName, struct Subcriber* ptSubcriber)
{
	RegisterSubscriber(pcTopicName,ptSubcriber);
}

struct Subcriber g_tSubcriberAsam = {
	.Subscribe = Subscribe,
	.HandleEvent = HandlerAsam
};
 
void HandlerTifa(void *msgdata)
{
	printf("������Tifa Handle.\r\n");
}

struct Subcriber g_tSubcriberTifa = {
	.Subscribe = Subscribe,
	.HandleEvent = HandlerTifa
};


//�Է�����
void message_test(void)
{
    printf("message_test��ʼ.\r\n");
    
	//ע��MSG
    RegisterMessage("test");

	//�ڻ����м��붩����
	g_tSubcriberAsam.Subscribe("test",&g_tSubcriberAsam);
	g_tSubcriberTifa.Subscribe("test",&g_tSubcriberTifa);
    
	//������Ϣ
	g_tPublisher.Publish(&g_tMsgTest);
    
}

