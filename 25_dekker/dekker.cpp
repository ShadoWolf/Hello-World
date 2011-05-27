#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_CREATE_ERR 1
#define THREAD_AFFINITY_ERR 2
#define THREAD_JOIN_ERR 3

volatile bool flag[2]={false, false};
volatile bool turn=0;
volatile int criticalResource=0;

void dekkerLock(bool id) {
	flag[id]=true;
	while (flag[id^1]==true) {
		if (turn!=id) {
			flag[id]=false;
			while (turn!=id);
			flag[id]=true;
		}
	}
}

void dekkerUnlock(bool id) {
	flag[id]=false;
	turn=id^1;
}

static void *aThread(void *argv) {
	bool id=(bool)argv;
	for (int i=0; i<1000000; i++) {
		dekkerLock(id);
		criticalResource++;
		dekkerUnlock(id);
	}
}

int main(int argc,char **argv) {
	pthread_t thread1, thread2;
	cpu_set_t cpu1, cpu2;
	CPU_ZERO(&cpu1);
	CPU_SET(0, &cpu1);
	CPU_ZERO(&cpu2);
	CPU_SET(1, &cpu2);
	if (pthread_create(&thread1, NULL, aThread, (void*)0) || pthread_create(&thread2, NULL, aThread, (void*)1)) {
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
