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
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->deadline = deadline;
    t->wait_time = 0;
    newTask->tid = task_id_counter++;

    insert_end(&task_list, newTask);
}

// invoke the scheduler
void schedule() {
    start_timer();

    while (task_list != NULL) {
        struct node *temp = task_list;
        struct node *earliest = task_list;

        // Escolhe a tarefa com menor deadline
        while (temp != NULL) {
            if (temp->task->deadline < earliest->task->deadline) {
                earliest = temp;
            }
            temp = temp->next;
        }

        printf("Time %d: Running task %s (Burst: %d, Deadline: %d)\n",
               timer, earliest->task->name, earliest->task->burst, earliest->task->deadline);

        // Executa a tarefa por cada unidade de tempo (tick)
        for (int i = 0; i < earliest->task->burst; i++) {
            while (flag_estouro == 0);  // Espera próximo tick
            flag_estouro = 0;

            run(earliest->task, 1); // Executa 1 unidade de tempo
        }

        printf("Time %d: Finished task %s\n", timer, earliest->task->name);

        delete(&task_list, earliest->task);  // <- Agora está no lugar certo
    }
}

