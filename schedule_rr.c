//////////// Foi alterado o list.c para poder fazer fila fifo e inserir no final
// schedule_rr.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "CPU.h"

#define QUANTUM 10

struct node *task_list = NULL;
int tid_counter = 0;

void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->deadline = 0;
    t->tid = tid_counter++;

    insert_end(&task_list, t); 
}


void schedule(){
    struct node *aux;

    while (task_list != NULL) {
        aux = task_list;
        Task *t = aux->task;

        int exec_time;
        if (t->burst > QUANTUM) {
            exec_time = QUANTUM;
        } else {
            exec_time = t->burst;
        }

        run(t, exec_time);
        t->burst -= exec_time;

        delete(&task_list, t);

        // se ainda restar burst, reinserir no fim
        if (t->burst > 0) {
            insert_end(&task_list, t);
        } else {
            free(t->name);
            free(t);
        }
    }
}
