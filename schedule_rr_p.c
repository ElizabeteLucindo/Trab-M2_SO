// rr_p
// Prioridade menor = mais alta prioridade
#include "schedule_rr_p.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "CPU.h"

#define QUANTUM 10

struct node *priority_fila[MAX_PRIORITY + 1];
int tid_counter = 0;

void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->deadline = 0;
    t->wait_time = 0;
    t->tid = tid_counter++;

    insert_end(&priority_fila[priority], t); 
}


void schedule() {
    
    for (int prioridade = MIN_PRIORITY; prioridade <= MAX_PRIORITY; prioridade++) {
        if (priority_fila[prioridade] == NULL)
            continue;

        while (priority_fila[prioridade] != NULL) {
            struct node *aux = priority_fila[prioridade];
            Task *t = aux->task;

            int exec_time;
            if (t->burst > QUANTUM) {
                exec_time = QUANTUM;
            } else {
                exec_time = t->burst;
            }

            run(t, exec_time);
            t->burst -= exec_time;

            delete(&priority_fila[prioridade], t);

            if (t->burst > 0) {
                insert_end(&priority_fila[prioridade], t);
            } else {
                free(t->name);
                free(t);
            }
            
        }
    }
    
}

