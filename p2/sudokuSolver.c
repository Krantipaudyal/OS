#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define threadCounts 27

//struct for the parameters
typedef struct
{
	int row;
	int column;
}parameters;

//given soducu by professor
int sudoku[9][9] = {
	{6, 2, 4, 5, 3, 9, 1, 8, 7}, //7
	{5, 1, 9, 7, 2, 8, 6, 3, 4},
	{8, 3, 7, 6, 1, 4, 2, 9, 5},
	{1, 4, 3, 8, 6, 5, 7, 2, 9},
	{9, 5, 8, 2, 4, 7, 3, 6, 1},
	{7, 6, 2, 3, 9, 1, 4, 5, 8},
	{3, 7, 1, 9, 5, 6, 8, 4, 2},
	{4, 9, 6, 1, 8, 2, 5, 7, 3},
	{2, 8, 5, 4, 7, 3, 9, 1, 6}

};
int tCompleted = 0;
//this array is used to put the result after the thread goes through the row or 
//column or the grid nad updates as 1 or 0
int tResult[threadCounts];
//assignment didnt ask to use mutexx tho so not using it
//to lock the critical field while updating 
//pthread_mutex_t lock;


void* colValidator(void* arg)
{
	parameters *col = (parameters*)arg;
	int colNumber = col-> column;
	int rowNumber = col -> row;

	if(colNumber > 8 || rowNumber != 0)
	{
		printf("Row or column is invalid at row: %d, column: %d \n",rowNumber, colNumber );
		pthread_exit(NULL);
	}
	
	int isValid[9] = {0};
	int i;
	for(i = 0; i < 9; i++)
	{
		int data = sudoku[i][colNumber];
		if(data < 1 || data > 9 || isValid[data - 1]== 1)
		{
			pthread_exit(NULL);

		}
		else
		{
			isValid[data - 1] = 1;
		}
	}
	//column is valid
	tResult[18 + colNumber] = 1;
	//the thread execution ends here
	pthread_exit(NULL);

}

//this traverces through the row and validates if the row is correct
//for the sudoku to be valid
void* rowValidator(void* arg)
{
	parameters *roww = (parameters*)arg;
	int colNumber = roww-> column;
	int rowNumber = roww -> row;

	if(rowNumber > 8 || colNumber != 0)
	{
		printf("Row or column is invalid at row: %d, column: %d \n",rowNumber, colNumber );
		pthread_exit(NULL);
	}
	
	int isValid[9] = {0};
	int i;
	for(i = 0; i < 9; i++)
	{
		int data = sudoku[rowNumber][i];
		if(data < 1 || data > 9 || isValid[data - 1]== 1)
		{
			pthread_exit(NULL);

		}
		else
		{
			isValid[data - 1] = 1;
		}
	}
	//column is valid
	tResult[9 + rowNumber] = 1;
	//the thread execution ends here
	pthread_exit(NULL);

}

//this checks the subgrid of size 3X3 and validates if the numbers are between 1-9
//without repeating or without any invalid numbers
void *gridValidator(void* arg)
{
	parameters *grid = (parameters *) arg;
	int colNumber = grid -> column;
	int rowNumber = grid -> row;

	int isValid[9] = {0};
	int i, j;
	for(i = rowNumber; i < rowNumber + 3; i++ )
	{
		for(j = colNumber; j < colNumber + 3; j++)
		{
			int data = sudoku[i][j];
			if (data < 1 || data > 9 || isValid[data - 1] == 1) {
				pthread_exit(NULL);
			} else {
				isValid[data - 1] = 1;		
			}
		}
	}
	tResult[rowNumber + colNumber/3] = 1;
	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[threadCounts];
	int threadID = 0;
	
	int i, j;
	
	//creating threads for rows, columns and the grid. each row has one thread and each 
	//column has one thread  and each grid has one thread
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if (i == 0) 
			{
				parameters *colData = (parameters *) malloc(sizeof(parameters));	
				colData->row = i;		
				colData->column = j;
				pthread_create(&threads[threadID++], NULL, colValidator, colData);	
			}
			if (j == 0) 
			{
				parameters *rowData = (parameters *) malloc(sizeof(parameters));	
				rowData->row = i;		
				rowData->column = j;
				pthread_create(&threads[threadID++], NULL, rowValidator, rowData);
			}
			if (i%3 == 0 && j%3 == 0) 
			{
				parameters *grid = (parameters *) malloc(sizeof(parameters));	
				grid->row = i;		
				grid->column = j;
				pthread_create(&threads[threadID++], NULL, gridValidator, grid);
			}
		}
	}
	
	for (i = 0; i < threadCounts; i++) 
	{
		//waits for all threads to finish
		pthread_join(threads[i], NULL);		
	}

	// if tResult[i] = 0, the sudoko is invalid
	for (i = 0; i < threadCounts; i++)
	{
		if (tResult[i] == 0) 
		{
			printf("Sudoku is invalid!\n");
			return 0;
		}
	}
	printf("Sudoku solution is valid!\n");
}










































