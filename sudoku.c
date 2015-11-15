/*
Copyright (c) 2015 Apoorva Pacharne

 This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

 This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/



#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"sudoku.h"
#define SIZE 9
#define BLOCK_SIZE 3
static int c;
int **a;
int **r;
int row, col, icount;
int mode;

/*this function prints the sudoku*/
void print(){
	int i, j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++){
			printf("%d\t", a[i][j]);
		}
		printf("\n");	
	}
}

/* all the elements(except the numbers of the puzzle question) of the sudoku are initialised to pow(2.0, (SIZE + 1)) - 2 i.e. 1111111110 in binary. this is the possibility list of each block*/
void init(){
	int x;
	int i, j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j< SIZE; j++){
			x = a[i][j];
			if(x == 0){
				a[i][j] = pow(2.0, (SIZE + 1)) - 2;
			}			
			else{
				a[i][j] = pow (2.0, x);
			}
		}

	}
}

/*finds the log to the base 2 of the number given*/
int Log2(int x){
	int y = 0;
	while((x = (x >> 1))){
		y++;
	}
	return y;
}
 /*checks whether the given number is a power of 2*/
 
int checkpow2(int n){
	return((n & (n-1)) == 0);
}

/*checks whether the nth bit is set in the given number i.e. whether the number is in the possibility list of the specific block of sudoku*/
int checkbitset(int n, int j){
	return((n>>j)&1);
}

/*removes the numbers in the column and row of the block from the possibility list of the block*/
void removecnr(){
	int i, j, m;
	int x, y;
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
void removeb(){
	int i, j, m, n, l, k;
	int x, y;
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
/*initiates array required for further calculations*/
void initsum(int *sum[]){
	int i, j;
	for(i = 0; i<(SIZE + 1); i++){
		for(j = 0; j<(SIZE + 1);j++)
			sum[i][j] = 0;
	}
}	

/*removes numbers in the possibility list that occur only once in the row*/
void removerow(){
	int i, j, k, m, n, t, p;
	int **sum;
	sum = calloc(30,sizeof(int));
	for(i = 0; i < 30; i++)
		sum[i] = calloc(30,sizeof(int));
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
}

/*removes numbers in the possibility list that occur only once in the column*/	
void removecolumn(){
	int i, j, k, m, n, t, p;
	int **sum;
	sum = calloc(30,sizeof(int));
	for(i = 0; i < 30; i++)
		sum[i] = calloc(30,sizeof(int));
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

}

/*removes numbers in the possibility list that occur only once in the block*/
void removeblock(){
	int i, j, k, l, m, n, t, p, x, y;
	int **sum;
	sum = calloc(30,sizeof(int));
	for(i = 0; i < 30; i++)
		sum[i] = calloc(30,sizeof(int));
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
					}
				}
			}
			for(m = 0; m < (SIZE + 1); m++){
				for(n = 1; n < (SIZE + 1); n++){
					sum[0][m] += sum[n][m];
				}
			}
			for(t = 0; t < (SIZE + 1); t++){
				if(sum[0][t] == 1){
					p = 1;
					c = 1;
					while(sum[p][t] != 1)
						p++;
					a[i + (p-1)/BLOCK_SIZE][j + (p-1)%BLOCK_SIZE] = pow(2.0, t);
				}

			}
		}
	}
}


/*converts all the sudoku elements from binary to decimal*/
void makesudoku(){
	int i, j;
	for(i = 0 ; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(checkpow2(a[i][j]))
				a[i][j] = Log2(a[i][j]);
			else
				a[i][j] = 0;
				
		}
	}
	return;
}

/*combines all the functions to solve sudoku by elimination*/
int eliminate(){
	init();
	int i, j, k = 1;
	c = 1;
	while(c){
		c = 0;
		removecnr();
		removeb();
		removerow();
		removecnr();
		removeb();
		removecolumn();
		removecnr();
		removeb();
		removeblock();
	}
	makesudoku();
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(!checkpow2(a[i][j])){
				k = 0;
				break;
			}
		}
	}
	return k;
}



/*checks whether given number isgiven in the puzzle or a manual entry*/
isfixed(int count, int m, int n){
	int i;
	for(i = 0; i < count; i++)
		if(m == r[i][0] && n == r[i][1])
		return(1);	
	return(0);
}

/*checks validity in block*/
int checkmat(int m, int n){
	int x,y;
	for(x=(m/BLOCK_SIZE)*BLOCK_SIZE;x<((m/BLOCK_SIZE)*BLOCK_SIZE+BLOCK_SIZE);x++)
		for(y=(n/BLOCK_SIZE)*BLOCK_SIZE;y<((n/BLOCK_SIZE)*BLOCK_SIZE+BLOCK_SIZE);y++){
			if(m!=x && n!=y)
				if(a[m][n]==a[x][y])
					return(0);
		}
	return(1);
}

/*checks validity in row*/
int checkrow(int m, int n) {
	int x;
	for(x=0;x<SIZE;x++){
		if(x!=n)
			if(a[m][n]==a[m][x])
			return(0);
	}
	return(1);
}

/*checks validity in column*/
int checkcolumn(int m, int n){
	int x;
	for(x=0;x<SIZE;x++){
		if(x!=m)
			if(a[m][n]==a[x][n])
			return(0);
	}
	return(1);
}

/*FUNCTION TO CHECK CONSISTENCY OF AN ELEMENT*/

int check(int m,int n){
	int c;
	c=checkmat(m,n) & checkrow(m,n) & checkcolumn(m,n);
	return(c);
}

/*FUNCTION TO SOLVE SUDOKU*/

int solve(){
	int i,m=0,n=0,number,c,fixed;
	
	mainallot:

	for(m=row;m<SIZE;m++){
		n=col;
		while(n<SIZE){
			number=1;
			fixed=isfixed(icount,m,n);
			if(fixed==0){
				allot:
				a[m][n]=number;
				c=check(m,n);
				if(c==0){
					number++;
					if(number>SIZE){
						a[m][n]=0;
						if(n==0){
							row=m-1;
							col=(SIZE - 1);
							if(row<0)
								return(0);
							goto backadjust;
						}
						else{
							row=m;
							col=n-1;
							if(row<0 || col<0)
								return(0);
							goto backadjust;
						}
					}
				goto allot;
				}
			}
		n++;
		if(n>(SIZE - 1))
		col=0;
		}
	}
	return(1);
	
	backadjust:

	fixed=isfixed(icount,row,col);
	if(fixed==0){
		number=a[row][col];
		allot1:
		number++;
		a[row][col]=number;
		if(number>SIZE){
			a[row][col]=0;
			if(col==0){
				row--; col=(SIZE - 1);
				if(row<0)
					return(0);
				goto backadjust;
			}
			else{	
				col--;
				if(col<0)
					return(0);
				goto backadjust;
			}
		}
		c=check(row,col);
		if(c==0)
		goto allot1;
		else{
			if(col==(SIZE - 1)){
				row++;
				if(row>(SIZE - 1))
					return(0);
				col=0;
				goto mainallot;
			}
			else{
				col++;
				if(col>(SIZE - 1))
					return(0);
				goto mainallot;
			}
		}
	}
	else{
		if(col==0){
			row--; col=(SIZE - 1);
			if(row<0)
				return(0);
			goto backadjust;
		}
		else{
			col--;
			if(col<0)
				return(0);
			goto backadjust;
		}
	}
	
}

int randomize(int n){
	time_t tt;
	srandom(time(&tt));
	return (random() % n);
}
/*FUNCTION TO GENERATE SUDOKU*/

void generate(){
	int b[20][20],c,i,m,n,count=1,fixed,error;
	char choice;
	generate:
	for(m=0;m<SIZE;m++)
		for(n=0;n<SIZE;n++)
			a[m][n]=0;
	for(i=0;i<100;i++){
		r[i][0]=20; r[i][1]=20;
	}
	switch(mode){
		case '1': mode=34+randomize(4);
			  break;
		case '2': mode=29+randomize(4);
			  break;
		case '3': mode=27+randomize(2);
			  break;
		case '4': mode=25+randomize(2);
			  break;
		default : getchar();
			  goto generate;
	}
	a[0][0]=1+randomize((SIZE - 1));
	r[0][0]=0;
	r[0][1]=0;
	for(count=1;count<(SIZE + 1);count++){
		choose:
		m=randomize(SIZE);
		n=randomize(SIZE);
		fixed=isfixed(icount,m,n);
		if(fixed==1)
			goto choose;
		r[count][0]=m;
		r[count][1]=n;
		allot:
		a[m][n]=1+randomize(SIZE);
		if(a[m][n]==0)
			goto allot;
		c=check(m,n);
		if(c==0)
		goto allot;
	}
	icount=count;
	error=solve();
	for(m=0;m<SIZE;m++)
		for(n=0;n<SIZE;n++)
			b[m][n]=a[m][n];
	row=0;
	col=0;
	for(i=0;i<100;i++){
		r[i][0]=20;
		r[i][1]=20;
	}
	if(error==0)
		goto generate;
	for(count=0;count<mode/2;count++){
		choose2:
		m=randomize(SIZE);
		n=randomize(5);
		fixed=isfixed(count,m,n);
		if(fixed==1)
			goto choose2;
		r[count][0]=m;
		r[count][1]=n;
		r[count+mode/2][0]=(SIZE - 1)-m;
		r[count+mode/2][1]=(SIZE - 1)-n;
	}
	for(m=0;m<SIZE;m++)
		for(n=0;n<SIZE;n++){
			fixed=isfixed(mode,m,n);
			if(fixed!=1)
				a[m][n]=0;
		}
	icount=mode;
	
	getchar();
	return;
}
/*allocates memory*/
void create(){
	int i, j;
	a = calloc(20,sizeof(int));
	for(i = 0; i < 20; i++)
		a[i] = calloc(20,sizeof(int));
	r = calloc(100,sizeof(int));
	for(i = 0; i < 100; i++)
		r[i] = calloc(2,sizeof(int));
	return;
}
	
/*solves the sudoku read from the text file and saves the solution in another text file*/
void solve_sudoku(){
	int i, j;
	create();
	FILE *fp;
	fp = fopen("gen.txt", "r");
	if(!fp)
		printf("open failed");
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++)
			fscanf(fp,"%d", &a[i][j]);
	}
	fclose(fp);
	eliminate();
	solve();
	fp = fopen("solve.txt", "w");
	int p, q;
	for(p = 0; p < 9; p++){
		for(q = 0; q < 9; q++){
			fprintf(fp,"%d\t",a[p][q]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	free(a);
	free(r);
	return;
}

/*generates sudoku and saves it in a text file*/
void generate_sudoku(int k){
	mode = k;
	create();
	generate();
	FILE *fp;
	fp = fopen("gen.txt", "w");
	int p, q;
	for(p = 0; p < 9; p++){
		for(q = 0; q < 9; q++){
			fprintf(fp,"%d\t",a[p][q]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
	free(a);
	free(r);
	return;
}


