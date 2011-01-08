#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define X_SIZE 10000
#define Y_SIZE 100
#define ITER_NUM 100

void die(char * message){
	printf("%s", message);
	exit(1);
}

int main()
{
	int **A = (int **)malloc(Y_SIZE*sizeof(int *));
	if(A==NULL)
		die("Y malloc error");
	for(int k=0; k<Y_SIZE; ++k) {
		A[k] = (int *)malloc(X_SIZE*sizeof(int));
		if(A[k]==NULL)
			die("X malloc error");
	}
	int S=0;
	time_t t1 = time(NULL);
	for(int k=0; k<ITER_NUM; k++) {
		for(int i=0; i<Y_SIZE; i++) {
			for(int j=0; j<X_SIZE; j++) {
				S += A[i][j];
			}
		}
	}
	time_t t2 = time(NULL);
	S=0;
	for(k=0; k<M; ++k) {
		for(i=0; i<X; ++i) {
			for(j=0; j<Y; ++j) {
				S += A[j][i];
			}
		}
	}
	time_t t3 = time(NULL);
	for(k=0; k<Y; ++k) {
		free(A[k]);
	}
	free(A);
	printf("by rows:\t%d\nby columns:\t%d\n", (int)(t2-t1), (int)(t3-t2));
	return 0;
}