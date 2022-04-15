//#include <stdio.h>
#include <stdlib.h> //for malloc

//#include "schedulers.h"
#include "cpu.h"
#include "list.h" //already included in schedulers.h

struct node *head;
//total tasks at the begining is 0
int taskCount = 0;

void add(char *name, int priority, int brust)
{
    Task *newTask = malloc(sizeof(struct node));
    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = brust;
    //inserting in the list
    insert(&head, newTask);
    taskCount++;
    
}

void reverseList()
{
    struct node* revHead, *temp;
    temp = head;
    while(temp != NULL)
    {
        insert(&revHead, temp->task);
        delete(&head, temp->task);
        temp = temp->next;
    }
    head = revHead;
    
}

void schedule()
{
    //float TAT; // turn around time  //= completion time - arrival time 
    //float WT; //waiting time //  tat - brust time
    int CT = 0; //completion time // from gnatt chart // for now 0 will add up

    //empties the tasks from the list
    //till the count of tasks gets down to zero

    reverseList();
    while(taskCount > 0)
    {
        Task* toRun = head->task;
        CT += toRun->burst;
        run(toRun, toRun->burst);
        delete(&head, toRun);
        taskCount --;
    }

    //No need to calculate TAT and waiting time and all 
    //average turnaround time and waiting time could be calculated easily here though
}
