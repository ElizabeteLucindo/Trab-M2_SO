// Prioridades com Aging
// Prioridade menor = mais alta prioridade
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "CPU.h"
#define MIN_PRIORITY 1
#define AGING_INTERVAL 5 

struct node *task_list = NULL;
int tid_counter = 1;
int timer = 0;

void add(char *name, int priority, int burst) {
    Task *t = malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->deadline = 0;
    t->tid = tid_counter++;

    insert_end(&task_list, t); 
}


void schedule() {
    while (task_list != NULL) {
        struct node *temp = task_list;
        struct node *highest = task_list;
 
        // Encontra a tarefa com maior prioridade
        while (temp != NULL) {
            if (temp->task->priority < highest->task->priority) {
                highest = temp;
            }
            temp = temp->next;
        }
 
        // Executa a tarefa selecionada
        run(highest->task, highest->task->burst);
        delete(&task_list, highest->task);
 
        // Aplica o envelhecimento (aging) nas demais tarefas
        temp = task_list;
        while (temp != NULL) {
            if (temp->task->priority > MIN_PRIORITY) {
                temp->task->priority--; // aumenta prioridade (se possível)
            }
            temp = temp->next;
        }
    }
}
