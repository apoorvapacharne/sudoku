#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
typedef unsigned long int usint;
static int z;
void print(usint (*a)[9]){
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			printf("%lu\t", a[i][j]);
		}
		printf("\n");	
	}
}

usint randomize(){
	usint n;
	time_t tt;
	srandom(time(&tt));
	
	n = (usint)random()%100;
	return ((n*13)%9 + 1);
}

int checkrow(usint (*a)[9], int i, int j, usint n){
	int c =1, m;
	for(m = 0; m < 9; m++){
		if(a[i][m] == n)
			c = 0;
	}
//printf("c is %d \n",c);
	return c;
}

int checkcol(usint (*a)[9], int i, int j, usint n){
	int c =1, m;
	for(m = 0; m < 9; m++){
		if(a[m][j] == n)
			c = 0;
	}
//printf("c is %d \n",c);
	return c;
}

int checkblock(usint (*a)[9], int i, int j, usint n){
	int c =1, m, l, x, y;
	x = i/3;
	y = j/3;
	for(m = 0; m < 3; m++){
		for(l = 0; l < 3; l++){
			if(a[x*3 +m][y*3 + l] == n)
				c = 0;
		}
	}
//printf("c is %d \n",c);
	return c;
}

usint getnext(usint (*a)[9], int i, int j){
	usint n;
	int k;
	n = randomize();
	k = checkrow(a, i, j, n) & checkcol(a, i, j, n) & checkblock(a, i, j, n);
	if(k == 1)
		return n;
	else 
		return getnext(a, i, j);
}

void generate(usint (*a)[9]){
	int i, j;
	usint n;
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			n = getnext(a, i, j);
			a[i][j] = n;
printf("a[%d][%d] is %lu \n", i, j, n);
		}
	}
}

int main(){
	usint a[9][9] = {{0}};
	generate(a);
	print(a);
	/*usint a[9][9];
	usint n;
	int i, j, k ;
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++)
			scanf("%lu", &a[i][j]);
	}
	printf("checkrow for 2,0 is %d\n", checkrow(a,2,0,9));
	printf("checkcolumn for 2,0 is %d\n", checkcol(a,2,0,9));
	printf("checkblock for 2,0 is %d\n", checkblock(a,2,0,9));*/
/*int i, j ;
usint n;
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			n = getnext(a,i,j);
			printf("n is %lu\n",n);
		}
	}
	*/
	return 0;
}
	






