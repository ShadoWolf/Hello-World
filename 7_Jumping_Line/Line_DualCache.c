#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define SIZE 100
#define ITER_NUM 100000000

volatile long long array[SIZE];

void die(int exitCode, char* message) {
	printf("%s", message);
	exit(exitCode);
}

static void *thread_counter(void *args) {
	int id;
	id = (int)args;
	for(array[id]=0; array[id]<ITER_NUM; array[id]++);
	pthread_exit(NULL);
}



int main() {
	cpu_set_t cpu1, cpu2;
	CPU_ZERO(&cpu1);
	CPU_SET(0, &cpu1);
	CPU_ZERO(&cpu2);
	CPU_SET(1, &cpu2);
	pthread_t thread1;
	pthread_t thread2;
	if (pthread_create(&thread1, NULL, thread_counter, (void*)0) != 0)
		die(11, "failed to create thread 1");
	if (pthread_create(&thread2, NULL, thread_counter, (void*)10) != 0)
		die(21, "failed to create thread 2");
	if (pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpu1) != 0)
		die(10, "failed to setaffinity thread 1");
	if (pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpu2) != 0)
		die(20, "failed to setaffinity thread 2");
	time_t t1 = time(NULL);
	if (pthread_join(thread1, NULL) != 0)
		quit(12, "failed join thread 1");
	if (pthread_join(thread2, NULL) != 0)
		quit(22, "failed join thread 2");
	time_t t2 = time(NULL);
	printf("Different caches time: %d sec\n", (int)(t2-t1));
	return 0;
}
