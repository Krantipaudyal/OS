#include <stdlib.h> //for malloc
#include <stdio.h>
#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *head;
int taskCount = 0; // number of tasks

void add(char *name, int priority, int burst)
{
    Task *newTask = malloc(sizeof(struct node));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    //adding to the list
    insert(&head, newTask);
    taskCount++; //incrementing when new task is added to list

}

//returns the task that has highest prioroty
Task *highestPriorityTask()
{
    struct node* temp;
    Task *nextTask = head->task;
    temp = head->next;

    while(temp != NULL)
    {
        if(temp->task->priority >= nextTask ->priority)
        {
            nextTask = temp->task;
        }
        temp = temp->next;
    }
    return nextTask;
}

void schedule()
{
    Task *toRun;
    while (taskCount > 0)
    {
        toRun = highestPriorityTask();
        run(toRun, toRun->burst);
        delete(&head, toRun);
        taskCount --;
        
    }
    
}

