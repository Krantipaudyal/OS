#include <stdlib.h>

#include "task.h"
#include "schedulers.h"
#include "cpu.h"
#include "list.h"

struct node *head;
struct node *curTaskNode;
int taskCount = 0;

void add(char *name, int priority, int burst)
{
    Task *newTask = malloc(sizeof(struct node));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;

    //adding to the lsit
    insert(&head, newTask);
    taskCount++; //incrementing with new task added

}

//picks next task, deletes task from list if task is completed
void pickNextTask()
{
    struct node *temp;
    temp = curTaskNode;

    //when there is no next, comes back to head
    //if head is also  null, task finished 
    if (curTaskNode->next == NULL)
    {
        curTaskNode = head;
    }
    else
    {
        curTaskNode = curTaskNode->next;
    }
    //deletes if the task is completed
    if (temp->task->burst == 0)
    {
        delete (&head, temp->task);
    }
}

//reusing code from schedule_priority
void highestPriorityTask()
{
    struct node* temp;
    Task *nextTask = head->task;
    temp = head->next;

    while(temp != NULL)
    {
        if(temp->task->priority >= nextTask ->priority)
        {
            curTaskNode = temp;
            nextTask = temp->task;
        }
        temp = temp->next;
    }
}


void schedule()
{
    int slice;
    //reverseList();
    //traverse(head);
    Task *curTask;
    curTaskNode = head;
    while (head != NULL)
    {
        curTask = curTaskNode->task;
        //if burst time is less thatn quantum, slice is burst, else quantum
        if(QUANTUM <= curTask->burst)
        {
            slice = QUANTUM;
        }
        else
        {
            slice = curTask->burst;
        }
        run(curTask, slice);
        curTask->burst -= slice; // subtracting to keep track of time so task can be deleted later
        pickNextTask();
        highestPriorityTask();
    }
}

