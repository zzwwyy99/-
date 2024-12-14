#ifndef __LOGIC_CONTROL_H
#define __LOGIC_CONTROL_H

#ifndef NULL
#define NULL (void *)0
#endif

//����Ӧ����Elementʵ��һ��accept�ӿ�,Ȼ��Visitor����Element.
//void accpet(Visitor)
//{
//	Visitor.visit(this);
//}

/* Element */
typedef struct Element{
    void (*FirstKeyShortPress)(void); 
    void (*SecondKeyShortPress)(void);
    void (*ThirdKeyShortPress)(void);
	void (*FirstKeyLongPress)(void); 
    void (*SecondKeyLongPress)(void);
    void (*ThirdKeyLongPress)(void);
}Element,*PElement;

/* Visitor */
typedef struct Visitor{
	void (*visit)(struct Element *ptElement);
}Visitor,*PVisitor;

void LogicThreadInit(void);
void LogicThreadEntry(void *arg);

#endif	/*__LOGIC_CONTROL_H*/

