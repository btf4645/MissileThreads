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
//#include "help.h"



int COLUMNS;
int LINES;
int c;
int l;
pthread_mutex_t thread;
//pthread_mutex_t threadinitial;
int ch;
signed int maximum_missiles;
int a;
int b;

typedef struct defender{
  int row;
  int column;
}D;


typedef struct missiles{
  int currentx;
  int currenty;
  char *graphic;
} Bombs;

D * make_defender(int row, int column){
  D *d = malloc(sizeof(struct defender));
  d->row = row;
  d->column = column;
}


void *defenders(void *defender){
  //struct D *d = arguements;
  //or it is struct defender 
  
  D *d = (D*)defender;
  //Lock
  //move to the middle of the screen
  //print out the defender
  //unlock 

  mvaddstr(d->row, d->column, "####");

  while(ch != 'q'){
    //mvaddstr(d->row, d->column, "####");
                                                                                     
    if(ch == '?'){
      pthread_exit(0);
      break;
    }
    refresh();

    if(ch == KEY_RIGHT){
      pthread_mutex_lock(&thread);
      mvaddstr(d->row, d->column + 1, "#####");
      mvaddstr(d->row, d->column, " ");
      d->column++;
      refresh();
      pthread_mutex_unlock(&thread);
    }

    if(ch == KEY_LEFT){
      pthread_mutex_lock(&thread);
      mvaddstr(d->row, d->column - 1, "#####");
      mvaddstr(d->row, d->column + 4, " ");
      d->column--;
      refresh();
      pthread_mutex_unlock(&thread);
    }
    ch = getch();
    refresh();
  }
  return 0;
}


Bombs * make_missile(int currentx, int currenty){
  Bombs *m = malloc(sizeof(struct missiles));
  m->currentx = currentx;
  //int randomy = rand();
  //m->currenty = randomy % (b + 1);
  m->currenty = currenty;
  m->graphic = '|';
}

void destroy_missile(Bombs *missile){
  free(missile);
}

void *run(void *missile){
  Bombs *m = (Bombs*)missile;
  int rest = rand() % 1000;
  int randomy = rand() % m->currenty;
  usleep(rest);
  pthread_mutex_lock(&thread);
  mvaddch(2, randomy, '|');
  refresh();
  pthread_mutex_unlock(&thread);
  while(m->graphic != '?'){
    /*
    int rest = rand() % 1000;
    usleep(rest);
    pthread_mutex_lock(&thread);
    mvaddch(m->currentx, m->currenty, '|');
    refresh();
    pthread_mutex_unlock(&thread);
    m->currentx++;
    */
    
    if(m->currentx <= 2){
      pthread_mutex_lock(&thread);
      mvaddch(m->currentx + 1, randomy, '?');
      mvaddch(m->currentx, randomy, ' ');
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    else if(mvinch(m->currentx + 1, randomy) == '_'){
      pthread_mutex_lock(&thread);
      mvaddch(m->currentx, randomy, ' ');
      mvaddch(m->currentx + 1, randomy, '?');
      mvaddch(m->currentx + 2, randomy, '*');
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    else if(mvinch(m->currentx + 1, randomy) == '#'){
      pthread_mutex_lock(&thread);
      mvaddch(m->currentx, randomy, ' ');
      mvaddch(m->currentx, randomy, '?');
      mvaddch(m->currentx + 1, randomy, '*');
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    else{
      usleep(rest);
      pthread_mutex_lock(&thread);
      mvaddch(m->currentx, randomy, ' ');
      mvaddch(m->currentx + 1, randomy, '|');
      refresh();
      pthread_mutex_unlock(&thread);
      m->currentx++;
    }
    m->currentx++;
  }
  destroy_missile(m);
  return (void*) "Over";
}
    
  
/*
void *missile(void *arguements){
  struct missiles *m = arguements;
  //int randomx = rand() % m->currentx;
  int randomy = rand() % currenty;
  while(ch != 'q'){
    for(int i = 0; i < maximum_missiles; i++){
      mvaddch(randomx, m->currenty, '|');
      if(randomx != '-'){
	sleep(1000);
	pthread_mutex_lock(&thread);
	mvaddch(randomx + 1, m->currenty, '|');
	mvaddch(randomx, m->currenty, ' ');
	randomx++;
	pthread_mutex_unlock(&thread);
	
	refresh();
      }
      else if(i >= m->currenty || i == '-'){
	pthread_mutex_lock(&thread);
	mvaddch(randomx, m->currenty, ' ');
	mvaddch(randomx + 1, m->currenty, '?');
	mvaddch(randomx + 1, m->currenty, '*');
	randomx++;
	pthread_mutex_unlock(&thread);
	
	refresh();
      }
      else{
	pthread_mutex_lock(&thread);
	mvaddch(randomx + 1, m->currenty, '?');
	mvaddch(randomx, m->currenty, ' ');
	randomx++;
	pthread_mutex_unlock(&thread);
	
	refresh();
      }
      //refresh();
    }
    ch = getch();
    refresh();
  }
  return 0;
}
*/

int main(int argc, char * argv[]){
  FILE *fps;
  int maximum = 1;
  char filename[255];
  char defense_force[81];
  char attack_force[81];
  //int * city_layout = malloc(sizeof(int) * maximum);
  int city_layout[1000];
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
  getmaxyx(stdscr, a, b);
  WINDOW * win = newwin(LINES, COLUMNS, 0, 0);
  
  // pthread_t t;
  //const char* m = "thread";
  pthread_mutex_init(&thread, NULL);
  //pthread_mutex_init(&threadinitial, NULL);
  /*
  for(int i = 0; i <= sizeof(city_layout); i++){
    printf("%d", city_layout[i]);
  }
  */
  
  int curx = LINES;
  int cury = COLUMNS;
  ch = getch();
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
      while(city_layout[i] < 0 && i < LINES){
	  mvwaddch(win, LINES - cv, i, '_');
      }

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

  maximum_y++;
  
  //int row = (l - maximum_y);
  
  int value = maximum_missiles;
  int b1;
  int b2;
  pthread_t y1;
  pthread_t y2[value];

  int row = ( l - maximum_y);
  int column = (c / 2);
  
  D *d = make_defender(row, column);
  b1 = pthread_create(&y1, NULL, defenders, (void *)d);

  
  for(int i = 0; i < maximum_missiles; i++){
    Bombs *m = make_missile(a, b);
    b2 = pthread_create(&y2[i], NULL, run, (void*)m);
    if(b2){
      fprintf("%s", "is created");
      return EXIT_FAILURE;
    }
  }
  //int b1;
  //int b2;
  //pthread_t y1;
  //pthread_t y2;

  //b1 = pthread_create(&y1, NULL, &defenders, (void *)&d);
  //b2 = pthread_create(&y2, NULL, &run, (void*)&mam);
  
  
  //row = ( l - maximum_y);
  //column = (c / 2);
  //mam.currentx = a;
  //mam.currenty = b;

  void *r;
  void *f;
  
  ch = getch();
  if(ch != 'q'){
    pthread_join(y1, r);
    for(int i = 0; i < maximum_missiles; i++){
      pthread_join(y2[i], f);
    }
  
    /*
    mvaddstr(row, column, "####");
    //ch = getch();    
    if(ch == '?'){
      return EXIT_FAILURE;
    }
    refresh();
      
    if(ch == KEY_RIGHT){
      pthread_mutex_lock(&threadinitial);
      mvaddstr(row, column + 1, "#####");
      mvaddstr(row, column, " ");
      column++;
      pthread_mutex_unlock(&threadinitial);
      refresh();
    }

      
    if(ch == KEY_LEFT){
      pthread_mutex_lock(&threadinitial);
      mvaddstr(row, column - 1, "#####");
      mvaddstr(row, column + 4, " ");
      column--;
      pthread_mutex_unlock(&threadinitial);
      refresh();
    }

    //ch = getch();    
    //wrefresh(win);

    
    for(int i = 0; i < maximum_missiles; i++){
      if(i < COLUMNS && i != '-'){ 
	usleep(1000);
	pthread_mutex_lock(&thread);
	mvaddch(currentx + 1, currenty, '|');
	mvaddch(currentx, currenty, ' ');
	currentx++;
	pthread_mutex_unlock(&thread); 

	refresh();
      }
      else if(i >= COLUMNS || i == '-'){
	pthread_mutex_lock(&thread);
	mvaddch(currentx, currenty, ' ');
        mvaddch(currentx + 1, currenty, '?');
	mvaddch(currentx + 1, currenty, '*');
        currentx++;
        pthread_mutex_unlock(&thread);
	
        refresh();
      }
      else{
	pthread_mutex_lock(&thread);
        mvaddch(currentx + 1, currenty, '?');
	mvaddch(currentx, currenty, ' ');
        currentx++;
        pthread_mutex_unlock(&thread);
	
	refresh();
      }
    }
    */
    
    ch = getch();
    wrefresh(win);
    }


  char errors;

  //error messages for the window
  mvwaddch(win, 0, 1, errors);


  //free(city_layout);
  
	      

  //for when the program is over
  endwin();  

}
