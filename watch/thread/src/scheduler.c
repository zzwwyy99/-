#include <rtthread.h>
#include <rthw.h>
#include <stdio.h>

/* �߳̿��ƿ�ָ�룬����ָ��ǰ�߳� */
struct rt_thread *rt_current_thread;

/* �߳̿��ƿ�ָ�룬����ָ��ǰ�߳� */
struct rt_thread *g_ptScheduleThread;


/* ��ʼ��ϵͳ������ */
void rt_system_scheduler_init(void)
{	
	/* ��ʼ����ǰ�߳̿��ƿ�ָ�� */
	rt_current_thread = RT_NULL;

}

/* ����ϵͳ������ */
void rt_system_scheduler_start(struct rt_thread *first_thread)
{
	register struct rt_thread *to_thread;
    
    /* �ֶ�ָ����һ�����е��߳� */
    to_thread = first_thread;
    
	rt_current_thread = to_thread;
														
	/* �л�����һ���̣߳��ú�����context_rvds.S��ʵ�֣���rthw.h������
       ����ʵ�ֵ�һ�������л�����һ����ຯ����C�ļ��е��õ�ʱ��
       ������βΣ���ִ�е�ʱ��Ὣ�βδ��뵽�Ĵ���r0��*/
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
}

/* ϵͳ���� */
void rt_schedule(struct rt_thread *to_thread)
{	
    struct rt_thread *from_thread;
	
    /* �л���ָ�����߳� */
    from_thread = rt_current_thread;
    rt_current_thread = to_thread;
    
    if(from_thread == to_thread){
        //printf("�ж�:from_thread = to_thread = %s,�������л�.\r\n",to_thread->name);
        return; 
    }else{
       // printf("�ж�:from_thread = %s,to_thread = %s,�����л�.\r\n",from_thread->name,to_thread->name);
        /* �����������л� */
        rt_hw_context_switch((rt_uint32_t)&from_thread->sp,(rt_uint32_t)&to_thread->sp);
    }
}
