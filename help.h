

typedef struct missiles{
  int currentx;
  int currenty;
}Bombs;


Bombs * make_missile(int currentx, int currenty);

void destroy_missile(Bombs *missile);

void *run(void *missile);

void *missile(void *arguements);
