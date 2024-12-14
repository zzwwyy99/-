#ifndef __OPTION_MANAGER_H
#define __OPTION_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

#include "node_manager.h" 

enum OPTION_CMD{
	OPTION_CMD_BACK,
	OPTION_CMD_NEXT,
	OPTION_CMD_ENTER
};

enum OPTION_STATUS{
	OPTION_CHOOSE,		//选定该选项
	OPTION_ENTER		//进入该选项
};
	
typedef struct Option{
	char *name;
	Node node;
	enum OPTION_STATUS status;		
	
	void (*choose)(struct Option *ptOption);	//选定该选项
	void (*enter)(struct Option *ptOption);		//进入该选项
}Option,*POption;	

struct menu_status{
    int EnterNodeCount;
    int EnterStatusCount;
};

void OptionRegister(struct Option *ptOption,char *name);
POption __GetOption(char *name);
void OptionControl(int cmd);
void InitMenuOption(void);
void MenuThreadEntry(void *arg);

void MenuThreadInit(void);

#endif	/*__OPTION_MANAGER_H*/

