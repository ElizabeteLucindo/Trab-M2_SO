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
#define AGING_THRESHOLD  10
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
        // Escolhe a tarefa de maior prioridade
        struct node *temp = task_list;
        struct node *highest = task_list;

        // Aging pré-seleção
        while (temp != NULL) {
            if (temp->task->wait_time >= AGING_THRESHOLD && temp->task->priority > MIN_PRIORITY) {
                temp->task->priority--;
                temp->task->wait_time = 0;
                printf("Aging: Tarefa %s subiu prioridade para %d\n", temp->task->name, temp->task->priority);
            }
            temp = temp->next;
        }

        // Seleciona a task com maior prioridade (e mais espera em caso de empate)
        temp = task_list;
        while (temp != NULL) {
            if (temp->task->priority < highest->task->priority ||
                (temp->task->priority == highest->task->priority &&
                 temp->task->wait_time > highest->task->wait_time)) {
                highest = temp;
            }
            temp = temp->next;
        }

        int run_time = highest->task->burst;

        for (int i = 0; i < run_time; i++) {
            while (flag_estouro == 0); // espera o tick
            flag_estouro = 0;
            run(highest->task, 1);

            // Aging dinâmico: incrementa wait_time das outras tarefas
            temp = task_list;
            while (temp != NULL) {
                if (temp != highest) {
                    temp->task->wait_time++;

                    if (temp->task->wait_time >= AGING_THRESHOLD &&
                        temp->task->priority > MIN_PRIORITY) {
                        temp->task->priority--;
                        temp->task->wait_time = 0;
                        printf("Aging: Tarefa %s subiu prioridade para %d\n", temp->task->name, temp->task->priority);
                    }
                }
                temp = temp->next;
            }
        }

        highest->task->burst = 0; // tarefa finalizada
        delete(&task_list, highest->task);
    }

    printf("Todas as tarefas foram finalizadas.\n");
}
