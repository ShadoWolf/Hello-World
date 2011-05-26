#include "lifecalc.h"
#include <stdlib.h>


int calcLife(char* cells, int nCol, int nRow, int x, int y, bool bIsTor) {
	int n = 0, nCol1 = nCol+1;
	if (bIsTor) {
		int tx;
		int ty1=y-1;
		int ty2=y+1;
		if(ty1<0) ty1=nRow-1;
		if(ty2>=nCol) ty2=0;
		if(cells[ty1*nCol1+x] == '*') n++;
		if(cells[ty2*nCol1+x] == '*') n++;
		if((tx=x-1)<0) tx=nCol-1;
		if(cells[ty1*nCol1+tx] == '*') n++;
		if(cells[ty2*nCol1+tx] == '*') n++;
		if(cells[y*nCol1+tx] == '*') n++;
		if((tx=x+1)>=nCol) tx=0;
		if(cells[ty1*nCol1+tx] == '*') n++;
		if(cells[ty2*nCol1+tx] == '*') n++;
		if(cells[y*nCol1+tx] == '*') n++;
		return n;
	}
	bool fX = x<(nCol-1);
	if(y>0) {
		if(cells[(y-1)*nCol1+x] == '*') n++;
		if(x>0 && cells[(y-1)*nCol1+x-1] == '*') n++;
		if(fX && cells[(y-1)*nCol1+x+1] == '*') n++;
	}
	if(x>0 && cells[y*nCol1+x-1] == '*') n++;
	if(fX && cells[y*nCol1+x+1] == '*') n++;
	if(y<(nRow-1)) {
		if(cells[(y+1)*nCol1+x] == '*') n++;
		if(x>0 && cells[(y+1)*nCol1+x-1] == '*') n++;
		if(fX && cells[(y+1)*nCol1+x+1] == '*') n++;
	}
	return n;
}

void iterateLife(int nCol, int nRow, char * cells, char *ncells, bool bIsTor) {
	for(int y = 0; y < nRow; y++) {
		for(int x = 0; x < nCol; x++) {
			int n = calcLife(cells, nCol, nRow, x,y, bIsTor);
			if(cells[y*(nCol+1)+x] == '*') {
				ncells[y*(nCol+1)+x] = (n==2 || n==3) ? '*' : ' ';
			}
			else {
				ncells[y*(nCol+1)+x] = (n==3) ? '*' : ' ';
			}
		}
		ncells[y*(nCol+1)+nCol] = '\n';
	}
	ncells[nRow*(nCol+1)] = 0;
}

void generateCells(int nCol, int nRow, char * cells) {
	for(int y = 0; y < nRow; y++) {
		for(int x = 0; x < nCol; x++) {
			cells[y*(nCol+1)+x] = rand()%2 ? '*' : ' ';
		}
		cells[y*(nCol+1)+nCol] = '\n';
	}
	cells[nRow*(nCol+1)] = 0;
}
