#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MEMSIZE 80 // max size of a memorya


char *memory; 

// reads command from the commandline
int readline(char** buffer) 
{
  size_t len;   
  int numChar = getline(buffer, &len, stdin);
   if (numChar > 0)
  {
    (*buffer)[numChar - 1] = '\0';
  }
  return numChar;
}

//parses the lines to token the cmd
int tokenize(char* cmd, char** tokens)
{
  char* pch;
  pch =  strtok(cmd, " ");
  int num = 0;
  while (pch != NULL)
  {
    tokens[num] = pch;
    ++num;
    pch =  strtok(NULL, " ");
  }
  return num;
}

void bestFit(char name, int size)
{
  //printf("best fit");
  if(size > MEMSIZE)
  {
    return;
  }
  int count = 0;
  int start = 0;
  int min = 90;
  int minStart = -1;
  for (int i = 0; i < MEMSIZE; ++i) {
    if (memory[i] == '.') {
      start = i;
      int j = i;
      while (memory[j] == '.') {
        count++;
        j++;
      }
      if (count < min) {
        min = count;
        minStart = start;
      }
      count = 0;
      i = j - 1;
    }
  }
  if (min < size)
    return;
  for (int i = minStart; i < minStart + size; ++i) {
    if (memory[i] == '.')
      memory[i] = name;
  }

}
void firstFit(char name, int size)
{
  //printf("first fit");
    if (size > MEMSIZE)
    return;
  int count = 0;
  int start = 0;
  for (int i = 0; i < MEMSIZE; ++i) {
    if (memory[i] == '.') {
      start = i;
      int j = i;
      while (memory[j] == '.' && j < MEMSIZE) {
        count++;
        j++;
      }
      if (count >= size)
        break;
      count = 0;
      i = j - 1;
    }
  }
  if (count < size)
    return;
  for (int i = start; i < start + size; ++i) {
    if (memory[i] == '.')
      memory[i] = name;
  }
}
void worstFit(char name, int size)
{
 // printf("worst fit");
  if (size > MEMSIZE)
    return;
  int count = 0;
  int start = 0;
  int max = -1;
  int maxStart = -1;
  for (int i = 0; i < MEMSIZE; ++i) {
    if (memory[i] == '.') {
      start = i;
      int j = i;
      while (memory[j] == '.') {
        count++;
        j++;
      }
      if (count > max) {
        max = count;
        maxStart = start;
      }
      count = 0;
      i = j - 1;
    }
  }
  if (max < size)
    return;
  for (int i = maxStart; i < maxStart + size; ++i) {
    if (memory[i] == '.')
      memory[i] = name;
  }
}

void show() {
  printf("Memory status in the memory pool\n");
  for(int i = 0; i < MEMSIZE; i++) {
    printf("%c", memory[i]);
  }
  printf("\n");
}

//compress the whole memory pool 
//leaves empty spaces to the end
void compress()
{
  char* memCopy = malloc(MEMSIZE* sizeof(char));
  for (int i = 0; i< MEMSIZE; i++)
  {
    memCopy[i] = '.';
  }
   
  int idx = 0;
  for(int i = 0; i< MEMSIZE; i++)
  {
    //checks of "." before copying
    if(memory[i] != '.')
    {
      memCopy[idx++] = memory[i];
    }
  }
  //copying back everything from memCopy to memory
  for(int i = 0; i < MEMSIZE; i++)
  {
    memory[i] = memCopy[i];
  }

  free(memCopy); //deletes the copy of memory
 
  show();

}

void start()
{
  for(int i = 0; i < MEMSIZE; i++) {
    memory[i] = '.';
  }
}

//prints out error
void sayError()
{
  printf("Invalid data");
}

//allocates memory based on the given algorithm
void allocate(char name, int size, char algo) {
  switch (algo)
  {
  case 'F':
    firstFit(name, size);
    break;
  case 'B': 
    bestFit(name, size);
    break;
  case 'W':
    worstFit(name, size);
    break;
  default:
    sayError();
    break;
  }
}

//frees the allocation of the given process name
void freeAllocation(char name)
{
  bool exist = false;
  for(int i = 0; i < MEMSIZE; i++)
  {
    if(memory[i] == name)
    {
      memory[i] = '.';
      exist = true;
    }
  }
  if(exist)
  {
    printf("Freed process: %c\n", name);
  }
  else
  {
    printf("Process: %c does not exist in the memory.", name);
  }
}



// read the text file line by line and execute the commands
void readFile(char *file) {
  //printf("executeCommands2\n");
  FILE *inFile;
  inFile = fopen(file, "r");
  if(inFile == NULL)
  {
    perror("File doesnt exist");
    exit(1);
  }
  char* args[MEMSIZE / 2 + 1];
  char* cmd = NULL;
  size_t length;
  //emptying args befroe reading
  for(int i = 0; i < MEMSIZE/2 + 1; i++)
  {
    args[i] = NULL;
  }

  //reading from the file
  while (getline(&cmd, &length, inFile))
  {
    int numToken = tokenize(cmd, args);
    if(length <= 0)
    {
      break;
    }
    //for empty
    else if(cmd == "")
    {
      continue;
    }
    else if(cmd[0] == 'S')//prints state
    {
      show();
      continue;
    }
    else if(cmd[0] == 'C')
    {
      compress();
      continue;
    }
    else if(cmd[0] == 'E')
    {
      free(cmd);
      free(memory);
      fclose(inFile);
      printf("Exit \n");
      exit(0);
    }
    else if (strcmp(args[0], "A") == 0)
    {
      allocate(args[1][0], atoi(args[2]), args[3][0]);
      continue;
    }
    else if (strcmp(args[0], "F") == 0)
    {
      freeAllocation(args[1][0]);
      continue;
    }
    else
    {
      perror("Invalid Command \n");
      exit(0);
    }
    for(int i = 0; i <= numToken; i++)
    {
      args[i] = NULL;
    }
  }
    free(cmd);
    fclose(inFile);
    free(memory);
  
}




int main(void) {
  memory = calloc(1, MEMSIZE);
  start();

  char *args[MEMSIZE/2 + 1]; /* command line arguments */
  for(int i =0; i < MEMSIZE/2 + 1; i++ )
  {
    args[i] = NULL;
  }
  char *cmd = (char *) malloc(MEMSIZE * sizeof(char));

  while (true) {
    fflush(stdout);
    int len = readline(&cmd);
    // for (int i = 0; i < MEMSIZE/2 + 1; ++i)
    // {
    //   args[i] = NULL;
    // }
    tokenize(cmd, args);
    if(len <= 0)
    {
      break;
    }
    else if (strcmp(cmd, "") == 0) 
    {
      continue; 
    }

    else if (strcmp(cmd, "S") == 0) 
    {
      show();
      continue;
    }
    else if (strcmp(cmd, "E") == 0) 
    {
      //exiting
      break;
    }
    else if (strcmp(cmd, "C") == 0)
    {
      //compress
      compress();
      continue;
    }
    else if (strcmp(args[0], "A") == 0)
    {
      allocate(args[1][0], atoi(args[2]), args[3][0]);
      continue;
    }
    else if (strcmp(args[0], "F") == 0)
    {
      freeAllocation(args[1][0]);
      continue;
    }
    else if (strcmp(args[0], "R") == 0)
    {
      readFile(args[1]);
      continue;
    }
    else
    {
      printf("Please enter a valid command!\n");
    }
  }
  free(cmd);
  free(memory);
  return 0;
}