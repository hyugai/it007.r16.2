#include "scheduler.h"

/* Non-preemptive Shortest Job First. */
int main(void) {
    printf("Enter the number of processes: ");
    int count = (int)read_number(1, MAX_PROCESSES);
    Process *p = read_processes(count, 1);
    long long now = 0;

    for (int completed = 0; completed < count; ++completed) {
        int chosen = -1;
        long long next_arrival = -1;
        for (int i = 0; i < count; ++i) {
            if (p[i].remaining == 0)
                continue;
            if (p[i].arrival > now) {
                if (next_arrival == -1 || p[i].arrival < next_arrival)
                    next_arrival = p[i].arrival;
                continue;
            }
            /* Equal burst: FCFS; equal arrival: input order. */
            if (chosen == -1 || p[i].burst < p[chosen].burst ||
                (p[i].burst == p[chosen].burst &&
                 p[i].arrival < p[chosen].arrival))
                chosen = i;
        }
        if (chosen == -1) {
            now = next_arrival; /* CPU is idle until the next arrival. */
            --completed;
            continue;
        }
        p[chosen].start = now;
        now += p[chosen].burst;
        p[chosen].finish = now;
        p[chosen].remaining = 0;
    }

    print_results(p, count);
    free(p);
    return EXIT_SUCCESS;
}
