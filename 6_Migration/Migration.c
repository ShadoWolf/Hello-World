int a[100][100], b[100][100], c[100][100], y, i, j;
int main(){
	for(y = 0; y < 1000; y++) {
		for(i = 0; i < 100; i++) {
			for(j = 0; j < 100; j++) {
				c[i][j] = a[i][j]*b[j][i];
			}
		}
	}
}