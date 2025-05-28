// Prioridades com Aging
// Prioridade menor = mais alta prioridade
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "CPU.h"
#include "timer.h"

#define MIN_PRIORITY 1
#define AGING  10
#define TIME_SLICE   10   

struct node *task_list = NULL;
int tid_counter = 1;

void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->deadline = 0;
    t->wait_time = 0;
    t->tid = tid_counter++;

    insert_end(&task_list, t); 
}

void schedule() {
    start_timer();

    while (task_list != NULL) {
        
        struct node *aux = task_list;
        struct node *maior_p = task_list;

        
        aux = task_list;
        while (aux != NULL) {
            if (aux->task->priority < maior_p->task->priority ||
                (aux->task->priority == maior_p->task->priority &&
                 aux->task->wait_time > maior_p->task->wait_time)) {
                maior_p = aux;
            }
            aux = aux->next;
        }

        for (int i = 0; i < maior_p->task->burst; i++) {
            while (flag_estouro == 0); 
            flag_estouro = 0;
            run(maior_p->task, 1);

            
            aux = task_list;
            while (aux != NULL) {
                if (aux != maior_p) {
                    aux->task->wait_time++;

                    if (aux->task->wait_time >= AGING && aux->task->priority > 1) {
                        aux->task->priority--;
                        aux->task->wait_time = 0;
                        printf("Aging: Tarefa %s subiu prioridade para %d\n", aux->task->name, aux->task->priority);
                    }
                }
                aux = aux->next;
            }
        }

        
        delete(&task_list, maior_p->task);
    }

    printf("Todas as tarefas foram finalizadas.\n");
}
