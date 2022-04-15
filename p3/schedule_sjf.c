#include <stdlib.h> //for malloc

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *head = NULL;

//returns a task with the shortest burst time 
//for sortest jhob first
Task *pickNextTask()
{
    struct node* temp;
    Task *nextTask = head->task;
    temp = head->next;

    while(temp != NULL)
    {
        if(temp->task->burst <= nextTask ->burst)
        {
            nextTask = temp->task;
        }
        temp = temp->next;
    }
    return nextTask;
}

//creates and adds new task to the list
void add(char* name, int priority, int burst)
{
    Task *newTasj = malloc(sizeof(struct node));
    newTasj->name = name;
    newTasj->burst = burst;
    newTasj->priority = priority;

    //inserts the newTask to the list
    insert(&head, newTasj);
}

void schedule()
{
    Task* toRun;
    while(head != NULL)
    {
        //picks next task to run
        toRun = pickNextTask();
        run(toRun, toRun->burst);
        delete(&head, toRun);
    }
}

