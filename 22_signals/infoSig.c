#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

volatile int flag = 1;

void sigHandler(int sigNum, siginfo_t *info, void *lalala)
{
	printf("Signal No.%d; pid=%d; pgid=%d; \n",info->si_signo, getpid(), getpgid());
	if(sigNum==20)
		flag=0;
}

int main()
{
	struct sigaction sigAct;
	sigAct.sa_sigaction = sigHandler;
	sigAct.sa_flags = SA_SIGINFO;
	if(sigfillset(&sigAct.sa_mask)) {
		return 1;
	}
	if (sigaction(2, &sigAct, NULL) || sigaction(20, &sigAct, NULL)) {
		return 1;
	}
	while(flag);
	return 0;
}

