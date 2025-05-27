#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "timer.h"

#define TIME_U 10  // 0.00s
#define TIME_SLICE   10     

volatile int timer = 0;
volatile int flag_estouro = 0;

void* timer_thread(void* arg) {
    int slice_counter = 0;

    while (1) {
        usleep(TIME_U); 
        timer++;         
        slice_counter++;

        if (slice_counter >= TIME_SLICE) {
            flag_estouro = 1; 
            slice_counter = 0;
        }
    }
    return NULL;
}

void start_timer() {
    pthread_t tid;
    pthread_create(&tid, NULL, timer_thread, NULL);
    pthread_detach(tid);
}
