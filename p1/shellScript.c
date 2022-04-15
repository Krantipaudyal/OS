#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <fcntl.h>

#define MAX  80 //maximum number of lines

//clears the command
void clearCommands(char* args[], int argv)
{
	int i = 0;
	while(args[i] != NULL && i < argv)
	{
		free(args[i]);
		i++;
		if(i == MAX) 
			break;
	}
}

void readCommand( char *args[], int *argv, int *isAmp)
{
	//declaring
	char cmd[MAX];
	int cmdlen = 0;
	char delimiter[] = " ";

	cmdlen = read(STDIN_FILENO, cmd, MAX);
	if(cmd[cmdlen - 1]== '\n')
	{
		cmd[cmdlen - 1] = '\0';
	}

	//checking for the history
	//if there is something in history, will execute
	//have not stored all commands in any array for now
	// this will only execute the previous action
	
	//like if ls -al was executed before and
	//if  !! is executed, tjis will execute ls -al again
	//if not then will error message
	if(strcmp(cmd, "!!") == 0)
	{
		//have not stored anything in the history
		////////////////////TODO/////////////////
		
		if(*argv == 0)
		{
			printf("There is no commands in history.");
			printf("\n");
		}
		
		//FOR NOW JUST PRINTS OUR IF THERE IS NOTHING BUT NEED TO IMPLEMENT
		///////////////////////
		return;
	}
   
	clearCommands(args, *argv);
	*argv = 0;
	*isAmp = 0;
	char *temp = strtok(cmd, delimiter);
	while(temp != NULL)
	{
		if(temp[0] == '&')
		{
			*isAmp = 1;
			temp = strtok(NULL, delimiter);
			continue;
		}
		*argv += 1;
		args[*argv - 1] = strdup(temp);
		temp = strtok(NULL, delimiter);
	}
	args[*argv] = NULL;
}

int main(void)
{

	char *args[MAX /2 + 1];
	int shouldRun = 1;
	pid_t pid;
	int isAmp = 0;
	int argv = 0;
	int using_pipe = 0;
	while(shouldRun)
	{
		//everyting the loop continues, 
		//reset the value of var
		using_pipe = 0;
		printf("osh>");
		fflush(stdout);
		readCommand(args, &argv, &isAmp);

		pid = fork();
		if(pid == 0)
		{
			if(argv == 0)
			{
				continue;
			}
			else
			{
				int reDirect = 0;
				int file;
				//checks for <, > ,|
				for(int i = 1; i <= argv - 1; i++ )
				{
					if(strcmp(args[i], "<") == 0)
					{
						//input from file
						file = open(args[i + 1], O_RDONLY);
						if(file == -1 || args[i + 1] == NULL)
						{
							printf("Invalid Command! \n");
							exit(1);
						}
						dup2(file, STDIN_FILENO);
						args[i] = NULL;
						args[i + 1] = NULL;
						reDirect = 1;
						break;

					}
					//creates a file if the command has > 
					else if (strcmp(args[i], ">") == 0)
					{
						file  = open(args[i + 1], O_WRONLY | O_CREAT, 0644);
						if(file == -1 || args[i + 1] == NULL)
						{
							printf("Invalud Command! \n");
							exit(1);
						}
						dup2(file, STDOUT_FILENO);
						args[i] = NULL;
						args[i + 1] = NULL;
						reDirect = 2;
						break;
					}
					else if (strcmp(args[i], "|") == 0)
					{
						using_pipe = 1;
						int fd1[2];

						if(pipe(fd1)== -1)
						{
							fprintf(stderr, "Pipe Failed \n");
							return 1;
						}

						char *firstCmd[i + 1];
						char *secondCmd[argv - i ];
						//stdout and command 1
						//stdin and command 2
						for(int j = 0; j < i; j++)
						{
							firstCmd[j] = args[j];
						}

						firstCmd[i] = NULL;
						for(int j = 0; j < argv - i - 1; j++)
						{
							secondCmd[j] = args[j + i + 1];
						}
						secondCmd[argv - i - 1] = NULL;

						int pidPipe = fork();
						if(pidPipe > 0)
						{
							wait(NULL);
							close(fd1[1]);
							dup2(fd1[0], STDIN_FILENO);
							close (fd1[0]);
							if(execvp(secondCmd[0], secondCmd) == -1)
							{
								printf("Invalid Command! \n");
								return 1;
							}

						}
						else if(pidPipe == 0)
						{
							close(fd1[0]);
							dup2(fd1[1], STDOUT_FILENO);
							close(fd1[1]);
							if(execvp(firstCmd[0], firstCmd)== -1)
							{
								printf("Invalid Command! \n");
								return 1;
							}
							exit (1);
						}
						close(fd1[0]);
						close(fd1[1]);
						break;

					}
				}
				
				//if there is no redirect in the arguments
				if(using_pipe  == 0)
				{
					if(execvp(args[0], args) == -1 )
					{
						printf("Invalid Command! \n");
						return 1;
					}
				}
				if(reDirect == 1)
				{
					close(STDIN_FILENO);
				}
				else if(reDirect == 2)
				{
					close(STDOUT_FILENO);
				}
				close(file);
			}
			exit(1);			
		}
		else if(pid > 0)
		{
			if(isAmp == 0)
				wait(NULL);
		}
		else
		{
			printf("Fork Error!");
		}
	
	}

}
