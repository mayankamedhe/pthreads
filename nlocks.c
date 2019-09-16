#include "types.h"
#include "stat.h"
#include "user.h"

void mutex_lock(int index)
{
	acquire_mutex_spinlock(index);
	
	while(get_mutex_value(index))
	{
		cond_wait(index+1000, index);
	}
	set_mutex_value(index, 1);
	release_mutex_spinlock(index);
}

void mutex_unlock(int index)
{
	acquire_mutex_spinlock(index);
	set_mutex_value(index, 0);
	cond_signal(index+1000);
	release_mutex_spinlock(index);
}

int main()
{
	init_counters();
	int val[10];

	for(int j=0; j<10; j++)
	{
		int ret;
		int i;	
		ret = fork();
		for(i=0; i<1000; i++)
		{
			mutex_lock(j);
			set_var(j, get_var(j)+1);
			mutex_unlock(j);
		}

		if(ret == 0)
		{
			exit();
		}
		else
		{
			wait();
			val[j] = get_var(j);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		printf(1, "data[%d] = %d\n", i, val[i]);
	}
	exit();
}


// #include "types.h"
// #include "stat.h"
// #include "user.h"

// void mutex_lock(int index)
// {
// 	acquire_mutex_spinlock(index);
	
// 	while(get_mutex_value(index))
// 	{
// 		cond_wait(index+1000, index);
// 	}
// 	set_mutex_value(index, 1);
// 	release_mutex_spinlock(index);
// }

// void mutex_unlock(int index)
// {
// 	acquire_mutex_spinlock(index);
// 	set_mutex_value(index, 0);
// 	cond_signal(index+1000);
// 	release_mutex_spinlock(index);
// }

// int main()
// {
// 	init_counters();
// 	int val[10];
// 	init_mutex();

// 	for(int j=0; j<10; j++)
// 	{
// 		int ret;
// 		int i;	
// 		ret = fork();
		
// 		if(ret == 0)
// 		{
// 			for(i=0; i<1000; i++)
// 			{
// 				mutex_lock(0);
// 				set_var(0, get_var(0)+1);
// 				mutex_unlock(0);		
// 			}
// 		}
// 		else
// 		{
// 			for(i=0; i<1000; i++)
// 			{
// 				mutex_lock(0);
// 				set_var(0, get_var(0)+1);
// 				mutex_unlock(0);		
// 			}
// 			exit();
			
// 			val[j] = get_var(j);
// 		}
// 	}

// 	for (int i = 0; i < 10; ++i)
// 	{
// 		wait();
// 	}

// 	for (int i = 0; i < 10; ++i)
// 	{
// 		printf(1, "data[%d] = %d\n", i, val[i]);
// 	}
// 	exit();
// }