#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_CREATE_ERR 1
#define THREAD_AFFINITY_ERR 2
#define THREAD_JOIN_ERR 3
#define mfence asm volatile ("mfence" : : : "memory")

bool flag[2]={false, false};
volatile int turn=0;
volatile int criticalResource=0;

static void *proc1(void *argv) {
	for (int i=0; i<1000000; i++) {
		flag[0]=true;
		mfence;
		while (flag[1]==true) {
			if (turn!=0) {
				mfence;
				flag[0]=false;
				while (turn!=0);
				flag[0]=true;
				mfence;
			}
		}
		criticalResource++;
		turn=1;
		mfence;
		flag[0]=false;
		mfence;
	}
}

static void *proc2(void *argv) {
	for (int i=0; i<1000000; i++) {
		flag[1]=true;
		mfence;
		while (flag[0]==true) {
			if(turn!=1) {
				mfence;
				flag[1]=false;
				while(turn!=1);
				flag[1]=true;
				mfence;
			}
		}
		criticalResource++;
		turn=0;
		mfence;
		flag[1]=false;
		mfence;
	}
}

int main(int argc,char **argv) {
	pthread_t thread1, thread2;
	cpu_set_t cpu1, cpu2;
	CPU_ZERO(&cpu1);
	CPU_SET(0, &cpu1);
	CPU_ZERO(&cpu2);
	CPU_SET(1, &cpu2);
	if (pthread_create(&thread1, NULL, proc1, NULL) || pthread_create(&thread2, NULL, proc2, NULL)) {
		return THREAD_CREATE_ERR;
	}
	if (pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpu1) || pthread_setaffinity_np(thread2, sizeof(cpu_set_t), &cpu2)) {
		return THREAD_AFFINITY_ERR;
	}
	if (pthread_join(thread1, NULL) || pthread_join(thread2, NULL)) {
		return THREAD_JOIN_ERR;
	}
	fprintf (stdout, "%s %d\n","criticalResource=", criticalResource);
	return 0;
}
