#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef unsigned short int usint;
void print(usint (*a)[9]){
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			printf("%hu\t", a[i][j]);
		}
		printf("\n");	
	}
}

void init(usint (*a)[9]){
	usint x;
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			x = a[i][j];
			if(x == 0){
				a[i][j] = 1022;
			}			
			else{
				a[i][j] = pow (2.0, x);
			}
		}

	}
}
usint Log2(usint x){
	usint y = 0;
	while(x = (x >> 1)){
		y++;
	}
	return y;
}

int checkpow2(usint n){
	return((n & (n-1)) == 0);
}

int checkbitset(usint n, int j){
	return((n>>j)&1);
}

void removecnr(usint (*a)[9]){
	int i, j, m;
	usint x, y;
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			x = a[i][j];
			if(checkpow2(x)){
				y = Log2(x);
				for(m = 0; m < 9; m++){
					if(m == j)
						;
					else{
						if(checkbitset(a[i][m], y))
							a[i][m] = a[i][m] - x;
					}
				}
				for(m = 0; m < 9; m++){
					if(m == i)
						;
					else{
						if(checkbitset(a[m][j], y))
							a[m][j] = a[m][j] - x;
					}
				}
			}
		}
	}
}	

void removeb(usint (*a)[9]){
	int i, j, m, n, l, k;
	usint x, y;
	for(i = 0; i < 9; i = i + 3){
		for(j = 0; j < 9; j = j + 3){
		for(m = i; m < i + 3; m++){
		for(n = j; n < j + 3; n++){			

			x = a[m][n];
			if(checkpow2(x)){
				y = Log2(x);
				for(k = 0; k < 3; k++){
					if(k == n)
						;
					else{
						if(checkbitset(a[m][k], y))
							a[m][k] = a[m][k] - x;
					}
				}
				for(l = 0; l < 3; l++){
					if(l == m)
						;
					else{
						if(checkbitset(a[l][n], y))
							a[l][n] = a[l][n] - x;
					}
				}
			}
		}
		}
		}
	}
}	



int main(){
	usint a[9][9];
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++)
			scanf("%hu", &a[i][j]);
	}
	print(a);
	init(a);
printf("original:\n");
	print(a);
	removecnr(a);
printf("\n");
	print(a);
	removeb(a);
	printf("new sudoku looks like:\n");
	print(a);
	return 0;
}
