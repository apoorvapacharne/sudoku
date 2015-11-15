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



#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <menu.h>
#include<math.h>
#include"sudoku.h"

#define WIDTH  6
#define HEIGHT 4
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
int **a, **b;
int startx, starty, deltax, deltay;
int mode;
int k, n;

char *choices[] = {	
			"Very Easy",
			"Easy",
			"Medium",
			"Difficult",
		};

void board(WINDOW *win, int starty, int startx, int lines, int cols, int tile_width, int tile_height);
void sudoku_board(int **a, int n);
void print_sudoku();
int check_row(int n, int i, int j);	
int check_col(int n, int i, int j);
int checkblock(int n, int i, int j);
int check_pos(int ch, int y, int x);
void save_value(int ch, int y, int x);
void move_cursor(int c);

enum { LEFT, RIGHT, UP, DOWN };

int main(){
	char str[20];
	ITEM  **my_items;
	int c;				
	MENU  *my_menu;
        int n, i;
	ITEM *cur_item;
	n = ARRAY_SIZE(choices);
	/* Initialize curses */	
	initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	
	mvprintw(LINES - 4, 0, "Select the Difficulty level");
	mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage (F1 to Exit)");
	
	my_items = (ITEM **)calloc(n + 1, sizeof(ITEM *));
        for(i = 0; i < n; ++i)
                my_items[i] = new_item(choices[i], NULL);
	my_items[n] = (ITEM *)NULL;

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set fore ground and back ground of the menu */
	set_menu_fore(my_menu, COLOR_PAIR(1) | A_REVERSE);
	set_menu_back(my_menu, COLOR_PAIR(2));

	post_menu(my_menu);
	refresh();
	k = 1;
	while((c = getch()) != KEY_F(1) && k){
	       switch(c){
			case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10: /* Enter */
				move(20, 0);
				clrtoeol();
				mvprintw(LINES - 4, 0, "Selected level is : %s      ", 
				item_name(current_item(my_menu)));
				strcpy(str, item_name(current_item(my_menu)));
				if(strcmp(str, "Very Easy") == 0)
					mode = 1;
				else if(strcmp(str, "Easy") == 0)
					mode = 2;
				else if(strcmp(str, "Medium") == 0)
					mode = 3;
				else if(strcmp(str, "Difficult") == 0)
					mode = 4;
						
				pos_menu_cursor(my_menu);
				k = 0;
				break;
		}
	}	
	unpost_menu(my_menu);
	for(i = 0; i < n; ++i)
		free_item(my_items[i]);
	free_menu(my_menu);
	clear();
	refresh();
	keypad(stdscr, FALSE);
	n = 9;
	a = calloc(n,sizeof(int));
	for(i = 0; i < n; i++)
		a[i] = calloc(n,sizeof(int));
	b = calloc(n,sizeof(int));
	for(i = 0; i < n; i++)
		b[i] = calloc(n,sizeof(int));	
	int j, ch, y, x;
	FILE *fp;
	cbreak();
	noecho();
	getch();
	fp = fopen("gen.txt", "r");
	for(i = 0; i < 9; i++){
		for(j = 0; j< 9; j++){
			fscanf(fp,"%d", &a[i][j]);
			b[i][j] = a[i][j];
		}
	}
				
	sudoku_board(a, n);
	//attron(A_BOLD|COLOR_PAIR(2));
	print_sudoku();
	//attroff(A_BOLD|COLOR_PAIR(2));
	refresh();
	getch();
	keypad(stdscr, TRUE);
	
	starty = (LINES - n * HEIGHT) / 2;
	startx = (COLS  - n * WIDTH) / 2;
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;
	x = startx + deltax;
	y = starty + deltay;
	move(y, x);
	refresh();
	int k = 1;
	while((ch = getch()) != KEY_F(1)){	
		switch(ch){	
			
			case KEY_ENTER:
				move(y,x);
				print_sudoku();
				break;
			case KEY_LEFT:
			case 'h':
					move_cursor(RIGHT);
				break;
				
			case 'j':
			case KEY_RIGHT:
					move_cursor(LEFT);
				break;
				
			case KEY_UP:
			case 'k':	
				move_cursor(DOWN);
				break;
				
			case KEY_DOWN:
			case 'l':
				move_cursor(UP);
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				getyx(stdscr, y, x);
				i = (x - startx) % WIDTH;
				j = (y - starty) % HEIGHT;
				if(check_pos(ch, i, j))
					save_value(ch, i, j);
				else{
					mvprintw(LINES - 1, 0, "Invalid!");
					sleep(15);
					mvprintw(LINES - 1, 0, "        ");
				}
				break;
				
			case 's':
			case 'S':
				solve_sudoku();
				fp = fopen("solve.txt", "r");
				for(i = 0; i < 9; i++){
					for(j = 0; j< 9; j++)
						fscanf(fp,"%d", &a[i][j]);
				}
				fclose(fp);
				sudoku_board(a, n);
				break;
				
			case 'g':
			case 'G':
				generate_sudoku(mode);	
				fp = fopen("gen.txt", "r");
				for(i = 0; i < 9; i++){
					for(j = 0; j< 9; j++){
						fscanf(fp,"%d", &a[i][j]);
						b[i][j] = a[i][j];
					}
				}
				mvprintw(LINES - 1, 0, "Wait!");
				sleep(1500);
				mvprintw(LINES - 1, 0, "        ");
				fclose(fp);
				break;
			case 'r':
			case 'R':
				for(i = 0; i < n; i++)
					for( j = 0; j < n; j++)
						a[i][j] = b[i][j];
				break;
			default:
				break;
		}
		sudoku_board(a, n);	
		refresh();	
	}
	getch();
	endwin();
	return 0;	
}



void board(WINDOW *win, int starty, int startx, int lines, int cols, int tile_width, int tile_height){
	int endy, endx, i, j;
	
	endy = starty + lines * tile_height;
	endx = startx + cols  * tile_width;
	
	for(j = starty; j <= endy; j += tile_height)
		for(i = startx; i <= endx; ++i){
			if(j%3 == 0){
				attron(A_BOLD | COLOR_PAIR(1));
				mvwaddch(win, j, i, ACS_HLINE);
				attroff(A_BOLD| COLOR_PAIR(1));
			}
			else
				mvwaddch(win, j, i, ACS_HLINE);
		}
	for(i = startx; i <= endx; i += tile_width)
		for(j = starty; j <= endy; ++j){
			if(i%3 == 0){
				attron(A_BOLD| COLOR_PAIR(1));
				mvwaddch(win, j, i, ACS_VLINE);
				attroff(A_BOLD| COLOR_PAIR(1));
			}
			else
				mvwaddch(win, j, i, ACS_VLINE);
		}
	attron(A_BOLD| COLOR_PAIR(1));			
	mvwaddch(win, starty, startx, ACS_ULCORNER);
	mvwaddch(win, endy, startx, ACS_LLCORNER);
	mvwaddch(win, starty, endx, ACS_URCORNER);
	mvwaddch(win, 	endy, endx, ACS_LRCORNER);
	attroff(A_BOLD| COLOR_PAIR(1));
	for(j = starty + tile_height; j <= endy - tile_height; j += tile_height){	
		if(j%3 == 0){
			attron(A_BOLD| COLOR_PAIR(1));
			mvwaddch(win, j, startx, ACS_LTEE);
			mvwaddch(win, j, endx, ACS_RTEE);
			attroff(A_BOLD| COLOR_PAIR(1));
		}
		else{
			mvwaddch(win, j, startx, ACS_LTEE);
			mvwaddch(win, j, endx, ACS_RTEE);
		}
		for(i = startx + tile_width; i <= endx - tile_width; i += tile_width){
			if(i%3 == 0){
				attron(A_BOLD| COLOR_PAIR(1));
				mvwaddch(win, j, i, ACS_PLUS);
				attroff(A_BOLD| COLOR_PAIR(1));
			}
			else
				mvwaddch(win, j, i, ACS_PLUS);
		}	
	}
	for(i = startx + tile_width; i <= endx - tile_width; i += tile_width){
		if(i%3 == 0){
			attron(A_BOLD| COLOR_PAIR(1));
			mvwaddch(win, starty, i, ACS_TTEE);
			mvwaddch(win, endy, i, ACS_BTEE);
			attroff(A_BOLD| COLOR_PAIR(1));
		}
		else{
			mvwaddch(win, starty, i, ACS_TTEE);
			mvwaddch(win, endy, i, ACS_BTEE);
		}
	}
	wrefresh(win);
	//getch();
	return;
}

void sudoku_board(int **a, int n)
{	int i,j, deltax, deltay;

	starty = (LINES - n * HEIGHT) / 2;
	startx = (COLS  - n * WIDTH) / 2;
	clear();
	mvprintw(starty, 0, "F1: Exit");
	mvprintw(starty + 5, 0, "Use arrow keys to navigate");
	mvprintw(starty + 1, 0, "g/G: Generate puzzle");
	mvprintw(starty + 2, 0, "s/S: Solve the puzzle");
	mvprintw(starty + 3, 0, "r/R: Reset puzzle");
	mvprintw(starty + 4, 0, "0: Delete an entry");
	refresh();
	board(stdscr, starty, startx, n, n, WIDTH, HEIGHT);
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;
	refresh();
	getch();
	return;
}
	
void print_sudoku(){
	int i,j;
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;
	for(j = 0; j < n; ++j)
		for(i = 0;i < n; ++i)
			if(a[i][j])
				mvprintw(starty + j*HEIGHT + deltay, startx + i*WIDTH + deltax,
				"%-2d", a[i][j]);
			else
				mvprintw(starty + j*HEIGHT + deltay, startx + i*WIDTH + deltax," ");
	refresh();
	getch();
	return;
}

int check_row(int n, int i, int j){
	int c =1, m;
	for(m = 0; m < n; m++){
		if(a[i][m] == n)
			c = 0;
	}
	return c;
}

int check_col(int n, int i, int j){
	int c =1, m;
	for(m = 0; m < n; m++){
		if(a[m][j] == n)
			c = 0;
	}
	return c;
}

int checkblock(int n, int i, int j){
	int c =1, m, l, x, y;
	x = i/3;
	y = j/3;
	for(m = 0; m < 3; m++){
		for(l = 0; l < 3; l++){
			if(a[x*3 +m][y*3 + l] == n)
				c = 0;
		}
	}
	return c;
}

int check_pos(int n, int i, int j){
	return (check_row( i, j, n) & check_col( i, j, n) & checkblock( i, j, n));
}


void save_value(int ch, int i, int j){
	a[i][j] = ch;
	return;
}
	
void move_cursor(int c){
	starty = (LINES - n * HEIGHT) / 2;
	startx = (COLS  - n * WIDTH) / 2;
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;
	int endx, endy, x, y, i, j;
	getyx(stdscr, y, x);
	endy = starty + n*HEIGHT;
	endx = startx + n*WIDTH;
	i = (x - startx) % WIDTH;
	j = (y - starty) % HEIGHT;
	if(c == DOWN){
		label1:
		if(j == 0){
			j = n-1;
			y = endy - deltay;
		}
		else{
			j--;
			y = y - HEIGHT;
		}
		if(!b[i][j])
			move(y,x);
		else
			goto label1;
	}
	else if(c == UP){
		label2:
		if(j == (n-1)){
			j = 0;
			y = starty + deltay;
		}
		else{
			j++;
			y = y + HEIGHT;
		}
		if(!b[i][j])
			move(y,x);
		else
			goto label2;
	}
	else if(c == LEFT){
		label3:
		if(i == (n-1)){
			i = 0;
			x = startx + deltax;
		}
		else{
			i++;
			x = x + WIDTH;
		}
		if(!b[i][j])
			move(y,x);
		else
			goto label3;
	}
	else if(c == RIGHT){
		label4:
		if(i == 0){
			i = n-1;
			x = endx - deltax;
		}
		else{
			i--;
			x = x - WIDTH;
		}
		if(!b[i][j])
			move(y,x);
		else
			goto label4;
	}
	return;
}
	
		
		

