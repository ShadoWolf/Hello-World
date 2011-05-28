#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "lifecalc.h"

#define	THREAD_CREATE_ERR 1
#define	THREAD_JOIN_ERR 2
#define MUTEX_ERR 3

pthread_mutex_t aMutex=PTHREAD_MUTEX_INITIALIZER;
const bool bIsTor=false;
volatile int nCol=15, nRow=15, lastCalcBuffer=0, printBuffer=-1;
char buffer[3][15000];

static void *life_thread(void *args) {
	srand(time(NULL));
	for (;;) {
		int newBuffer=-1;
		if (pthread_mutex_lock(&aMutex)) {
			pthread_exit((void*)MUTEX_ERR);
		}
		for (newBuffer=0; newBuffer<3 && (newBuffer==lastCalcBuffer || newBuffer==printBuffer); newBuffer++);
		if (pthread_mutex_unlock(&aMutex)) {
			pthread_exit((void*)MUTEX_ERR);
		}
		iterateLife(nCol, nRow, buffer[lastCalcBuffer], buffer[newBuffer], bIsTor);
		if (pthread_mutex_lock(&aMutex)) {
			pthread_exit((void*)MUTEX_ERR);
		}
		lastCalcBuffer=newBuffer;
		if (pthread_mutex_unlock(&aMutex)) {
			pthread_exit((void*)MUTEX_ERR);
		}
		sleep(1);
	}
}

static void *print_thread(void *args) {
	char c;
	for(;;) {
		if (scanf("%c", &c)) {
			if (pthread_mutex_lock(&aMutex)) {
				pthread_exit((void*)MUTEX_ERR);
			}
			printBuffer=lastCalcBuffer;
			if (pthread_mutex_unlock(&aMutex)) {
				pthread_exit((void*)MUTEX_ERR);
			}
			puts(buffer[printBuffer]);
			if (pthread_mutex_lock(&aMutex)) {
				pthread_exit((void*)MUTEX_ERR);
			}
			printBuffer=-1;
			if (pthread_mutex_unlock(&aMutex)) {
				pthread_exit((void*)MUTEX_ERR);
			}
		}
	}
}

int main() {
	generateCells(nCol, nRow, buffer[0]);
	pthread_t thread1, thread2;

	if (pthread_create(&thread1, NULL, life_thread, NULL) != 0) {
		return THREAD_CREATE_ERR;
	}
	if (pthread_create(&thread2, NULL, print_thread, NULL) != 0) {
		return THREAD_CREATE_ERR;
	}

	if (pthread_join(thread1, NULL) != 0) {
		return THREAD_JOIN_ERR;
	}
	if (pthread_join(thread2, NULL) != 0) {
		return THREAD_JOIN_ERR;
	}
	return 0;
}
