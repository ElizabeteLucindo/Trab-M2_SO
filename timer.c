#include <unistd.h>
#include <pthread.h>
#include "timer.h"

volatile int timer = 0;
volatile int flag_estouro = 0;

void* timer_thread(void* arg) {
    while (1) {
        usleep(10);  // 1 ms
        timer++;       
        flag_estouro = 1;  // libera 1 unidade de burst
    }
    return NULL;
}

void start_timer() {
    pthread_t tid;
    pthread_create(&tid, NULL, timer_thread, NULL);
    pthread_detach(tid);
}