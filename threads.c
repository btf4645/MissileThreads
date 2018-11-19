#include <curses.h>
#include <pthreads.h>
#include <stdlib.h>
#include <stdio.h>

int COLUMNS;
int LINES;
pthread_mutex_t thread;
char Char;

#define delay 1000


/*
void game_loop(int COLUMNS, int LINES, int ch){
  if(ch == 'q'){
    return EXIT_FAILURE;
  }
  mvaddch(COLUMNS, LINES, ch);


}
*/

void draw(int x, int y){
  pthread_mutex_lock(&thread);
  mvaddch(x, y, Char);
  refresh();
  pthread_mutex_lock(&thread);
}

void erase(int x, int y){
  pthread_mutex_lock(&thread);
  mvaddch(x, y, ' ');
  pthread_mutex_lock(&thread);
}

int main(int argc, char * argv[]){
  initscr();
  cbreak();
  noecho();

  
  FILE *fp;
  char filename[255];
  char defense_force[81];
  char attack_force[81];
  int maximum_missiles;
  int city_layout[1000000];
  
  fp = fopen(filename, "r");
  if(fp == NULL){
    printf("Cannot open a file\n");
    return EXIT_FAILURE;
  }
  else{
    while(fscanf(fp, "%c %c %d %d", defense_force, attack_force, maximum_missles, city_layout) != EOF){
      if(defense_force == NULL){
	//fprintf(stderr, "There was an error\n");
	mvaddch(0, 2, 'this is an incorrect configuration');
	return EXIT_FAILURE;
      }
      else if(attack_force == NULL){
	//fprintf(stderr, "There was an error\n");
	mvaddch(0, 2, 'this is an incorrect configuration');
	return EXIT_FAILURE;
      }
      else if(maximum_missiles == NULL){
	//fprintf(stderr, "There was an error\n");
	mvaddch(0, 2, 'this is an incorrect configuration');
        return EXIT_FAILURE;
      }
      else if(city_layout == NULL){
	//fprintf(stderr, "There was an error\n");
	mvaddch(0, 2, 'this is an incorrect configuration');
	return EXIT_FAILURE;
      }
    }
  }


  int ch;
  int st = getmaxx(stdscr);

  keypad(stdscr, TRUE);

  LINES = getmaxx(stdscr);
  COLUMNS = getmaxy(stdscr);

  WINDOW * win = newwin(nlines, ncols, COLUMNS, LINES);

  pthread_t t;
  const char* m = "thread";
  pthread_mutex_init(&thread, NULL);

  int curx = LINES;
  int cury = COLUMNS;

  int ch = getch();
  
  while(1){
    if(ch == '?'){
      return EXIT_FAILURE;
    }

    ch = getch();
    switch(ch){
    case KEY_RIGHT:
      erase(curx, cury);
      curx = curx + 1;
      draw_char(curx, cury);
    case KEY_LEFT:
      erase(curx, cury);
      curx = curx - 1;
      draw_char(curx, cury);
    }
    
    for(int i = 0; i < maximum_missiles; i++){
      if(i < COLUMNS && i != '-'){ 
	usleep(1000);
	erase(curx, cury);
	cury = cury - 1;
	mvaddch(cury, curx, '|');
	refresh();
      }
      else if(i >= COLUMNS || i == '-'){
	erase(curx, cury);
	mvaddch(cury, curx, '?');
	mvaddch(cury - 1, curx, '*');
	refresh();
      }
      else{
	erase(curx, cury);
	mvaddch(cury, curx, '?');
	refresh();
      }
    }



      
    clear();
    int random = rand();
    mvprintw(0,random, "|");
    refresh();
    usleep(delay);
    COLUMNS++;


  }


  char errors;

  //error messages for the window
  mvwaddch(win, 0, 1, errors);



  //for when the program is over
  endwin();

}
