#include <curses.h>
#include <pthreads.h>
#include <stdlib.h>
#include <stdio.h>

int COLUMNS;
int LINES;

int main(int argc, char * argv[]){
  int ch;
  int st = getmaxx(stdscr);
  nodelay(stdscr, TRUE);
  for(int i = 0; i <= st; i++){
    if((ch = getch()) == ERR){
      return EXIT_FAILURE;
    }
    else{
      keypad(stdscr, TRUE);

    }
  }


  initscr();
  cbreak();
  noecho();

  LINES = getmaxx(stdscr);
  COLUMNS = getmaxy(stdscr);

  WINDOW * win = newwin(nlines, ncols, COLUMNS, LINES);

  keypad(stdscr, TRUE);




  endwin();

}
