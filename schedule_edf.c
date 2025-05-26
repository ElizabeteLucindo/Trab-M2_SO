#include "schedule_edf.h"
#include "list.h"
#include "task.h"
#include "CPU.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node *task_list = NULL;
int task_id_counter = 1;

// add a task to the list 
void add_edf(char *name, int priority, int burst, int deadline){
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->deadline = deadline;
    newTask->tid = task_id_counter++;

    insert_end(&task_list, newTask);
}

// invoke the scheduler
void schedule(){
   while (task_list != NULL) {
      struct node *temp = task_list;
      struct node *earliest = task_list;

      while (temp != NULL) {
         if (temp->task->deadline < earliest->task->deadline) {
               earliest = temp;
         }
         temp = temp->next;
      }

      run(earliest->task, earliest->task->burst);

      delete(&task_list, earliest->task);
   }
}
