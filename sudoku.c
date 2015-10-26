#include<stdio.h>
#include<stdlib.h>
#include<math.h>

static int c;
int SIZE;
int BLOCK_SIZE;

typedef unsigned short int usint;

int bin(int num){
	if (num == 0)
		return 0;
	else
		return (num % 2) + 10 * bin(num / 2);
}
/*this function prints the sudoku*/
void print(usint (*a)[SIZE]){
	int i, j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++){
			printf("%hu\t", a[i][j]);
		}
		printf("\n");	
	}
}
void printb(usint (*a)[SIZE]){
	int b[SIZE][SIZE];
	int i, j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++){
			b[i][j] = (int)a[i][j];
		}
		
	}
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++){
			printf("%10d ",bin(b[i][j]));
		}
		printf("\n");	
	}
}
/* all the elements(except the numbers of the puzzle question) of the sudoku are initialised to 1022 i.e. 1111111110 in binary. this is the possibility list of each block*/
void init(usint (*a)[SIZE]){
	usint x;
	int i, j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++){
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
void removecnr(usint (*a)[SIZE]){
	int i, j, m;
	usint x, y;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			x = a[i][j];
			if(checkpow2(x)){
				y = Log2(x);
				for(m = 0; m < SIZE; m++){
					if(m == j)
						;
					else{
						if(checkbitset(a[i][m], y)){
							c = 1;
							a[i][m] = a[i][m] - x;
						}
					}
				}
				for(m = 0; m < SIZE; m++){
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

/*removes the numbers present in the BLOCK_SIZExBLOCK_SIZE matrix of the block from the possibility list*/
void removeb(usint (*a)[SIZE]){
	int i, j, m, n, l, k;
	usint x, y;
	for(i = 0; i < SIZE; i = i + BLOCK_SIZE){
		for(j = 0; j < SIZE; j = j + BLOCK_SIZE){
			for(m = i; m < i + BLOCK_SIZE; m++){
				for(n = j; n < j + BLOCK_SIZE; n++){			
			
					x = a[m][n];
					if(checkpow2(x)){
						y = Log2(x);
						for(k = 0; k < BLOCK_SIZE; k++){
							if(k == n)
								;
							else{
								if(checkbitset(a[m][k], y)){
								c = 1;
								a[m][k] = a[m][k] - x;
								}
							}
						}
						for(l = 0; l < BLOCK_SIZE; l++){
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
void initsum(usint (*a)[(SIZE + 1)]){
	int i, j;
	for(i = 0; i<(SIZE + 1); i++){
		for(j = 0; j<(SIZE + 1);j++)
			a[i][j] = 0;
	}
}	
void printsum(usint (*a)[(SIZE + 1)]){
	int i, j;
	printf("sum is : \n");
	for(i = 0; i<(SIZE + 1); i++){
		for(j = 0; j<(SIZE + 1);j++)
			printf("%hu\t",(a[i][j]));
			printf("\n");
	}
}

void removerow(usint (*a)[SIZE]){
	int i, j, k, m, n, t, p;
	usint sum[(SIZE + 1)][(SIZE + 1)] = {{0}};
	for(i = 0; i < SIZE; i++){
		initsum(sum);
		for(j = 0; j < SIZE; j++){
			if(checkpow2(a[i][j]))
				sum[0][Log2(a[i][j])] = 4;
			for(k = 0; k < (SIZE + 1); k++){
				sum[j + 1][k] = (a[i][j] >> k) & 1;
			}
		}
		for(m = 0; m < (SIZE + 1); m++){
			for(n = 1; n < (SIZE + 1); n++){
				sum[0][m] += sum[n][m];
			}
		}
//printf("row: %d", i);
//printsum(sum);
		for(t = 1; t < (SIZE + 1); t++){
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
	
void removecolumn(usint (*a)[SIZE]){
	int i, j, k, m, n, t, p;
	usint sum[(SIZE + 1)][(SIZE + 1)] = {{0}};
	for(j = 0; j < SIZE; j++){
		initsum(sum);
		for(i = 0; i < SIZE; i++){
			if(checkpow2(a[i][j]))
				sum[0][Log2(a[i][j])] = 4;
			for(k = 0; k < (SIZE + 1); k++){
				sum[i + 1][k] = (a[i][j] >> k) & 1;
			}
		}

		for(m = 0; m < (SIZE + 1); m++){
			for(n = 1; n < (SIZE + 1); n++){
				sum[0][m] += sum[n][m];
			}
		}
//printf("column: %d", j);
//printsum(sum);
		for(t = 1; t < (SIZE + 1); t++){
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
	
void removeblock(usint (*a)[SIZE]){
	int i, j, k, l, m, n, t, p, x, y;
	usint sum[(SIZE + 1)][(SIZE + 1)] = {{0}};
	for(i = 0; i < SIZE; i = i + BLOCK_SIZE){
		for(j = 0; j < SIZE; j = j + BLOCK_SIZE){
			initsum(sum);
			l = 0;
			for(x = i; x < i + BLOCK_SIZE; x++){
				for(y = j; y < j + BLOCK_SIZE; y++){			
					l++;
					if(checkpow2(a[x][y]))
						sum[0][Log2(a[x][y])] = 4;	
					for(k = 0; k<(SIZE + 1); k++){
						sum[l][k] = (a[x][y] >> k) & 1;
//printf("in loop k = %d\n",k);
					}
//printsum(sum);
				}
			}
//printf("array created\n");
			for(m = 0; m < (SIZE + 1); m++){
				for(n = 1; n < (SIZE + 1); n++){
					sum[0][m] += sum[n][m];
				}
			}
//printf("block: %d",i+j/BLOCK_SIZE);
//printsum(sum);
			for(t = 0; t < (SIZE + 1); t++){
//printf("in loop t = %d\n",t);
				if(sum[0][t] == 1){
					p = 1;
					c = 1;
					while(sum[p][t] != 1)
						p++;
					a[i + (p-1)/BLOCK_SIZE][j + (p-1)%BLOCK_SIZE] = pow(2.0, t);
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
void removefinal(usint (*a)[SIZE]){
	removerow(a);
	removecolumn(a);
	removeblock(a);
}




/*converts all the sudoku elements from binary to decimal*/
void makesudoku(usint (*a)[SIZE]){
	int i, j, k;
	for(i = 0 ; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			k = Log2(a[i][j]);
			a[i][j] = k;	
		}
	}
	return;
}

int getsize(){
	int k;
	printf("Choose size of the grid \n1. 4x4\n2. 9x9\n3. 16x16\n4. 25x25\n ");
	scanf("%d",&k);
	if((k > 0) && (k<5)) 
		return k;
	else
		return getsize();
}

int main(){
	usint a[30][30];
	int i, j;
	SIZE = getsize();
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++)
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
