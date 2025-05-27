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
        // Escolhe a tarefa de maior prioridade (menor valor numérico)
        struct node *temp = task_list;
        struct node *highest = task_list;

        while (temp != NULL) {
            // Aging: se a tarefa esperou tempo demais, melhora a prioridade
            if (temp->task->wait_time >= AGING_THRESHOLD && temp->task->priority > 1) {
                temp->task->priority--;
                temp->task->wait_time = 0;
                printf("Aging: Tarefa %s subiu prioridade para %d\n", temp->task->name, temp->task->priority);
            }

            if (temp->task->priority < highest->task->priority) {
                highest = temp;
            }

            temp = temp->next;
        }

        // Executa a tarefa escolhida por até TIME_SLICE ou até terminar
        int run_time = (highest->task->burst > TIME_SLICE) ? TIME_SLICE : highest->task->burst;

        for (int i = 0; i < run_time; i++) {
            while (flag_estouro == 0); // Espera próximo tick
            flag_estouro = 0;

            run(highest->task, 1);
        }

        highest->task->burst -= run_time;

        // Resetar wait_time da tarefa executada
        highest->task->wait_time = 0;

        // Atualiza wait_time das demais tarefas
        temp = task_list;
        while (temp != NULL) {
            if (temp != highest) {
                temp->task->wait_time += run_time;
            }
            temp = temp->next;
        }

        // Remove a tarefa se terminou
        if (highest->task->burst <= 0) {
            delete(&task_list, highest->task);
        }
    }

    printf("Todas as tarefas foram finalizadas.\n");
}