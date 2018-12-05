#include <curses.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

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

void draw(int x, int y, char charer){
  pthread_mutex_lock(&thread);
  mvaddch(x, y, charer);
  refresh();
  pthread_mutex_unlock(&thread);
}

void erasing(int x, int y){
  pthread_mutex_lock(&thread);
  mvaddch(x, y, ' ');
  pthread_mutex_unlock(&thread);
}

int main(int argc, char * argv[]){
  FILE *fps;
  int maximum = 0;
  char filename[255];
  char defense_force[81];
  char attack_force[81];
  signed int maximum_missiles;
  int * city_layout = malloc(sizeof(int) * maximum);
  size_t sizes = 0;
  char * buffer = NULL;
  int counts = 0;
  int temp_var = 0;
  char *token;
  char *tokenb;
  int another_temp;
  int * saver;

  
  if(argc < 2){
    printf("No file given\n");
    return EXIT_FAILURE;
  }
  
  fps = fopen(argv[1], "r");
  if(fps == NULL){
    printf("Cannot open a file\n");
    return EXIT_FAILURE;
  }

  while(getline(&buffer, &sizes, fps)  > 0){
    if(buffer[0] == ' ' || buffer[0] == '#'){
      continue;
    }
    else if(counts == 0){
      strcpy(defense_force, buffer);
      counts = counts + 1;
    }
    else if(counts == 1){
      strcpy(attack_force, buffer);
      counts = counts + 1;
    }
    else if(counts == 2){
      maximum_missiles = atoi(buffer);
      counts = counts + 1;
    }
    else if (counts >= 3){
      int temp_size = 1;
      token = strtok(buffer, " ");
      while(token != NULL){
	if(temp_var == maximum){
	  maximum = maximum + 1;
	  city_layout = realloc(city_layout, sizeof(int*) * maximum);	  
	}
	//city_layout = (int *)realloc(city_layout, temp_size * sizeof(int*));
	temp_size = temp_size + 1;
	sscanf(token, "%d", &another_temp);
	city_layout[temp_var] = another_temp;
	temp_var++;
	token = strtok(NULL, " ");
      }
    }
  }
   

  printf("%s\n", defense_force);
  printf("%s\n", attack_force);
  printf("%d\n", maximum_missiles);
  for(size_t i = 0; i < sizeof(city_layout); i++){
    printf("%d\n", city_layout[i]);
  }

  
  
  initscr();
  cbreak();
  noecho();

  keypad(stdscr, TRUE);
  
  LINES = getmaxx(stdscr);
  COLUMNS = getmaxy(stdscr);
  
  WINDOW * win = newwin(LINES, COLUMNS, 0, 0);
  
  //  pthread_t t;
  // const char* m = "thread";
  pthread_mutex_init(&thread, NULL);

  int curx = LINES;
  int cury = COLUMNS;
  int ch = getch();
  int counter;
  int maximum_y;

  //defender goes one row above the highest building
  //if the rest is at 2 from the width, then keep it at ground level
  
  //SET UP CITY LANSCAPE HERE and curser
  for(int i = 0; i <= sizeof(city_layout); i++){
    if(city_layout[i] == 2){
      mvaddch(curx - 2, cury, "_");
    }
    if(city_layout[i] > cury){
      mvaddch(curx, cury, "|");
      current_y = i;
    }
    else{
      current_y = i + 1;
    }
    while(city_layout[i + 1] != 2){
      counter = counter + i;
    }
    draw(2, current_y, '|'); 
  }

  refresh();

  /*
  while(1){
    mvaddch((LINES / 2), (COLUMNS / 2), '####');
    if(ch == '?'){
      return EXIT_FAILURE;
    }
    
    ch = getch();
    switch(ch){
    case KEY_RIGHT:
      erasing(curx, cury);
      curx = curx + 1;
      //mvaddch(cury, curx, '####');
      draw(curx, cury, '####');
    case KEY_LEFT:
      erasing(curx, cury);
      curx = curx - 1;
      //mvaddch(cury, curx, '####');
      draw(curx, cury, '####');
    }
    
    for(int i = 0; i < maximum_missiles; i++){
    //if i at cury is equal 
    if(i < COLUMNS && i != '-'){ 
	usleep(1000);
	erasing(curx, cury);
	cury = cury - 1;
	mvaddch(cury, curx, '|');
	refresh();
      }
      else if(i >= COLUMNS || i == '-'){
	erasing(curx, cury);
	mvaddch(cury, curx, '?');
	mvaddch(cury - 1, curx, '*');
	refresh();
      }
      else{
	erasing(curx, cury);
	mvaddch(cury, curx, '?');
	refresh();
      }
    }

  }


  char errors;

  //error messages for the window
  mvwaddch(win, 0, 1, errors);


  free(city_layout);

  //for when the program is over
  endwin();
  */  

}

