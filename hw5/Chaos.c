#include <stdio.h>        // printf
#include <stdlib.h>       // exit, atoi
#include <pthread.h>      // pthread*
//#include <windows.h>
#include <unistd.h>

#define BUFSIZE 1000
#define gMax    20

int NUMPROD = 1;    // number of producers
int NUMCONS = 1;    // number of consumers

int gBuf[BUFSIZE];  // global buffer
int gNum = 0;       // global counter

int gIn  = 0;       // input  cursor in gBuf
int gOut = 0;       // output cursor in gBuf


pthread_mutex_t lock;

void say(int me, char* msg, int x) {
    printf("%d ", me);
    //printf(msg); --------> not sure why this doesnt work
	printf("%s", msg);
    printf("%d \n", x);
}

void insert(int x) { 
  gBuf[gIn] = x;
  ++gIn;
}

int extract() {
  int x = gBuf[gOut];
  ++gOut;
  return x;
}

int incgNum() {
  int num = ++gNum;
  return num;
}

int getgNum() {
  int num = gNum;
  return num;
}


void* producer(void* arg) {
	//locking the mutex here 
	pthread_mutex_lock(&lock);
  int me = pthread_self();
  int num = getgNum();
  while (num < gMax) {
    num = incgNum();
    say(me, "Produced: ", num);
    insert(gNum);
  }
  //mutex unlock
 pthread_mutex_unlock(&lock);
  return NULL;
}

void* consumer(void* arg) {
	//mutex lock
  pthread_mutex_lock(&lock);
  int me = pthread_self();
  while (1) {
    int num = extract();
    say(me, "Consumed: ", num);
    if (num == gMax) { 
      break;
    }
  }
  
  //unlocks here
  pthread_mutex_unlock(&lock);
  return NULL;
}

void checkInput(int argc, char* argv[]) {
  if (argc == 1) {
    NUMPROD = 1;
    NUMCONS = 1;
    return;
  }

  if (argc != 3) {
    printf("Specify <producers>  <consumer> \n");
    printf("Eg:  2  3 \n");
    exit(0);
  }

  NUMPROD = atoi(argv[1]);
  if (NUMPROD < 1 || NUMPROD > 10) {
    printf("Number of producers must lie in the range 1..10 \n");
    exit(0);
  }

  NUMCONS = atoi(argv[2]);
  if (NUMCONS < 1 || NUMCONS > 10) {
    printf("Number of consumers must lie in the range 1..10 \n");
    exit(0);
  }
}

int main(int argc, char* argv[]) {

  checkInput(argc, argv);

  pthread_t prod[NUMPROD];
  pthread_t cons[NUMCONS];

  for (int i = 0; i < NUMPROD; ++i)
  {
	   pthread_create(&prod[i], 0, producer, NULL);
  }	 
  for (int i = 0; i < NUMCONS; ++i)
  {
	   pthread_create(&cons[i], 0, consumer, NULL);
  }	 

  for (int i = 0; i < NUMPROD; ++i)
  {
	   pthread_join(prod[i], NULL);
  }	 
  for (int i = 0; i < NUMCONS; ++i)
  {
	  pthread_join(cons[i], NULL);
  }	  
  
  //destroying all mutex
  pthread_mutex_destroy(&lock);
  printf("All done! Hit any key to finish \n");
  getchar();
  
  return 0;
}
