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
        
        struct node *temp = task_list;
        struct node *maior_p = task_list;

        
        temp = task_list;
        while (temp != NULL) {
            if (temp->task->priority < maior_p->task->priority ||
                (temp->task->priority == maior_p->task->priority &&
                 temp->task->wait_time > maior_p->task->wait_time)) {
                maior_p = temp;
            }
            temp = temp->next;
        }

        for (int i = 0; i < maior_p->task->burst; i++) {
            while (flag_estouro == 0); 
            flag_estouro = 0;
            run(maior_p->task, 1);

            
            temp = task_list;
            while (temp != NULL) {
                if (temp != maior_p) {
                    temp->task->wait_time++;

                    if (temp->task->wait_time >= AGING &&
                        temp->task->priority > 1) {
                        temp->task->priority--;
                        temp->task->wait_time = 0;
                        printf("Aging: Tarefa %s subiu prioridade para %d\n", temp->task->name, temp->task->priority);
                    }
                }
                temp = temp->next;
            }
        }

        maior_p->task->burst = 0; // tarefa finalizada
        delete(&task_list, maior_p->task);
    }

    printf("Todas as tarefas foram finalizadas.\n");
}
