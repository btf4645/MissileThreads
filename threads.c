#define _DEFAULT_SOURCE
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
int c;
int l;
pthread_mutex_t thread;
//char Char;


/*
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
*/

int main(int argc, char * argv[]){
  FILE *fps;
  int maximum = 1;
  char filename[255];
  char defense_force[81];
  char attack_force[81];
  signed int maximum_missiles;
  //int * city_layout = malloc(sizeof(int) * maximum);
  int city_layout[300];
  int sizes = 300;
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
      token = strtok(buffer, " \t\n");
      while(token != NULL){
	//if(temp_var == maximum){
	//maximum = maximum + 1;
	  //city_layout = realloc(city_layout, sizeof(int) * maximum);	  
	  //}
	sscanf(token, "%d", &another_temp);
	city_layout[temp_var] = another_temp;
	temp_var++;
	token = strtok(NULL, " \t\n");
      }
    }
  }
  
  
  initscr();
  cbreak();
  noecho();

  keypad(stdscr, TRUE);
  
  getmaxyx(stdscr, LINES, COLUMNS);
  getmaxyx(stdscr, l, c);
  WINDOW * win = newwin(LINES, COLUMNS, 0, 0);
  
  pthread_t t;
  const char* m = "thread";
  pthread_mutex_init(&thread, NULL);

  /*
  for(int i = 0; i <= sizeof(city_layout); i++){
    printf("%d", city_layout[i]);
  }
  */
  
  int curx = LINES;
  int cury = COLUMNS;
  int ch = getch();
  int counter;
  int maximum_y;
  int cv;
  
  /*
  if(temp_var < LINES){
    int a  = temp_var - 1;
    while(a < LINES){
      if(temp_var == maximum){
	maximum = maximum + 1;
	city_layout = realloc(city_layout, sizeof(int) * maximum);
      }
      city_layout[a] = 2;
      temp_var++;
      a++;
    }
  }
  */

  cv = 2;

  // int sizing = sizeof(city_layout);
  
  for(int i = 0; i < temp_var; i++){
    if(i < LINES){
      if(city_layout[i] < 0){
	//while(i < LINES){
	  mvwaddch(win, LINES - cv, i, '_');
	  //}
      }

      /*
      if (city_layout[i] == '/0' && i < LINES){
	int add = city_layout[i] - cv;
	int z = 0;
	while(i < LINES){
	  if (cv != 2){
	    while (z < add){
	      mvwaddch(win, LINES - cv - z, i, '|');
	      z = z + 1;
	    }
	  }
	  else{
	    mvwaddch(win, LINES - cv, i, '_');
	  }
	}
      }
      */
      if(city_layout[i] == cv){
	mvwaddch(win, LINES - cv, i, '_');
      }
      else if(city_layout[i] > cv){
	int additioning = city_layout[i] - cv;
	int c = 0;
	while(c < additioning){
	  mvwaddch(win, LINES - cv - c, i, '|');
	  c = c + 1;
	}
	int go_two = cv - 2;
	int p = 0;
	while(p < go_two){
	  mvwaddch(win, LINES - cv + p + 1, i, '|');
	  p = p + 1;
	}
      }
      else if(city_layout[i] < COLUMNS){
	if(city_layout[i] != 2){
	  int go_two = cv - 2;
	  int p = 0;
	  while(p < go_two){
	    mvwaddch(win, LINES - cv + p + 1, i , '|');
	    p = p + 1;
	  }
	}
	else{
	  int s = cv - city_layout[i];
	  int p = 0;
	  while(p < s){
	    mvwaddch(win, LINES - cv + p + 1, i , '|');
	    p =	p + 1;
	  }
	}
      }
      cv = city_layout[i];
      wrefresh(win);
    }
    //refresh();
  }

  //refresh();
  fclose(fps);
  for(int i = 0; i < temp_var; i++){
    if(city_layout[i] > maximum_y){
      maximum_y = city_layout[i];
    }
  }

  
  int row = (l - maximum_y);
  int column = (c / 2);
  
  while(ch != 'q'){
    mvaddstr(row, column, "####");
    ch = getch();    
    if(ch == '?'){
      return EXIT_FAILURE;
    }
    wrefresh(win);
      
    if(ch == KEY_RIGHT){
      mvaddstr(row, column + 1, "#####");
      mvaddstr(row, column, " ");
      column++;
      refresh();
    }

      
    if(ch == KEY_LEFT){
      mvaddstr(row, column - 1, "#####");
      mvaddstr(row, column + 4, " ");
      column--;
      refresh();
    }

    ch = getch();
    
    wrefresh(win);
  }

    /*
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
  */
	      

  //for when the program is over
  endwin();  

}
