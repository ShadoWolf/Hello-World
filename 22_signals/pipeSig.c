#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

int sigPipe[2];

void sigHandler(int sigNum) {
	write(sigPipe[1], &sigNum, sizeof(sigNum));
}

int main()
{
	if (pipe(sigPipe)) {
		return 1;
	}
	struct sigaction sigAct;
	sigAct.sa_handler=sigHandler;
	sigAct.sa_flags=0;
	if(sigfillset(&sigAct.sa_mask)) {
		return 2;
	}
	if (sigaction(2, &sigAct, NULL) || sigaction(20, &sigAct, NULL)) {
		return 3;
	}
	int sigNum=-1;
	while(sigNum!=20) {
		int readedNum=read(sigPipe[0],&sigNum,sizeof(sigNum));
		if (!readedNum || (readedNum==-1 && errno==EINTR)) {
			sigNum=-1;		
		}
		else if (readedNum!=-1) {
			printf("Signal No.%d; pid=%d; pgid=%d; \n",sigNum, getpid(), getpgid());
		}
		else {
			return 4;
		}
	}
	return 0;
}

