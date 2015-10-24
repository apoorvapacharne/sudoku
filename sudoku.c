#include<stdio.h>
#include<stdlib.h>
#include<math.h>
static int c;
typedef unsigned short int usint;

int bin(int num){
	if (num == 0)
		return 0;
	else
		return (num % 2) + 10 * bin(num / 2);
}
/*this function prints the sudoku*/
void print(usint (*a)[9]){
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			printf("%hu\t", a[i][j]);
		}
		printf("\n");	
	}
}
void printb(usint (*a)[9]){
	int b[9][9];
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			b[i][j] = (int)a[i][j];
		}
		
	}
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			printf("%10d ",bin(b[i][j]));
		}
		printf("\n");	
	}
}
/* all the elements(except the numbers of the puzzle question) of the sudoku are initialised to 1022 i.e. 1111111110 in binary. this is the possibility list of each block*/
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

/*finds the log to the base 2 of the number given*/
usint Log2(usint x){
	usint y = 0;
	while(x = (x >> 1)){
		y++;
	}
	return y;
}
 /*checks whether the given number is a power of 2*/
 
int checkpow2(usint n){
	return((n & (n-1)) == 0);
}

/*checks whether the nth bit is set in the given number i.e. whether the number is in the possibility list of the specific block of sudoku*/
int checkbitset(usint n, int j){
	return((n>>j)&1);
}

/*removes the numbers in the column and row of the block from the possibility list of the block*/
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
						if(checkbitset(a[i][m], y)){
							c = 1;
							a[i][m] = a[i][m] - x;
						}
					}
				}
				for(m = 0; m < 9; m++){
					if(m == i)
						;
					else{
						if(checkbitset(a[m][j], y)){
							c = 1;
							a[m][j] = a[m][j] - x;
						}
					}
				}
			}
		}
	}
}

/*removes the numbers present in the 3x3 matrix of the block from the possibility list*/
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
								if(checkbitset(a[m][k], y)){
								c = 1;
								a[m][k] = a[m][k] - x;
								}
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
void initsum(usint (*a)[10]){
	int i, j;
	for(i = 0; i<10; i++){
		for(j = 0; j<10;j++)
			a[i][j] = 0;
	}
}	
void printsum(usint (*a)[10]){
	int i, j;
	printf("sum is : \n");
	for(i = 0; i<10; i++){
		for(j = 0; j<10;j++)
			printf("%hu\t",(a[i][j]));
			printf("\n");
	}
}

void removerow(usint (*a)[9]){
	int i, j, k, m, n, t, p;
	usint sum[10][10] = {{0}};
	for(i = 0; i < 9; i++){
		initsum(sum);
		for(j = 0; j < 9; j++){
			if(checkpow2(a[i][j]))
				sum[0][Log2(a[i][j])] = 4;
			for(k = 0; k < 10; k++){
				sum[j + 1][k] = (a[i][j] >> k) & 1;
			}
		}
		for(m = 0; m < 10; m++){
			for(n = 1; n < 10; n++){
				sum[0][m] += sum[n][m];
			}
		}
//printf("row: %d", i);
//printsum(sum);
		for(t = 1; t < 10; t++){
			if(sum[0][t] == 1){
				p = 1;
				c = 1;
				while(sum[p][t] != 1)
					p++;
				a[i][p - 1] = pow(2.0, t);
			}
		}
	}
//printf("removerow sudoku looks like:\n");
//printb(a);
}
	
void removecolumn(usint (*a)[9]){
	int i, j, k, m, n, t, p;
	usint sum[10][10] = {{0}};
	for(j = 0; j < 9; j++){
		initsum(sum);
		for(i = 0; i < 9; i++){
			if(checkpow2(a[i][j]))
				sum[0][Log2(a[i][j])] = 4;
			for(k = 0; k < 10; k++){
				sum[i + 1][k] = (a[i][j] >> k) & 1;
			}
		}

		for(m = 0; m < 10; m++){
			for(n = 1; n < 10; n++){
				sum[0][m] += sum[n][m];
			}
		}
//printf("column: %d", j);
//printsum(sum);
		for(t = 1; t < 10; t++){
			if(sum[0][t] == 1){
				p = 1;
				c = 1;
				while(sum[p][t] != 1)
					p++;
				a[p - 1][j] = pow(2.0, t);
			}	
		}
	}
//printf("removecolumn sudoku looks like:\n");
//printb(a);
}
	
void removeblock(usint (*a)[9]){
	int i, j, k, l, m, n, t, p, x, y;
	usint sum[10][10] = {{0}};
	for(i = 0; i < 9; i = i + 3){
		for(j = 0; j < 9; j = j + 3){
			initsum(sum);
			l = 0;
			for(x = i; x < i + 3; x++){
				for(y = j; y < j + 3; y++){			
					l++;
					if(checkpow2(a[x][y]))
						sum[0][Log2(a[x][y])] = 4;	
					for(k = 0; k<10; k++){
						sum[l][k] = (a[x][y] >> k) & 1;
//printf("in loop k = %d\n",k);
					}
//printsum(sum);
				}
			}
//printf("array created\n");
			for(m = 0; m < 10; m++){
				for(n = 1; n < 10; n++){
					sum[0][m] += sum[n][m];
				}
			}
//printf("block: %d",i+j/3);
//printsum(sum);
			for(t = 0; t < 10; t++){
//printf("in loop t = %d\n",t);
				if(sum[0][t] == 1){
					p = 1;
					c = 1;
					while(sum[p][t] != 1)
						p++;
					a[i + (p-1)/3][j + (p-1)%3] = pow(2.0, t);
//printf("in if t = %d\n",t);
//print(a);				
				}

			}
		}
	}
//printf("removeblock sudoku looks like:\n");
//printb(a);
}

/*used in difficult problems. finds the block with only 1 possibility and assigns that block that value*/
void removefinal(usint (*a)[9]){
	removerow(a);
	removecolumn(a);
	removeblock(a);
}




/*converts all the sudoku elements from binary to decimal*/
void makesudoku(usint (*a)[9]){
	int i, j, k;
	for(i = 0 ; i < 9; i++){
		for(j = 0; j < 9; j++){
			k = Log2(a[i][j]);
			a[i][j] = k;	
		}
	}
	return;
}


int main(){
	usint a[9][9];
	int i, j, k = 3;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++)
			scanf("%hu", &a[i][j]);
	}
	print(a);
	init(a);
printf("original:\n");
	printb(a);
	c = 1;
	while(c){
		c = 0;
		removecnr(a);
		removeb(a);
		removerow(a);
		removecnr(a);
		removeb(a);
		removecolumn(a);
		removecnr(a);
		removeb(a);
		removeblock(a);
	}
printf("final:\n");
	printb(a);
	makesudoku(a);
	printf("final sudoku looks like:\n");
	print(a);
	return 0;
}
