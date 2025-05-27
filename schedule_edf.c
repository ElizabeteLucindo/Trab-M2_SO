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
    start_timer();  

    while (task_list != NULL) {
        struct node *aux = task_list;
        struct node *recente = task_list;

        // Encontra deadline mais próximo
        while (aux != NULL) {
            if (aux->task->deadline < recente->task->deadline) {
                recente = aux;
            }
            aux = aux->next;
        }

        // Verifica se deadline já foi perdido
        if (timer > recente->task->deadline) {
            printf("Time %d: Deadline perdido task %s Deadline: %d\n",
                   timer, recente->task->name, recente->task->deadline);
            delete(&task_list, recente->task);
            continue;
        }

        printf("Time %d: Running task %s (Burst: %d, Deadline: %d)\n",
               timer, recente->task->name, recente->task->burst, recente->task->deadline);

        
        for (int i = 0; i < recente->task->burst; ++i) {
        while (!flag_estouro); // espera estouro
        flag_estouro = 0;
        run(recente->task, 1);
    }

        if (timer <= recente->task->deadline) {
            printf("Time %d: Task finalizada %s \n", timer, recente->task->name);
        } else {
            printf("Time %d: Task finalizada %s (Deadline perdido)\n", timer, recente->task->name);
        }

        delete(&task_list, recente->task);
    }
}
