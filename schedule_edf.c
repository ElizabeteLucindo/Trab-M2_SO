#include "schedule_edf.h"
#include "list.h"
#include "task.h"
#include "CPU.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "timer.h"

extern volatile int timer;
extern volatile int flag_estouro;

struct node *task_list = NULL;
int task_id_counter = 1;

void add_edf(char *name, int priority, int burst, int deadline) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->deadline = deadline;
    t->wait_time = 0;
    t->tid = task_id_counter++;

    insert_end(&task_list, t);
}

void schedule() {
    start_timer();  // Inicia o timer em background

    while (task_list != NULL) {
        struct node *aux = task_list;
        struct node *recente = task_list;

        // Encontra a tarefa com deadline mais próximo
        while (aux != NULL) {
            if (aux->task->deadline < recente->task->deadline) {
                recente = aux;
            }
            aux = aux->next;
        }

        // Verifica se deadline já foi perdido antes de iniciar
        if (timer > recente->task->deadline) {
            printf("Time %d: Missed deadline for task %s (Deadline: %d)\n",
                   timer, recente->task->name, recente->task->deadline);
            delete(&task_list, recente->task);
            continue;
        }

        int start_time = timer;

        printf("Time %d: Running task %s (Burst: %d, Deadline: %d)\n",
               start_time, recente->task->name, recente->task->burst, recente->task->deadline);

        // Executa a tarefa, 1 unidade de tempo por vez
        for (int i = 0; i < recente->task->burst; ++i) {
        while (!flag_estouro);   // espera o “tick”
        flag_estouro = 0;
        run(recente->task, 1);
    }

        int finish_time = timer;
        int deadline_met = (finish_time <= recente->task->deadline);

        if (deadline_met) {
            printf("Time %d: Finished task %s (Met deadline)\n", finish_time, recente->task->name);
        } else {
            printf("Time %d: Finished task %s (Missed deadline)\n", finish_time, recente->task->name);
        }

        delete(&task_list, recente->task);
    }
}
