#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>

#include "message_manager.h"

//发送消息
void Publish(void *arg)
{
	//唤醒消息中心线程
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
	
//接受消息
void HandlerAsam(void *msgdata)
{
    int subscribe_data[5];
    
	printf("订阅者Asam Handle.\r\n");
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
	printf("订阅者Tifa Handle.\r\n");
}

struct Subcriber g_tSubcriberTifa = {
	.Subscribe = Subscribe,
	.HandleEvent = HandlerTifa
};


//自发自收
void message_test(void)
{
    printf("message_test开始.\r\n");
    
	//注册MSG
    RegisterMessage("test");

	//在话题中加入订阅者
	g_tSubcriberAsam.Subscribe("test",&g_tSubcriberAsam);
	g_tSubcriberTifa.Subscribe("test",&g_tSubcriberTifa);
    
	//发布消息
	g_tPublisher.Publish(&g_tMsgTest);
    
}

