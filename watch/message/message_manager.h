#ifndef __MESSAGE_MANAGER_H
#define __MESSAGE_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

struct msg_topic{
    char *name;
    void *msg_data;
};

typedef struct Publisher{
	void (*Publish)(void *arg);	
}Publisher,*PPublisher;

typedef struct Subcriber{
	void (*Subscribe)(char *pcTopicName, struct Subcriber* ptSubcriber);			
	void (*unSubscribe)(char *pcTopicName, struct Subcriber* ptSubcriber);		
	void (*HandleEvent)(void *msgdata);			
}Subcriber,*PSubcriber;

int RegisterMessage(char *pcTopicName);
int RegisterSubscriber(char *pcTopicName,struct Subcriber* ptSubcriber);
void CoreProcss(void *arg);

#endif	/*__MESSAGE_MANAGER_H*/

