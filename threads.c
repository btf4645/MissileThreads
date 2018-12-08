/*
 *Author: Rebecca Fried
 *This is a game that will try bombing a city, and there is a defender that will protect the city 
 *I know this project doesn't work completely, but I really tried. It has been a difficult semester. I hope you have a good rest of the semester yourself!
 */


//these are all of the include statements that will be used
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
#include <limits.h>


//these are all of the global variable instantiations that will be needed
int COLUMNS;
int LINES;
int c;
int l;
//mutex_t instantiation for the threa
pthread_mutex_t thread;
int ch;
signed int maximum_missiles;
int a;
int b;
int maximum_y;

/*
 *defender struct for the bar
 */
typedef struct defender{
  int row;
  int column;
}D;

/*
*missile struct instantion
*/
typedef struct missiles{
  int currentx;
  int currenty;
  char *graphic;
} Bombs;

/*
 *Making the defender and mallocing space to accomodate for the defender struct
 */
D * make_defender(int row, int column){
  D *d = malloc(sizeof(struct defender));
  d->row = row;
  d->column = column;
  return d;
}

/*
 *The run function for the defender which will create a defender instance, move to a specific 
 *area of the screen, and display the bar. If the key pressed it right, the column will increase
 *by one and then move. If pressed left, the column will decrease and then move. This is all done
 *by locking and unlocking.
 */
void *defenders(void *defender){ 
  D *d = (D*)defender;
  mvaddstr(d->row, d->column, "Press q to quit the game");
  mvaddstr(d->row, d->column, "####");

  while(ch != 'q'){                                                                
    if(ch == '?'){
      pthread_exit(0);
      break;
    }
    refresh();

    //If the key pressed is the right one
    if(ch == KEY_RIGHT){
      pthread_mutex_lock(&thread);
      mvaddstr(d->row, d->column + 1, "#####");
      mvaddstr(d->row, d->column, " ");
      d->column++;
      refresh();
      pthread_mutex_unlock(&thread);
    }

    //If the key pressed is the left one
    if(ch == KEY_LEFT){
      pthread_mutex_lock(&thread);
      mvaddstr(d->row, d->column - 1, "#####");
      mvaddstr(d->row, d->column + 4, " ");
      d->column--;
      refresh();
      pthread_mutex_unlock(&thread);
    }
    //gets a new character and refreshes the page
    ch = getch();
    refresh();
  }
  return 0;
}

/*
 *This makes the missiles and mallocs spaces for it 
 */
Bombs * make_missile(int currentx, int currenty){
  Bombs *m = malloc(sizeof(struct missiles));
  m->currentx = 1;
  int randomy = rand();
  m->currenty = randomy % (b + 1);
  m->graphic = "|";
  //returns a missile
  return m;
}

/*
 *This destroys a missile by having it be freed
 */
void destroy_missile(Bombs *missile){
  free(missile);
}

/*
 *This is the run method for having the missiles be thrown down in a random order. It is the infinty
 *version, which means there is an unlimited number of missiles that can be launched at a time. 
 *It also takes into an account the special characters that are in the file array at a time. 
 */
void *infinity(void *missile){
  Bombs *m = (Bombs*)missile;
  
  int rest = rand() % 1000;

  usleep(rest * 100);
  pthread_mutex_lock(&thread);
  mvaddstr(m->currentx, m->currenty, "|");
  refresh();
  pthread_mutex_unlock(&thread);

  //while the graphic does not equal a question mark
  while(!strcmp("?", m->graphic) == 0){    
    if((mvinch(m->currentx + 1, m->currenty) == '_')){   
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      mvaddstr(m->currentx + 1, m->currenty, "?");
      mvaddstr(m->currentx + 2, m->currenty, "*");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //if the coordinates one below are equal to #
    else if(mvinch(m->currentx + 1, m->currenty) == '#'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      mvaddstr(m->currentx, m->currenty, "?");
      mvaddstr(m->currentx + 1, m->currenty, "*");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //if the coordinates one below are equal to ?
    else if(mvinch(m->currentx + 1, m->currenty) == '?'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //if the coordinates one below are equal to *
    else if(mvinch(m->currentx + 1, m->currenty) == '*'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //if it is just an empty space, keep going
    else{
      usleep(rest * 100);
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      mvaddstr(m->currentx + 1, m->currenty, "|");
      mvaddstr(m->currentx, m->currenty, " ");
      refresh();
      pthread_mutex_unlock(&thread);
      m->currentx++;
    }
    usleep(rest * 100);          
  }
  destroy_missile(m);
  return (void*) "The game is over!";
}


/*                                                                                                  
 *This is the run method for having the missiles be thrown down in a random order. It is the fixed
 *amoutn version, which means there is an unlimited number of missiles that can be launched at a time
 *It also takes into an account the special characters that are in the file array at a time.
 */

void *run(void *missile){
  Bombs *m = (Bombs*)missile;
  int rest = rand() % 1000;
  
  usleep(rest * 100);
  pthread_mutex_lock(&thread);
  mvaddstr(m->currentx, m->currenty, "|");
  refresh();
  pthread_mutex_unlock(&thread);
  

  //when the graphic equals the question mark
  while(!strcmp("?", m->graphic) == 0){    
    if(mvinch(m->currentx + 1, m->currenty) == '_'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      mvaddstr(m->currentx, m->currenty, "?");
      mvaddstr(m->currentx + 1, m->currenty, "*");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //when the coordinates one below are equal to #
    else if(mvinch(m->currentx + 1, m->currenty) == '#'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      mvaddstr(m->currentx, m->currenty, "?");
      mvaddstr(m->currentx + 1, m->currenty, "*");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //when the coordinates one below are equal to ?
    else if(mvinch(m->currentx + 1, m->currenty) == '?'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
    //when the coordinates one below are equal to *
    else if(mvinch(m->currentx + 1, m->currenty) == '*'){
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      refresh();
      pthread_mutex_unlock(&thread);
      pthread_exit(0);
      break;
    }
      //if it is just an empty space, keep going
    else{
      usleep(rest * 100);
      pthread_mutex_lock(&thread);
      mvaddstr(m->currentx, m->currenty, " ");
      mvaddstr(m->currentx + 1, m->currenty, "|");
      mvaddstr(m->currentx, m->currenty, " ");
      refresh();
      pthread_mutex_unlock(&thread);
      m->currentx++;
    }
    usleep(rest * 100);
  }
  destroy_missile(m);
  return (void*) "Over";
}
    

/*
*This is the main function in which the city is constructed, the threads are created, and the file is
*fully read
*/
int main(int argc, char * argv[]){
  //instantiation of all variables that will be used
  FILE *fps;
  char filename[255];
  char defense_force[81];
  char attack_force[81];
  int city_layout[1000];
  int sizes = 300;
  char * buffer = NULL;
  int counts = 0;
  int temp_var = 0;
  char *token;
  int another_temp;

  //if there are fewer than four arguements, there will be an exit_failure
  if(argc < 2){
    printf("No file given\n");
    return EXIT_FAILURE;
  }
  //opens the file and sees if anything is contained within it
  fps = fopen(argv[1], "r");
  if(fps == NULL){
    printf("Cannot open a file\n");
    return EXIT_FAILURE;
  }

  //uses the getline function in order to read in the file line by line, and will assign different vaues to different variables depending on the line number
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
    //reads in the file, and using strtok, will fully create the function
    else if (counts >= 3){
      token = strtok(buffer, " \t\n");
      while(token != NULL){
	sscanf(token, "%d", &another_temp);
	city_layout[temp_var] = another_temp;
	temp_var++;
	token = strtok(NULL, " \t\n");
      }
    }
  }
  

  //initialize curses library with these functions, and initialize keyboard
  initscr();
  cbreak();
  noecho();

  keypad(stdscr, TRUE);

  //gets the max y and max x of the window, and creates a new window
  getmaxyx(stdscr, LINES, COLUMNS);
  getmaxyx(stdscr, l, c);
  getmaxyx(stdscr, a, b);
  WINDOW * win = newwin(LINES, COLUMNS, 0, 0);
  
  //initializes a new mutex thread
  pthread_mutex_init(&thread, NULL);
  
  
  ch = getch();
  int cv;

  cv = 2;

  //iterates over the array of the city layout in order to create the city using mvwaddch
  for(int i = 0; i < temp_var; i++){
    if(i < LINES){
      while(city_layout[i] < 0 && i < LINES){
	  mvwaddch(win, LINES - cv, i, '_');
      }
      //if city equals the layout
      if(city_layout[i] == cv){
	mvwaddch(win, LINES - cv, i, '_');
      }
      //if the city layout at i is greater than cv
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
      //if the city layout at i is less than cv
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
      //increments and refreshes window
      cv = city_layout[i];
      wrefresh(win);
    }
  }


  fclose(fps);

  //finds the maximum y of the city_layout
  for(int i = 0; i < temp_var; i++){
    if(city_layout[i] > maximum_y){
      maximum_y = city_layout[i];

    }
  }

  maximum_y++;

  //sets valyes determining if it is the infiinte version or finite version game
  int value;
  if(maximum_missiles > 0){
    value = maximum_missiles;
  }
  else if(maximum_missiles == 0){
    value = 1000000;
  }

  //initialization of important pthread variables
  int b1;
  int b2;
  pthread_t y1;
  pthread_t y2[value];
  int row = ( l - maximum_y);
  int column = (c / 2);

  //make instance and then create a thread of the defender
  D *d = make_defender(row, column);
  b1 = pthread_create(&y1, NULL, defenders, (void *)d);


  mvaddstr(l , c, "Press q to quit the game");

  //will create the missile threads depending on which version the file is
  if(maximum_missiles == 0){
    for(int i = 0; i < 1000000; i++){
      Bombs *m = make_missile(a, b);
      b2 = pthread_create(&y2[i], NULL, infinity, (void*)m);
    }
  }
  //for the finite version
  else{
    for(int i = 0; i < maximum_missiles; i++){
      Bombs *m = make_missile(a, b);
      usleep(1000);
      b2 = pthread_create(&y2[i], NULL, run, (void*)m);
    }
  }

  void *r;
  void *f;

  //requests for a character and then joins the threads and then refreshes the window
  ch = getch();
  if(ch != 'q'){
    pthread_join(y1, NULL);
    for(int i = 0; i < value; i++){
      pthread_join(y2[i], NULL);
    }
    wrefresh(win);
  }
  ch = getch();

  char errors;

  //prints out errors
  mvwaddch(win, 0, 1, errors);

  //closes the window
  endwin();  
}
