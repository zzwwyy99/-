#include <rtthread.h>
#include <rthw.h>
#include <stdio.h>

/* 线程控制块指针，用于指向当前线程 */
struct rt_thread *rt_current_thread;

/* 线程控制块指针，用于指向当前线程 */
struct rt_thread *g_ptScheduleThread;


/* 初始化系统调度器 */
void rt_system_scheduler_init(void)
{	
	/* 初始化当前线程控制块指针 */
	rt_current_thread = RT_NULL;

}

/* 启动系统调度器 */
void rt_system_scheduler_start(struct rt_thread *first_thread)
{
	register struct rt_thread *to_thread;
    
    /* 手动指定第一个运行的线程 */
    to_thread = first_thread;
    
	rt_current_thread = to_thread;
														
	/* 切换到第一个线程，该函数在context_rvds.S中实现，在rthw.h声明，
       用于实现第一次任务切换。当一个汇编函数在C文件中调用的时候，
       如果有形参，则执行的时候会将形参传入到寄存器r0。*/
	rt_hw_context_switch_to((rt_uint32_t)&to_thread->sp);
}

/* 系统调度 */
void rt_schedule(struct rt_thread *to_thread)
{	
    struct rt_thread *from_thread;
	
    /* 切换到指定的线程 */
    from_thread = rt_current_thread;
    rt_current_thread = to_thread;
    
    if(from_thread == to_thread){
        //printf("中断:from_thread = to_thread = %s,不进行切换.\r\n",to_thread->name);
        return; 
    }else{
       // printf("中断:from_thread = %s,to_thread = %s,进行切换.\r\n",from_thread->name,to_thread->name);
        /* 产生上下文切换 */
        rt_hw_context_switch((rt_uint32_t)&from_thread->sp,(rt_uint32_t)&to_thread->sp);
    }
}
