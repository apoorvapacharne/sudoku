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



#define SIZE 9
#define BLOCK_SIZE 3



void print();  
void init();  
int Log2(int x);  
int checkpow2(int n);  
int checkbitset(int n, int j);  
void removecnr();  
void removeb();  
void initsum(int *sum[]);  
void removerow();  
void removecolumn();  
void removeblock();  
void makesudoku();  
int eliminate();  
int isfixed(int count, int m, int n);
int checkmat(int m, int n);
int checkrow(int m, int n);
int checkcolumn(int m, int n);
int check(int m,int n);
void create();
int solve();
void solve_sudoku();
int randomize(int n);  
void generate();
void generate_sudoku(int k);
