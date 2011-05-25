#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#define INVALID_ARGUMENTS 1
#define MALLOC_ERR 2
#define SSCANF_ERR 3
#define SELECT_ERR 4

int main (int argc, char **argv) {
	
	if (argc<2) {
		printf("\nUsage: 2.c desc1 desc2 [desc3 ...]\n");
		return INVALID_ARGUMENTS;
	}
	int *descNumbers=(int*)malloc((argc-1)*sizeof(int));
	if (descNumbers==NULL) {
		return MALLOC_ERR;
	}
	fd_set sigSet;
	FD_ZERO(&sigSet);
	int maxNumOfSig=0;
	for (int i=0; i<argc-1; i++) {
		
		if (!sscanf(argv[i+1], "%d", descNumbers+i)) {
			free(descNumbers);
			return SSCANF_ERR;
		}

		FD_SET(descNumbers[i], &sigSet);
		if (descNumbers[i]>maxNumOfSig) {
			maxNumOfSig=descNumbers[i];
		}
	}
	while(true) {
		fd_set tempSet=sigSet;
		if (select(maxNumOfSig+1, &sigSet, NULL, NULL, 0)==-1) {
			free(descNumbers);
			return SELECT_ERR;
		}
		char readBuf[20];
		int readLen;
		for (int i=0; i<argc-1; i++) {
			if (FD_ISSET(descNumbers[i], &sigSet)) {
				readLen=read(descNumbers[i], readBuf, 20);
				write(1, readBuf, readLen);
			}
		}
		sigSet=tempSet;
	}
	free(descNumbers);
	return 0;
}
