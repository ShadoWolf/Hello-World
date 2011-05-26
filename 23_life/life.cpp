#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "lifecalc.h"

#define	THREAD_CREATE_ERR 1
#define	THREAD_JOIN_ERR 2

pthread_mutex_t aMutex=PTHREAD_MUTEX_INITIALIZER;
const bool bIsTor=false;
volatile int nCol=15, nRow=15, lastCalcBuffer=0, printBuffer=-1;
char buffer[3][15000];

static void *life_thread(void *args) {
	srand(time(NULL));
	for (;;) {
		int newBuffer=-1;
		pthread_mutex_lock(&aMutex);
		for (newBuffer=0; newBuffer<3 && (newBuffer==lastCalcBuffer || newBuffer==printBuffer); newBuffer++);
		pthread_mutex_unlock(&aMutex);
		iterateLife(nCol, nRow, buffer[lastCalcBuffer], buffer[newBuffer], bIsTor);
		pthread_mutex_lock(&aMutex);
		lastCalcBuffer=newBuffer;
		pthread_mutex_unlock(&aMutex);
		sleep(1);
	}
}

static void *print_thread(void *args) {
	char c;
	for(;;) {
		if (scanf("%c", &c)) {
			pthread_mutex_lock(&aMutex);
			printBuffer=lastCalcBuffer;
			pthread_mutex_unlock(&aMutex);
			puts(buffer[printBuffer]);
			pthread_mutex_lock(&aMutex);
			printBuffer=-1;
			pthread_mutex_unlock(&aMutex);
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
