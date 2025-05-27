#include "schedule_edf.h"
#include "list.h"
#include "task.h"
#include "CPU.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"

struct node *task_list = NULL;
int task_id_counter = 1;

// add a task to the list 
void add_edf(char *name, int priority, int burst, int deadline){
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->deadline = deadline;
    t->wait_time = 0;
    t->tid = task_id_counter++;

    insert_end(&task_list, t);
}

// invoke the scheduler
void schedule() {
    start_timer();

    while (task_list != NULL) {
        struct node *aux = task_list;
        struct node *recente = task_list;

        while (aux != NULL) {
            if (aux->task->deadline < recente->task->deadline) {
                recente = aux;
            }
            aux = aux->next;
        }

        printf("Time %d: Running task %s (Burst: %d, Deadline: %d)\n",
               timer, recente->task->name, recente->task->burst, recente->task->deadline);

        for (int i = 0; i < recente->task->burst; i++) {
            while (flag_estouro == 0);  
            flag_estouro = 0;

            run(recente->task, 1); 
        }

        printf("Time %d: Finished task %s\n", timer, recente->task->name);

        delete(&task_list, recente->task); 
    }
}

