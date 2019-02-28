#ifndef _SHIELD_H
#define _SHIELD_H


typedef struct shield_s{
  int row;
  int column;
  char *graphic;
} Shield;



Shield * make_shields(char *name, int row);

void destroy_racer(Shield *shield);

void * running_shield(void *shielder);

#endif


	    
