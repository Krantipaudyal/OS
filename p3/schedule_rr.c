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
    int slice;
    reverseList();
    //traverse(head);
    Task *toRun;
    curTaskNode = head;
    while (head != NULL)
    {
        toRun = curTaskNode->task;
        //if burst time is less thatn quantum, slice is burst, else quantum
        if(QUANTUM <= toRun->burst)
        {
            slice = QUANTUM;
        }
        else
        {
            slice = toRun->burst;
        }
        run(toRun, slice);
        toRun->burst -= slice; // subtracting to keep track of time so task can be deleted later
        pickNextTask();
    }
}

// void schedule()
// {
//     struct node* temp = head;
//     while(taskCount > 0)
//     {
//         Task *toRun = temp->task;
//         if(toRun->burst > QUANTUM)
//         {
//             //run the task for the fixed time slice
//             run(toRun, QUANTUM);
//             toRun->burst -= QUANTUM; //remaining time after the slice, to run later
//             //trying to pick next task if exist
//             temp = temp->next; 
//             if(temp != NULL)
//             {
//                 //next running task
//                 toRun = temp->task;
//             }
//             else
//             {
//                 temp = head;
//                 if(temp == NULL)
//                 {
//                     return;
//                 } 
//                 else
//                 {
//                     toRun = temp->task;
//                 }
//             }

//         }
//         else
//         {
//             int timeLeft = toRun->burst;
//             //runs for time either less than QUANTUM or the remaining time from  the previously run process
//             run(toRun, timeLeft); 
//             toRun->burst -= timeLeft;
//             temp = temp->next;
//             delete(&head, toRun);
//             if(temp != NULL)
//             {
//                 toRun =  temp->task;
//             }
//             else
//             {
//                 temp = head;
//                 if(temp == NULL)
//                 {
//                     return;
//                 }
//                 else
//                 {
//                     toRun = temp->task;
//                 }
//             }
//         }

//     }
// }
