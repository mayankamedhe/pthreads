#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

	int
sys_fork(void)
{
	return fork();
}

	int
sys_exit(void)
{
	exit();
	return 0;  // not reached
}

	int
sys_wait(void)
{
	return wait();
}

	int
sys_kill(void)
{
	int pid;

	if(argint(0, &pid) < 0)
		return -1;
	return kill(pid);
}

	int
sys_getpid(void)
{
	return myproc()->pid;
}

	int
sys_sbrk(void)
{
	int addr;
	int n;

	if(argint(0, &n) < 0)
		return -1;
	addr = myproc()->sz;
	if(growproc(n) < 0)
		return -1;
	return addr;
}

	int
sys_sleep(void)
{
	int n;
	uint ticks0;

	if(argint(0, &n) < 0)
		return -1;
	acquire(&tickslock);
	ticks0 = ticks;
	while(ticks - ticks0 < n){
		if(myproc()->killed){
			release(&tickslock);
			return -1;
		}
		sleep(&ticks, &tickslock);
	}
	release(&tickslock);
	return 0;
}

// return how many clock tick interrupts have occurred
// since start.
	int
sys_uptime(void)
{
	uint xticks;

	acquire(&tickslock);
	xticks = ticks;
	release(&tickslock);
	return xticks;
}


/*************** Lab 8 ***************/

#define NLOCKS 20             

struct mutex
{
int value;    // stores lock state if used as mutex and 
// stores buffer size for producer-consumer
struct spinlock s;
};

// struct sleeplock1 {
//   uint locked;  
// };

struct mutex m[NLOCKS];       
int counters[NLOCKS];         //  Shared counters

// int num_array[10];
// struct sleeplock1 sleep_array[10];

int 
sys_acquire_mutex_spinlock(void)
{
	int n;
	if(argint(0, &n) < 0)
		return -1;
	spinlock_acquire(&m[n].s);       // defined in spinlock.c
	return 0;
}

int 
sys_release_mutex_spinlock(void)
{
	int n;
	if(argint(0, &n) < 0)
		return -1;
	struct spinlock * sl = &m[n].s;
	spinlock_release(sl);      // defined in spinlock.c
	return 0;
}
int
sys_cond_wait(void)
{
	int variable;
	if(argint(0, &variable) < 0)
		return -1;
	int n;
	if(argint(1, &n) < 0)
		return -1;
	struct spinlock * sl = &m[n].s;
	cv_sleep(variable, sl);               // defined in proc.c
	return 0;
}
int 
sys_cond_signal(void)
{
	int variable;
	if(argint(0, &variable) < 0)
		return -1;
	wakeup((void *)variable);               // Use existing xv6 wake up
	return 0;
}

int 
sys_get_mutex_value(void)
{
	int n;
	if(argint(0, &n) < 0)
		return -1;
	
	return m[n].value;
}


	int 
sys_set_mutex_value(void)
{
	int n;
	if(argint(0, &n) < 0)
		return -1;
	int value;
	if(argint(1, &value) < 0)
		return -1;

	m[n].value = value;
	return 0;
}


	int 
sys_init_mutex(void)
{
	for (int i = 0; i < NLOCKS; ++i)
	{
		m[i].value = 0;
	}
	return 0;
}


	int 
sys_init_counters(void)
{
	for(int i=0; i<10; i++)
	{
	counters[i] = 0;
	// sleep_array[i].locked = 0;
	}

	return 1;
}

int sys_get_var(void)
{
  int num;

  if(argint(0, &num) < 0)
    return -1;

  if(num >= 0 || num < 10)
  {
    return counters[num];
  }
  return -1;

}


	int 
sys_set_var(void)
{
	int num;

	if(argint(0, &num) < 0)
	return -1;

	int newVal;

	if(argint(1, &newVal) < 0)
	return -1;

	if(num >= 0 || num < 10)
	{
	counters[num] = newVal;
	return 1;
	}
	return -1;
}

/*************** Lab 8 ***************/
