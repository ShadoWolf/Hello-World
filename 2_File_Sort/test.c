#include <stdio.h>
#include <stdlib.h>
#define MAXNUMBERS 100000

void die(char * message){
	printf("%s", message);
	exit(1);
}
int arr[MAXNUMBERS], count;

int main(int argc, char** argv) {
	int i,j,t, readed, writed;

	if (argc < 2)
		die("malo faylow");
	count = 0;
	
	for(i = 1; i < argc-1; i++){	
		FILE * f = fopen(argv[i], "r");
		if (!f)
			die("Bad");
		while (!feof(f)){
			readed = fscanf(f, "%d", &arr[count++]);
			if (!readed){
				fclose(f);
				die("v fayle kaka");
			}
			if (count >= MAXNUMBERS){
				fclose(f);
				die("U menya ne hvataet pamati");
			}
		}
		fclose(f);
	}	

	for(i = 0; i < count; i++)
		for(j = 0; j < i; j++){
			if (arr[i] < arr[j]){
				t = arr[i];
				arr[i] = arr[j];
				arr[j] = t;
			}
		}
	FILE * fout = fopen(argv[argc-1], "w");
	if (!fout)
		die("WTF");
	for(i = 0; i < count; i++){
		writed = fprintf(fout, "%d\n", arr[i]);
		if (writed < 0){
			fclose(fout);
			die("ne shmogla zapisat");
		}
	}
	fclose(fout);
	die("All Ok");
	return 0;
}
