#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

#include "task.h"

void add_edf(char *name, int priority, int burst, int deadline);

// invoke the scheduler
void schedule();
