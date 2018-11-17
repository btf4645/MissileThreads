#include <curses.h>
#include <pthreads.h>
#include <stdlib.h>
#include <stdio.h>

int COLUMNS;
int LINES;

int main(int argc, char * argv[]){
  initscr();
  cbreak();
  noecho();

  LINES = getmaxx(stdscr);
  COLUMNS = getmaxy(stdscr);
  
  WINDOW * win = newwin(nlines, ncols, COLUMNS, LINES);
 
  keypad(stdscr, TRUE);




  endwin();
  
}
